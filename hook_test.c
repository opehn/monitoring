#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h> 

ssize_t (*origin_send)(int, const void *, size_t, int);

ssize_t send(int sockfd, const void *buf, size_t len, int flags)
{
	printf("send hooked by hook_test.so!!\n");
	printf("Hooked len: %ld\n", len);

	origin_send = (ssize_t(*)(int, const void *, size_t, int))dlsym(RTLD_NEXT, "send");
	return (*origin_send)(sockfd, buf, len, flags);
}
