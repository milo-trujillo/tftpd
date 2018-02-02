#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <string>
int checkFileError(int num);
std::string sanitizePath(std::string path);
FILE *openFileRead(char * filename);
FILE *openFileWrite(char * filename);

#endif
