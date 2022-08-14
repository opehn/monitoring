#include "monitoring.h"

void	print_info(proc_info **p, cpu_info **c, mem_info **m, pac_info **pc)
{
	printf("p addr : %p\n", p);
	printf("--------------------PROCESS INFO----------------------\n");
	while ((*p))
	{
		printf("name : %s\n pid : %d\n ppid : %d\n cputime : %d\n cpuusage: %f\n user_name: %s\ncmdline : %s\n",
				(*p)->proc_name, (*p)->pid, (*p)->ppid, (*p)->cpu_time, (*p)->cpu_usage, (*p)->user_name, (*p)->cmdline);
		(*p) = (*p)->next;
	}

	printf("--------------------CPU INFO----------------------\n");
	int i = 0;
	while ((*c))
	{
		printf("core %d\n usr : %d\n sys : %d\n iowait : %d\n idle : %d\n",
				i, (*c)->usr, (*c)->sys, (*c)->iowait, (*c)->idle);
		(*c) = (*c)->next;
		i++;
	}
	
	printf("--------------------PACKET INFO----------------------\n");
	i = 0;
	while(*pc)
	{
		printf("device %d\nin_cnt : %d\nin_byte : %d\nout_cnt : %d\nout_byte : %d\n",
			i, (*pc)->in_cnt, (*pc)->in_byte, (*pc)->out_cnt, (*pc)->out_byte);
		*pc = (*pc)->next;
		i++;
	}

	printf("--------------------MEMORY INFO----------------------\n");
	printf("total : %d\nfree : %d\nused : %d\nswap_used : %d\n",
			(*m)->total, (*m)->free, (*m)->used, ((*m))->swap_used);
}

int main(void)
{
	proc_info	*p;
	cpu_info	*c;
	mem_info	*m;
	pac_info	*pc;

	p = get_proc_info();
	printf("p addr : %p\n", p);
	c = get_cpu_info();
	m = get_mem_info();
	pc = get_pac_info();
	print_info(&p, &c, &m, &pc);

	return (0);
}
