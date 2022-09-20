#include "agent.h"
#include <dlfcn.h>

ssize_t	*send(int sockfd, const void *buf, size_t len, int flags)
{
	orgin_send = (ssize_t *(*)(int, const void, size_t, int))dlsym(RTLD_NEXT, "send");
	return (*origin_send)(sockfd, buf, len, flags);
}
