#include "monitoring.h"

static void		parse_line(cpu_info *c)
{
	char	*res;
	int		i = 1;

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
	free(get_next_line(fd, 0)); //skip first line
	while ((line = get_next_line(fd, 0)))
	{
		first_word = strtok(line, " ");
		if (!strcmp(first_word, "intr"))
		{
			free(line);
			break;
		}
		new = init_cpu();
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
	get_next_line(fd, 1); //free buffer
	close(fd);
	return (head);
}

cpu_info	*get_cpu_info(void)
{
	int				fd;

	fd = open("/proc/stat", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	return (make_cpu(fd));
}
