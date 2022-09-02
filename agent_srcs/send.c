#include "collect.h"
#include "agent_queue.h"

void	print_sendinfo(int signature, char *buf)
{
	if (signature == 11)
	{
		printf("send mem info free : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("send mem info total : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("send mem info used : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("send mem info swap_used : %d\n", *(uint32_t *)buf); buf += sizeof(uint32_t); printf("-------------------------------------------\n");
	}

	if (signature == 22)
	{
		printf("send net info in_cnt : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("send net info out_cnt : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("send net info in_byte : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("send net info out_byte : %d\n", *(uint32_t *)buf);
		buf += sizeof(uint32_t);
		printf("-------------------------------------------\n");
	}
}

int	print_sendheader(char *buf)
{
	int	signature;

	signature = *((uint16_t *) buf);
	if (signature == M)
		printf("send mem_info\n");
	else if (signature == N)
		printf("send net_info\n");
	else if (signature == C)
		printf("send cpu_info\n");
	else if (signature == P)
		printf("send proc_info\n");
	printf("send header signature : %d\n", signature);
	buf += sizeof(uint16_t);
	printf("send header length : %d\n", *(uint32_t *)buf);
	buf += sizeof(uint32_t);
	printf("send header aid : %d\n", *(uint16_t *)buf);
	buf += sizeof(uint16_t);
	printf("\n");
	return (signature);
}

void	send_packet(int clientfd, aqueue *q, pthread_mutex_t *aqueue_mutex)
{
	printf("send_packet, size : %d\n", q->size);
	packet	*data;
	char	*buf;
	int		signature;

	data = NULL;
	if (q->size > 0)
	{
		pthread_mutex_lock(&q->aqueue_lock);
		data = peek(q);
		pthread_mutex_unlock(&q->aqueue_lock);
	}
	if (data)
	{
		buf = data->payload;
		signature = print_sendheader(buf);
		buf += sizeof(packet_header);
		print_sendinfo(signature, buf);
		if (0 > send(clientfd, data->payload, data->length, 0))
		{
			perror("send error");
			free_head(q);
			exit (EXIT_FAILURE);
		}
	}
	pthread_mutex_lock(&q->aqueue_lock);
	free_head(q);
	pthread_mutex_unlock(&q->aqueue_lock);
}

void	*send_routine(void *arg)
{
	int				clientfd;
	SA_IN			serveraddr;
	int				res;
	aparam			*p;
	aqueue			*q;

	p = (aparam *)arg;
	q = p->q;
	if (0 > (clientfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket open error");
		exit(EXIT_FAILURE);
	}
	memset(&serveraddr, 0, sizeof(SA_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = SERVERPORT;
	if (0 > (connect(clientfd, (SA *)&serveraddr, sizeof(serveraddr))))
	{
		perror("connect error");
		exit(EXIT_FAILURE);
	}
	sleep(5);
	while(1)
	{
		send_packet(clientfd, q, &q->aqueue_lock); //prev가 NULL일경우 에러처리
		sleep(2);
	}
	close(clientfd);
}
