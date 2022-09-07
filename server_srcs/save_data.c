#include "server.h"

char	*make_filename(int aid)
{
	printf("make_filename\n");
	time_t		cur_time; 
	struct tm*	time_struct;
	char		*file_name;
	int			year;
	int			month;
	int			day;

	if (!(file_name = malloc(sizeof(char) * 50)))
	{
		perror("malloc failed");
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

void	write_memory(int fd, squeue *q)
{
	mem_info	*m;
	char		*payload;
	char		buf[90];

	payload = q->head->payload;
	m = (mem_info *)payload;
	sprintf(buf, "MemTotal : %d MemFree : %d Used : %d SwapUsed : %d\n\n",
			m->total, m->free, m->used, m->swap_used);
	write(fd, buf, strlen(buf));
	free_shead(q);
}

void	write_network(int fd, squeue *q)
{
	net_info	*n;
	char		*payload;
	int			length;

	payload = q->head->payload;
	length = q->head->header->length - sizeof(packet_header);
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
	free_shead(q);
}

void	write_cpu(int fd, squeue *q)
{
	cpu_info	*c;
	char		*payload;
	char		*buf;
	int			length;
	char		core[15];
	int			i = 0;

	payload = q->head->payload;
	payload = q->head->payload;
	length = q->head->header->length - sizeof(packet_header);
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
	free_shead(q);
}

void	write_process(int fd, squeue *q)
{
	proc_info	*p;
	char		*payload;
	char		*buf;
	int			length;
	char		core[15];
	int			i = 0;

	payload = q->head->payload;
	payload = q->head->payload;
	length = q->head->header->length - sizeof(packet_header);
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
	free_shead(q);
}

void	write_data(int fd, squeue *q)
{
	char	type[10];

	switch (q->head->header->signature)
	{
		case 11 :
			write(fd, "MEMORY\n", 7);
			write_memory(fd, q);
			break;
		case 22 :
			write(fd, "NETWORK\n", 8);
			write_network(fd, q);
			break;
		case 33 :
			write(fd, "CPU\n", 4);
			write_cpu(fd, q);
			break;
		case 44 :
			write(fd, "PROCESS\n", 8);
			write_process(fd, q);
			write(fd, "----------------------------------------------------------------------------------------------------\n", 101);
			break;
	}
}

void	save_file(squeue *q)
{
	printf("save_file\n");
	char	*file_name;
	int		fd;

	file_name = make_filename(q->head->header->agent_id);
	if (0 > (fd = open(file_name, O_RDWR | O_APPEND | O_CREAT, S_IRWXU)))
	{
		perror("file open error");
		return ;
	}
	free(file_name);
	write_data(fd, q);
}