#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "daemonize.h"

int main()
{
	printf("Daemonizing...\n");
	int daemonized = daemonize();
	if( daemonized )
	{
		int proof = open("daemonization_succeeded", O_RDWR | O_CREAT, 0644);
		close(proof);
	} else {
		fprintf(stderr, "Something went wrong with daemonization!\n");
	}
	return 0;
}
