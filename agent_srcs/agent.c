#include "collect.h"
#include "queue.h"

int main(void)
{
	pthread_t	collect_tid;
	pthread_t	send_tid;
	param		*p;
	queue		*q;
	
	p = malloc(sizeof (param));
	printf("main param addr : %p\n", p);
	p->q = init_queue();
	if (pthread_mutex_init(&p->queue_lock, NULL))
	{
		perror("metex init error");
		exit (EXIT_FAILURE);
	}
	pthread_create(&collect_tid, NULL, collect_routine, (void *)p);
	pthread_create(&send_tid, NULL, send_routine, (void *)p);

	pthread_join(collect_tid, NULL);
	pthread_join(send_tid, NULL);

	pthread_mutex_destroy(&p->queue_lock);
	free(p->q);
	free(p);
	return (0);
}
