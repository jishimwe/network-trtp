#include <bits/types/FILE.h>

#ifndef NEW_NETWORK_PROJECT_SENDER_H
#define NEW_NETWORK_PROJECT_SENDER_H

void sender(int network_socket,char * filename,FILE * file_to_read);
int sender_main(int argc,char ** argv);
int sender_stats_to_file(char * file);

#endif //NEW_NETWORK_PROJECT_SENDER_H
