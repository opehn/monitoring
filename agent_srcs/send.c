#include "collect.h"
#include "agent_queue.h"

void	sendinfo_log(char *buf, int logfd, pthread_mutex_t *log_lock)
{
	packet_header	*pht;
	int		signature;
	int		length;
	char	*msg;

	msg = malloc(100);
	signature = *((uint16_t *)buf);
	buf += sizeof(uint16_t);
	length = *((uint32_t *)buf);
	if (signature == M)
		sprintf(msg, "send memory info, byte size : %d", length);
	else if (signature == N)
		sprintf(msg, "send network info, byte size : %d", length);
	else if (signature == C)
		sprintf(msg, "send cpu info, byte size : %d", length);
	else if (signature == P)
		sprintf(msg, "send process info, byte size : %d", length);
	agent_logging(msg, logfd, log_lock);
	free(msg);
}

void	err_log(char *err_type, int logfd, pthread_mutex_t *log_lock)
{
	char	*msg;

	msg = malloc(150);
	sprintf(msg, "%s : %s", err_type, strerror(errno));
	agent_logging(msg, logfd, log_lock);
	free(msg);
}

void	send_packet(int clientfd, aparam *p, pthread_mutex_t *aqueue_mutex, pthread_mutex_t *log_lock)
{
	packet	*data;
	char	*buf;
	int		signature;
	aqueue	*q = p->q;

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
		if (0 > send(clientfd, data->payload, data->length, 0))
			err_log("send err", p->logfd, log_lock);
		else
			sendinfo_log(buf, p->logfd, log_lock);
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
		err_log("connect err", p->logfd, &p->log_lock);
		close(clientfd);
		exit(EXIT_FAILURE);
	}
	sleep(5);
	int i = 0;
	while(1)
	{
		send_packet(clientfd, p, &q->aqueue_lock, &p->log_lock);
		sleep(1);
	}
	close(clientfd);
}
