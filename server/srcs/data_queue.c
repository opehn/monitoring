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

data_queue_node	*dequeue(data_queue *dq)
{
	data_queue_node	*data;

	if (dq->size == 0)
	{
		return (NULL);
	}
	else
	{
		data = peek(dq);
		if (dq->head == dq->tail)
		{
			//When data_queue contains only one node
			dq->head = NULL;
			dq->tail = NULL;
		}
		else
		{
			dq->head = dq->head->next;
			dq->head->prev = NULL;
		}
		dq->size--;
	}
	return (data);
}

void		free_data(data_queue_node *data)
{
	free(data->header);
	free(data->payload);
	free(data);
}
