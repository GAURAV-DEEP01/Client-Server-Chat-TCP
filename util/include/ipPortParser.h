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

// parses socket string ang returns ip (as sting) and port (as unsigned short) 
extern Argstatus SOCK_IpPortParse(char *ipPortString, IpPort *ipPort);


/* handles user arguments 
types
loopback : just to be explicit
    ./client loopback 
default : default with not arguments is loopback address with 9090 port 
    ./client 
Ip and port : pass the ip and port as arguments to connect to custom socket 
    ./client 192.168.29.64:5050
*/
extern Argstatus SOCK_HandleArgs(int argc, char* argv[], IpPort *ipPort);

#endif