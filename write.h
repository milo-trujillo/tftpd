#ifndef WRITE_H
#define WRITE_H

#include <sys/socket.h>

void writeRequest(char* filename, char* mode, struct sockaddr_in client);

#endif
