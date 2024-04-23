#include "../include/ipPortParser.h"

Argstatus SOCK_IpPortParse(char *ipPortString, IpPort *ipPort){
    char *seperator = strchr(ipPortString, ':');
    char *port  = seperator + 1;

    if((ipPort->port = atoi(port)) == 0)
        return ARG_ERROR; 

    int lenfrmsep = strlen(seperator); 
    strncpy(ipPort->ip,ipPortString, strlen(ipPortString) - lenfrmsep);
    memset(&ipPort->ip[strlen(ipPortString) - lenfrmsep],0,sizeof(ipPort));
    printf("ip is : %s\nport is %d\n",ipPort->ip, ipPort->port );
    return IP_SOCKET;
}

Argstatus SOCK_HandleArgs(int argc, char* argv[], IpPort *ipPort){
    if(argc == 1){
        strcpy(ipPort->ip, LOOPBACK_ADDRESS);
        ipPort->port = DEFAULT_PORT;  
        return NO_ARGS;
    }   
    if(strcmp(argv[1] ,"loopback") == 0){
        strcpy(ipPort->ip, LOOPBACK_ADDRESS);
        if(argc == 3){
            unsigned short port = atoi(argv[2]); 
            if ( port == 0 ) 
                return ARG_ERROR;
            ipPort->port = port; 
        }else
            ipPort->port = DEFAULT_PORT;
        return LOOP_BACK;    
    }
    return SOCK_IpPortParse(argv[1], ipPort) == ARG_ERROR ? ARG_ERROR : IP_SOCKET;
}