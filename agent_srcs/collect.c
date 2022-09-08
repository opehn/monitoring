#include "collect.h"
#include "agent_queue.h"

void *collect_routine	(void *arg)
{
	aparam	*p;
	aqueue	*q;
	int		res;

	p = (aparam *)arg;
	q = p->q;
	while (1)
	{
		logging(p->logfd, "push mem_info");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_mem_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
		logging(p->logfd, "push net_info");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_net_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
		logging(p->logfd, "push cpu_info");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_cpu_info());
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
		logging(p->logfd, "push proc_info");
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_proc_info(p->logfd));
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
	}
	return (0);
}
