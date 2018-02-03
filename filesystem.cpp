#include <stdlib.h>
#include <stdio.h>
#include <strings.h> // bzero,strlen (Unix systems)
#include <string.h> // bzero,strlen (Linux systems)
#include <string>
#include <iostream>
#include <errno.h>

#include "constants.h"
#include "filesystem.h"



// When passed a error value (errno), this function will return a fileError
// struct which contains the error message and error numbers
fileError checkFileError(int num)
{
    fileError error;
    switch(num)
    {
        case 1:
            //Operation not permitted
            error.sysError = num;
            error.tftpError = DENIED;
            error.msg = "Operation not permitted.";
            return error;
            break;
        case 2:
            //File does not exist
            error.sysError = num;
            error.tftpError = NOTFOUND;
            error.msg = "File does not exist.";
            return error;
            break;
        case 5:
            //IO Error
            error.sysError = num;
            error.tftpError = UNDEF;
            error.msg = "IO Error.";
            return error;
            break;
        case 13:
            //permission denied
            error.sysError = num;
            error.tftpError = DENIED;
            error.msg = "Permission Denied.";
            return error;
            break;
        case 17:
            //file exists
            error.sysError = num;
            error.tftpError = FILEEXISTS;
            error.msg = "File already exists.";
            return error;
            break;
        case 24:
            //Too many open files
            error.sysError = num;
            error.tftpError = ILLEGAL;
            error.msg = "Too many open files.";
            return error;
            break;
        case 28:
            //no space left
            error.sysError = num;
            error.tftpError = ILLEGAL;
            error.msg = "No space left on disk.";
            return error;
            break;
        default:
            error.sysError = num;
            error.tftpError = UNDEF;
            error.msg = "Undefined error.";
            return error;
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
