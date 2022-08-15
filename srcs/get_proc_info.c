#include "monitoring.h"

static void	parse_stat(char *content, proc_info *p)
{
	char	*res;
	int		u_time;
	int		s_time;
	int		all_time;
	int		i = 1;

	res = strtok(content, " ");
	p->pid = atoi(res);
	while (i <= 21)
	{
		res = strtok(NULL, " ");
		if (i == 1)
			p->proc_name = strdup(res);
		if (i == 3)
			p->ppid = atoi(res);
		if (i == 13)
			u_time = atoi(res);
		if (i == 14)
			s_time = atoi(res);
		if (i == 21)
			all_time = atoi(res);
		i++;
	}
	p->cpu_time = u_time + s_time;
	p->cpu_usage = p->cpu_time / all_time;
}

static void	get_username(proc_info *p, const char *path)
{
	struct stat		buf;
	struct passwd	*pw;

	if (0 > stat(path, &buf))
	{
		perror("stat error");
		exit(EXIT_FAILURE);
	}
	if((pw = getpwuid(buf.st_uid)) == NULL)
	{
		if (errno)
		{
			perror("getpwuid error");
			exit(EXIT_FAILURE);
		}
		else
		{
			fprintf(stderr, "getpwuid : can't find matching entry from userbase\n");
			exit(EXIT_FAILURE);
		}
	}
	p->user_name = strdup(pw->pw_name);	
}

static void	get_stat(proc_info *p, char *stat_path)
{
	int		fd;
	char	*content;

	fd = open(stat_path, O_RDONLY);
	if (fd < 0)
	{
		if (errno != ENOENT)
		{
			perror("file open error");
			exit(EXIT_FAILURE);
		}
		errno = 0;
	}
	content = read_file(fd);
	parse_stat(content, p);
	close(fd);
	free(content);
}

static void	get_cmdline(proc_info *p, char *cmd_path)
{
	int		fd;
	char	*content;

	fd = open(cmd_path, O_RDONLY);
	if (fd < 0)
	{
		if (errno != ENOENT)
		{
			perror("file open error");
			exit(EXIT_FAILURE);
		}
		errno = 0;
	}
	p->cmdline = read_file(fd);
	close(fd);
}

static proc_info	*init_proc(char *d_name)
{
	proc_info		*new;
	char			stat_path[1000];
	char			cmd_path[1000];
	char			dir_path[1000];

	if (!(new = (struct proc_info *)malloc(sizeof(proc_info))))
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	sprintf(dir_path, "/proc/%s", d_name);
	sprintf(stat_path, "/proc/%s/stat", d_name);
	sprintf(cmd_path, "/proc/%s/cmdline", d_name);
	get_stat(new, stat_path);
	get_username(new, dir_path);
	get_cmdline(new, cmd_path);
	new->next = NULL;
	return (new);
}

proc_info	*get_proc_info(void)
{
	proc_info		*head;
	proc_info		*cur;
	proc_info		*new;
	DIR				*cur_dir; 
	struct dirent	*cur_dir_info;

	cur_dir = opendir("/proc/");
	cur_dir_info = readdir(cur_dir);
	if (!cur_dir || !cur_dir_info)
	{
		perror("dir open err");
		exit(EXIT_FAILURE);
	}
	head = NULL;
	while (cur_dir_info != NULL)
	{
		if (isnum(cur_dir_info->d_name))
		{
			new = init_proc(cur_dir_info->d_name);
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
		}
		cur_dir_info = readdir(cur_dir);
	}
	closedir(cur_dir);
	return (head);
}
