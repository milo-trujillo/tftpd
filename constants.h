#ifndef CONSTANTS_H
#define CONSTANTS_H

#define FALSE 0
#define TRUE 1

// After 5 seconds, abort
#define MAX_TIMEOUT 5

// TFTP OpCodes
#define RRQ   1
#define WRQ   2
#define DATA  3
#define ACK   4
#define ERROR 5

// TFTP Error Codes
#define UNDEF      0
#define NOTFOUND   1
#define DENIED     2
#define ILLEGAL    3
#define UNKNOWNTID 4
#define FILEEXISTS 5
#define NOSUCHUSER 6

// 512 bytes of data, 2 bytes opcode, 2 bytes block number
#define DATABLOCK_SIZE 516

#endif
