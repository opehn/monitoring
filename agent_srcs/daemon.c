#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>

int main(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork failed\n");
		exit(EXIT_FAILURE);
	}
	else if (pid != 0)
		exit(0); //parent process exits
	chdir ("/");
	umask(0);
	setsid();

	//implement demon process code
	printf("strat logging");
	while (1)
	{
	}
}

