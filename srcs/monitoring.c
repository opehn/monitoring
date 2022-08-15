#include "monitoring.h"

void	print_info(proc_info **p, cpu_info **c, mem_info **m, pac_info **pc)
{
	printf("p addr : %p\n", p);
	printf("--------------------PROCESS INFO----------------------\n");
	while ((*p))
	{
		printf("name : %s\n pid : %d\n ppid : %d\n cputime : %d\n user_name: %s\ncmdline : %s\n",
				(*p)->proc_name, (*p)->pid, (*p)->ppid, (*p)->cpu_time,(*p)->user_name, (*p)->cmdline);
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
		printf("name : %s\nin_cnt : %d\nin_byte : %d\nout_cnt : %d\nout_byte : %d\n",
			(*pc)->net_name, (*pc)->in_cnt, (*pc)->in_byte, (*pc)->out_cnt, (*pc)->out_byte);
		*pc = (*pc)->next;
		i++;
	}

	printf("--------------------MEMORY INFO----------------------\n");
	if (*m)
	{
	printf("total : %d\nfree : %d\nused : %d\nswap_used : %d\n",
			(*m)->total, (*m)->free, (*m)->used, ((*m))->swap_used);
	}
}

void	free_info(proc_info **p, cpu_info **c, mem_info **m, pac_info **pc)
{
	proc_info	*ptemp;
	cpu_info	*ctemp;
	mem_info	*mtemp;
	pac_info	*pctemp;

	while(*p)
	{
		free((*p)->proc_name);
		free((*p)->user_name);
		free((*p)->cmdline);
		ptemp = (*p)->next;
		free(*p);
		*p = ptemp;
	}
	while(*c)
	{
		ctemp = (*c)->next;
		free(*c);
		*c = ctemp;
	}
	while(*pc)
	{
		pctemp = (*pc)->next;
		free((*pc)->net_name);
		free(*pc);
		*pc = pctemp;
	}
	free(*m);
	*m = NULL;
}

int main(void)
{
	proc_info	*p, *ptemp;
	cpu_info	*c, *ctemp;
	mem_info	*m, *mtemp;
	pac_info	*pc, *pctemp;

	while(i)
	{
		c = get_cpu_info();
		p = get_proc_info();
		m = get_mem_info();
		pc = get_pac_info();

		ptemp = p;
		ctemp = c;
		mtemp = m;
		pctemp = pc;

		print_info(&ptemp, &ctemp, &mtemp, &pctemp);
		free_info(&p, &c, &m, &pc);
		usleep(1000);
	}

	return (0);
}
