#include "server.h"

extern sshare		*g_sshare;
extern data_queue	*g_dq;

char	*make_filename(int aid)
{
	time_t		cur_time; 
	struct tm*	time_struct;
	char		*file_name;
	int			year;
	int			month;
	int			day;

	if (!(file_name = malloc(sizeof(char) * 50)))
	{
		err_log("malloc failed");
		return (NULL);
	}
	cur_time = time(NULL);
	time_struct = localtime(&cur_time);

	year = time_struct->tm_year + 1900;
	month = time_struct->tm_mon + 1;
	day = time_struct->tm_mday;
	sprintf(file_name, "./data/%d_%d-%02d-%02d", aid, year, month, day);
	return (file_name);
}

void	write_memory(int fd, data_queue_node *cur)
{
	mem_info	*m;
	char		*payload;
	char		buf[90];

	payload = cur->payload;
	m = (mem_info *)payload;
	sprintf(buf, "MemTotal : %d MemFree : %d Used : %d SwapUsed : %d\n\n",
			m->total, m->free, m->used, m->swap_used);
	write(fd, buf, strlen(buf));
	free_data(cur);
}

void	write_network(int fd, data_queue_node *cur)
{
	net_info	*n;
	char		*payload;
	int			length;

	payload = cur->payload;
	length = cur->header->length - sizeof(packet_header);
	n = (net_info *)payload;
	while(length)
	{
		char	buf[200];
		sprintf(buf, "%s Received bytes : %ld Received cnt : %ld Transmit bytes : %ld Transmit cnts : %ld\n\n",
			n->net_name, n->in_byte, n->in_cnt, n->out_cnt, n->out_byte);
		write(fd, buf, strlen(buf));
		length -= sizeof(net_info);
		n++;
	}
	free_data(cur);
}

void	write_cpu(int fd, data_queue_node *cur)
{
	cpu_info	*c;
	char		*payload;
	char		*buf;
	int			length;
	char		core[15];
	int			i = 0;

	payload = cur->payload;
	payload = cur->payload;
	length = cur->header->length - sizeof(packet_header);
	c = (cpu_info *)payload;
	while(length)
	{
		sprintf(core, "cpu%d", i);
		char	buf[200];
		sprintf(buf, "%s User : %ld System : %ld iowait : %ld idle : %ld\n\n",
				core, c->usr, c->sys, c->iowait, c->idle);
		write(fd, buf, strlen(buf));
		length -= sizeof(cpu_info);
		c++;
		i++;
	}
	free_data(cur);
}

void	write_process(int fd, data_queue_node *cur)
{
	proc_info	*p;
	char		*payload;
	char		*buf;
	int			length;
	char		core[15];
	int			i = 0;

	payload = cur->payload;
	payload = cur->payload;
	length = cur->header->length - sizeof(packet_header);
	p = (proc_info *)payload;
	while(length)
	{
		char	buf[300];
		sprintf(buf, "%s\npid : %d ppid : %d User name : %s CPU time : %ld Cmdline : %s\n",
				p->proc_name, p->pid, p->ppid, p->user_name, p->cpu_time, p->cmdline);
		write(fd, buf, strlen(buf));
		length -= sizeof(proc_info);
		p++;
		i++;
	}
	free_data(cur);
}

static void	write_data(int fd, data_queue_node *cur)
{
	char	type[10];

	switch (cur->header->signature)
	{
		case 11 :
			server_logging("save memory data");
			write(fd, "MEMORY\n", 7);
			write_memory(fd, cur);
			break;
		case 22 :
			server_logging("save network data");
			write(fd, "NETWORK\n", 8);
			write_network(fd, cur);
			break;
		case 33 :
			server_logging("save cpu data");
			write(fd, "CPU\n", 4);
			write_cpu(fd, cur);
			break;
		case 44 :
			server_logging("save process data");
			write(fd, "PROCESS\n", 8);
			write_process(fd, cur);
			write(fd, "----------------------------------------------------------------------------------------------------\n", 101);
			break;
	}
}

void	save_file(data_queue_node *cur)
{
	printf("save_file\n");
	char	*file_name;
	int		fd;

	file_name = make_filename(cur->header->agent_id);
	printf("file_name : %s\n", file_name);
	if (0 > (fd = open(file_name, O_RDWR | O_APPEND | O_CREAT, S_IRWXU)))
	{
		err_log("file open error");
		return ;
	}
	free(file_name);
	write_data(fd, cur);
}
