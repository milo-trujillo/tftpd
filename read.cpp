#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // bzero,strlen (Unix systems)
#include <string.h> // bzero,strlen (Linux systems)
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h> // INADDR_ANY
#include <arpa/inet.h> // ntohs
#include <sys/time.h>
#include <iostream>

#include "constants.h"
#include "filesystem.h"



// First, check whether we're allowed to read the file, etc
// if we aren't, then send an error to the client and abort
FILE* getFile(int socket, struct sockaddr_in client, char* filename)
{
	socklen_t clientLen = sizeof(client);
	FILE* f = NULL;
	f = openFileRead(filename);
    fileError error;
	if( f <= 0 )
	{
		error = checkFileError(errno);
		sendto(socket, error.header[error.headerNum], strlen(error.header[error.headerNum]+4)+4, 0, (struct sockaddr*)&client, clientLen);
		fclose(f);
		exit(EXIT_FAILURE);
	}
	return f;
}

// Blocks until ack message is received
// On timeout, kills the child
void waitForAck(int socket, struct sockaddr_in client)
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
		else if( FD_ISSET(socket, &inputs) ) // ACK packet arrived
			return;
	}
	printf("Connection timed out\n");
	exit(EXIT_SUCCESS);
}


void sendData(char buffer[], int read, int blockNumber, int sock, struct sockaddr_in client)
{
	char head[4] = "\x00\x03\x00";
	unsigned short blockN = htons(blockNumber);
	head[2] = ((char*)&blockN)[0];
	head[3] = ((char*)&blockN)[1];

	char packetData[4 + strlen(buffer) + 2];
	bzero(packetData, 4 + strlen(buffer) + 2);


    // Stuff header info into the packet buffer
	for (int i = 0; i < 4; i++)
	{
		packetData[i] = head[i];
	}

    // Stuff file data into packet buffer
	for (int i = 4; i < read + 4; i++)
	{
		packetData[i] = buffer[i-4];
	}


	socklen_t clientLen = sizeof(client);
	int ret = sendto(sock, packetData, 4+strlen(buffer), 0, (struct sockaddr*)&client, clientLen);
	std::cout << "RET=" << ret << "\n";
	if (ret <= 0) {
		std::cout << "ERROR SENDING DATA!\n";
	}
}


void readRequest(char* filename, char* mode, struct sockaddr_in client) {

	char block[DATABLOCK_SIZE];

    int blockNumber = 1; // Track the number of blocks sent
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	socklen_t clientLen = sizeof(client);

	FILE* f = getFile(sock, client, filename);

    char buffer[DATABLOCK_SIZE];
	bzero(buffer, DATABLOCK_SIZE);

	unsigned short opcode = 0;
	unsigned short acknum = 0;
	bool ackRecieved = false;

    if (f != NULL)
    {
        int read = 0;
        while ((read = fread(buffer, 1, DATABLOCK_SIZE-4, f)) > 0)
        {
			// Set flag to track that we haven't received an ACK yet
			ackRecieved = false;

			while (ackRecieved == false)
			{
				// Send packet to client
				sendData(buffer, read, blockNumber, sock, client);

				std::cout << "WAITING FOR ACK.\n";
				// Wait for an ACK packet to be sent back
				waitForAck(sock, client);

				// We received a response, read the data
				bzero(block, DATABLOCK_SIZE);
				ssize_t bytes = recvfrom(sock, block, DATABLOCK_SIZE, 0, (struct sockaddr*)&client, &clientLen);
				opcode  = ntohs(*(unsigned short*)(block));   // convert from network byte order
				acknum = ntohs(*(unsigned short*)(block+2)); // convert from network byte order
				std::cout << "PACKET RECIEVED. OPCODE: " << opcode << " BLOCK#: " << acknum << std::endl;

				// If the packet we got back was a ACK packet with the correct
				// block number we can move on to the next packet
				if ((opcode == 4) && (acknum == blockNumber))
				{
					blockNumber++;
					ackRecieved = true;
					bzero(buffer, DATABLOCK_SIZE);
				}
			}
        }
		fclose(f);
		exit(EXIT_SUCCESS);
    } else {
		printf("Bad file handle!\n");
		fclose(f);
		exit(EXIT_FAILURE);
	}




}
