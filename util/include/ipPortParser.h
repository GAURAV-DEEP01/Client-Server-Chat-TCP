#ifndef IPPORTPARSER_H
#define IPPORTPARSER_H
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LOOPBACK_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 9090

typedef struct {
    char ip[16]; 
    unsigned short port;
} IpPort;

typedef enum {
    ARG_ERROR,
    NO_ARGS,
    LOOP_BACK,
    IP_SOCKET
} Argstatus;

extern Argstatus SOCK_IpPortParse(char *ipPortString, IpPort *ipPort);

extern Argstatus SOCK_HandleArgs(int argc, char* argv[], IpPort *ipPort);

#endif