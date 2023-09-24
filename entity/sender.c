#include "sender.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdbool.h>
#include "packet_interface.h"
#include "socket_helper.h"
#include "list.h"

// Transfer stats
int data_sent = 0;
int fec_sent = 0;
int ack_received = 0;
int nack_received = 0;
int min_rtt = 0;
int max_rtt = 0;
int packet_retransmitted = 0;

u_int8_t WINDOW_SIZE = 31;
uint16_t ACK_RESEND = 4;
size_t ACK_SIZE = 10;
struct sockaddr_in6 servaddr,cliaddr;
socklen_t socklen;
uint8_t seqnum_global = 0;

void send_packet(int sock, pkt_t * pkt) {
    size_t len = 528;
    char buffer[len];
    pkt_encode(pkt,buffer,&len);
    size_t n = sendto(sock,buffer,len,0,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if (n < 0) {
       // fprintf(stderr,"Oh dear, something went wrong with sendto()! %s\n", strerror(errno));
    }
}


pkt_t * fileToPacket(FILE * file_to_read){
        pkt_t * packet = pkt_new();
        char buffer[MAX_PAYLOAD_SIZE];
        if(file_to_read != NULL){
            size_t read = fread(buffer,1,MAX_PAYLOAD_SIZE,file_to_read);
            if(read > 0){
                pkt_set_payload(packet,buffer,read);
                pkt_set_length(packet,read);
                pkt_set_type(packet,PTYPE_DATA);
                pkt_set_window(packet,WINDOW_SIZE);
                return packet;
            }else{
                pkt_del(packet);
                return NULL;
            }
        }else{
            pkt_del(packet);
        }
        return NULL;
}


void sender(int network_socket,char * filename,FILE * file_to_read){
    struct list * list = makelist();
    struct stat st;
    stat(filename,&st);
    size_t total_bytes_read = 0;
    size_t size_of_file = st.st_size;
    do {
        if(size(list) < WINDOW_SIZE){
            pkt_t * packet = fileToPacket(file_to_read);
            if(packet == NULL) exit(1);
            pkt_set_timestamp(packet,(uint32_t)time(NULL));
            pkt_set_seqnum(packet, seqnum_global);
            seqnum_global = (seqnum_global+1)%MAX_SEQ_NUM;
            total_bytes_read+= packet->length;
            add(packet,list);
            //l = orderInsert(l,packet);
            //add_in_order(list,packet);
            send_packet(network_socket,packet);
        }
            char buffer[PKT_MIN_HEADERLEN];
            long bytes_read = (int) recvfrom(network_socket, buffer, PKT_MIN_HEADERLEN, 0,(struct sockaddr *)&cliaddr,&socklen);
            if (bytes_read > 0) {
                pkt_t *pkt = pkt_new();
                pkt_decode((const char *) buffer, PKT_MIN_HEADERLEN, pkt);
                int seqnum = pkt->seqnum;
                if(seqnum == 0){
                    deleteAllPacketUnderSeqnum(254,list);
                }else{
                    deleteAllPacketUnderSeqnum(seqnum-1,list);
                }
            }
            else{
                struct nodes * root = list->head;
                while(root != NULL){
                    double diff = difftime(time(NULL),(time_t) root->data->timestamp);
                    if(diff >= (double)(ACK_RESEND)){
                        pkt_set_timestamp(root->data,(uint32_t)time(NULL));
                        send_packet(network_socket,root->data);
                        printf("Packet resent. diff time : %f\n",diff);
                    }
                    root = root->next;
                }
            }
    }while(!isEmpty(list) || (total_bytes_read < size_of_file));
    pkt_t *last_pkt = pkt_new();
    pkt_set_seqnum(last_pkt, seqnum_global-1);
    pkt_set_length(last_pkt, 0);
    pkt_set_type(last_pkt, PTYPE_DATA);
    send_packet(network_socket,last_pkt);
    fprintf(stderr, "All files read\n");
}

int sender_main(int argc,char ** argv){
    char *filename = NULL;
    char *stats_filename = NULL;
    char *receiver_ip = NULL;
    char *receiver_port_err;
    bool fec_enabled = false;
    uint16_t receiver_port;

    // Default values (to change maybe?)
    filename = "big_file.pdf";
    stats_filename = "sender_stats.csv";
    receiver_ip = "::";
    receiver_port = 12345;

    int opt;

    while ((opt = getopt(argc, argv, "f:s:hc")) != -1) {
        switch (opt) {
        case 'f':
            filename = optarg;
            break;
        case 'h':
            return print_usage(argv[0]);
        case 's':
            stats_filename = optarg;
            break;
    case 'c':
        fec_enabled = true;
        break;
        default:
            return print_usage(argv[0]);
        }
    }

    if (optind + 3 != argc) {
        //fprintf(stderr, "Unexpected number of positional arguments");
        return print_usage(argv[0]);
    }

    receiver_ip = argv[optind+1];
    receiver_port = (uint16_t) strtol(argv[optind + 2], &receiver_port_err, 10);
    if (*receiver_port_err != '\0') {
        fprintf(stderr, "Receiver port parameter is not a number");
        return print_usage(argv[0]);
    }


    int network_socket;
    network_socket = socket(AF_INET6, SOCK_DGRAM, 0);
    if (network_socket == -1) {
        //fprintf(stderr, "SOCKET NOT CREATED\n");
        exit(1);
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin6_family = AF_INET6;
    servaddr.sin6_port = htons(receiver_port);
    inet_pton(AF_INET6, receiver_ip, &servaddr.sin6_addr);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 100;
    if (setsockopt(network_socket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
        //fprintf(stderr, "Error setting up socket options\n");
    }
    FILE * ptr = fopen(filename,"r");
    sender( network_socket,filename,ptr);

    int stats = sender_stats_to_file(stats_filename);
    if(stats < 0)
    {
        ///fprintf(stderr, "Could not write the stats to file %s\n", stats_filename);
    }

    return EXIT_SUCCESS;
}

int sender_stats_to_file(char * file)
{
    FILE *f = fopen(file, "w");
    if(file == NULL)
    {
        return print_usage(file);
    }
    fprintf(f, "data_sent:%d\n", data_sent);
    fprintf(f, "fec_sent:%d\n", fec_sent);
    fprintf(f, "ack_received:%d\n", ack_received);
    fprintf(f, "nack_received:%d\n", nack_received);
    fprintf(f, "min_rtt:%d\n", min_rtt);
    fprintf(f, "max_rtt:%d\n", max_rtt);
    fprintf(f, "packet_retransmitted:%d\n", packet_retransmitted);

    return 0;
}