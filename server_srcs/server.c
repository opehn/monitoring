#include "server_queue.h"
#include "server.h"
#include "packet.h"

static void	init_socket(int *listenfd)
{
	int res;

	//socket()
	if (0 > (*listenfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket open error");
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
		perror("bind error");
		exit(EXIT_FAILURE);
	}

	//listen()
	if(0 > (listen(*listenfd, SOMAXCONN)))
	{
		perror("listen error");
		exit(EXIT_FAILURE);
	}
}

static void	create_recv_thread(sparam *p)
{
	pthread_t		recv_tid;
	if (pthread_mutex_init(&p->squeue_lock, NULL))
	{
		perror("mutex init error");
		exit (EXIT_FAILURE);
	}
	pthread_create(&recv_tid, NULL, receive_routine, (void *)p);
	pthread_join(recv_tid, NULL);
}

int		main(void)
{
	int					clientfd;
	int					listenfd;
	SA_IN				clientaddr;
	socklen_t			addrlen;
	sparam				*p;
	squeue				*q;
	struct sigaction	act;

	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);
	p = malloc(sizeof(sparam));
	p->q = init_squeue();

	init_socket(&listenfd);
	addrlen = sizeof(clientaddr);
	while(1)
	{
		if(0 > (clientfd = accept(listenfd, (SA *)&clientaddr, &addrlen)))
		{
			perror("accpet error");
			break;
		}
		printf("\n[TCP 서버] 클라이언트 점속: IP 주소 = %s, 포트 번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
		p->clientfd = clientfd;
		create_recv_thread(p);
//		create_work_thread(p);
		close(clientfd);
	}
	printf("\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
	pthread_mutex_destroy(&p->squeue_lock);
	free(p->q);
	free(p);
	close(listenfd);
	return (0);
}
