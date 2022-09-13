#include "agent.h"
#include "agent_queue.h"

extern ashare	*g_ashare;
extern int		g_connfd;

void	send_packet(void)
{
	packet	*data;
	char	*buf;
	int		signature;
	aqueue	*q;

	q = g_ashare->q;
	data = NULL;
	if (q->size > 0)
	{
		pthread_mutex_lock(&g_ashare->aqueue_lock);
		data = dequeue(q);
		pthread_mutex_unlock(&g_ashare->aqueue_lock);
	}
	if (data)
	{
		buf = data->payload;
		if (0 > send(g_connfd, data->payload, data->length, 0))
		{
			err_log("send err");
			pause();
		}
		else
			sendinfo_log(buf);
	}
	pthread_mutex_lock(&g_ashare->aqueue_lock);
	free_node(data);
	pthread_mutex_unlock(&g_ashare->aqueue_lock);
}

void	connect_wrap(void)
{
	SA_IN			serveraddr;

	if (0 > (g_connfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		err_log("socket open error");
	}
	memset(&serveraddr, 0, sizeof(SA_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = SERVERPORT;
	if (0 > (connect(g_connfd, (SA *)&serveraddr, sizeof(serveraddr))))
	{
		err_log("connect err");
		connect_wrap();
	}
}

void	*send_routine(void *arg)
{
	int		res;

	connect_wrap();
	sleep(3);
	while(1)
	{
		send_packet();
		usleep(10000);
	}
	close(g_connfd);
}
