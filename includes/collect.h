#ifndef	COLLECT_H
#define COLLECT_H

#include "packet.h"
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

typedef struct sockaddr SA; 
typedef struct sockaddr_in SA_IN; 
#define SERVERIP	"127.0.0.1"
#define SERVERPORT	8000
#define BUFSIZE		512

//parsing
packet			*get_proc_info(void);
packet			*get_cpu_info(void);
packet			*get_mem_info(void);
packet			*get_net_info(void);

//util
char			*my_strjoin(char *s1, char *s2);
void			free_all(char **arr);
int				isnum(char *str);
char			*skip_bracket(char *src);
void			read_stat(char *path);
void			serialize_header(int signature, int length, int agent_id, char *payload);

//file_util
char			*get_next_line(int fd, int flag);
char*			read_file(int fd);
int				find_newline(char *str);
char			*my_strndup(char *origin, int len);
int				nullguard_strlen(char *str);

//collect
void			*collect_routine(void *param);

//send
void			*send_routine(void *param);

#endif
