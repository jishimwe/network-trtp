#ifndef LINKEDLIST_HEADER
#define LINKEDLIST_HEADER

#include "packet_interface.h"


typedef struct list List;
struct list {
    struct nodes * head;
};

struct nodes {
    pkt_t * data;
    struct nodes * next;
};


List * makelist();
void add(pkt_t * data, List * list);
void delete(int seqnum, List * list);
void deleteAllPacketUnderSeqnum(int seqnum,List * list);
int size(List * list);
bool isEmpty(List * list);

#endif