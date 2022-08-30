#include "collect.h"
#include "agent_queue.h"

aqueue *init_aqueue(void)
{
	aqueue *q;

	q = malloc(sizeof(aqueue));
	q->head = NULL;
	q->tail = NULL;
	q->flag = 0;
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
	packet	*res;

	if (q->size == 0)
		return (NULL);
	else
	{
		res = malloc(sizeof(packet));
		res->length = q->head->data->length;
		res->payload = malloc(sizeof(res->length));
		memcpy(res->payload, q->head->data->payload, res->length);
	}
		return (res);
}

void	enqueue (aqueue *q, packet *data)
{
	aqueue_node	*new;

//	printf("enaqueue payload addr : %p\n", data->payload);
	new = init_node(data, q->tail);
	//printf("new node addr : %p\n", new);
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
}

packet  *dequeue(aqueue *q)
{
	packet		*copied_data;
	aqueue_node	*temp;

	printf("queue size : %d\n", q->size);
	if (q->size == 0)
	{
		return (NULL);
	}
	else
	{
		//printf("peek data addr : %p\n", data);
		if (q->head == q->tail)
		{
			//When aqueue contains only one node
			copied_data = peek(q);
			free(q->head->data);
			free(q->head);
			q->tail = NULL;
			q->head = NULL;
		}
		else
		{
			//printf("head addr : %p\n", q->head);
		//	printf("head next : %p\n", q->head->next);
		//	printf("head prev : %p\n", q->head->prev);
			copied_data = peek(q);
			temp = q->head;
			q->head = q->head->next;
			q->head->prev = NULL;
			free(temp->data);
			free(temp);
		}
		q->size--;
		return (copied_data);
	}
}
