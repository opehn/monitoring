#include "agent.h"

extern ashare *g_ashare;

static char			*second_word(char *line)
{
	char	*res;
	char	*temp;

	temp = NULL;
	strtok_r(line, " ", &temp);
	res = strtok_r(NULL, " ", &temp);
	return (res);
}

static void	serialize_mem(char *payload)
{
	int			fd;

	fd = open("/proc/meminfo", O_RDONLY);
	if (fd < 0)
	{
		err_log("file open error");
		exit(EXIT_FAILURE);
	}
	close(fd);
}

packet			*get_mem_info(void)
{
	packet			*mem_packet;
	int				packet_length;
	char			*payload;

	packet_length = sizeof(packet_header) + sizeof(mem_info);
	mem_packet = malloc(sizeof(packet));
	mem_packet->payload = malloc(packet_length);
	if (!mem_packet || !mem_packet->payload)
	{
		err_log("malloc error");
		exit(EXIT_FAILURE);
	}
	mem_packet->length = packet_length;
	payload = mem_packet->payload;
	serialize_header(M, packet_length, g_ashare->aid, payload);
	payload += sizeof(packet_header);
	serialize_mem(payload);
	return (mem_packet);
}
