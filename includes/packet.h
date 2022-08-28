#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define M 11
#define N 22
#define C 33
#define P 44
#define AID	42

#define CMD_LEN 1000

#pragma pack(1)

typedef struct packet_header {
	uint16_t	signature;
	uint32_t	length;
	uint16_t	agent_id;
} packet_header;

typedef	struct	packet {
	int				length;
	char			*payload;
} packet;

typedef struct  proc_info {
    uint32_t            pid;
    uint32_t            ppid;
    uint64_t            cpu_time;
    char                proc_name[16];//max 16
    char                user_name[32];//max 32
    char                cmdline[CMD_LEN];
} proc_info;

typedef struct  cpu_info {
    uint64_t            usr;
    uint64_t            sys;
    uint64_t            iowait;
    uint64_t            idle;
} cpu_info;

typedef struct mem_info {
    uint32_t            free;
    uint32_t            total;
    uint32_t            used;
    uint32_t            swap_used;
} mem_info;

typedef struct net_info {
    char                net_name[16];//max 16
    uint64_t            in_cnt;
    uint64_t            out_cnt;
    uint64_t            in_byte;
    uint64_t            out_byte;
} net_info;

#pragma pack()
#endif
