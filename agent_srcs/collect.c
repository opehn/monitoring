#include "collect.h"
#include "agent_queue.h"

void	free_queue(aqueue *q)
{
	aqueue_node	*temp;
	aqueue_node	*head;
	head = q->head;
	while (head)
	{
		temp = head->next;
		free(head->data->payload);
		free(head->data);
		free(head);
		head = temp;
	}
}

void *collect_routine	(void *arg)
{
	aparam	*p;
	aqueue	*q;
	int		res;

	p = (aparam *)arg;
	q = p->q;
	while (1)
	{
		printf("push mem_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_mem_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
		printf("push net_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_net_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
		printf("push cpu_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_cpu_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
		printf("push proc_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_proc_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
	}
	
	return (0);
}
