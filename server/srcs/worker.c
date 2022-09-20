#include "server.h"

extern sshare		*g_sshare;
extern data_queue	*g_dq;

static int	  check_signature(int signature)
{
	int signature_arr[4] = {11, 22, 33, 44};
	int	i;

	while (i < 4)
	{
		if (signature == signature_arr[i])
			return (1);
		i++;
	}
	server_logging("Invalid signature\n");
	return (0);
}

void	*worker_routine(void *args)
{
	data_queue_node	*cur;
	
	args = (int *)args;

	pthread_mutex_lock(&g_sshare->cond_lock);
	pthread_cond_wait(&g_sshare->cond, &g_sshare->cond_lock);
	pthread_mutex_unlock(&g_sshare->cond_lock);
	server_logging("worker thread start working");

	while (1)
	{
		pthread_mutex_lock(&g_sshare->dq_lock);
		cur = dequeue(g_dq);
		pthread_mutex_unlock(&g_sshare->dq_lock);
		if (cur)
		{
			if (check_signature(cur->header->signature))
				save_file(cur);
		}
	}
	server_logging("worker thread destroied");
}
