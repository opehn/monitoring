#include "agent.h"

extern	ashare	*g_ashare;

void	err_log(char *err_type)
{
	char	*msg;

	msg = malloc(150);
	sprintf(msg, "%s : %s", err_type, strerror(errno));
	agent_logging(msg);
	free(msg);
}

void	sendinfo_log(char *buf)
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
	agent_logging(msg);
	free(msg);
}

void	agent_logging(char *msg)
{
	time_t		cur = time(NULL);
	struct tm	*t = localtime(&cur);
	char		log_msg[150];
	int			msg_len;

	sprintf(log_msg, "[%02d-%02d-%02d] : %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
	msg_len = strlen(log_msg);
	pthread_mutex_lock(&g_ashare->log_lock);
	write(g_ashare->logfd, log_msg, msg_len);
	pthread_mutex_unlock(&g_ashare->log_lock);
}
