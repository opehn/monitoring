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
#include <time.h>
#include "server_queue.h"

#define SERVERPORT 8000
#define WORKER_NUM 4

typedef struct	sockaddr SA;
typedef struct	sockaddr_in SA_IN;

typedef struct sshare
{
	int					listenfd;
    int                 logfd;
    pthread_mutex_t     dq_lock;
    pthread_mutex_t     log_lock;
    pthread_mutex_t     accept_lock;
    pthread_mutex_t     cond_lock;
	pthread_cond_t		cond;
	int					flag;
} sshare;

void	*accept_perthread(void *args);
void	*worker_routine(void *args);
void	save_file(data_queue_node *cur);

void	server_logging(char *msg);
void	receive_logging(char *msg, int aid);
void	err_log(char *err_type);
void	send_packet(int coonfd);

#endif
