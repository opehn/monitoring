#include "server.h"

static int	recv_wrap(int clientfd, char *buf, int size, int flag)
{
	printf("receive byte size : %d, ", size);
	int res;

	if (0 > (res = recv(clientfd, buf, size, flag)))
	{
		perror("server recv error");
		return (-1);
	}
	else if (res == 0)
		return (-2);
	return (0);
}

void	print_deserialize(int signature, char *payload)
{
	if (signature == M)
	{
		printf(" deserialize mem info\n");
		printf("mem free : %d\n", *(uint32_t *)payload);
		payload += sizeof(uint32_t);
		printf("mem total : %d\n", *(uint32_t *)payload);
		payload += sizeof(uint32_t);
		printf("mem used : %d\n", *(uint32_t *)payload);
		payload += sizeof(uint32_t);
		printf("mem swap_used : %d\n", *(uint32_t *)payload);
		printf("----------------------------------------------\n");
	}
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
			perror("malloc error");
			return (NULL);
		}
		if (0 > recv_wrap(p->clientfd, header_buf, sizeof(packet_header), MSG_WAITALL))
		{
			printf("here\n");
			free(header_buf);
			return (NULL);
		}
		header = (packet_header *)header_buf;
		body_length = header->length - sizeof(packet_header);
		if (!(payload_buf = malloc(body_length)))
		{
			perror("malloc error");
			return (NULL);
		}
		if (0 > recv_wrap(p->clientfd, payload_buf, body_length, MSG_WAITALL))
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
	return (NULL);
}
