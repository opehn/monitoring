#ifndef	SERVER_QUEUE_H
#define	SERVER_QUEUE_H

#include <pthread.h>
#include "packet.h"

typedef struct data_queue_node 
{
	packet_header				*header;
	char						*payload;
	struct data_queue_node		*prev;
	struct data_queue_node		*next;
} data_queue_node;

typedef struct data_queue 
{
	struct data_queue_node	*head;
	struct data_queue_node	*tail;
	int						size;
} data_queue;

data_queue			*init_data_queue(void);
data_queue_node		*init_node(packet_header *header, char *payload, data_queue_node *prev);
data_queue_node		*peek(data_queue *q);
void				enqueue (data_queue *q, packet_header *header, char *payload);
data_queue_node		*dequeue(data_queue *dq);
void				free_data(data_queue_node *data);

#endif
