#include "server.h"
#include "../udp_hook_srcs/hooking.h"

static int	udp_recv_wrap(int udpsockfd, char *buf, int buf_len, int flag, SA *cliaddr, int cli_len)
{
	int		n;

	n = recvfrom(udpsockfd, buf, buf_len, 0, &cliaddr, sizeof(cliaddr));
	if (n != buf_len)
	{
		free(buf);
		char msg[100]
		sprintf(msg, "Data lost, expected : %d received : %d lost : %d\n",
				buf_len, n, buf_len - n);
		udp_logging(msg);
	}
	//check_cli(cliaddr);
	return (n);

}

void	write_begin(int data_fd, char *payload)
{
	begin_p	*begin;
	char	buf[200];
	char	*cuf_time;
	
	cur_time = cur_time();
	
	begin = (begin_p *)payload;

	sprintf(buf, "%s : Receive begin packe, send at %s\n, Pck_no : %d, Host_id : %d, pid : %d, Process_name : %s, Peer id : %s, Port : %d\n", 
			cur_time, begin->begin_time, begin->pkt_no, begin->host_id, begin->pid, begin->process_name, begin->peer_ip, begin->port);
	write(data_fd, buf, strlen(buf));
}

void	write_end(int data_fd, char *payload)
{
	end_p	*end;
	char	buf[200];
	char	*cuf_time;
	
	cur_time = cur_time();
	end = (end_p *)payload;

	sprintf(buf, "%s : Receive end packet\n, Host id : %d, pid : %d, Process name : %s, Send byte: %ld, Elapse time : %ld\n", 
			cur_time, end->host_id, end->pid, end->process_name, end->send_byte, end->elapse_time);
	write(data_fd, buf, strlen(buf));
	free(cur_time);
}

void	*udp_receive_rooutine(void *arg)
{
	int		udpsockfd;
	int		data_fd;
	SA		cliaddr;
	char	*begin;
	char	*end;
	char	*check = malloc(1);
	char	*filename;

	filename = make_filename("udp_data");
	if (!(data_fd = open(filename, O_RDWR | O_APPEND | O_CREAT | O_NOCTTY, S_IRWXU)))
	{
		err_log("udp data file open error");
		free(filename);
		return ;
	}
	free(filename);

	udpsockfd = *((int *) arg);
	end = malloc(sizeof(end_p));
	if (!begin || !end)
		udp_err_logging("malloc error");
	while (1)
	{
		recvfrom(udpsockfd, check, 1, MSG_PEEK, &cliaddr, sizeof(cliaddr));
		if (check[0] == '[')
		{
			begin = malloc(sizeof(begin_p));
			udp_recv_wrap(udpsockfd, begin, sizeof(begin_p), 0, &cliaddr, sizeof(cliaddr));
			write_begin(data_fd, begin);
			free(begin);
		}
		else if (check[0] == ']')
		{
			end = malloc(sizeof(begin_p));
			udp_recv_wrap(udpsockfd, end, sizeof(end_p), 0, &cliaddr, sizeof(cliaddr));
			write_end(data_fd, end);
			free(end);
		}
		else
			udp_logging("Data lost : invalid check");
	}
	close(data_fd);
}
