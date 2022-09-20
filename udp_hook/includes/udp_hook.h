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

#pragma pack(1)
typedef struct begin_p {
	char			check;
	uint32_t		host_id;
	char			process_name[16];
	uint32_t		pid;
	char			peer_ip[16];
	uint32_t		port;
	char			begin_time[11];
	uint32_t		pkt_no;
} begin_p;

typedef struct end_p {
	char			check;
	uint32_t		host_id;
	char			process_name[16];
	uint32_t		pid;
	uint64_t		send_byte;
	clock_t		elapse_time;
} end_p;
#pragma pack()

void    udp_logging(char *msg);
void    udp_err_logging(char *err_type);
