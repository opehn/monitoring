#ifndef	SERVER_QUEUE_H
#define	SERVER_QUEUE_H

#include <pthread.h>
#include "packet.h"

typedef struct squeue_node 
{
	packet_header		*header;
	char				*payload;
	struct squeue_node	 *prev;
	struct squeue_node	 *next;
} squeue_node;

typedef struct squeue 
{
	struct squeue_node	*head;
	struct squeue_node	*tail;
	int					size;
} squeue;

typedef struct sparam
{
	squeue				*q;
	int					clientfd;
	int					logfd;
	pthread_mutex_t squeue_lock;
	pthread_mutex_t log_lock;
} sparam;

squeue		*init_squeue(void);
squeue_node	*init_node(packet_header *header, char *payload, squeue_node *prev);
squeue_node	*peek(squeue *q);
void		enqueue (squeue *q, packet_header *header, char *payload);
void		free_shead(squeue *q);

#endif
