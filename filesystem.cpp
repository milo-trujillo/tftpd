#include <stdlib.h>
#include <stdio.h>
#include <strings.h> // bzero,strlen (Unix systems)
#include <string.h> // bzero,strlen (Linux systems)
#include <iostream>
#include <errno.h>

#include "constants.h"


// When passed a error value (errno), this function will return the appropriate
// TFTP error number related to file operations
int checkFileError(int num)
{
    switch(num)
    {
        case 1:
            //Operation not permitted
            return DENIED;
            break;
        case 2:
            //File does not exist
            return NOTFOUND;
            break;
        case 5:
            //IO Error
            return UNDEF;
            break;
        case 13:
            //permission denied
            return DENIED;
            break;
        case 17:
            //file exists
            return FILEEXISTS;
            break;
        case 24:
            //Too many open files
            return ILLEGAL;
            break;
        case 28:
            //no space left
            return ILLEGAL;
            break;
        default:
            return UNDEF;
            break;
    }
}


FILE *openFileRead(char * filename)
{
    FILE * openFile  = NULL;
    openFile = fopen(filename, "r");

    return openFile;

}

FILE *openFileWrite(char * filename)
{
    FILE * openFile  = NULL;
    openFile = fopen(filename, "w");

    return openFile;
}
