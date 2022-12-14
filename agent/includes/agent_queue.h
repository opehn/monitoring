#ifndef AGENT_QUEUE_H
#define AGENT_QUEUE_H

#include "packet.h"

typedef struct aqueue_node {
    packet                *data;
    struct aqueue_node     *prev;
    struct aqueue_node     *next;
} aqueue_node;

typedef struct aqueue {
	struct aqueue_node		*head;
	struct aqueue_node		*tail;
	int						size;
} aqueue;

aqueue		*init_aqueue(void);
aqueue_node	*init_node(packet *data, aqueue_node *prev);
void		enqueue (aqueue *q, packet *data);
aqueue_node	*dequeue (aqueue *q);
void		free_node(aqueue_node *cur);

#endif
