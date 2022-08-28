#include "packet.h"

typedef struct queue_node {
    packet                *data;
    struct queue_node     *prev;
    struct queue_node     *next;
} queue_node;

typedef struct queue {
	struct queue_node		*head;
	struct queue_node		*tail;
	int						size;
} queue;

typedef struct param
{
	queue			*q;
	pthread_mutex_t	queue_lock;
} param;

queue		*init_queue(void);
queue_node	*init_node(packet *data, queue_node *prev);
packet		*peek(queue *q);
void		enqueue (queue *q, packet *data);
packet		*dequeue(queue *q);
