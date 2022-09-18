#include "server.h"

extern	sshare	*g_sshare;

void err_log(char *err_type)
{
	char	msg[150];

	sprintf(msg, "%s : %s", err_type, strerror(errno));
	server_logging(msg);
}

void	data_time_logging(char	*ag_time)
{
	time_t		cur = time(NULL);
	struct tm	*t = localtime(&cur);
	char		log_msg[150];
	int			msg_len;

	sprintf(log_msg, "Collect time : %s  Received tiem : [%02d-%02d-%02d]", ag_time, t->tm_hour, t->tm_min, t->tm_sec);
	msg_len = strlen(log_msg);
	pthread_mutex_lock(&g_sshare->log_lock);
	write(g_sshare->logfd, log_msg, msg_len);
	pthread_mutex_unlock(&g_sshare->log_lock);
}

void	receive_logging(char *msg, int aid)
{
	char	log_msg[150];

	sprintf(log_msg, "%s from %d", msg, aid);
	server_logging(log_msg);

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
