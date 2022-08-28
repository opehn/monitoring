#include "collect.h"
#include "queue.h"

void	free_queue(queue *q)
{
	queue_node	*temp;
	queue_node	*head;
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
	param	*p;
	queue	*q;

	p = (param *)arg;
	printf("collect param addr : %p\n", p);
	q = p->q;
//	while (1)
//	{
		printf("push mem_info\n");
		pthread_mutex_lock(&p->queue_lock);
		enqueue(q, get_mem_info());
		pthread_mutex_unlock(&p->queue_lock);
		sleep(1);
		printf("push net_info\n");
		pthread_mutex_lock(&p->queue_lock);
		enqueue(q, get_net_info());
		pthread_mutex_unlock(&p->queue_lock);
		sleep(1);
		printf("push cpu_info\n");
		pthread_mutex_lock(&p->queue_lock);
		enqueue(q, get_cpu_info());
		pthread_mutex_unlock(&p->queue_lock);
		sleep(1);
		printf("push proc_info\n");
		pthread_mutex_lock(&p->queue_lock);
		enqueue(q, get_proc_info());
		pthread_mutex_unlock(&p->queue_lock);
		sleep(1);
//	}
	
	free_queue(q);
	return (0);
}
