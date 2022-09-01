#include "collect.h"
#include "agent_queue.h"

aqueue *init_aqueue(void)
{
	aqueue *q;

	q = malloc(sizeof(aqueue));
	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	if (pthread_mutex_init(&q->aqueue_lock, NULL))
    {
        perror("mutex init error");
        exit (EXIT_FAILURE);
    }

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

void	free_head(aqueue *q)
{
	packet		*data;
	aqueue_node	*temp;

	if (!q->size)
		return ;
	else
	{
		if (q->head == q->tail)
		{
			free(q->head->data);
			free(q->head);
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			temp = q->head;
			q->head = q->head->next;
			q->head->prev = NULL;
			free(temp->data);
			free(temp);
		}
		q->size--;
	}
}
