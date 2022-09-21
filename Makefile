all:
	@$(MAKE) -C ./libs/udp_hook
	@$(MAKE) -C ./agent
	@$(MAKE) -C ./server
fclean:
	@$(MAKE) fclean -C ./libs/udp_hook
	@$(MAKE) fclean -C ./agent
	@$(MAKE) fclean -C ./server
clean:
	@$(MAKE) clean -C ./libs/udp_hook
	@$(MAKE) clean -C ./agent
	@$(MAKE) clean -C ./server
re:
	@$(MAKE) re -C ./libs/udp_hook
	@$(MAKE) re -C ./agent
	@$(MAKE) re -C ./server
hook:
	@$(MAKE) -C ./libs/udp_hook
server:
	@$(MAKE) -C ./libs
	@$(MAKE) -C ./server
agent:
	@$(MAKE) -C ./libs
	@$(MAKE) -C ./agent

.PHONY: all fclean clean re server agent SMSutils collector
