#include <string.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	printf("String copy src : %s\n", argv[1]);
	char tmp[100];
	printf("Trying to string copy src to tmp buf...\n");
	strcpy(tmp, argv[1]);
	printf("tmp buffer : %s\n", tmp);
	return (0);
}
