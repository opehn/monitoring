AGENT = agent
SERVER = server

CC = gcc
FLAGS = -g -fsanitize=address -Wall -Werror -Wextra

INC = -I$(HEADER_DIR) -I$(LIBFT_DIR)

LIBFT = $(LIBFT_DIR)libft.a
LIBFT_DIR = ./libft/

HEADER_DIR = ./includes/
HEADER_LIST = minitalk.h
HEADERS = $(addprefix $(HEADER_DIR), $(HEADER_LIST))

SRC_DIR = ./srcs/
S_SRC := server.c \
		 util.c \
		 util2.c
S_OBJ = $(S_SRC:.c=.o) 
C_SRC := agent.c \
		 util.c \
		 util2.c
C_OBJ = $(C_SRC:.c=.o) 

all : $(SERVER) $(AGENT)

$(SERVER) : $(LIBFT) $(S_OBJ)
	$(CC) $(FLAGS) $^ -o $@ 

$(AGENT) :$(LIBFT) $(C_OBJ)
	$(CC) $(FLAGS) $^ -o $@ 

%.o : $(SRC_DIR)%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@ $(INC)

$(LIBFT) :
	@$(MAKE) -C $(LIBFT_DIR) all

clean :
	@$(MAKE) -C $(LIBFT_DIR) clean
	@rm $(S_OBJ) $(C_OBJ)

fclean :
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@rm $(S_OBJ) $(C_OBJ) $(SERVER) $(AGENT)

bonus : all
