#include "server.h"
#include "server_queue.h"
#include "packet.h"

squeue *init_squeue(void)
{
	squeue *q;

	q = malloc(sizeof(squeue));
	q->head = NULL;
	q->tail = NULL;

	return (q);
}

squeue_node  *init_node(packet_header *header, char *payload, squeue_node *tail)
{
	squeue_node  *new;

	new = malloc(sizeof(squeue_node));
	new->header = header;
	new->payload = payload;
	new->prev = tail;
	new->next = NULL;
}

char	 *peek(squeue *q)
{
	if (!q->size)
		return (NULL);
	else
		return (q->head->payload);
}

void	enqueue (squeue *q, packet_header *header, char *payload)
{
	squeue_node	*new;

	new = init_node(header, payload, q->tail);
	if (q->head == NULL) //adding first node of squeue
	{
		q->head = new;
		q->size = 1;
	}
	else
	{
		q->tail->next = new;
		q->size += 1;
	}
	q->tail = new;
}

char  *dequeue(squeue *q)
{
	char		*payload;

	if (q->size == 0)
	{
		return (NULL);
	}
	else
	{
		payload = peek(q);
		printf("peek payload addr : %p\n", payload);
		if (q->head == q->tail)
		{
			//When squeue contains only one node
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			q->head = q->head->next;
			q->head->prev = NULL;
		}
		q->size--;
		return (payload);
	}
}
