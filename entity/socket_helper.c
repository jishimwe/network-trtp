#include "socket_helper.h"

const char * real_address(const char *address, struct sockaddr_in6 *rval)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *addr;
    int ret = getaddrinfo(address, NULL, &hints, &addr);
    if(ret != 0)
    {
        return gai_strerror(ret);
    }
    memcpy(rval, addr->ai_addr, sizeof (struct sockaddr_in6));
    freeaddrinfo(addr);
    return NULL;
}

int create_socket(struct sockaddr_in6 *source_addr,
                 int src_port,
                 struct sockaddr_in6 *dest_addr,
                 int dst_port)
{
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if(s < 0)
    {
        fprintf(stderr, "Error creating the socket.\n");
        return -1;
    }
    if(source_addr != NULL)
    {
        if(src_port > 1023 && src_port < 65536)
        {
            source_addr->sin6_port = htons(src_port);
        }
        if(src_port < 1024)
        {
            fprintf(stderr, "The %i is a reserved port number.\n", src_port);
        }
        int bound = bind(s, (struct sockaddr *)source_addr, sizeof (struct sockaddr_in6));
        if(bound != 0)
        {
            fprintf(stderr, "Error binding the socket.\n");
            return -1;
        }
    }

    if(dest_addr != NULL)
    {
        if(dst_port > 1023 && dst_port < 65536)
        {
            dest_addr->sin6_port = htons(dst_port);
        }
        if(dst_port < 1024)
        {
            fprintf(stderr, "The %i is a reserved port number.\n", dst_port);
            return -1;
        }
        int connected = connect(s, (struct sockaddr *) source_addr, sizeof (struct sockaddr_in6));
        if(connected != 0)
        {
            fprintf(stderr, "Error connecting to the socket.\n");
            return -1;
        }
    }
    return s;
}

void read_write_loop(const int sfd);

int wait_for_client(int sfd, struct sockaddr_in6 *s)
{
    char buff[1024];

//    struct sockaddr_in6 s;
    socklen_t len = sizeof(*s);

    int read = recvfrom(sfd, buff, sizeof(char)*1024, MSG_PEEK,(struct sockaddr *) s, &len);
//    int sent = sendto(sfd, "OK", 2, 0, (struct sockaddr *) s, len);
    if(read < 0)
    {
        fprintf(stderr, "Error receiving from the socket.\n");
        return -1;
    }

    int connected = connect(sfd, (struct sockaddr *) s, (int) len);
    if(connected < 0)
    {
        fprintf(stderr, "Error connecting to the socket.\n");
        return -1;
    }
    return 0;
}
