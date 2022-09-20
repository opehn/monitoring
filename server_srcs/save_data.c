#include "server.h"

extern sshare		*g_sshare;
extern data_queue	*g_dq;

char	*make_data_filename(int aid)
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

void	write_time(int fd, char *ag_time)
{
	time_t      cur = time(NULL);
    struct tm   *t = localtime(&cur);
    char        time_msg[150];
    int         msg_len;

	sprintf(time_msg, "%s collected [%02d-%02d-%02d] received\n", ag_time, t->tm_hour, t->tm_min, t->tm_sec);
    msg_len = strlen(time_msg);
    write(fd, time_msg, msg_len);
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
		sprintf(buf, "%s User : %ld System : %ld iowait : %ld idle : %ld\n",
				core, c->usr, c->sys, c->iowait, c->idle);
		write(fd, buf, strlen(buf));
		length -= sizeof(cpu_info);
		c++;
		i++;
	}
	write(fd, "\n", 1);
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
	write(fd, "\n", 1);
	free_data(cur);
}

static void	write_data(int fd, data_queue_node *cur)
{
	switch (cur->header->signature)
	{
		case 11 :
			receive_logging("Save memory data", cur->header->agent_id);
			write(fd, "MEMORY  | ", 10);
			write_time(fd, cur->header->time);
			write_memory(fd, cur);
			break;
		case 22 :
			receive_logging("Save network data", cur->header->agent_id);
			write(fd, "NETWORK | ", 10);
			write_time(fd, cur->header->time);
			write_network(fd, cur);
			break;
		case 33 :
			receive_logging("Save cpu data", cur->header->agent_id);
			write(fd, "CPU     | ", 10);
			write_time(fd, cur->header->time);
			write_cpu(fd, cur);
			break;
		case 44 :
			receive_logging("Save process data", cur->header->agent_id);
			write(fd, "PROCESS | ", 10);
			write_time(fd, cur->header->time);
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

	file_name = make_data_filename(cur->header->agent_id);
	printf("file_name : %s\n", file_name);
	if (0 > (fd = open(file_name, O_RDWR | O_APPEND | O_CREAT, S_IRWXU)))
	{
		err_log("file open error");
		return ;
	}
	free(file_name);
	write_data(fd, cur);
}
