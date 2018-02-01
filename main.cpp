#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <strings.h> // bzero,strtok (Unix systems)
#include <string.h> // bzero,strtok (Linux systems)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // INADDR_ANY

#include "constants.h"
#include "write.h"

#define BUFFER_SIZE 256

// Parse that datagram, figure out what type of request it is
void processRequest(char* req, struct sockaddr_in client)
{
	char type = req[1];
	char* filename = strtok(req + 2, "\0");
	int fnamelength = strlen(filename);
	char* mode = strtok(req + 3 + fnamelength, "\0");
	int modelength = strlen(mode);

	if( type == RRQ || type == WRQ )
	{
		pid_t pid = fork();
		if( pid == 0 )
		{
			char* fname = (char*)malloc(fnamelength);
			char* m = (char*)malloc(modelength);
			strncpy(fname, filename, fnamelength);
			strncpy(m, mode, modelength);
			if( type == RRQ )
				printf("Would trigger read handler for '%s'\n", fname);
			else
				writeRequest(fname, mode, client);
			exit(EXIT_SUCCESS);
		}
	} else {
		printf("Invalid opcode %d\n", type);
	}
}

// Load next datagram
void handleNewConnection(int sock)
{
	char buffer[BUFFER_SIZE];
	struct sockaddr_in client;
	int read = 0;
	socklen_t clientSize = sizeof(client);
	bzero(&client, clientSize);
	read = recvfrom(sock, buffer, BUFFER_SIZE-1, 0, (struct sockaddr*)&client, &clientSize);
	buffer[read] = '\0';
	processRequest(buffer, client);
}

// Sets up original listening socket
int main()
{
	int udpServerSock;
	int options = 1; // Immediately release the address on program exit
	struct sockaddr_in udpServerStruct;
	socklen_t sa_len = sizeof(udpServerStruct);

	// Set up the UDP overhead
	udpServerSock = socket(AF_INET, SOCK_DGRAM, 0);
	setsockopt(udpServerSock, SOL_SOCKET, SO_REUSEADDR, (const void*)&options, sizeof(int));
	udpServerStruct.sin_family = AF_INET;
	udpServerStruct.sin_port = htons(0); // Let the system choose a port
	udpServerStruct.sin_addr.s_addr = htonl(INADDR_ANY);

	// Attempt the binding
	if( bind(udpServerSock, (struct sockaddr*)&udpServerStruct, sa_len) < 0 )
	{
		fprintf(stderr, "ERROR: Unable to bind udp socket\n");
		exit(EXIT_FAILURE);
	} else {
		getsockname(udpServerSock, (struct sockaddr*)&udpServerStruct, &sa_len);
		printf("Bound to UDP port: %d\n", ntohs(udpServerStruct.sin_port));
	}

	while(1)
		handleNewConnection(udpServerSock);

	close(udpServerSock);
	return 0;
}
