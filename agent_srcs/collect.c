#include "collect.h"
#include "agent_queue.h"

void *collect_routine	(void *arg)
{
	aparam	*p;
	aqueue	*q;
	int		res;

	p = (aparam *)arg;
		write(p->logfd, "hi5\n", 4);
	q = p->q;
	//while (1)
//	{
		printf("push mem_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_mem_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		write(p->logfd, "hi6\n", 4);
		sleep(1);
		printf("push net_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_net_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		write(p->logfd, "hi7\n", 4);
		sleep(1);
		printf("push cpu_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_cpu_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		write(p->logfd, "hi8\n", 4);
		sleep(1);
		printf("push proc_info\n");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_proc_info(p->logfd));
		pthread_mutex_unlock(&q->aqueue_lock);
		write(p->logfd, "hi9\n", 4);
		sleep(1);
//	}
	return (0);
}
