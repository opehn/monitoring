#include "server.h"
#include "server_queue.h"
#include "packet.h"

static int	recv_wrap(int serverfd, char *buf, int size, int flag)
{
	printf("receive byte size : %d, ", size);
	int res;

	if (0 > (res = recv(serverfd, buf, size, flag)))
	{
		perror("server recv error");
		return (-1);
	}
	else if (res == 0)
		return (-2);
	return (0);

}

static packet_header		*deserialize_header(char *buf)
{
	printf("deserialize_header\n");
	packet_header	*header;
//	packet_header	*temp;
	char			*temp;

	header = malloc(sizeof(packet_header));
	temp = buf;
 //   temp = (packet_header *)buf;
	/*header->signature = ntohs(temp->signature);
    header->length = ntohl(temp->signature);
    header->agent_id = ntohl(temp->agent_id);*/

	header->signature = ntohs(*(uint16_t *)buf);
	buf += sizeof(uint16_t);
	header->length = ntohl(*(uint32_t *)buf);
	buf += sizeof(uint32_t);
	header->agent_id = ntohs(*(uint16_t *)buf);
	printf("signature : %d\n", header->signature);
	printf("length : %d\n", header->length);
	printf("agent_id : %d\n", header->agent_id);
//	free(temp);
//	temp = NULL;
	return (header);
}

void	print_deserialize(int signature, char *payload)
{
	if (signature == M)
	{
		printf(" deserialize mem info\n");
		printf("mem free : %d\n", ntohl(*(uint32_t *)payload));
		payload += sizeof(uint32_t);
		printf("mem total : %d\n", ntohl(*(uint32_t *)payload));
		payload += sizeof(uint32_t);
		printf("mem used : %d\n", ntohl(*(uint32_t *)payload));
		payload += sizeof(uint32_t);
		printf("mem swap_used : %d\n", ntohl(*(uint32_t *)payload));
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
		if (0 > recv_wrap(p->serverfd, header_buf, sizeof(packet_header), MSG_WAITALL))
			break;
		header = deserialize_header(header_buf);
		payload_buf = malloc(header->length);
		if (0 > recv_wrap(p->serverfd, payload_buf, header->length - sizeof(packet_header), MSG_WAITALL))
		{
			free(payload_buf);
			payload_buf = NULL;
			break;
		}
		print_deserialize(header->signature, payload_buf);
		enqueue(p->q, header, payload_buf);
	}
}
