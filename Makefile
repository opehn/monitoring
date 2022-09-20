all:
	make -C ./libs/udp_hook
	make -C ./agent
	make -C ./server
fclean:
	make fclean -C ./libs/udp_hook
	make fclean -C ./agent
	make fclean -C ./server
clean:
	make clean -C ./libs/udp_hook
	make clean -C ./agent
	make clean -C ./server
re:
	make re -C ./libs/udp_hook
	make re -C ./agent
	make re -C ./server
hook:
	make -C ./libs/udp_hook
server:
	make -C ./libs
	make -C ./server
agent:
	make -C ./libs
	make -C ./agent

.PHONY: all fclean clean re server agent SMSutils collector
