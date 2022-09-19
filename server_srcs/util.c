#include "server.h"

char    *cur_time(void)
{
    time_t      cur = time(NULL);
    struct  tm  *t = localtime(&cur);
    char        *time_log;
    int         msg_len;

    time_log = malloc(11);
    sprintf(time_log, "[%02d-%02d-%02d]", t->tm_hour, t->tm_min, t->tm_sec);
    return (time_log);
}

char *make_filename(char *dir_name)
{
    time_t      cur_time;
    struct tm*  time_struct;
    char        *file_name;
    int         year;
    int         month;
    int         day;

    if (!(file_name = malloc(sizeof(char) * 50)))
    {
        perror("malloc failed");
        return (NULL);
    }
    cur_time = time(NULL);
    time_struct = localtime(&cur_time);

    year = time_struct->tm_year + 1900;
    month = time_struct->tm_mon + 1;
    day = time_struct->tm_mday;
    sprintf(file_name, "./%s/%d-%02d-%02d", dir_name, year, month, day);
    return (file_name);
}
