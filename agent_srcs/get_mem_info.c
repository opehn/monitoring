#include "collect.h"

static char			*second_word(char *line)
{
	char	*res;
	char	*temp;

	temp = NULL;
	strtok_r(line, " ", &temp);
	res = strtok_r(NULL, " ", &temp);
	return (res);
}

static void			parse_info(char *payload, int fd)
{
	char	*line;
	int		i = 0;
	int		mtotal;
	int		mfree;
	int		stotal;
	int		sfree;
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
//	printf("collected meminfo free : %d\n", mfree);
//	printf("collected meminfo total : %d\n", mtotal);
//	printf("collected meminfo used : %d\n", mtotal - mfree);
//	printf("collected meminfo swap_used : %d\n", stotal - sfree);
	
	printf("collected meminfo free : %d\n", m->free);
	printf("collected meminfo total : %d\n", m->total);
	printf("collected meminfo used : %d\n", m->used);
	printf("collected meminfo swap_used : %d\n", m->swap_used);
	printf("---------------------------------------------------\n");
	get_next_line(fd, 1); //free buffer
}

static void	serialize_mem(char *payload)
{
	int			fd;

	fd = open("/proc/meminfo", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
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
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	mem_packet->length = packet_length;
	payload = mem_packet->payload;
	serialize_header(M, packet_length, AID, payload);
	payload += sizeof(packet_header);
	serialize_mem(payload);

	return (mem_packet);
}
