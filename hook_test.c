#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <string.h> 

char *(*origin_strcpy)(char *dest, const char *src);

char *strcpy(char *dest, const char *src);

char *strcpy(char *dest, const char *src)
{
	printf("strcpy hooked by hook_test.so!!\n");
	printf("Hooked src contents : %s\n", src);

	origin_strcpy = (char *(*)(char *, const char *))dlsym(RTLD_NEXT, "strcpy");
	return (*origin_strcpy)(dest, src);
}
