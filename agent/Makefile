AGENT = agent 

CC = gcc
FLAGS = -g -Wall -Wl,--dynamic-list-data #-Werror -Wextra 

HEADER_DIR1 = ./includes/
HEADER_DIR2 = ../libs/util/includes/

INC = $(HEADER_DIR1) $(HEADER_DIR2)
INC_PARAMS = $(addprefix -I, $(INC))

HEADER_LIST1 := agent.h \
			  agent_queue.h
HEADER_LIST2 := packet.h\
				util.h

HEADER1 = $(addprefix $(HEADER_DIR1), $(HEADER_LIST1))
HEADER2 = $(addprefix $(HEADER_DIR2), $(HEADER_LIST2))

SRC_DIR1 = ./srcs/
SRC_DIR2 = ../libs/util/srcs/

SRC_LIST1 := read_file.c \
		 get_proc_info.c \
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
SRC_LIST2 := util.c

SRC1 := $(addprefix $(SRC_DIR1), $(SRC_LIST1))
SRC2 := $(addprefix $(SRC_DIR2), $(SRC_LIST2))

OBJS1 = $(SRC1:.c=.o)
OBJS2 = $(SRC2:.c=.o)

all : $(AGENT)

$(AGENT) : $(OBJS1) $(OBJS2)
	$(CC) $(FLAGS) $^ -o $@

$(SRC_DIR)%.o: $(SRC_DIR)%.c $(HEADER1) $(HEADER2)
	$(CC) $(FLAGS) -c -g $< -o $@ $(INC_PARAMS)

clean :
	@rm -rf $(OBJS1) $(OBJS2)

fclean : clean
	@rm -rf $(AGENT)

variable  :
	@echo SRC : $(SRC)
	@echo OBJS : $(OBJS)
	@echo INC_PARAMS : $(INC_PARAMS)

re : fclean all

.PHONY: all clean fclean re
