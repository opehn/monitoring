SERVER = server

CC = gcc
FLAGS = -g -Wall -Werror -Wextra

HEADER_DIR1 = ./includes/
HEADER_DIR2 = ../libs/util/includes/
HEADER_DIR3 = ../libs/udp_hook/includes/

INC = $(HEADER_DIR1) $(HEADER_DIR2) $(HEADER_DIR3)
INC_PARAMS = $(addprefix -I, $(INC))

HEADER_LIST1 := server.h \
			  server_queue.h
HEADER_LIST2 := packet.h
HEADER_LIST3 := udp_hook.h

HEADER1 = $(addprefix $(HEADER_DIR1), $(HEADER_LIST1))
HEADER2 = $(addprefix $(HEADER_DIR2), $(HEADER_LIST2))
HEADER3 = $(addprefix $(HEADER_DIR3), $(HEADER_LIST3))

SRC_DIR1 = ./srcs/
SRC_DIR2 = ../libs/util/srcs/

SRC_LIST1 := server.c \
		 data_queue.c \
		 receive.c \
		 save_data.c \
		 worker.c \
		 log.c \
		 udp_receive_routine.c \
		 udp_log.c

SRC_LIST2 := util.c

SRC1 := $(addprefix $(SRC_DIR1), $(SRC_LIST1))
SRC2 := $(addprefix $(SRC_DIR2), $(SRC_LIST2))

OBJS1 = $(SRC1:.c=.o)
OBJS2 = $(SRC2:.c=.o)

all : $(SERVER)

$(SERVER) : $(OBJS1) $(OBJS2)
	$(CC) $(FLAGS) $^ -o $@

$(SRC_DIR)%.o: $(SRC_DIR)%.c $(HEADER1) $(HEADER2) $(HEADER3)
	$(CC) $(FLAGS) -c -g $< -o $@ $(INC_PARAMS)

clean :
	@rm -rf $(OBJS1) $(OBJS2)

fclean : clean
	@rm -rf $(SERVER)

variable  :
	@echo SRC : $(SRC)
	@echo OBJS : $(OBJS)
	@echo INC_PARAMS : $(INC_PARAMS)

re : fclean all

.PHONY: all clean fclean re
