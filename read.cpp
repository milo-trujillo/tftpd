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
		exit(EXIT_FAILURE);
	}
	return f;
}


void readRequest(char* filename, char* mode, struct sockaddr_in client) {

    int blocksSent = 0; // Track the number of blocks sent
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	socklen_t clientLen = sizeof(client);

	FILE* f = getFile(sock, client, filename);

    char buffer[DATABLOCK_SIZE];
    if (f != NULL)
    {
        int read = 0;
        while ((read = fread(buffer, 1, DATABLOCK_SIZE, f)) > 0)
        {
            for (int i=0; i < read; i++)
            {

            }
        }
    }




}
