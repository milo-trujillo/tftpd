#include <stdlib.h>
#include <stdio.h>
#include <strings.h> // bzero,strlen (Unix systems)
#include <string.h> // bzero,strlen (Linux systems)
#include <string>
#include <iostream>
#include <stdio.h>
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
            strcpy(error.msg, "Operation not permitted.");
            error.headerNum = 0;
            return error;
            break;
        case 2:
            //File does not exist
            error.sysError = num;
            error.tftpError = NOTFOUND;
            strcpy(error.msg, "File does not exist.");
            error.headerNum = 1;
            return error;
            break;
        case 5:
            //IO Error
            error.sysError = num;
            error.tftpError = UNDEF;
            strcpy(error.msg, "IO Error.");
            error.headerNum = 2;
            return error;
            break;
        case 13:
            //permission denied
            error.sysError = num;
            error.tftpError = DENIED;
            strcpy(error.msg, "Permission Denied.");
            error.headerNum = 3;
            return error;
            break;
        case 17:
            //file exists
            error.sysError = num;
            error.tftpError = FILEEXISTS;
            strcpy(error.msg, "File already exists.");
            error.headerNum = 4;
            return error;
            break;
        case 24:
            //Too many open files
            error.sysError = num;
            error.tftpError = ILLEGAL;
            strcpy(error.msg, "Too many open files.");
            error.headerNum = 5;
            return error;
            break;
        case 28:
            //no space left
            error.sysError = num;
            error.tftpError = ILLEGAL;
            strcpy(error.msg, "No space left on disk.");
            error.headerNum = 6;
            return error;
            break;
        default:
            error.sysError = num;
            error.tftpError = UNDEF;
            strcpy(error.msg, "Undefined error.");
            error.headerNum = 7;
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
    openFile = fopen(cleanFilename.c_str(), "rb");
    return openFile;
}

FILE *openFileWrite(char * filename)
{
    std::string cleanFilename = sanitizePath(filename);

    FILE * openFile  = NULL;
    openFile = fopen(cleanFilename.c_str(), "wbx");
    return openFile;
}
