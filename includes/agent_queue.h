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

typedef struct aparam
{
	aqueue			*q;
	pthread_mutex_t	aqueue_lock;
} aparam;

aqueue		*init_aqueue(void);
aqueue_node	*init_node(packet *data, aqueue_node *prev);
packet		*peek(aqueue *q);
void		enqueue (aqueue *q, packet *data);
packet		*dequeue(aqueue *q);
