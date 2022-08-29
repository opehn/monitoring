#include "server.h"
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
	struct squeue_node	 *head;
	struct squeue_node	 *tail;
	int					 size;
} squeue;

typedef struct sparam
{
	squeue			*q;
	int				serverfd;
	pthread_mutex_t squeue_lock;
} sparam;

squeue		*init_squeue(void);
squeue_node	*init_node(packet_header *header, char *payload, squeue_node *prev);
char		*peek(squeue *q);
void		enqueue (squeue *q, packet_header *header, char *payload);
char		*dequeue(squeue *q);
