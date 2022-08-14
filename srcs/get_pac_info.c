#include "monitoring.h"

static void	parse_line(pac_info *p, char *line)
{
	char	*res;
	int		i = 1;

	strtok(line, " ");
	while(i <= 10)
	{
		res = strtok(NULL, " ");
		if (i == 1)
			p->in_byte = atoi(res);
		if (i == 2)
			p->in_cnt = atoi(res);
		if (i == 9)
			p->out_byte = atoi(res);
		if (i == 10)
			p->out_cnt = atoi(res);
		i++;
	}
}

static pac_info	*init_pac(char *line)
{
	pac_info	*p;

	if (!(p = (struct pac_info *)malloc(sizeof(pac_info))))
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	parse_line(p, line);
	p->next = NULL;

}

static pac_info	*make_pac(int fd)
{
	int			i = 0;
	char		*line;
	pac_info	*head;
	pac_info	*new;
	pac_info	*cur;
	
	head = NULL;
	while (i++ < 3)
		free(get_next_line(fd, 0));
	while((line = get_next_line(fd, 0)))
	{
		new = init_pac(line);
		if (!head)
		{
			head = new;
			cur = new;
		}
		else
		{
			cur->next = new;
			cur = cur->next;
		}
		free(line);
	}
	close(fd);
	return (head);
}

pac_info	*get_pac_info(void)
{
	int		fd;

	fd = open("/proc/net/dev", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	return (make_pac(fd));
}
