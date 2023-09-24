#include <stdlib.h>
#include <stdio.h>
#include <netinet/in.h>
#include <string.h>

#include "packet_interface.h"
#include "receiver.h"

int window = MAX_WINDOW_SIZE;
int last_ack = 0;
int last_written = -1;
int potentialEOF = 0;
int last_ack_to_send = -2;
uint32_t time_last_data = 0;

struct sockaddr_in6 receiver_addr, sender_addr;

int fd = STDOUT_FILENO;
int TIMEOUT = 2500;

int data_received = 0;
int data_truncated_received = 0;
int fec_received = 0;
int ack_sent = 0;
int nack_sent = 0;
int packet_ignored = 0;
int packet_duplicated = 0;
int packet_recovered = 0;

int in_window(int seqnum)
{
    if(seqnum == last_written){
        return 0;
    } else if(seqnum < last_written) {
        if(seqnum + MAX_SEQ_NUM - last_written <= window){
            return 1;
        }
        return 0;
    } else {
        if(seqnum - last_written < window){
            return 1;
        }
        return 0;
    }
}

int to_file(list_t *l)
{
    node_t *current = l->first;
    pkt_t *pkt;
    list_print(l);
    int current_seqnum = current->pkt->seqnum;
    while (current != NULL && current_seqnum == (last_written + 1)%MAX_SEQ_NUM)
    {
        pkt = current->pkt;
        ssize_t written = write(fd, pkt->payload, pkt->length);
        if(written < 0)
        {
            //fprintf(stderr, "Can't write to file %i\n", fd);
            return -1;
        }
        //fprintf(stderr, "Packet %d written to file %i\n", pkt->seqnum, fd);
        last_written = pkt->seqnum;

        current = current->next;
        if(current == NULL) {
            remove_list_seqnum(l, pkt->seqnum);
            break;
        }
        current_seqnum = current->pkt->seqnum;
        remove_list_seqnum(l, pkt->seqnum);
    }
    return 1;
}

int send_ack(int sfd, int seqnum, uint8_t type, uint8_t w)
{
    pkt_t *pkt = pkt_new();
    if(pkt == NULL)
    {
        //fprintf(stderr, "Error creating an ACK packet\n");
        return -1;
    }

    pkt->type = type;
    pkt->seqnum = seqnum;
    pkt->tr = 0;
    pkt->window = w;
    pkt->timestamp = time_last_data;
    pkt->payload = NULL;
    pkt->length = 0;

    char buff[PKT_MIN_LEN];
    size_t len;
    pkt_status_code pkt_status = pkt_encode(pkt, buff, &len);
    if(pkt_status != PKT_OK)
    {
        //fprintf(stderr, "Error enconding the ACK packet : %d\n", pkt_status);
        pkt_del(pkt);
        return -1;
    }
    int s_len = sizeof (sender_addr);

    ssize_t sent = send(sfd, buff, len, 0);
    if(sent < 0)
    {
        //fprintf(stderr, "Error sending the (N)ACK packet : %zd\n", sent);
        pkt_del(pkt);
        return -1;
    }

    if(type == PTYPE_ACK)
    {
        last_ack = seqnum;
        //fprintf(stderr, "ACK %d was sent successfully\n", seqnum);
        ack_sent++;
    } else {
        //fprintf(stderr, "NACK %d was sent successfully\n", seqnum);
        nack_sent++;
    }
    pkt_del(pkt);
    return 0;
}

