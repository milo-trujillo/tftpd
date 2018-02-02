#ifndef FILESYSTEM_H
#define FILESYSTEM_H

int checkFileError(int num);
FILE *openFileRead(char * filename);
FILE *openFileWrite(char * filename);

#endif
