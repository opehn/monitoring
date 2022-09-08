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
	sprintf(file_name, "./agent_log/%d-%02d-%02d", year, month, day);
	return (file_name);
}

static int daemonize(void)
{
	pid_t	pid = 0;
	int		fd;
	int		logfd;
	char	*filename;
	int		pid_fd;

	pid = fork();

	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	if (setsid() < 0) {
		exit(EXIT_FAILURE);
	}

	signal(SIGCHLD, SIG_IGN);

	pid = fork();

	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	umask(0);

	signal(SIGHUP, SIG_IGN);

	close(0);
	close(1);
	close(2);
	fopen("/dev/null", "r");
	fopen("/dev/null", "w+");
	fopen("/dev/null", "w+");

	char str[256];
	pid_fd = open("./agent_log/daemon_pid", O_RDWR|O_CREAT | O_NOCTTY, 0640);
	if (pid_fd < 0) {
		exit(EXIT_FAILURE);
	}
	sprintf(str, "%d\n", getpid());
	write(pid_fd, str, strlen(str));

	filename = make_filename();
	if (!(logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		perror("file open error");
		free(filename);
		exit(EXIT_FAILURE);
	}
	free(filename);

	return (logfd);
}

static void	set_signal(void)
{
	struct sigaction act;

	act.sa_handler = SIG_IGN;
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);	

}

static aparam *init_param(int logfd)
{
	aparam			*p;

	p = malloc(sizeof (aparam));
	p->q = init_aqueue();
	pthread_mutex_init(&p->q->aqueue_lock, NULL);
	pthread_mutex_init(&p->log_lock, NULL);
	p->logfd = logfd;
	return (p);
}

int main(void)
{
	pthread_t		collect_tid;
	pthread_t		send_tid;
	aparam			*p;
	aqueue			*q;
	int				logfd;
	char			*filename;

	logfd = daemonize();
	p = init_param(logfd);
	set_signal();
	pthread_create(&collect_tid, NULL, collect_routine, (void *)p);
	agent_logging(logfd, &p->log_lock, "collect thread created");
	pthread_create(&send_tid, NULL, send_routine, (void *)p);
	agent_logging(logfd, &p->log_lock, "send thread created");

	pthread_join(collect_tid, NULL);
	pthread_join(send_tid, NULL);

	pthread_mutex_destroy(&p->q->aqueue_lock);
	free_queue(p->q);
	free(p);
	close(logfd);
	return (0);
}
