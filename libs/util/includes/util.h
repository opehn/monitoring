#ifndef UTIL_H
#define UTIL_H

#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char    *cur_time(void);
char	*make_filename(char *dir_name);
int		nullguard_strlen(char *str);

#endif