void receiver(int sfd)
{
    list_t *to_receive = list_init();
    struct timespec start, end;
    int reset_timer = 1;
    int n_accumulated = 0;
    int eof_ack = 0;
    ssize_t s = 1;

    while (last_ack != last_ack_to_send && s > 0 && eof_ack == 0)
    {
        int err;
        pkt_t *pkt = pkt_new();
        memset(pkt, 0, sizeof (&pkt));
        pkt_status_code pkt_status;
        char buff[PKT_MAX_LEN];
        memset(buff, 0, PKT_MAX_LEN);
        int s_len = sizeof(sender_addr);
        s = recv(sfd, buff, PKT_MAX_LEN, 0);
        if (s < 0) {
            //fprintf(stderr, "Could not recvfrom socket %d\n", sfd);
        }
        if (reset_timer) {
            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
            reset_timer = 0;
        }
        clock_gettime(CLOCK_MONOTONIC_RAW, &end);
        n_accumulated++;

        pkt_status = pkt_decode(buff, s, pkt);
        if (pkt_status != PKT_OK && (pkt->type != PTYPE_DATA || pkt->type != PTYPE_FEC)) {
            //fprintf(stderr, "Incorrect packet format : %i\n", pkt->type);
            pkt_del(pkt);
            eof_ack = 1;
            packet_ignored++;
        } else {
            //fprintf(stderr, "Packet %d received\n", pkt->seqnum);

            if (in_list(to_receive, pkt->seqnum)) {
                //fprintf(stderr, "Packet %d is duplicated. List size %zu\n", pkt->seqnum, to_receive->size);
                eof_ack = 1;
                packet_duplicated++;
            } else if (in_window(pkt->seqnum)) {
                if (pkt->tr == 1) {
                    send_ack(sfd, pkt->seqnum, PTYPE_NACK, window - to_receive->size);
                    //printf("\nBILAL JEAN PAULACK SEND BROL\n");
                    continue;
                } else if (pkt->type == PTYPE_DATA && pkt->length == 0) {
                    //fprintf(stderr, "Last packet %d sent\n", pkt->seqnum);
                    last_ack_to_send = pkt->seqnum;
                    eof_ack = 1;
                } else {
                    err = add_in_order(to_receive, pkt);
                    if (err < 0) {
                        //fprintf(stderr, "Error adding packet %d to the list\n", pkt->seqnum);
                        return;
                    }
                    time_last_data = (uint32_t) time(NULL);
                    err = to_file(to_receive);
                    if (err < 0) {
                        //fprintf(stderr, "Error writing to file %d\n", fd);
                    }
                    if (window > MAX_WINDOW_SIZE) {
                        window = MAX_WINDOW_SIZE;
                    }
                }
            } else {
                //fprintf(stderr, "Packet %d out of the window\n", pkt->seqnum);
            }
        }

            if(send_ack(sfd, (last_written + 1) % MAX_SEQ_NUM, PTYPE_ACK, window - to_receive->size) < 0)
            {
                //fprintf(stderr, "Error sending a (n)ack\n");
            }else{
                //printf("\nBILAL JEAN PAULACK SEND BROL\n");
            }
            n_accumulated = 0;
            reset_timer = 1;
            eof_ack = 0;

    }
    free_list(to_receive);
}

int receiver_main(int argc, char ** argv)
{
    char *file = NULL;
    char *stats = NULL;
    char *listen_ip = NULL;
    char *listen_port_nan = NULL;
    uint16_t listen_port;
    int opt = getopt(argc, argv, "f:s:h");

    while(opt != -1)
    {
        if(opt == 'f'){
            file = optarg;
            opt = getopt(argc, argv, "f:s:h");
        } else if(opt == 'h') {
            return print_usage(argv[0]);
        } else if(opt == 's') {
            stats = optarg;
            opt = getopt(argc, argv, "f:s:h");
        } else {
            return print_usage(argv[0]);
        }
    }
    if(optind + 3 != argc)
    {
        //fprintf(stderr, "Wrong number of arguments %d\n", argc);
        return print_usage(argv[0]);
    }

    listen_ip = argv[optind+1];
    listen_port = (uint16_t) strtol(argv[optind+2], &listen_port_nan, 10);

    if(*listen_port_nan != '\0')
    {
        //fprintf(stderr, "Receiver port is not valid %s\n", listen_port_nan);
        return print_usage(argv[0]);
    }

    if(file != NULL)
    {
        fd = open(file, O_CREAT|O_RDWR);
        if(fd < 0)
        {
            //fprintf(stderr, "Could not open the file %s\n", file);
            return print_usage(file);
        }
    } else {
        file = "STDOUT";
    }

    //fprintf(stderr, "Receiving data from %s [IP6] on the %u [PORT] to be written in %s [OUTPUT]\n", listen_ip, listen_port, file);

    const char *addr_err = real_address(listen_ip, &sender_addr);
    if(addr_err)
    {
        //fprintf(stderr, "Could not resolve hostname %s [IP] %d [PORT]\n", listen_ip, listen_port);
        return print_usage(argv[0]);
    }

    int sfd = create_socket(&sender_addr, listen_port, NULL, -1);
    if(sfd < 0)
    {
        //fprintf(stderr, "Could not create socket for %s [IP] %d [PORT]\n", listen_ip, listen_port);
        return print_usage(argv[0]);
    }

    int waiting = wait_for_client(sfd, &sender_addr);
    if(waiting < 0)
    {
        //fprintf(stderr, "Could not connect to the socket %s [IP] %d [PORT]\n", listen_ip, listen_port);
        close(sfd);
        return print_usage(argv[0]);
    }

    receiver(sfd);

    int stats_writing = stats_to_file(stats);
    if(stats_writing < 0)
    {
        //fprintf(stderr, "Could not write the stats to file %s\n", stats);
        return print_usage(argv[0]);
    }
    return EXIT_SUCCESS;
}

int stats_to_file(char *file) {
    FILE *f = fopen(file, "w");
    if(file == NULL)
    {
        return print_usage(file);
    }
    fprintf(f,"data_received:%d\n",data_received);
    fprintf(f,"data_truncated_received:%d\n",data_truncated_received);
    fprintf(f,"fec_received:%d\n", fec_received);
    fprintf(f,"ack_sent:%d\n",ack_sent);
    fprintf(f,"nack_sent:%d\n",nack_sent);
    fprintf(f,"packet_ignored:%d\n",packet_ignored);
    fprintf(f,"packet_duplicated:%d\n",packet_duplicated);
    fprintf(f,"packet_recovered:%d\n",packet_recovered);
    return 0;
}