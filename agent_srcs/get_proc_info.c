#include "collect.h"

static void	parse_stat(char *payload, char *content)
{
	char			*res;
	long long int	u_time;
	long long int	s_time;
	int				all_time;
	int				i = 1;
	int				j;
	char			*buf;
	proc_info		*p;

	p = (proc_info *)payload;
	res = strtok(content, " ");
	p->pid = htonl(atoi(res));
	while (i <= 14)
	{
		j = 0;
		res = strtok(NULL, " ");
		if (i == 1)
		{
			buf = skip_bracket(res);
			memset(p->proc_name, 0, 16);
			while(j < nullguard_strlen(buf))
			{
				p->proc_name[j] = buf[j];
				j++;
			}
			free(buf);
			buf = NULL;
		}
		if (i == 3)
			p->ppid = htonl(atoi(res));
		if (i == 13)
			u_time = strtoll(res, NULL, 10); 
		if (i == 14)
			s_time = strtoll(res, NULL, 10);
		i++;
	}
	p->cpu_time = u_time + s_time; //hton64..
}

static void	get_stat(char *payload, char *stat_path)
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
	parse_stat(payload, content);
	close(fd);
	free(content);
	content = NULL;
}


static void	get_username(char *payload, const char *path)
{
	struct stat		buf;
	struct passwd	*pw;
	char			*temp;
	int				i  = 0;

	if (0 > stat(path, &buf))
	{
		char *null = "NULL";
		while (i < 4)
		{
			payload[i] = null[i];
			i++;
		}
		return;
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
	temp = strdup(pw->pw_name);	
	while(i < nullguard_strlen(temp))
	{
		payload[i] = temp[i];
		i++;
	}
	free(temp);
	temp = NULL;
}

static void	get_cmdline(char *payload, char *cmd_path)
{
	int		fd;
	char	*content;
	char	*buf;
	int		i = 0;
	
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
	buf = read_file(fd);
	while (i < nullguard_strlen(buf))
	{
		payload[i] = buf[i];
		i++;
	}
	free(buf);
	buf = NULL;
	close(fd);
}

static void	serialize_proc(char *payload, char *d_name)
{
	char			stat_path[1000];
	char			cmd_path[1000];
	char			dir_path[1000];

	sprintf(dir_path, "/proc/%s", d_name);
	sprintf(stat_path, "/proc/%s/stat", d_name);
	sprintf(cmd_path, "/proc/%s/cmdline", d_name);
	get_stat(payload, stat_path);
	payload += sizeof(proc_info) - CMD_LEN - 32;
	get_username(payload, dir_path);
	payload += 32;
	get_cmdline(payload, cmd_path);
	payload += CMD_LEN;
}

static int	*iter_dir(char *payload)
{
	DIR				*cur_dir; 
	int				cur_cmd_len;
	struct dirent	*cur_dir_info;

	cur_dir = opendir("/proc/");
	cur_dir_info = readdir(cur_dir);
	if (!cur_dir || !cur_dir_info || errno)
	{
		perror("dir open err");
		exit(EXIT_FAILURE);
	}
	while (cur_dir_info != NULL)
	{
		if (isnum(cur_dir_info->d_name))
		{
			serialize_proc(payload, cur_dir_info->d_name);
			payload += sizeof(proc_info);

		}
		cur_dir_info = readdir(cur_dir);
	}
	closedir(cur_dir);
}

static packet	*make_packet(int proc_cnt)
{
	packet			*proc_packet;
	int				packet_length;
	char			*payload;

	packet_length = sizeof(packet_header) + sizeof(proc_info) * proc_cnt;
	proc_packet = malloc(sizeof(packet));
	proc_packet->payload = malloc(packet_length);
	if(!proc_packet || !proc_packet->payload)
	{
		perror("malloc error");
		exit(EXIT_FAILURE);
	}
	proc_packet->length = packet_length;
	payload = proc_packet->payload;
	serialize_header(P, packet_length, AID, payload);
	payload += sizeof(packet_header);
	iter_dir(payload);
	
	return (proc_packet);
}

static int	count_proc(void)
{
	DIR				*cur_dir; 
	int				cur_cmd_len;
	struct dirent	*cur_dir_info;
	int				proc_cnt = 0;

	cur_dir = opendir("/proc/");
	cur_dir_info = readdir(cur_dir);
	if (!cur_dir || !cur_dir_info || errno)
	{
		perror("dir open err");
		exit(EXIT_FAILURE);
	}
	while (cur_dir_info != NULL)
	{
		if (isnum(cur_dir_info->d_name))
			proc_cnt++;
		cur_dir_info = readdir(cur_dir);
	}
	closedir(cur_dir);
	return (proc_cnt);
}


packet		*get_proc_info(void)
{
	int			proc_cnt;

	proc_cnt = count_proc();
	return (make_packet(proc_cnt));
}
