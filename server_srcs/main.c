#include "collect.h"
#include "server_queue.h"

void	init_socket(int *listenfd)
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
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
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

void	create_recv_thread(sparam *p)
{
	pthread_t		recv_tid;
	if (pthread_mutex_init(&p->squeue_lock, NULL));
	{
		perror("mutex init error");
		exit (EXIT_FAILURE);
	}
	pthread_create(&recv_tid, NULL, receive_routine, (void *)p);
	pthread_join(recv_tid, NULL);
	return (0);
}


int		main(void)
{
	int				serverfd;
	int				listenfd;
	SA_IN			clientaddr;
	socklen_t		addrlen;
	sparam			*p;
	squeue			*q;

	p = malloc(sizeof(sparam));
	p->q = init_squeue();

	init_socket(&listenfd);
	addrlen = sizeof(clientaddr);
	while(1)
	{
		if(0 > (serverfd = accept(listenfd, (SA *)&clientaddr, &addrlen)))
		{
			perror("accpet error");
			break;
		}
		printf("\n[TCP 서버] 클라이언트 점속: IP 주소 = %s, 포트 번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		p->serverfd = serverfd;
		create_recv_thread(p);
//		create_work_thread(p);
	}
	printf("\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	pthread_mutex_destroy(&p->aqueue_lock);
	free(p->q);
	free(p);
	close(serverfd);
	close(listenfd);
	return (0);
}
