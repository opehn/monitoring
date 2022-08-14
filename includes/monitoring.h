#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <pwd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1000

typedef struct	proc_info { 
	int					pid;
	int					ppid;
	int					cpu_time;
	float				cpu_usage;
	char				*proc_name;
	char				*user_name;
	char				*cmdline;
	struct proc_info	*next;
} proc_info;

typedef struct	cpu_info {
	int					usr;
	int					sys;
	int					iowait;
	int					idle;
	struct cpu_info		*next;
} cpu_info;

typedef struct mem_info {
	int		free;
	int		total;
	int		used;
	int		swap_used;
} mem_info;

typedef struct pac_info {
	int					in_cnt;
	int					out_cnt;
	int					in_byte;
	int					out_byte;
	struct pac_info		*next;
} pac_info;

//parsing
proc_info	*get_proc_info(void);
cpu_info	*get_cpu_info(void);
mem_info	*get_mem_info(void);
pac_info	*get_pac_info(void);
int		isnum(char *nptr);
void	read_stat(char *path);
char	**split(char const *str, char c);

//util
char	*my_strjoin(char *s1, char *s2);
void	free_all(char **arr);

//get_next_line
char    *get_next_line(int fd, int flag);
char*   read_file(int fd);
int     find_newline(char *str);
char    *my_strndup(char *origin, int len);
