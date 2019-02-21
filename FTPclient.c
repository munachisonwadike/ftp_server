#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	// currently assuming 39 characters for IPv6
	char ip[39];
	int port;

	strcpy(ip, argv[1]);
	sscanf(argv[2], "%d", &port);


	printf("%s\t%d\n\n\n", ip, port);
	return 0;
}

