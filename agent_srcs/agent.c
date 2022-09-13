#include "agent_queue.h"
#include "agent.h"

ashare	*g_ashare;
int		g_connfd;

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

static char	*make_filename(int aid)
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
	sprintf(file_name, "./agent_log/%d_%d-%02d-%02d", aid, year, month, day);
	return (file_name);
}

static int daemonize(int aid)
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
	fopen("/dev/null", "r");
	fopen("/dev/null", "w+");

	filename = make_filename(aid);
	if (!(logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		perror("file open error");
		free(filename);
		exit(EXIT_FAILURE);
	}
	dup2(logfd, STDERR_FILENO);
	free(filename);


	return (logfd);
}

static void signal_handler(int signal)
{
	if (signal == SIGPIPE)
	{
		agent_logging("received SIG_PIPE, reconnect . . .");
		connect_wrap();
	}
	if (signal == SIGINT)
	{
		agent_logging("received SIG_INT");
	}
	
}

static void set_signal(void)
{
	struct sigaction act;

	act.sa_handler = signal_handler;
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);
	sigaction(SIGINT, &act, NULL);
}

static void init_share(int logfd, int aid)
{
	g_ashare = malloc(sizeof(ashare));
	g_ashare->q = init_aqueue();
	if (pthread_mutex_init(&g_ashare->aqueue_lock, NULL))
	{
		err_log("mutex init error");
		exit (EXIT_FAILURE);
	}
	if (pthread_mutex_init(&g_ashare->log_lock, NULL))
	{
		err_log("mutex init error");
		exit (EXIT_FAILURE);
	}
	g_ashare->logfd = logfd;
	g_ashare->aid = aid;
}

int main(int argc, char **argv)
{
	pthread_t		collect_tid;
	pthread_t		send_tid;
	aqueue			*q;
	int				logfd;
	int				aid;

	if (argc != 2)
		return (0);
	aid = atoi(argv[1]);
	
//	logfd = daemonize(aid);

	char *filename;
	filename = make_filename(aid);
	if (!(logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		perror("file open error");
		free(filename);
		exit(EXIT_FAILURE);
	}
	free(filename);
	init_share(logfd, aid);
	set_signal();
	pthread_create(&collect_tid, NULL, collect_routine, NULL);
	agent_logging("collect thread created");
	pthread_create(&send_tid, NULL, send_routine, NULL);
	agent_logging("send thread created");

	pthread_join(collect_tid, NULL);
	pthread_join(send_tid, NULL);

	pthread_mutex_destroy(&g_ashare->aqueue_lock);
	pthread_mutex_destroy(&g_ashare->log_lock);
	free_queue(g_ashare->q);
	free(g_ashare);
	close(logfd);
	return (0);
}
