#include "monitoring.h"

void	print_info(proc_info *p, cpu_info *c)
{
/*	while (p)
	{
		printf("name : %s\n pid : %d\n ppid : %d\n cputime : %d\n cpuusage: %f\n cmdline : %s\n",
				p->proc_name, p->pid, p->ppid, p->cpu_time, p->cpu_usage, p->cmdline);
		p = p->next;
	}

	while (c)
	{
		printf("core %d\n usr : %d\n sys : %d\n iowait : %d\n idle : %d\n",
				i, c->usr, c->sys, c->iowait, c->idle);
		c = c->next;
		i++;
	}*/
}

int main(void)
{
	proc_info	*p;
	cpu_info	*c;

	p = get_proc_info();
	c = get_cpu_info();
	int i = 0;
	return (0);
}
