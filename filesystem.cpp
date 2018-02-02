#include <stdlib.h>
#include <stdio.h>
#include <strings.h> // bzero,strlen (Unix systems)
#include <string.h> // bzero,strlen (Linux systems)
#include <string>
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

// Removes illegal characters from filepath
std::string sanitizePath(std::string path)
{
    std::string removeChars = "\\/:?\"<>| ";
    for (int c = 0; c < (int)path.length(); c++)
    {
        bool found = removeChars.find(path[c]) != std::string::npos;
        if (found)
        {
            path[c] = '\0';
        }
    }
    return path;
}


FILE *openFileRead(char * filename)
{
    std::string cleanFilename = sanitizePath(filename);

    FILE * openFile  = NULL;
    openFile = fopen(cleanFilename.c_str(), "r");
    return openFile;
}

FILE *openFileWrite(char * filename)
{
    std::string cleanFilename = sanitizePath(filename);

    FILE * openFile  = NULL;
    openFile = fopen(cleanFilename.c_str(), "w");
    return openFile;
}
