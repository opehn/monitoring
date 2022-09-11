#include "agent_queue.h"
#include "agent.h"

extern ashare	*g_ashare;

void *collect_routine(void *arg)
{
	int		res;

	while (1)
	{
		agent_logging("push memory info");
		pthread_mutex_lock(&g_ashare->aqueue_lock);
		enqueue(g_ashare->q, get_mem_info());
		pthread_mutex_unlock(&g_ashare->aqueue_lock);
		agent_logging("push network info");
		pthread_mutex_lock(&g_ashare->aqueue_lock);
		enqueue(g_ashare->q, get_net_info());
		pthread_mutex_unlock(&g_ashare->aqueue_lock);
		agent_logging("push cpu info");
		pthread_mutex_lock(&g_ashare->aqueue_lock);
		enqueue(g_ashare->q, get_cpu_info());
		pthread_mutex_unlock(&g_ashare->aqueue_lock);
		agent_logging("push proces info");
		pthread_mutex_lock(&g_ashare->aqueue_lock);
		enqueue(g_ashare->q, get_proc_info());
		pthread_mutex_unlock(&g_ashare->aqueue_lock);
		sleep(1);
	}
	return (0);
}
