HOOK_DYNAMIC_LIB = send_hooking.so

CC = gcc
FLAGS = -fPIC -shared -ldl

HEADER_LIST = ./includes/udp_hook.h \
			  ../includes/agent.h

HOOK_SRC = ./srcs/udp_hook.c \
		   ../agent_srcs/util.c \
		   ../agent_srcs/read_file.c

all : $(HOOK_DYNAMIC_LIB)

$(HOOK_DYNAMIC_LIB) : $(HOOK_SRC) $(HEADER)
	$(CC) $(FLAGS) $^ -o $@ -I ../includes -I ./includes

clean : 
	@rm -rf $(HOOK_DYNAMIC_LIB) 

fclean : 
	@rm -rf $(HOOK_DYNAMIC_LIB) 
re : fclean all

.PHONY : all clean fclean re

