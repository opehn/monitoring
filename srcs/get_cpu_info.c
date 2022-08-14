#include "monitoring.h"

static void		parse_line(cpu_info *c)
{
	char	*res;
	int		i = 1;

	while (i <= 5)
	{
		res = strtok(NULL, " ");
		if (i == 1)
			c->usr = atoi(res);
		if (i == 3)
			c->sys = atoi(res);
		if (i == 4)
			c->idle = atoi(res);
		if (i == 5)
			c->iowait = atoi(res);
		i++;
	}
}


static cpu_info *init_cpu(void)
{
	cpu_info	*new;

	if (!(new = (struct cpu_info *)malloc(sizeof(cpu_info))))
	{
		perror("malloc error");
		exit (EXIT_FAILURE);
	}
	parse_line(new);
	new->next = NULL;
	return (new);
}

struct cpu_info	*make_cpu(int fd)
{
	char		*line;
	char		*save;
	char		*first_word;
	cpu_info	*head;
	cpu_info	*new;
	cpu_info	*cur;

	head = NULL;
	free(get_next_line(fd)); //skip first line
	while ((line = get_next_line(fd)))
	{
		first_word = strtok(line, " ");
		if (!strcmp(first_word, "intr"))
			break;
		new = init_cpu();
		cur->next = new;
		cur = cur->next;
		if (!head)
			head = new;
		free(line);
	}
	close(fd);
	return (head);
}

cpu_info	*get_cpu_info(void)
{
	cpu_info		*head;
	int				fd;

	fd = open("/proc/stat", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	return (make_cpu(fd));
}
