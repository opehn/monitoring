#include "hooking.h"

extern int g_udp_logfd;

void    udp_logging(char *msg)
{
    time_t      cur = time(NULL);
    struct tm   *t = localtime(&cur);
    char        log_msg[150];
    int         msg_len;

    sprintf(log_msg, "[%02d-%02d-%02d] : %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
    msg_len = strlen(log_msg);
    write(g_udp_logfd, log_msg, msg_len);
}

void    udp_err_logging(char *err_type)
{
    char    *msg;

    msg = malloc(150);
    sprintf(msg, "%s : %s", err_type, strerror(errno));
    udp_logging(msg);
    free(msg);
}
