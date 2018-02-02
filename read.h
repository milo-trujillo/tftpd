#ifndef READ_H
#define READ_H

#include <sys/socket.h>

void readRequest(char* filename, char* mode, struct sockaddr_in client);

#endif
