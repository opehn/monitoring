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
		agent_logging("push memory info", p->logfd, &p->log_lock);
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_mem_info(p->aid, p->logfd));
		pthread_mutex_unlock(&q->aqueue_lock);
		agent_logging("push network info", p->logfd, &p->log_lock);
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_net_info(p->aid, p->logfd));
		pthread_mutex_unlock(&q->aqueue_lock);
		agent_logging("push cpu info", p->logfd, &p->log_lock);
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_cpu_info(p->aid, p->logfd));
		pthread_mutex_unlock(&q->aqueue_lock);
		agent_logging("push proces info", p->logfd, &p->log_lock);
		pthread_mutex_lock(&q->aqueue_lock);
		enqueue(q, get_proc_info(p->aid, p->logfd));
		pthread_mutex_unlock(&q->aqueue_lock);
		sleep(1);
	}
	return (0);
}
