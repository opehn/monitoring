#include "server.h"
#include "server_queue.h"
#include "packet.h"

data_queue *init_data_queue(void)
{
	data_queue *dq;

	dq = malloc(sizeof(data_queue));
	dq->head = NULL;
	dq->tail = NULL;
	dq->size = 0;

	return (dq);
}

data_queue_node  *init_node(packet_header *header, char *payload, data_queue_node *tail)
{
	data_queue_node  *new;

	new = malloc(sizeof(data_queue_node));
	new->header = header;
	new->payload = payload;
	new->prev = tail;
	new->next = NULL;
	return (new);
}

data_queue_node	 *peek(data_queue *dq)
{
	if (!dq->size)
		return (NULL);
	else
		return (dq->head);
}

void	enqueue(data_queue *dq, packet_header *header, char *payload)
{
	data_queue_node	*new;

	new = init_node(header, payload, dq->tail);
	if (dq->head == NULL) //adding first node of data_queue
	{
		dq->head = new;
		dq->size = 1;
	}
	else
	{
		dq->tail->next = new;
		dq->size += 1;
	}
	dq->tail = new;
}

void  free_shead(data_queue *dq)
{
	data_queue_node	*temp;

	if (dq->size == 0)
		return ;
	else
	{
		if (dq->head == dq->tail)
		{
			//When data_queue contains only one node
			free(dq->head->header);
			free(dq->head->payload);
			free(dq->head);
			dq->tail = NULL;
			dq->head = NULL;
		}
		else
		{
			temp = dq->head;
			dq->head = dq->head->next;
			dq->head->prev = NULL;
			free(temp->header);
			free(temp->payload);
			free(temp);
		}
		dq->size--;
	}
}
