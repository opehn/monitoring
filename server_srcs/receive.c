#include "server.h"
#include "server_queue.h"
#include "packet.h"

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

static int		check_signature(int signature)
{
	int	signature_arr[4] = {11, 22, 33, 44};
	int	i = 0;

	while (i < 4)
	{
		if (signature = signature_arr[i])
			return (1);
	}
	return (0);
}

static packet_header		*deserialize_header(char *buf)
{
	printf("deserialize_header\n");
	packet_header	*header;

	header = malloc(sizeof(packet_header));

	header->signature = *(uint16_t *)buf;
	buf += sizeof(uint16_t);
	if (!check_signature(header->signature))
	{
		perror("invalid packet signature");
		return (NULL);
	}
	header->length = *(uint32_t *)buf;
	buf += sizeof(uint32_t);
	header->agent_id = *(uint16_t *)buf;
	printf("signature : %d\n", header->signature);
	printf("length : %d\n", header->length);
	printf("agent_id : %d\n", header->agent_id);
	return (header);
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
	sparam			*p;
	int				res;

	header_buf = malloc(sizeof(packet_header));
	p = (sparam *)arg;

	while (1)
	{
		if (0 > recv_wrap(p->clientfd, header_buf, sizeof(packet_header), MSG_WAITALL))
			break;
		header = deserialize_header(header_buf);
		if (!header)
			break;
		payload_buf = malloc(header->length);
		if (0 > recv_wrap(p->clientfd, payload_buf, header->length - sizeof(packet_header), MSG_WAITALL))
		{
			free(payload_buf);
			payload_buf = NULL;
			break;
		}
		print_deserialize(header->signature, payload_buf);
		enqueue(p->q, header, payload_buf);
	}
}
