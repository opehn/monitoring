AGENT = agent

CC = gcc
#CC = clang-14 
FLAGS = -g -Wall -Werror -Wextra

INC = -I$(HEADER_DIR)

HEADER_DIR = ./includes/
HEADER_LIST = monitoring.h
HEADERS = $(addprefix $(HEADER_DIR), $(HEADER_LIST))

SRC_DIR = ./srcs/
A_SRC := read_file.c \
		 util.c \
		 get_proc_info.c \
		 monitoring.c \
		 get_next_line.c \
		 get_cpu_info.c \
		 get_mem_info.c \
		 get_pac_info.c

A_OBJ = $(A_SRC:.c=.o) 

all : $(AGENT)

#$(SERVER) : $(S_OBJ)
#	$(CC) $(FLAGS) $^ -o $@ 

$(AGENT) :$(A_OBJ)
	$(CC) $(FLAGS) $^ -o $@ 

%.o : $(SRC_DIR)%.c $(HEADERS)
	$(CC) $(CFLAGS) -c -g $< -o $@ $(INC)

clean :
	@rm -rf $(A_OBJ)

fclean : clean
	@rm -rf $(AGENT)

re : fclean all	

.PHONY: all clean fclean re
