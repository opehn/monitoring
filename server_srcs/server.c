#include "server.h"

static char *make_filename(void)
{
    time_t      cur_time;
    struct tm*  time_struct;
    char        *file_name;
    int         year;
    int         month;
    int         day;

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
    sprintf(file_name, "./server_log/%d-%02d-%02d", year, month, day);
    return (file_name);
}

static int daemonize(void)
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

    char str[256];
    pid_fd = open("./server_log/daemon_pid", O_RDWR | O_CREAT | O_NOCTTY, 0640);
    if (pid_fd < 0) {
		err_log("log error", logfd);
        exit(EXIT_FAILURE);
    }
    sprintf(str, "%d\n", getpid());
    write(pid_fd, str, strlen(str));

    filename = make_filename();
    if (!(logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
    {
		err_log("log file open error", logfd);
        free(filename);
        exit(EXIT_FAILURE);
    }
    free(filename);
    return (logfd);
}

static void	init_socket(int *listenfd, int logfd)
{
	int res;

	//socket()
	if (0 > (*listenfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		err_log("socket open error", logfd);
		exit(EXIT_FAILURE);
	}

	//bind()
	SA_IN   serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = INADDR_ANY;
	serveraddr.sin_port = SERVERPORT;
	if (0 > (res = bind(*listenfd, (SA *)&serveraddr,\
			sizeof(serveraddr))))
	{
		err_log("bind error", logfd);
		exit(EXIT_FAILURE);
	}

	//listen()
	if(0 > (listen(*listenfd, SOMAXCONN)))
	{
		err_log("listen error", logfd);
		exit(EXIT_FAILURE);
	}
}

static pthread_t *create_worker_thread(sparam *p)
{
	pthread_t	*worker_id;
	int			i = 0;

	if (!(worker_id = malloc(sizeof(pthread_t) * WORKER_NUM)))
	{
		err_log("malloc error", p->logfd);
		exit (EXIT_FAILURE);
	}
	while (i < WORKER_NUM)
	{
		pthread_create(&worker_id[i],  NULL, worker_routine, (void *)p);
		if (worker_id < 0)
		{
			err_log("thread create error", p->logfd);
			exit (EXIT_FAILURE);
		}
		server_logging("worker thread created, waiting. . .", p->logfd);
		i++;
	}
}

static void	create_recv_thread(sparam *p)
{
	pthread_t		recv_tid;

	pthread_create(&recv_tid, NULL, receive_routine, (void *)p);
	pthread_join(recv_tid, NULL);
}

sparam	*init_param(int logfd)
{
	sparam				*p;
	squeue				*q;

	p = malloc(sizeof(sparam));
	p->q = init_squeue();
	p->clientfd = 0;
	p->logfd = logfd;
	if (pthread_mutex_init(&p->squeue_lock, NULL))
	{
		err_log("mutex init error", logfd);
		exit (EXIT_FAILURE);
	}
	if (pthread_mutex_init(&p->log_lock, NULL))
	{
		err_log("mutex init error", logfd);
		exit (EXIT_FAILURE);
	}
	return (p);
}

void	set_signal(void)
{
	struct sigaction	act;

	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);
}

int		main(void)
{
	int					clientfd;
	int					listenfd;
	SA_IN				clientaddr;
	socklen_t			addrlen;
	sparam				*p;
	pthread_t			*worker_tid;
	int					logfd;

	logfd = daemonize();
	p = init_param(logfd);
	worker_tid = create_worker_thread(p);
	init_socket(&listenfd, logfd);
	addrlen = sizeof(clientaddr);
	while(1)
	{
		if(0 > (clientfd = accept(listenfd, (SA *)&clientaddr, &addrlen)))
		{
			err_log("accept error", logfd);
			break;
		}
		char	*msg;
		msg = malloc (100);
		sprintf(msg, "\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
		server_logging(msg, logfd);
		free(msg);
		p->clientfd = clientfd;
		create_recv_thread(p);
		close(clientfd);
	}
	for (int i = 0; i < WORKER_NUM; i++)
		pthread_join(worker_tid[i], NULL);
	char	*msg;
	msg = malloc(100);
	sprintf(msg, "\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
	server_logging(msg, logfd);
	free(msg);
	pthread_mutex_destroy(&p->squeue_lock);
	pthread_mutex_destroy(&p->log_lock);
	free(p->q);
	free(p);
	close(listenfd);
	return (0);
}
