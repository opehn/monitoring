#include "collect.h"

static char	*final_line(char **backup)
{
	int		 newline_idx;
	int		 len;
	char	*res;

	if (!backup || !*backup)
		return (NULL);
	len = strlen(*backup);
	res = my_strndup(*backup, len);
	free(*backup);
	backup = NULL;
	return (res);
}

static int	 append_backup(char **backup, char *buf)
{
	char	*res;

	res = NULL;
	if (!*backup)
		res = my_strndup(buf, strlen(buf));
	else
	{
		res = my_strjoin(*backup, buf);
		free(*backup);
	}
	*backup = res;
	return (find_newline(*backup));
}

char	*read_file(int fd)
{
	char		*backup;
	char		*buf;
	int			 read_size;

	backup = NULL;
	buf = (char *)calloc(BUFFER_SIZE, sizeof(char));
	if (!buf)
		return (NULL);
	if (fd < 0)
		return (NULL);
	while (0 < (read_size = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		append_backup(&backup, buf);
		free(buf);
		buf = (char *)calloc(BUFFER_SIZE, sizeof(char));
		if (!buf)
			return (NULL);
	}
	if (buf)
		free(buf);
	if (read_size < 0)
	{
		perror("read error");
		exit(EXIT_FAILURE);
	}
	return (final_line(&backup));
}
