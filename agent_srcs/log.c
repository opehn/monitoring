#include "collect.h"

void    err_log(char *err_type, int logfd, pthread_mutex_t *log_lock)
{
    char    *msg;

    msg = malloc(150);
    sprintf(msg, "%s : %s", err_type, strerror(errno));
    agent_logging(msg, logfd, log_lock);
    free(msg);
}

void	agent_logging(char *msg, int logfd, pthread_mutex_t *log_lock)
{
	time_t		cur = time(NULL);
	struct tm	*t = localtime(&cur);
	char		log_msg[150];
	int			msg_len;

	sprintf(log_msg, "[%02d-%02d-%02d] : %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
	msg_len = strlen(log_msg);
	pthread_mutex_lock(log_lock);
	write(logfd, log_msg, msg_len);
	pthread_mutex_unlock(log_lock);
}
