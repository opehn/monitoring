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

void	safe_enqueue(aqueue *q, packet *p)
{
	int	res;

	pthread_mutex_lock(&q->aqueue_lock);
	res = (q->flag = 0);
	pthread_mutex_unlock(&q->aqueue_lock);

	while (q->flag == 0)
	{
		pthread_mutex_lock(&q->aqueue_lock);
		q->flag = 1;
		pthread_mutex_unlock(&q->aqueue_lock);

		enqueue(q, p);

		pthread_mutex_lock(&q->aqueue_lock);
		q->flag = 0;
		pthread_mutex_unlock(&q->aqueue_lock);
	}
}

void *collect_routine	(void *arg)
{
	aparam	*p;
	aqueue	*q;
	int		res;

	p = (aparam *)arg;
	q = p->q;
//	while (1)
//	{
		printf("push mem_info\n");
		enqueue(q, get_mem_info());
		//safe_enqueue(q, get_mem_info());
		sleep(1);
		printf("push net_info\n");
		enqueue(q, get_net_info());
		//safe_enqueue(q, get_net_info());
		sleep(1);
		printf("push proc_info\n");
		//safe_enqueue(q, get_proc_info());
		enqueue(q, get_proc_info());
		sleep(1);
		printf("push cpu_info\n");
		enqueue(q, get_cpu_info());
		//safe_enqueue(q, get_cpu_info());
		sleep(1);
//	}
	
//	free_queue(q);
	return (0);
}
