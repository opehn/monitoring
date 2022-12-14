#ifndef	AGENT_H
#define AGENT_H

#include "packet.h"
#include <signal.h>
#include <dirent.h>
#include <math.h>
#include <stdio.h>
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

#define BUFFER_SIZE 1000
#define SERVERIP	"127.0.0.1"
#define TCPSERVERPORT	8000
#define BUFSIZE		512

typedef struct sockaddr SA; 
typedef struct sockaddr_in SA_IN; 
typedef struct aqueue aqueue;

typedef struct ashare
{
	int				aid;
	int				logfd;
	int				udp_cnt;
	aqueue			*q;
	pthread_mutex_t aqueue_lock;
	pthread_mutex_t log_lock;
} ashare;

//parsing
packet			*get_mem_info(void);
packet			*get_net_info(void);
packet			*get_cpu_info(void);
packet			*get_proc_info(void);

//util
char			*my_strjoin(char *s1, char *s2);
void			free_all(char **arr);
int				isnum(char *str);
char			*skip_bracket(char *src);
void			read_stat(char *path);
void			serialize_header(int signature, int length, int agent_id, char *payload);
char			*cur_time(void);

//file_util
char			*get_next_line(int fd, int flag);
char*			read_file(int fd);
int				find_newline(char *str);
char			*my_strndup(char *origin, int len);
int				nullguard_strlen(char *str);

//collect
void			*collect_routine(void *param);

//send
void			connect_wrap(void);
void			*send_routine(void *param);

//log
void			agent_logging(char *msg);
void			err_log(char *err_type);
void			sendinfo_log(char *buf);

#endif
