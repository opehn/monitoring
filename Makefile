AGENT = agent
SERVER = server

CC = gcc 
FLAGS = -g -Wall -Werror -Wextra

INC = -I$(HEADER_DIR)

HEADER_DIR = ./includes/
HEADER_LIST = collect.h \
			  packet.h \
			  agent_queue.h \
			  server.h \
			  server_queue.h 
HEADERS = $(addprefix $(HEADER_DIR), $(HEADER_LIST))

A_SRC_DIR = ./agent_srcs/
A_SRC_LIST := read_file.c \
		 get_proc_info.c \
		 monitoring.c \
		 get_next_line.c \
		 get_cpu_info.c \
		 get_mem_info.c \
		 get_net_info.c \
		 agent.c \
		 util.c \
		 collect.c \
		 send.c \
		 agent_queue.c \
		 log.c
A_SRC := $(addprefix $(A_SRC_DIR), $(A_SRC_LIST))
A_OBJS = $(A_SRC:.c=.o)

S_SRC_DIR = ./server_srcs/
S_SRC_LIST := server.c \
		 server_queue.c \
		 receive.c \
		 save_data.c \
		 worker.c \
		 log.c
S_SRC := $(addprefix $(S_SRC_DIR), $(S_SRC_LIST))
S_OBJS = $(S_SRC:.c=.o)

all : $(AGENT) $(SERVER)

$(AGENT) : $(A_OBJS)
	$(CC) $(FLAGS) $^ -o $@ 

$(SERVER) : $(S_OBJS)
	$(CC) $(FLAGS) $^ -o $@ 

$(A_SRC_DIR)%.o: $(A_SRC_DIR)%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -g $< -o $@ $(INC)
	
$(S_SRC_DIR)%.o: $(S_SRC_DIR)%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -g $< -o $@ $(INC)

clean :
	@rm -rf $(A_OBJS) $(S_OBJS)

fclean : clean
	@rm -rf $(AGENT)

variable  :
	@echo S_SRC : $(S_SRC)
	@echo A_SRC : $(A_SRC)
	@echo S_OBJS : $(S_OBJS)
	@echo A_OBJS : $(A_OBJS)

re : fclean all	

.PHONY: all clean fclean re
