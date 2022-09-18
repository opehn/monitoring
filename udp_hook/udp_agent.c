#include "hooking.h"
ssize_t (*origin_send)(int, const void *, size_t, int);

char	*cur_time(void)
{
	time_t		cur = time(NULL);
	struct  tm	*t = localtime(&cur);
	char		*timelog;

	time_log = malloc(11);
	sprintf(time_log, "[%02d-%02d-%02d]", t->tm_hour, t->tm_min, t->tm_sec);
	return (time_log);
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
	printf("send hooked by hook_test.so!!\n");
	printf("Hooked len: %ld\n", len);

	int		sockfd;
	char	*time;
	SA_IN	serveraddr;
	begin_p	begin;
	char	*payload;

	/* open udp socket */
	if (0 > (sockfd = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		err_logging("socket open error");
		exit(EXIT_FAILURE);
	}

	/* set sockaddr struct */
	memset(&serveraddr, 0, sizeof(SA_IN);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = SERVERPORT;

	/* set begin packet */
	payload = sizeof(begin_p);
	begin = (begin_p)payload;
	time = cur_time();
	for(int i = 0; i < 11; i++)
		begin->time[i] = time[i];
	free(time);
	begin->len = len + 12 + sizeof(int);
	begin->check = '[';

	/* send begin packet */
	sendto(sockfd, payload, (11 + sizeof(int)), 0, servAddr->ai_addr, servAddr->ai_addrlen);

	/* send hooked info */
	origin_send = (ssize_t(*)(int, const void *, size_t, int))dlsym(RTLD_NEXT, "send");

	/* send end packet */
	sendto(sockfd, ']',  1, 0, servAddr->ai_addr, servAddr->ai_addrlen);

	return (0);
}
