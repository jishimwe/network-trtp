#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

struct nodes * createnode(pkt_t * data);

struct nodes * createnode(pkt_t * data){
    struct nodes * newNode = malloc(sizeof(struct nodes));
    if (!newNode) {
        return NULL;
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

List * makelist(){
    List * list = malloc(sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}


void add(pkt_t * data, List * list){
    struct nodes * current = NULL;
    if(list->head == NULL){
        list->head = createnode(data);
    }
    else {
        current = list->head;
        while (current->next!=NULL){
            current = current->next;
        }
        current->next = createnode(data);
    }
}

int size(List * list){
    struct nodes * root = list->head;
    int cpt = 0;
    if(list->head == NULL){
        return 0;
    }
    while(root->next != NULL){
        cpt++;
        root = (struct nodes *) root->next;
    }
    return cpt;
}

bool isEmpty(List * list){
    return size(list) == 0;
}

void delete(int seqnum, List * list){
    struct nodes * current = list->head;
    struct nodes * previous = current;
    while(current != NULL){
        if(current->data->seqnum == seqnum){
            previous->next = current->next;
            if(current == list->head)
                list->head = current->next;
            pkt_del(current->data);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

void deleteAllPacketUnderSeqnum(int seqnum,List * list){
    for (int i = 0; i <= seqnum; ++i) {
        delete(seqnum,list);
    }
}
