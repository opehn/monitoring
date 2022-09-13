#include "agent.h"
#include "agent_queue.h"

extern ashare	*g_ashare;

void	send_packet(int connfd)
{
	aqueue_node	*cur;
	char		*buf;
	int			signature;
	aqueue		*q;

	q = g_ashare->q;
	cur = NULL;
	if (q->size > 0)
	{
		pthread_mutex_lock(&g_ashare->aqueue_lock);
		cur = dequeue(q);
		pthread_mutex_unlock(&g_ashare->aqueue_lock);
	}
	if (cur)
	{
		buf = cur->data->payload;
		if (0 > send(connfd, cur->data->payload, cur->data->length, 0))
		{
			err_log("send err");
			pause();
		}
		else
			sendinfo_log(buf);
		free_node(cur);
	}
}

int	connect_wrap(void)
{
	int				connfd;
	SA_IN			serveraddr;

	if (0 > (connfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		err_log("socket open error");
	}
	memset(&serveraddr, 0, sizeof(SA_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = SERVERPORT;
	if (0 > (connect(connfd, (SA *)&serveraddr, sizeof(serveraddr))))
	{
		err_log("connect err");
		close(connfd);
		//connect();
	}
	return (connfd);
}

void	*send_routine(void *arg)
{
	int		res;
	int		connfd;

	connfd = connect_wrap();
	sleep(3);
	while(1)
	{
		send_packet(connfd);
		usleep(10000);
	}
	close(connfd);
}
