#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <string>

//Struct to hold file system error messages and error numbers
struct fileError {
    int sysError;
    int tftpError;

    char msg[30];

    int headerNum;
    char header[8][35] =
    {
        {"\x00\x05\x00\x02Operation not permitted."},
        {"\x00\x05\x00\x01 File does not exist."},
        {"\x00\x05\x00\x00IO Error."},
        {"\x00\x05\x00\x02Permission Denied."},
        {"\x00\x05\x00\x05 File already exists."},
        {"\x00\x05\x00\x03Too many open files."},
        {"\x00\x05\x00\x03No space left on disk."},
        {"\x00\x05\x00\x00Undefined error."}
    };
};

fileError checkFileError(int num);
std::string sanitizePath(std::string path);
FILE *openFileRead(char * filename);
FILE *openFileWrite(char * filename);

#endif
