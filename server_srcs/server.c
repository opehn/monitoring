#include "collect.h"

void	init_socket(int *listenfd)
{
	int	res;

	//socket()
	if (0 > (*listenfd = socket(AF_INET, SOCK_STREAM, 0)))
	{
		perror("socket open error");
		exit(EXIT_FAILURE);
	}

	//bind()
	SA_IN	serveraddr;
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

int	recv_wrap(int listenfd, char *buf, int size, int flag)
{
	int res;

	if (0 > (res = recv(listenfd, buf, sizeof(packet_header), MSG_WAITALL)))
	{
		perror("server recv error");
		return (-1);
	}
	else if (res == 0)
		return (-1);
	return (0);

}

packet_header		*deserialize_header(char *buf)
{
	packet_header	*header;

	header = malloc(sizeof(packet_header));
	header->signature = ntohs(*(uint16_t *)buf);
	buf += sizeof(uint32_t);
	printf("signature : %d\n", header->signature);
}

int main(void)
{
	int				serverfd;
	int				listenfd;
	SA_IN			clientaddr;
	int				addrlen;
	char			*header_buf;
	char			*payload_buf;
	packet_header	*header;
	packet			*payload;

	init_socket(&listenfd);

	while(1)
	{
		addrlen = sizeof(clientaddr);
		if(0 > (serverfd = accept(listenfd, (SA *)&clientaddr, &addrlen)))
		{
			perror("accpet error");
			break;
		}
		printf("\n[TCP 서버] 클라이언트 점속: IP 주소 = %s, 포트 번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		while (1)
		{
			if (0 > recv_wrap(listenfd, header_buf, sizeof(packet_header), MSG_WAITALL))
				break;
			header = deserialize_header(header_buf);
			if (0 > recv_wrap(listenfd, payload_buf, sizeof(header->length), MSG_WAITALL))
				break;
			//deserialize_payload(header->signature, payload_buf);
		}
		close(listenfd);
		printf("\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 = %d\n",
				inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}
	close(serverfd);
	close(listenfd);
	return (0);
}
