#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define SERVERIP "127.0.0.1"
#define TCPSERVERPORT 8000
#define UDPSERVERPORT 9000

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

void    udp_logging(char *msg);
void    udp_err_logging(char *err_type);
