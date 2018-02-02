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

void readRequest(char* filename, char* mode, struct sockaddr_in client) {

}
