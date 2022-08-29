#include "collect.h"
#include "agent_queue.h"

aqueue *init_aqueue(void)
{
	aqueue *q;

	q = malloc(sizeof(aqueue));
	q->head = NULL;
	q->tail = NULL;

	return (q);
}

aqueue_node  *init_node(packet *data, aqueue_node *prev)
{
	aqueue_node  *new;

	new = malloc(sizeof(aqueue_node));
	new->data = data;
	new->prev = prev;
	new->next = NULL;
}

packet	 *peek(aqueue *q)
{
	packet	*res;

	if (!q->size)
		return (NULL);
	else
	{
		res = malloc(sizeof(packet));
		res->length = q->head->data->length;
		res->payload = strndup(q->head->data->payload, res->length);
	}
		return (res);
}

void	enqueue (aqueue *q, packet *data)
{
	aqueue_node	*new;

//	printf("enaqueue payload addr : %p\n", data->payload);
	new = init_node(data, q->tail);
	if (q->head == NULL) //adding first node of aqueue
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

packet  *dequeue(aqueue *q)
{
	packet		*data;
	aqueue_node	*cur;

	printf("queue size : %d\n", q->size);
	if (q->size == 0)
	{
		return (NULL);
	}
	else
	{
		data = peek(q);
		//printf("peek data addr : %p\n", data);
		if (q->head == q->tail)
		{
			//When aqueue contains only one node
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			cur = q->head;
			q->head = q->head->next;
			q->head->prev = NULL;
			free(cur->data);
			free(cur);
			cur = NULL;
		}
		q->size--;
		return (data);
	}
}
