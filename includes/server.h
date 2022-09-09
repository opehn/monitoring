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

void			*receive_routine(void *arg);
void			*worker_routine(void *args);
void			save_file(sparam *p);

void	server_logging(char *msg, int logfd, pthread_mutex_t *log_lock);
void	err_log(char *err_type, int logfd, pthread_mutex_t *log_lock);
void	send_packet(int clientfd, sparam *p, pthread_mutex_t *aqueue_mutex);

#endif
