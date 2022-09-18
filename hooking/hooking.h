#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <dlfcn.h>

#define _GNU_SOURCE

typedef struct begin_p {
	char	time[11];
	int		len;
	char	check;
} begin_p;
