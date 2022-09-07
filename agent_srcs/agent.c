#include "collect.h"
#include "agent_queue.h"

static void	free_queue(aqueue *q)
{
	aqueue_node *temp;
	aqueue_node *head;
	head = q->head;
	int i = 0;
	while (head)
	{
		temp = head->next;
		free(head->data->payload);
		free(head->data);
		free(head);
		head = temp;
		i++;
	}
	free(q);
}

static char	*make_filename(void)
{
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
	sprintf(file_name, "./log/%d-%02d-%02d", year, month, day);
	return (file_name);
}

static int daemonize(void)
{
	pid_t	pid = 0;
	int		fd;
	int		logfd;
	char	*filename;
	int		pid_fd;

	/* Fork off the parent process */
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	/* Success: Let the parent terminate */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* On success: The child process becomes session leader */
	if (setsid() < 0) {
		exit(EXIT_FAILURE);
	}

	/* Ignore signal sent from child to parent process */
//	signal(SIGCHLD, SIG_IGN);

	/* Fork off for the second time*/
//	pid = fork();

	/* An error occurred */
//	if (pid < 0) {
//		exit(EXIT_FAILURE);
//	}

	/* Success: Let the parent terminate */
	//if (pid > 0) {
	//	exit(EXIT_SUCCESS);
	//}

	/* Set new file permissions */
//	umask(0);
	pid_fd = open("./log/daemon_pid", O_RDWR|O_CREAT | O_NOCTTY, 0640);
	filename = make_filename();
	if (!(logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		perror("file open error");
		free(filename);
		exit(EXIT_FAILURE);
	}
	write(logfd, "hi\n", 3);

	signal(SIGHUP, SIG_IGN);
	/* Close all open file descriptors */
		close(0);
		close(1);
		close(2);

	write(logfd, "hi2\n", 3);
	char str[256];
	if (pid_fd < 0) {
		/* Can't open lockfile */
		exit(EXIT_FAILURE);
	}
	if (lockf(pid_fd, F_TLOCK, 0) < 0) {
		/* Can't lock file */
		exit(EXIT_FAILURE);
	}
	/* Get current PID */
	sprintf(str, "%d\n", getpid());
	/* Write PID to lockfile */
	write(pid_fd, str, strlen(str));

	free(filename);
	return (logfd);
}

int main(void)
{
	pthread_t		collect_tid;
	pthread_t		send_tid;
	aparam			*p;
	aqueue			*q;
	struct sigaction act;
	int				logfd;
	char			*filename;

	logfd = daemonize();
	filename = make_filename();
/*	if (!(logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		perror("file open error");
		free(filename);
		exit(EXIT_FAILURE);
	}
	write(logfd, "hi3\n", 3);*/
	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);	
	p = malloc(sizeof (aparam));
	p->q = init_aqueue();
	p->logfd = logfd;
	write(logfd, "hi4\n", 3);
	pthread_create(&collect_tid, NULL, collect_routine, (void *)p);
	pthread_create(&send_tid, NULL, send_routine, (void *)p);

	pthread_join(collect_tid, NULL);
	pthread_join(send_tid, NULL);

	write(logfd, "hi5\n", 3);
	pthread_mutex_destroy(&p->q->aqueue_lock);
	free_queue(p->q);
	free(p);
	close(logfd);
	return (0);
}
