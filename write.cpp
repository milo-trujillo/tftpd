#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // bzero,strlen (Unix systems)
#include <string.h> // bzero,strlen (Linux systems)
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h> // INADDR_ANY
#include <arpa/inet.h> // ntohs
#include <sys/time.h>

#include "constants.h"
#include "write.h"

/*
                         2 bytes     2 bytes
                         ---------------------
                        | Opcode |   Block #  |
                         ---------------------

                         Figure 5-3: ACK packet
*/

void sendAck(int socket, struct sockaddr_in client, int block)
{
	char ack[4] = "\x00\x04\x00";
	unsigned short blockno = htons(block);
	ack[2] = ((char*)&blockno)[0];
	ack[3] = ((char*)&blockno)[1];
	socklen_t clientLen = sizeof(client);
	sendto(socket, ack, 4, 0, (struct sockaddr*)&client, clientLen);
}

// First, check whether we're allowed to create the file, etc
// if we aren't, then send an error to the client and abort
FILE* startFilesystem(int socket, struct sockaddr_in client, char* filename)
{
	socklen_t clientLen = sizeof(client);
	//FILE* f = NULL;
	FILE* f = (FILE*)1;
	// f = openFileWrite(filename);
	if( f <= 0 )
	{
		char error[] = "\x00\x05\x00\x00Something went wrong";
		sendto(socket, error, strlen(error+4)+4, 0, (struct sockaddr*)&client, clientLen);
		exit(EXIT_FAILURE);
	} else {
		sendAck(socket, client, 0);
	}
	return f;
}

// Blocks until timeout or data available
// On timeout, kills the child
void blockOnDataAvailable(int socket, struct sockaddr_in client, int blocksSeen)
{
	fd_set inputs;
	struct timeval timeout;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	FD_ZERO(&inputs);
	for( int timeouts = 0; timeouts < MAX_TIMEOUT; timeouts++ )
	{
		FD_SET(socket, &inputs);
		int ready = select(socket+1, &inputs, NULL, NULL, &timeout);
		if( ready == -1 ) // Input error
			exit(EXIT_FAILURE);
		else if( FD_ISSET(socket, &inputs) ) // Data is here!
			return;
		sendAck(socket, client, blocksSeen);
	}
	printf("Connection timed out\n");
	exit(EXIT_SUCCESS);
}

/*
                   2 bytes     2 bytes      n bytes
                   ----------------------------------
                  | Opcode |   Block #  |   Data     |
                   ----------------------------------

                        Figure 5-2: DATA packet
*/

void writeRequest(char* filename, char* mode, struct sockaddr_in client)
{
	int blocksSeen = 0; // Track the last known block
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	socklen_t clientLen = sizeof(client);

	FILE* f = startFilesystem(sock, client, filename);

	// Read a block at a time until all data received, error, or timeout
	while(1)
	{
		char block[DATABLOCK_SIZE];
		unsigned short opcode = 0;
		unsigned short blockno = 0;

		blockOnDataAvailable(sock, client, blocksSeen);

		// We got an actual block, let's process it
		ssize_t bytes = recvfrom(sock, block, DATABLOCK_SIZE, 0, (struct sockaddr*)&client, &clientLen);
		timeouts = 0;
		opcode  = ntohs(*(unsigned short*)(block));   // Dark magic
		blockno = ntohs(*(unsigned short*)(block+2)); // from the deeps
		//printf("Got block #%hu, %ld long, opcode %hu\n", blockno, bytes, opcode);
		if( opcode == DATA && blockno == blocksSeen + 1 )
		{
			blocksSeen++;
			sendAck(sock, client, blockno);
			if( bytes < DATABLOCK_SIZE )
			{
				//printf("Final block, killing child\n");
				//fclose(f);
				exit(EXIT_SUCCESS);
			}
		} else if( opcode != DATA ) {
			printf("Invalid opcode %d\n", opcode);
			exit(EXIT_FAILURE);
		} else {
			printf("Expected block #%d got #%d\n", blocksSeen+1, blockno);
		}
	}
}
