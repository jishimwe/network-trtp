#include "linked_list.h"

list_t * list_init()
{
    return (list_t *) calloc(1, sizeof (list_t));
}

int add_in_order(list_t * l, pkt_t *pkt)
{
    node_t *n = (node_t *) malloc(sizeof (node_t));
    if(n == NULL)
    {
        fprintf(stderr, "Memory allocation error while creating a node_t:\n");
        return errno;
    }
    n->pkt = pkt;
    if(is_empty(l))
    {
        n->next = NULL;
        n->prev = NULL;
        l->first =n;
        l->last =n;
    } else {
        node_t *current = l->last;
        uint8_t seqnum = pkt->seqnum;
        int f = 0;
        if(current->pkt->seqnum < MAX_WINDOW_SIZE && seqnum > (MAX_SEQ_NUM-MAX_WINDOW_SIZE))
        {
            current = l->first;
            if(seqnum < current->pkt->seqnum || (seqnum > current->pkt->seqnum && current->pkt->seqnum < MAX_WINDOW_SIZE))
            {
                f = 1;
            }
            uint8_t next_seqnum = current->next->pkt->seqnum;
            while(seqnum > next_seqnum && f != 1 && next_seqnum >= MAX_WINDOW_SIZE && seqnum > (MAX_SEQ_NUM - MAX_WINDOW_SIZE))
            {
                current = current->next;
                next_seqnum = current->next->pkt->seqnum;
            }
        } else {
            uint8_t prev_seqnum = current->pkt->seqnum;
            while(seqnum < prev_seqnum && prev_seqnum >= (MAX_SEQ_NUM - MAX_WINDOW_SIZE) && seqnum < MAX_WINDOW_SIZE)
            {
                if(current == l->first)
                {
                    if(seqnum < l->first->pkt->seqnum)
                    {
                        f = 1;
                    }
                    break;
                }
            }
        }
        if(f)
        {
            n->prev = current->next;
            n->next = current;
            current->prev->next = n;
            current->prev = n;
            l->first = n;
        } else {
            if(n->pkt->seqnum < current->pkt->seqnum)
            {
                n->next = current;
                n->prev = current->prev;
                current->prev = n;
            } else if (n->pkt->seqnum > current->pkt->seqnum) {
                n->next = current->next;
                current->next = n;
                n->prev = current;
            }
            if(current == l->last)
            {
                l->last = n;
            }
        }
    }
    l->size += 1;
    return 0;
}

pkt_t * remove_list_first(list_t * l)
{
    pkt_t *pkt = NULL;
    if(l == NULL || l->size == 0)
    {
        return pkt;
    }
    node_t *current = l->first;
    if(l->size <= 1)
    {
        l->first = NULL;
        l->last = NULL;
        pkt_t *pkt1 = current->pkt;
        free_node(current);
        l->size -= 1;
        return pkt1;
    }
    current->prev->next = current->next;
    current->next->prev = current->prev;
    l->first = l->first->next;
    pkt = current->pkt;
    free_node(current);
    return pkt;
}

pkt_t * remove_list_seqnum(list_t *l, uint8_t seqnum)
{
    printf("NULsL\n");
    node_t *current = l->first;
    if(current == NULL)
    {
        printf("NULL\n");
        return NULL;
    }
    uint8_t next_seqnum = current->pkt->seqnum;
    printf("SEQNUM : %d\n",next_seqnum);
    while(next_seqnum != seqnum)
    {
        current = current->next;
        if(current == l->first)
        {
            fprintf(stderr, "No packet with %i as seqnum", seqnum);
            return NULL;
        }
        next_seqnum = current->pkt->seqnum;
    }
    if(current == l->first)
    {
        if(l->size <= 1)
        {
            l->first = NULL;
            l->last = NULL;
            pkt_t *pkt = current->pkt;
            free_node(current);
            l->size = 0;
            return pkt;
        }
        //current->prev->next = current->next;
        //current->next->prev = current->prev;
        //l->last = l->last->prev;
    } else if (current == l->last){
        current->prev->next = current->next;
        current->next->prev = current->prev;
        l->last = l->last->prev;
    } else {
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }
    l->size -= 1;
    if(l->size == 0)
    {
        l->first = NULL;
        l->last = NULL;
    }
    current->next = NULL;
    current->prev = NULL;

    pkt_t *pkt = current->pkt;
    free_node(current);
    return pkt;
}

int remove_list_ack(list_t *l, uint8_t seqnum)
{
    //printf("BROL 1\n");
    if(l->size == 0 || !in_list(l, seqnum)) {
        return 0;
    }
    //printf("BROL 2\n");
    int count = 0;
    uint8_t first_seqnum = l->first->pkt->seqnum;
    //printf("BROL 3\n");
    //printf("SEQNUM %d\n",first_seqnum);
    while (seqnum <= first_seqnum)
    {
        //printf("loop\n");
        first_seqnum = l->first->pkt->seqnum;
        pkt_t *pkt = remove_list_seqnum(l, first_seqnum);
        //printf("loop 1\n");
        printf("SEQNUM : %d",pkt->seqnum);
        if(pkt->seqnum == 85){
            printf("Bilal");
        }
        //pkt_del(pkt);
        printf("loop 2\n");
        count++;
        if(l->size == 0)
        {
            break;
        }
    }
    //printf("BROL 4\n");
    return count;
}

pkt_t * get_list_packet(list_t *l, uint8_t seqnum)
{
    if(l->size == 0)
    {
        return NULL;
    }
    node_t *current = l->first;
    uint8_t next_seqnum = current->pkt->seqnum;
    while (next_seqnum != seqnum)
    {
        current = current->next;
        if(current == l->first)
        {
            return NULL;
        }
        next_seqnum = current->pkt->seqnum;
    }
    return current->pkt;
}

pkt_t * get_list_unsent(list_t *l)
{
    pkt_t *pkt = NULL;
    if(!l || l->size == 0)
    {
        return NULL;
    }
    node_t *current = l->first;
    for(int i = 0; i < (int) l->size; i++)
    {
        if(current->pkt->timestamp == 0)
        {
            return pkt;
        }
        current = current->next;
    }
    return NULL;
}

int in_list(list_t *l, uint8_t seqnum)
{
    if(l == NULL || l->size <= 0)
    {
        return 0;
    }

    node_t * current = l->first;
    for (int i = 0; i < (int) l->size; ++i) {
        uint8_t new_seqnum = current->pkt->seqnum;
        if(new_seqnum == seqnum)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

size_t list_size(list_t *l)
{
    if(l == NULL)
    {
        perror("Not a valid list");
        return 0;
    }
    return l->size;
}

bool is_empty(list_t *l)
{
    return !(bool)l->size;
}

void free_list(list_t *l)
{
    while (l->first != NULL)
    {
        uint8_t seqnum = l->first->pkt->seqnum;
        pkt_t *pkt = remove_list_seqnum(l, seqnum);
        pkt_del(pkt);
    }
    free(l);
}

void free_node(node_t *n)
{
    if(n == NULL)
    {
        return;
    }
    //pkt_del(n->pkt);
    free(n);
    n = NULL;
}

void list_print(list_t *l)
{
    fprintf(stderr, "Beginning the printing of the list... \n");
    node_t *current = l->first;
    for (int i = 0; i < (int)l->size; i++)
    {
        fprintf(stderr, "Seqnum : %i \t Payload %s\n", current->pkt->seqnum, current->pkt->payload);
        current = current->next;
    }
    fprintf(stderr, "Ending the printing of the list.\n");
}