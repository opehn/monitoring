#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	DIR				*cur_dir; 
	struct dirent	*cur_dir_info;
	char			*path;

	cur_dir = opendir("/proc/");
	cur_dir_info = readdir(cur_dir);
	if (!cur_dir || !cur_dir_info)
	{
		fprintf(stderr, "dir error\n");
		exit(EXIT_FAILURE);
	}
	while (cur_dir_info != NULL)
	{
		if (isnum(cur_dir_info->d_name) == 0)
		{
			path = make_path(cur_dir_info->d_name);
			get_stat(path);
			free(path);
		}
		cur_dir_info = readdir(cur_dir);
	}
	closedir(cur_dir);
	return (0);
}

char	*make_path(char *d_name)
{
	char	*res1;
	char	*res2;

	res1 = strjoin("/proc/", d_name);
	res2 = strjoin(res1, "/stat");
	free(res1);
	return (res2);
}

int	isnum(char *nptr)
{
	int	i;

	i = 0;
	while(nptr[i])
	{
		if (isdigit(nptr[i]))
			return (1);
		i++;
	}
	return (0);
}

void	get_stat(char *path)
{
	int		fd;
	int		res;
	char	*buf;
	char	*stat;

	fd = open(path);
	while(0 <= (res = read(fd, buf, 1000)))
	{
		if (res < 0)
		{
			fprintf(stderr, "read error\n");
			exit(EXIT_FAILURE);
		}
	}
	int i = 0;
	while (stat = strtok(buf, " "))
	{
		printf("%d : %s\n", i, stat);
		stat = strtok(buf, " ");
		i++;
	}
}
