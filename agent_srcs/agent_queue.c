#include "agent.h"
#include "agent_queue.h"

aqueue *init_aqueue(void)
{
	aqueue *q;

	q = malloc(sizeof(aqueue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;

	return (q);
}

aqueue_node  *init_node(packet *data, aqueue_node *tail)
{
	aqueue_node  *new;

	new = malloc(sizeof(aqueue_node));
	new->data = data;
	new->prev = tail;
	new->next = NULL;
	return (new);
}

packet	 *peek(aqueue *q)
{
	int		i;

	printf("peek, size : %d\n", q->size);
	if (q->size == 0)
		return (NULL);
	else
		return (q->head->data);
}

void	enqueue (aqueue *q, packet *data)
{
	aqueue_node	*new;

	new = init_node(data, q->tail);
	if (q->head == NULL) //adding first node of aqueue
	{
		q->head = new;
	}
	else
	{
		q->tail->next = new;
		new->prev = q->tail;
	}
	q->tail = new;
	q->size += 1;
	printf("enqueue, size : %d\n", q->size);
}

aqueue_node	*dequeue(aqueue *q)
{
	aqueue_node	*temp;
	aqueue_node	*data;

	if (!q->size)
		return (NULL);
	else
	{
		data = peek(q);
		if (q->head == q->tail)
		{
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			q->head = q->head->next;
			q->head->prev = NULL;
		}
		q->size--;
	}
	return (data);
}

void	free_node(aqueue_node *cur)
{
	free(cur->data);
	free(cur);
}
