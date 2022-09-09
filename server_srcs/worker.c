#include "server.h"

static int	  check_signature(int signature)
{
	int signature_arr[4] = {11, 22, 33, 44};
	int i = 0;
	printf("signature : %d\n", signature);

	while (i < 4)
	{
		if (signature == signature_arr[i])
			return (1);
		i++;
	}
	return (0);
}

void	*worker_routine(void *args)
{
	sparam		*p;
	squeue_node	*cur;
	int			res;

	p = (sparam *)args;
	while (1)	
	{
		if (p->clientfd)
			break;
		usleep(10000);
	}
	server_logging("worker thread start working", p->logfd, &p->log_lock);
	while (1)
	{
		while (1)
		{
			pthread_mutex_lock(&p->squeue_lock);
			res = (p->flag == 0);
			pthread_mutex_unlock(&p->squeue_lock);
			if (res)
			{
				pthread_mutex_lock(&p->squeue_lock);
				p->flag = 1;
				pthread_mutex_unlock(&p->squeue_lock);
				break;
			}
			usleep(10000);
		}
		cur = peek(p->q);
		if (cur)
		{
			if (check_signature(cur->header->signature))
				save_file(p);
		}
		pthread_mutex_lock(&p->squeue_lock);
		p->flag = 0;
		pthread_mutex_unlock(&p->squeue_lock);
		sleep(1);
	}
	server_logging("worker thread end", p->logfd, &p->log_lock);
}
