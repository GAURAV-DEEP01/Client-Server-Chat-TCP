#include "../include/concurrency.h"

void SOCK_FlCleanUp(char *err){
    fprintf(stderr,"\nError : %s\nErrCode : %d\n",err, WSAGetLastError());
    WSACleanup();
    return;
}

DWORD WINAPI SOCKTH_receive(SendRecvDescipt *sendRecvDesc){
    char buffer[BUFFER_SIZE];
    char *exit_cmd = "leave";

    while(1){
        memset(buffer, 0, sizeof(buffer));
        size_t bytes_read;
        if((bytes_read  = recv(sendRecvDesc->sockfh, buffer, sizeof(buffer),0)) < 0 ){
            SOCK_FlCleanUp("Unable to revieve data!");
            closesocket(sendRecvDesc->sockfh);
            return 0;
        } 
        char *msgStartPtr = strchr(buffer, ':');
        msgStartPtr = msgStartPtr + 2;
        if((bytes_read == 0) || !strncmp(msgStartPtr, exit_cmd, strlen(exit_cmd))){
            fprintf(stdout,"\nConnection closed End Of Chat\n");        
            closesocket(sendRecvDesc->sockfh);
            return 0;
        }
        fprintf(stdout, "%s\n",buffer);
    }
}

DWORD WINAPI SOCKTH_send(SendRecvDescipt *sendRecvDesc){
    char *seperator = " : ";
    strncat(sendRecvDesc->application, seperator, strlen(seperator));

    char buffer[BUFFER_SIZE];
    int senderWithSepLen = strlen(sendRecvDesc->application);
    strncat(buffer, sendRecvDesc->application, senderWithSepLen);

    while(1){
        memset(&buffer[senderWithSepLen], 0, sizeof(buffer) - senderWithSepLen);

        fgets(&buffer[senderWithSepLen], sizeof(buffer) - senderWithSepLen, stdin);
        int bufferlen = strlen(buffer);
        buffer[bufferlen - 1] == '\n' ? buffer[bufferlen - 1] = '\0': 0; 
        if(send(sendRecvDesc->sockfh, buffer, strlen(buffer), 0) < 0){
            SOCK_FlCleanUp("Unable to send data!");
            closesocket(sendRecvDesc->sockfh);
            return 0;
        }
    }
}
