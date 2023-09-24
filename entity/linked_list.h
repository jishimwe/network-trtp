#ifndef NETWORK_LINKED_LIST_H
#define NETWORK_LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

#include "log.h"
#include "packet_interface.h"

typedef struct node {
    struct node *next;
    struct node *prev;
    pkt_t *pkt;
} node_t;

typedef struct linked_list {
    struct node *first;
    struct node *last;
    size_t size;
}list_t;

list_t * list_init();

int add_in_order(list_t * l, pkt_t *pkt);

pkt_t * remove_list_first(list_t * l);

pkt_t * remove_list_seqnum(list_t *l, uint8_t seqnum);

int remove_list_ack(list_t *l, uint8_t seqnum);

pkt_t * get_list_packet(list_t *l, uint8_t seqnum);

pkt_t * get_list_unsent(list_t *l);

int in_list(list_t *l, uint8_t seqnum);

size_t list_size(list_t *l);

bool is_empty(list_t *l);

void free_list(list_t *l);

void free_node(node_t *n);

void list_print(list_t *l);

#endif //NETWORK_LINKED_LIST_H
