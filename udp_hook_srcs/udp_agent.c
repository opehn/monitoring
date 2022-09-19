#include "hooking.h"
#include "../includes/agent.h"

extern ashare *g_ashare;

ssize_t (*origin_send)(int, const void *, size_t, int);

static int				udp_cnt = 0;
static int				udpfd;
static SA_IN			servaddr;
static int				udp_logfd;

void    udp_logging(char *msg)
{
    time_t      cur = time(NULL);
    struct tm   *t = localtime(&cur);
    char        log_msg[150];
    int         msg_len;

    sprintf(log_msg, "[%02d-%02d-%02d] : %s\n", t->tm_hour, t->tm_min, t->tm_sec, msg);
    msg_len = strlen(log_msg);
    write(udp_logfd, log_msg, msg_len);
}

void    udp_err_logging(char *err_type)
{
    char    *msg;

    msg = malloc(150);
    sprintf(msg, "%s : %s", err_type, strerror(errno));
    udp_logging(msg);
    free(msg);
}

static char *make_filename(void)
{
    time_t      cur_time;
    struct tm*  time_struct;
    char        *file_name;
    int         year;
    int         month;
    int         day;

    if (!(file_name = malloc(sizeof(char) * 50)))
    {
        perror("malloc failed");
        return (NULL);
    }
    cur_time = time(NULL);
    time_struct = localtime(&cur_time);

    year = time_struct->tm_year + 1900;
    month = time_struct->tm_mon + 1;
    day = time_struct->tm_mday;
    sprintf(file_name, "./udp_hook_log/%d-%02d-%02d", year, month, day);
    return (file_name);
}

static void	initialize(void)
{
	char			*filename;

	/* open log file */
	filename = make_filename();
    if (!(udp_logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
    {
        perror("file open error");
        free(filename);
        exit(EXIT_FAILURE);
    }
    free(filename);

	/* open udp socket */
	if (0 > (udpfd = socket(AF_INET, SOCK_DGRAM, 0)))
	{
		udp_err_logging("socket open error");
		exit(EXIT_FAILURE);
	}

	/* set sockaddr struct */
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVERIP);
	servaddr.sin_port = SERVERPORT;
}

char	*get_pname(int pid)
{
	char	path[25];
	int		fd;
	char	*content;
	char	*res;
	char	*temp;
	char	*buf;

	sprintf(path, "/proc/%d/stat", pid);
	fd = open(path, O_RDONLY);
	if (fd < 0)
	{
		if (errno != ENOENT)
		{
			udp_err_logging("file open error");
			exit(EXIT_FAILURE);
		}
		errno = 0;
	}
	content = read_file(fd);
	res = strtok_r(content, " ", &temp);
	res = strtok_r(NULL, " ", &temp);
	buf = skip_bracket(res);
	close(fd);
	printf("pname : %s\n", buf);
	return (buf);
}

void	set_end_packet(char *payload, int length, clock_t elapse)
{
	char			*time;
	end_p			*end;
	int				i = 0;

	end = (end_p *)payload;
	end->host_id = g_ashare->aid;
	end->pid = getpid();
	end->send_byte = length;
	end->elapse_time = elapse;
	end->check = '[';
}

void	set_begin_packet(char *payload)
{
	char			*time;
	begin_p			*begin;
	int				i = 0;
	char			*buf;

	begin = (begin_p *)payload;
	begin->host_id = g_ashare->aid;
	begin->pid = getpid();
	while (i < strlen(SERVERIP))
	{
		begin->peer_ip[i] = SERVERIP[i];
		i++;
	}
	i = 0;
	buf = get_pname(begin->pid);
	while (i < nullguard_strlen(buf))
	{
		begin->process_name[i] = buf[i];
		i++;
	}
	i = 0;
	free(buf);
	time = cur_time();
	for(int i = 0; i < 11; i++)
		begin->begin_time[i] = time[i];
	free(time);
	begin->port = LOCALPORT;
	begin->pkt_no = udp_cnt;
	begin->check = '[';
}

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
	printf("send hooked by hook_test.so!!\n");
	printf("Hooked len: %ld\n", len);

	char			*begin_packet;
	char			*end_packet;
	int				length;
	uint64_t		elapse_time;
	clock_t			start, end;

	/* initialize */
	initialize();

	/* set begin packet */
	if (!(begin_packet = calloc(sizeof(begin_p), 1)))
	{
		udp_err_logging("malloc error");
		return (-1);
	}
	set_begin_packet(begin_packet);

	/* send begin packet */
	sendto(udpfd, begin_packet, (11 + sizeof(int)), 0, (SA *) &servaddr, sizeof(servaddr));
	free(begin_packet);
	udp_logging("Send begin packet");

	/* send real packet */
	start = clock();
	origin_send = (ssize_t(*)(int, const void *, size_t, int))dlsym(RTLD_NEXT, "send");
	end = clock();

	/* set end packet */
	if (!(end_packet = calloc(sizeof(end_p), 1)))
	{
		udp_err_logging("Malloc error");
		return (-1);
	}
	length = nullguard_strlen((char *)buf);
	set_end_packet(end_packet, length, start - end);

	/* send end packet */
	sendto(udpfd, "]",  1, 0, (SA *) &servaddr, sizeof(servaddr));
	udp_logging("Send end packet");
	free(end_packet);
	udp_cnt++;

	return (0);
}
