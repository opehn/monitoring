#include "collect.h"

static void	serialize_cpu(char *payload)
{
	char	*res;
	int		i = 1;
	cpu_info	*c;

	c = (cpu_info *)payload;
	while (i <= 5)
	{
		res = strtok(NULL, " ");
		if (i == 1)
			c->usr = roundf(atoi(res) / sysconf(_SC_CLK_TCK));
		if (i == 3)
			c->sys = roundf(atoi(res) / sysconf(_SC_CLK_TCK));
		if (i == 4)
			c->idle = roundf(atoi(res)/ sysconf(_SC_CLK_TCK));
		if (i == 5)
			c->iowait = roundf(atoi(res) / sysconf(_SC_CLK_TCK));
		i++;
	}
}

static void	parse_cpu(char *payload)
{
	int			fd;
	char		*line;
	char		*first_word;

	fd = open("/proc/stat", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	free(get_next_line(fd, 0)); //skip first line
	while ((line = get_next_line(fd, 0)))
	{
		first_word = strtok(line, " ");
		if (!strcmp(first_word, "intr"))
		{
			free(line);
			break;
		}
		serialize_cpu(payload);
		payload += sizeof(cpu_info);
		free(line);
	}
	line = NULL;
	get_next_line(fd, 1); //free buffer
	close(fd);
}

static packet	*make_packet(int cpu_cnt)
{
	packet			*cpu_packet;
	int				packet_length = 0;
	cpu_info		*temp;
	char			*payload;

	packet_length = sizeof(packet_header) + sizeof(cpu_info) * cpu_cnt;
	cpu_packet = malloc(sizeof(packet));
	cpu_packet->payload = malloc(packet_length);
	if (!cpu_packet || !cpu_packet->payload)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	printf("get cpu payload : %p\n", cpu_packet->payload);
	cpu_packet->length = packet_length;
	payload = cpu_packet->payload;
	serialize_header(C, packet_length, AID, payload);
	payload += sizeof(packet_header);
	parse_cpu(payload);
	return (cpu_packet);
}

static int	count_cpu(void)
{
	int			fd;
	char		*line;
	char		*first_word;
	int			cpu_cnt = 0;

	fd = open("/proc/stat", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	free(get_next_line(fd, 0)); //skip first line
	while ((line = get_next_line(fd, 0)))
	{
		first_word = strtok(line, " ");
		if (!strcmp(first_word, "intr"))
		{
			free(line);
			break;
		}
		free(line);
		cpu_cnt++;
	}
	line = NULL;
	get_next_line(fd, 1); //free buffer
	close(fd);
	return (cpu_cnt);
}

packet		*get_cpu_info(void)
{
	int				cpu_cnt;

	cpu_cnt = count_cpu();
	return (make_packet(cpu_cnt));
}
