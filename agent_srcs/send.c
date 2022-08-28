#include "collect.h"
#include "queue.h"

void	*send_packet(int clientfd, queue *q, pthread_mutex_t *queue_mutex)
{
	packet	*data;

	data = NULL;
	printf("send info\n");
	printf("size : %d\n", q->size);
	pthread_mutex_lock(queue_mutex);
	if (q->size > 0)
		data = dequeue(q);
	pthread_mutex_unlock(queue_mutex);

	if (data)
	{
		if (0 > send(clientfd, data->payload, data->length, 0))
		{
			perror("send error");
			exit (EXIT_FAILURE);
		}
	}
}

void	*send_routine(void *arg)
{
	int				clientfd;
	SA_IN			serveraddr;
	int				res;
	param			*p;
	queue			*q;

	p = (param *)arg;
	q = p->q;
	printf("send param addr : %p\n", p);
	if (0 > (clientfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket open error");
		exit(EXIT_FAILURE);
	}
	memset(&serveraddr, 0, sizeof(SA_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	if (0 > (connect(clientfd, (SA *)&serveraddr, sizeof(serveraddr))))
	{
		perror("connect error");
		exit(EXIT_FAILURE);
	}
	while(1)
	{
		send_packet(clientfd, q, &p->queue_lock); //prev가 NULL일경우 에러처리
		sleep(1);
	}
	close(clientfd);
}
