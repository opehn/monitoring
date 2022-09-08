#include "server.h"

static int sendinfo_log(char *buf, int logfd)
{
	packet_header   *pht;
	int	 signature;
	int	 length;
	char	*msg;

	msg = malloc(100);
	signature = *((uint16_t *)buf);
	buf += sizeof(uint16_t);
	length = *((uint32_t *)buf);
	if (signature == M)
		sprintf(msg, "send memory info, byte size : %d", length);
	else if (signature == N)
		sprintf(msg, "send network info, byte size : %d", length);
	else if (signature == C)
		sprintf(msg, "send cpu info, byte size : %d", length);
	else if (signature == P)
		sprintf(msg, "send process info, byte size : %d", length);
	printf("msg : %s\n", msg);
	server_logging(msg, logfd);
	free(msg);
}

int err_log(char *err_type, int logfd)
{
	char	*msg;

	msg = malloc(150);
	sprintf(msg, "%s : %s", err_type, strerror(errno));
	server_logging(msg, logfd);
	free(msg);
}

void	server_logging(char *msg, int logfd)
{
	time_t		cur = time(NULL);
	struct tm	*t = localtime(&cur);
	char		time[11];
	int			msg_len;

	sprintf(time, "[%02d-%02d-%02d]", t->tm_hour, t->tm_min, t->tm_sec);
	msg_len = strlen(msg);
	write(logfd, time, 10);
	write (logfd, " : ", 3);
	write(logfd, msg, msg_len);
	write(logfd, "\n", 1);
}
