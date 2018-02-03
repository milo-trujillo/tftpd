#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <string>

//Struct to hold file system error messages and error numbers
struct fileError {
    int sysError;
    int tftpError;
    const char* msg;
};

fileError checkFileError(int num);
std::string sanitizePath(std::string path);
FILE *openFileRead(char * filename);
FILE *openFileWrite(char * filename);

#endif
