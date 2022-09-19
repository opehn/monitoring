#include "server.h"

sshare			*g_sshare;
data_queue		*g_dq;
pthread_cond_t	g_cond;
int				g_udp_logfd;

static void daemonize(void)
{
    pid_t   pid = 0;
    int     fd;
    int     logfd;
    char    *filename;
    int     pid_fd;

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

    filename = make_filename("server_log");
    if (!(g_sshare->logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
    {
		perror("log file open error");
        free(filename);
        exit(EXIT_FAILURE);
    }
	dup2(g_sshare->logfd, STDERR_FILENO);
    free(filename);

    filename = make_filename("udp_hook_log");
    if (!(g_udp_logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
    {
		perror("log file open error");
        free(filename);
        exit(EXIT_FAILURE);
    }
    free(filename);
}

static int init_socket(int logfd, pthread_mutex_t *log_lock)
{
	int res;
	int	listenfd;

	//socket()
	if (0 > (listenfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		err_log("socket open error");
		exit(EXIT_FAILURE);
	}
	//bind()
	SA_IN   serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = SERVERPORT;
	if (0 > (res = bind(listenfd, (SA *)&serveraddr,\
			sizeof(serveraddr))))
	{
		err_log("bind error");
		exit(EXIT_FAILURE);
	}

	//listen()
	if(0 > (listen(listenfd, SOMAXCONN)))
	{
		err_log("listen error");
		exit(EXIT_FAILURE);
	}
	return (listenfd);
}

static pthread_t *create_worker_thread(int max_cli)
{
	pthread_t	*worker_tid;
	int			i = 0;

	if (!(worker_tid = malloc(sizeof(pthread_t) * max_cli)))
	{
		err_log("malloc error");
		exit (EXIT_FAILURE);
	}
	while (i < max_cli)
	{
		pthread_create(&worker_tid[i],  NULL, worker_routine, NULL);
		if (worker_tid < 0)
		{
			err_log("thread create error");
			exit (EXIT_FAILURE);
		}
		server_logging("worker thread created, waiting. . .");
		i++;
	}
	return (worker_tid);
}

static pthread_t *create_recv_thread(int max_cli)
{
	pthread_t	*recv_tid;
	int			i = 0;

	if (!(recv_tid = malloc(sizeof(pthread_t) * max_cli)))
	{
		err_log("malloc error");
		exit (EXIT_FAILURE);
	}
	while (i < max_cli)
	{
		pthread_create(&recv_tid[i],  NULL, accept_perthread, NULL);
		if (recv_tid < 0)
		{
			err_log("thread create error");
			exit (EXIT_FAILURE);
		}
		server_logging("receive thread created, waiting. . .");
		i++;
	}
	return (recv_tid);
}

void	init_sshare(int logfd)
{
	g_dq = init_data_queue();
	g_sshare = malloc(sizeof(sshare));
	g_sshare->logfd = logfd;
	g_sshare->flag = 0;
	if (pthread_mutex_init(&g_sshare->log_lock, NULL))
	{
		err_log("mutex init error");
		exit (EXIT_FAILURE);
	}
	if (pthread_mutex_init(&g_sshare->dq_lock, NULL))
	{
		err_log("mutex init error");
		exit (EXIT_FAILURE);
	}
	if (pthread_mutex_init(&g_sshare->accept_lock, NULL))
	{
		err_log("mutex init error");
		exit (EXIT_FAILURE);
	}
}

void	set_signal(void)
{
	struct sigaction	act;

	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);
}

int	init_udpsocket(void)
{
	SA_IN	servaddr;
	int		udpsockfd;

	udpsockfd = socket(AF_INET, SOCK_DGRAM, 0);

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERVERPORT);

	bind(udpsockfd, (SA *) &servaddr, sizeof(servaddr));
	return (udpsockfd);
}

int		main(int argc, char **argv)
{
	SA_IN				clientaddr;
	int					max_cli;
	char				*filename;
	pthread_t			*worker_tid;
	pthread_t			*recv_tid;
	pthread_t			udp_tid;
	int					udpsockfd;

	if (argc != 2)
		return (0);
	max_cli = atoi(argv[1]);

//	daemonize();

    filename = make_filename("server_log");
    if (!(g_sshare->logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
    {
		perror("log file open error");
        free(filename);
        exit(EXIT_FAILURE);
    }
    free(filename);

	write(g_sshare->logfd, "----------------------------------------------------------------------\n", 71);
	init_sshare(g_sshare->logfd);

	/* open listen socket */
	g_sshare->listenfd = init_socket(g_sshare->logfd, &g_sshare->log_lock);

	/* open udp(hook) socket*/
	udpsockfd = init_udpsocket();

	/* create tcp receive thread / worker thread */
	recv_tid = create_recv_thread(max_cli);
	worker_tid = create_worker_thread(max_cli);

	/*create udp receive thread*/
	pthread_create(&udp_tid, NULL, udp_receive_routine, (void *)&udpsockfd);

	for (int i = 0; i < max_cli; i++)
	{
		pthread_join(worker_tid[i], NULL);
		pthread_join(recv_tid[i], NULL);
	}
	pthread_join(udp_tid, NULL);

	/* logging */
	char	*msg;
	msg = malloc(100);
	sprintf(msg, "\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
	server_logging(msg);
	free(msg);

	/* free resource */
	pthread_mutex_destroy(&g_sshare->dq_lock);
	pthread_mutex_destroy(&g_sshare->log_lock);
	free(g_dq);
	close(g_sshare->listenfd);
	free(g_sshare);
	return (0);
}
