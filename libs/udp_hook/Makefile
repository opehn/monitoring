HOOK_DYNAMIC_LIB = send_hooking.so

CC = gcc
FLAGS = -fPIC -shared -ldl

HEADER_DIR1 = ./includes/
HEADER_DIR2 = ../util/includes/
HEADER_DIR3 = ../../agent/includes/

INC = $(HEADER_DIR1) $(HEADER_DIR2) $(HEADER_DIR3)
INC_PARAMS = $(addprefix -I, $(INC))

HEADER_LIST1 := udp_hook.h
HEADER_LIST2 := packet.h \
				util.h
HEADER_LIST3 := agent.h

HEADER1 = $(addprefix $(HEADER_DIR1), $(HEADER_LIST1))
HEADER2 = $(addprefix $(HEADER_DIR2), $(HEADER_LIST2))
HEADER3 = $(addprefix $(HEADER_DIR3), $(HEADER_LIST3))

SRC_DIR1 = ./srcs/
SRC_DIR2 = ../../agent/srcs/
SRC_DIR3 = ../util/srcs/

SRC_LIST1 := udp_hook.c 
SRC_LIST2 := util.c \
			read_file.c
SRC_LIST3 := util.c

SRC1 := $(addprefix $(SRC_DIR1), $(SRC_LIST1))
SRC2 := $(addprefix $(SRC_DIR2), $(SRC_LIST2))
SRC3 := $(addprefix $(SRC_DIR3), $(SRC_LIST3))

all : $(HOOK_DYNAMIC_LIB)

$(HOOK_DYNAMIC_LIB) : $(SRC1) $(SRC2) $(SRC3) $(HEADER1) $(HEADER2) $(HEADER3)
	$(CC) $(FLAGS) $^ -o $@ $(INC_PARAMS)

clean : 
	@rm -rf $(HOOK_DYNAMIC_LIB) 

fclean : 
	@rm -rf $(HOOK_DYNAMIC_LIB) 
re : fclean all

variable :
	@echo INC_PARAMS : $(INC_PARAMS)
	@echo SRC1 : $(SRC1)
	@echo SRC2 : $(SRC2)
	@echo SRC3 : $(SRC3)
	@echo HEADER1 : $(HEADER1)
	@echo HEADER2 : $(HEADER2)


.PHONY : all clean fclean re

