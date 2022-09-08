#include "server.h"
#include "server_queue.h"
#include "packet.h"
squeue *init_squeue(void)
{
	squeue *q;

	q = malloc(sizeof(squeue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;

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
	return (new);
}

squeue_node	 *peek(squeue *q)
{

	if (!q->size)
		return (NULL);
	else
		return (q->head);
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

void  free_shead(squeue *q)
{
	squeue_node	*temp;

	if (q->size == 0)
		return ;
	else
	{
		if (q->head == q->tail)
		{
			//When squeue contains only one node
			free(q->head->header);
			free(q->head->payload);
			free(q->head);
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			temp = q->head;
			q->head = q->head->next;
			q->head->prev = NULL;
			free(temp->header);
			free(temp->payload);
			free(temp);
		}
		q->size--;
	}
}
