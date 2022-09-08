#include "server.h"

static int	recv_wrap(int clientfd, int logfd, char *buf, int size, int flag)
{
	int res;

	if (0 > (res = recv(clientfd, buf, size, flag)))
	{
		err_log("server rece error", logfd);
		return (-1);
	}
	else if (res == 0)
		return (-2);
	return (0);
}

void	*receive_routine(void *arg)
{
	char			*header_buf;
	char			*payload_buf;
	packet_header	*header;
	int				body_length;
	sparam			*p;
	int				res;

	p = (sparam *)arg;

	while (1)
	{
		if (!(header_buf = malloc(sizeof(packet_header))))
		{
			err_log("malloc error", p->logfd);
			return (NULL);
		}
		if (0 > recv_wrap(p->clientfd, p->logfd, header_buf, sizeof(packet_header), MSG_WAITALL))
		{
			free(header_buf);
			return (NULL);
		}
		header = (packet_header *)header_buf;
		body_length = header->length - sizeof(packet_header);
		if (!(payload_buf = malloc(body_length)))
		{
			err_log("malloc error", p->logfd);
			return (NULL);
		}
		if (0 > recv_wrap(p->clientfd, p->logfd, payload_buf, body_length, MSG_WAITALL))
		{
			printf("here\n");
			free(header_buf);
			free(payload_buf);
			return (NULL);
		}
		pthread_mutex_lock(&p->squeue_lock);
		enqueue(p->q, (packet_header *)header_buf, payload_buf);
		pthread_mutex_unlock(&p->squeue_lock);
	}
	server_logging("receive thread end", p->logfd);
	return (NULL);
}
