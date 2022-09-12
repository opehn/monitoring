#include "server.h"

extern	sshare	*g_sshare;

void err_log(char *err_type)
{
	char	*msg;

	msg = malloc(150);
	sprintf(msg, "%s : %s", err_type, strerror(errno));
	server_logging(msg);
	free(msg);
}

void	server_logging(char *msg)
{
	time_t		cur = time(NULL);
	struct tm	*t = localtime(&cur);
	char		log_msg[150];
	int			msg_len;

	sprintf(log_msg, "[%02d-%02d-%02d] : %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
	msg_len = strlen(log_msg);
	pthread_mutex_lock(&g_sshare->log_lock);
	write(g_sshare->logfd, log_msg, msg_len);
	pthread_mutex_unlock(&g_sshare->log_lock);
}
