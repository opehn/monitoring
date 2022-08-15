#include "monitoring.h"

static char			*second_word(char *line)
{
	char	*res;

	strtok(line, " ");
	res = strtok(NULL, " ");
	return (res);
}

static void			parse_info(mem_info *m, int fd)
{
	char	*line;
	int		i = 0;
	int		mtotal;
	int		mfree;
	int		s_total;
	int		s_free;
	
	while(i <= 15)
	{
		line = get_next_line(fd, 0);
		if (i == 0)
			mtotal = atoi(second_word(line));
		if (i == 1)
			mfree = atoi(second_word(line));
		if (i == 14)
			s_total = atoi(second_word(line));
		if (i == 15)
			s_free = atoi(second_word(line));
		free(line);
		i++;
	}
	m->free = mfree;
	m->total = mtotal;
	m->used = mtotal - mfree;
	m->swap_used = s_total - s_free;
	get_next_line(fd, 1);
}

static mem_info		*init_mem(mem_info *m)
{
	int	fd;

	fd = open("/proc/meminfo", O_RDONLY);
	if (fd < 0)
	{
		perror("file open error");
		exit(EXIT_FAILURE);
	}
	parse_info(m, fd);
	close(fd);
	return (m);
}

mem_info			*get_mem_info(void)
{
	mem_info	*m;

	if (!(m = (struct mem_info *)malloc(sizeof(mem_info))))
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	return (init_mem(m));
}
