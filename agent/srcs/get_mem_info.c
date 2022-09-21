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

static void		 parse_info(char *payload, int fd)
{
	char	*line;
	int		 i = 0;
	int		 mtotal;
	int		 mfree;
	int		 stotal;
	int		 sfree;
	mem_info	*m;

	m = (mem_info *)payload;
	while(i <= 15)
	{
		line = get_next_line(fd, 0);
		if (i == 0)
			mtotal = atoi(second_word(line));
		if (i == 1)
			mfree = atoi(second_word(line));
		if (i == 14)
			stotal = atoi(second_word(line));
		if (i == 15)
			sfree = atoi(second_word(line));
		free(line);
		i++;
	}
	line = NULL;
	m->free = mfree;
	m->total = mtotal;
	m->used = mtotal - mfree;
	m->swap_used = stotal - sfree;
	get_next_line(fd, 1); //free buffer
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
	parse_info(payload, fd);
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
