#ifndef READ_H
#define READ_H

#include <sys/socket.h>

void readRequest(char* filename, char* mode, struct sockaddr_in client);
void sendData(char buffer[], int blockNumber, int read, int sock, struct sockaddr_in client);

#endif
