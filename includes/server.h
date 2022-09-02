#ifndef	SERVER_H
#define SERVER_H

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <pthread.h>

#define SERVERPORT 8000

typedef struct	squeue squeue;
typedef struct	sockaddr SA;
typedef struct	sockaddr_in SA_IN;

void			*receive_routine(void *arg);

typedef struct sparam
{
	squeue			*q;
	int				clientfd;
	pthread_mutex_t	squeue_lock;
} sparam;
#endif
