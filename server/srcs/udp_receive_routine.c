#include "server.h"
#include "udp_hook.h"

int	g_udp_logfd;

static int	udp_recv_wrap(int udpsockfd, char *buf, int buf_len, int flag, SA *cliaddr, socklen_t *cli_len)
{
	int		n;

	n = recvfrom(udpsockfd, buf, buf_len, flag, cliaddr, cli_len);
	if (n != buf_len)
	{
		free(buf);
		char msg[100];
		sprintf(msg, "Data lost, expected : %d received : %d lost : %d\n",
				buf_len, n, buf_len - n);
		udp_logging(msg);
		return (-1);
	}
	return (n);

}

void	write_begin(int data_fd, char *payload)
{
	begin_p	*begin;
	char	buf[200];
	char	*cur;
	
	cur = cur_time();
	
	begin = (begin_p *)payload;

	sprintf(buf, "BEGIN | sended %s received %s\nPck_no : %d Host_id : %d pid : %d Process_name : %s Peer id : %s Port number %d\n", 
			begin->begin_time, cur, begin->pkt_no, begin->host_id, begin->pid, begin->process_name, begin->peer_ip, begin->port);
	write(data_fd, buf, strlen(buf));
	free(cur);
}

void	write_end(int data_fd, char *payload)
{
	end_p	*end;
	char	buf[200];
	char	*cur;
	
	cur = cur_time();
	end = (end_p *)payload;

	sprintf(buf, "END   | received %s\nHost id : %d pid : %d Process name : %s Send byte: %ld Elapse time : %ld\n", 
			cur, end->host_id, end->pid, end->process_name, end->send_byte, end->elapse_time);
	write(data_fd, buf, strlen(buf));
	free(cur);
}

int init_udpsock(void)
{
    SA_IN   servaddr;
    int     udpsockfd;

    udpsockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(UDPSERVERPORT);

    if (0 > (bind(udpsockfd, (SA *) &servaddr, sizeof(servaddr))))
		udp_err_logging("bind error");
    return (udpsockfd);
}

void	*udp_receive_routine(void *arg)
{
	int		udpsockfd;
	int		data_fd;
	SA_IN	cliaddr;
	socklen_t	cli_len = sizeof(cliaddr);
	char	*begin;
	char	*end;
	char	*check = malloc(1);
	char	*filename;

	arg = (int *)arg;
	filename = make_filename("./log/udp_hook_log");
    if (!(g_udp_logfd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
    {
        perror("udp log file open error");
        free(filename);
        exit(EXIT_FAILURE);
    }
    free(filename);

	filename = make_filename("./data/udp_data");
	if (!(data_fd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		udp_err_logging("udp data file open error");
		free(filename);
		return (NULL) ;
	}
	free(filename);

	udpsockfd = init_udpsock();
	while (1)
	{
		recvfrom(udpsockfd, check, 1, MSG_PEEK, (SA *)&cliaddr, &cli_len);
		if (*check == '[')
		{
			if (!(begin = malloc(sizeof(begin_p))))
			{
				udp_err_logging("malloc error");
				exit(EXIT_FAILURE);
			}
			if (0 > (udp_recv_wrap(udpsockfd, begin, sizeof(begin_p), 0, (SA *)&cliaddr, &cli_len)))
				continue;
			write_begin(data_fd, begin);
			free(begin);
			udp_logging("Received begin packet");
		}
		else if (*check == ']')
		{
			if (!(end = malloc(sizeof(end_p))))
			{
				udp_err_logging("malloc error");
				exit(EXIT_FAILURE);
			}
			if (0 > (udp_recv_wrap(udpsockfd, end, sizeof(end_p), 0, (SA *)&cliaddr, &cli_len)))
				continue;
			write_end(data_fd, end);
			free(end);
			udp_logging("Received end packet");
		}
		else
			udp_logging("Data lost : invalid check");
	}
	close(data_fd);
}
