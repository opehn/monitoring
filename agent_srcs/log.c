#include "collect.h"

void	logging(int logfd, char *msg)
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
	free(msg);
}
