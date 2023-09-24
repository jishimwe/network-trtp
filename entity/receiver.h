#include <bits/stdint-uintn.h>
#ifndef NEW_NETWORK_PROJECT_RECEIVER_H
#define NEW_NETWORK_PROJECT_RECEIVER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <time.h>

#include "packet_interface.h"
#include "linked_list.h"
#include "socket_helper.h"
#include "log.h"

int in_window(int seqnum);

int to_file(list_t *l);

int send_ack(int sfd, int seqnum, uint8_t type, uint8_t w);

void receiver(int sfd);

int receiver_main(int argc, char ** argv);

int stats_to_file(char *file);

#endif // NEW_NETWORK_PROJECT_RECEIVER_H
