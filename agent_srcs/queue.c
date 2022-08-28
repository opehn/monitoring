#include "collect.h"
#include "queue.h"

queue *init_queue(void)
{
	queue *q;

	q = malloc(sizeof(queue));
	q->head = NULL;
	q->tail = NULL;

	return (q);
}

queue_node  *init_node(packet *data, queue_node *prev)
{
	queue_node  *new;

	new = malloc(sizeof(queue_node));
	new->data = data;
	new->prev = prev;
	new->next = NULL;
}

packet	 *peek(queue *q)
{
	if (!q->size)
		return (NULL);
	else
		return (q->head->data);
}

void	enqueue (queue *q, packet *data)
{
	queue_node	*new;

	printf("enqueue payload addr : %p\n", data->payload);
	new = init_node(data, q->tail);
	if (q->head == NULL) //adding first node of queue
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

packet  *dequeue(queue *q)
{
	packet		*data;

	if (q->size == 0)
	{
		return (NULL);
	}
	else
	{
		data = peek(q);
		printf("peek data addr : %p\n", data);
		if (q->head == q->tail)
		{
			//When queue contains only one node
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			q->head = q->head->next;
			q->head->prev = NULL;
		}
		q->size--;
		return (data);
	}
}
