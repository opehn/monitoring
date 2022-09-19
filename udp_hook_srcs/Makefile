HOOK_DYNAMIC_LIB = send_hooking.so

CC = gcc
FLAGS = -Wl,--dynamic-list-data -fPIC -shared -ldl

HOOK_SRC = ./udp_agent.c \
		   ../agent_srcs/util.c \
		   ../agent_srcs/read_file.c

HEADER = ../includes/agent.h \
		 ./hooking.h

all : $(HOOK_DYNAMIC_LIB)

$(HOOK_DYNAMIC_LIB) : $(HOOK_SRC) $(HEADER)
	$(CC) $(FLAGS) $^ -o $@ -I ../includes

clean : 
	@rm -rf $(HOOK_DYNAMIC_LIB) 

fclean : 
	@rm -rf $(HOOK_DYNAMIC_LIB) 
re : fclean all

.PHONY : all clean fclean re

