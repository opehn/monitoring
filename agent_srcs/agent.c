#include "collect.h"
#include "agent_queue.h"

int main(void)
{
	pthread_t	collect_tid;
	pthread_t	send_tid;
	aparam		*p;
	aqueue		*q;
	
	p = malloc(sizeof (aparam));
	p->q = init_aqueue();
	pthread_create(&collect_tid, NULL, collect_routine, (void *)p);
	pthread_create(&send_tid, NULL, send_routine, (void *)p);

	pthread_join(collect_tid, NULL);
	pthread_join(send_tid, NULL);

	pthread_mutex_destroy(&q->aqueue_lock);
	free(p->q);
	free(p);
	return (0);
}
