#include "server.h"

extern sshare			*g_sshare;
extern data_queue		*g_dq;

static int	recv_wrap(int connfd, char *buf, int size, int flag)
{
	int res;

	if (0 > (res = recv(connfd, buf, size, flag)))
	{
		err_log("server receive error");
		return (-1);
	}
	else if (res == 0)
	{
		server_logging("end receive");
		return (-2);
	}
	return (0);
}

void	*receive_routine(int connfd)
{
	char			*header_buf;
	char			*payload_buf;
	packet_header	*header;
	int				body_length;
	int				res;

	while (1)
	{
		if (!(header_buf = malloc(sizeof(packet_header))))
		{
			err_log("malloc error");
			return (NULL);
		}
		if (0 > (res = recv_wrap(connfd, header_buf, sizeof(packet_header), MSG_WAITALL)))
		{
			free(header_buf);
			return (NULL);
		}
		header = (packet_header *)header_buf;
		body_length = header->length - sizeof(packet_header);
		if (!(payload_buf = malloc(body_length)))
		{
			err_log("malloc error");
			return (NULL);
		}
		if (0 > recv_wrap(connfd, payload_buf, body_length, MSG_WAITALL))
		{
			free(header_buf);
			free(payload_buf);
			return (NULL);
		}
		pthread_mutex_lock(&g_sshare->dq_lock);
		enqueue(g_dq, (packet_header *)header_buf, payload_buf);
		printf("enqueue, size : %d\n", g_dq->size);
		pthread_mutex_unlock(&g_sshare->dq_lock);
	}
	return (NULL);
}

void	*accept_perthread(void *args)
{
	SA_IN		clientaddr;
	socklen_t	addrlen;
	int			connfd;
	
	addrlen = sizeof(clientaddr);
	while(1)
	{
		/* accept */
		pthread_mutex_lock(&g_sshare->accept_lock);
		if(0 > (connfd = accept(g_sshare->listenfd, (SA *)&clientaddr, &addrlen)))
		{
			err_log("accept error");
			break;
		}
		if (!g_sshare->flag)
			pthread_cond_signal(&g_sshare->cond);
		g_sshare->flag++;
		pthread_mutex_unlock(&g_sshare->accept_lock);

		/* logging */
		char	*msg;
		msg = malloc (100);
		sprintf(msg, "\n[TCP 서버] 클라이언트 접속: IP 주소 = %s, 포트 번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);
		server_logging(msg);
		free(msg);

		/* start receive routine */
		receive_routine(connfd);

		/* end current connection, loop */
		close(connfd);
	}
	/* end logging */
	server_logging("receive thread end");
	return (NULL);
}
