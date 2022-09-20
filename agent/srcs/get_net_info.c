#include "agent.h"

extern ashare	*g_ashare;

static void	parse_net(char *line, char *payload)
{
	char		*res; 
	char		*buf;
	int			i = 1;
	int			j = 0;
	net_info	*n;
	char		*temp;

	temp = NULL;
	n = (net_info *)payload;
	res = strtok_r(line, " ", &temp);
	buf = strndup(res, strlen(res) - 1); //ignore colon 
	memset(n->net_name, 0, 16);
	while(j < nullguard_strlen(buf))
	{
		n->net_name[j] = buf[j];
		j++;
	}
	free(buf);
	buf = NULL;
	while(i <= 10)
	{
		res = strtok_r(NULL, " ", &temp);
		if (i == 1)
			n->in_byte = strtoll(res, NULL, 10);
		if (i == 2)
			n->in_cnt = strtoll(res, NULL, 10);
		if (i == 9)
			n->out_byte = strtoll(res, NULL, 10);
		if (i == 10)
			n->out_cnt = strtoll(res, NULL, 10);
		i++;
	}
}

static void	serialize_net(char *payload)
{
	int			fd;
	int			i = 0;
	char		*line;
	
	fd = open("/proc/net/dev", O_RDONLY);
	if (fd < 0)
	{
		err_log("file open error");
		exit(EXIT_FAILURE);
	}
	while (i++ < 3)
		free(get_next_line(fd, 0)); //skip line
	while((line = get_next_line(fd, 0)))
	{
		parse_net(line, payload);
		payload += sizeof(net_info);
		free(line);
	}
	line = NULL;
	get_next_line(fd, 1); //free buf
	close(fd);
}

static packet	*make_packet(int net_cnt)
{
	packet		*net_packet;
	int			packet_length = 0;
	char		*payload;

	packet_length = sizeof(packet_header) + sizeof(net_info) * net_cnt;
	net_packet = malloc(sizeof(packet));
	net_packet->payload = malloc(packet_length);
	if (!net_packet || !net_packet->payload)
	{
		err_log("malloc error");
		exit(EXIT_FAILURE);
	}
	net_packet->length = packet_length;
	payload = net_packet->payload;
	serialize_header(N, packet_length, g_ashare->aid, payload);
	payload += sizeof(packet_header);
	serialize_net(payload);

	return (net_packet);
}

static int	count_net(void)
{
	int			fd;
	int			net_cnt = 0;
	int			i = 0;
	char		*line;
	
	fd = open("/proc/net/dev", O_RDONLY);
	if (fd < 0)
	{
		err_log("file open error");
		exit(EXIT_FAILURE);
	}
	while (i++ < 3)
		free(get_next_line(fd, 0)); //skip line
	while((line = get_next_line(fd, 0)))
	{
		free(line);
		net_cnt++;
	}
	line = NULL;
	get_next_line(fd, 1); //free buf
	close(fd);
	return (net_cnt);
}

packet	*get_net_info(void)
{
	int		net_cnt;

	net_cnt = count_net();
	return (make_packet(net_cnt));
}
