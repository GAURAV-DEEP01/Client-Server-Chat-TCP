#include "../include/concurrency.h"

DWORD WINAPI SOCKTH_revieve(char *err){
    fprintf(stderr,"\nError : %s\nErrCode : %d\n",err, WSAGetLastError());
    WSACleanup();
    return;
}

DWORD WINAPI SOCKTH_revieve(SOCKET socket_fh){
    char buffer[BUFFER_SIZE];
    
    while(1){
        memset(buffer, 0, sizeof(buffer));
        size_t bytes_read;
        if((bytes_read  = recv(socket_fh, buffer, sizeof(buffer),0)) < 0 ){
            SOCK_FlCleanUp("Unable to revieve data!");
            closesocket(socket_fh);
            return 1;
        }else if(bytes_read == 0){
            fprintf(stdout,"Connection closed by Client\n");
            closesocket(socket_fh);
        }else{
            fprintf(stdout, "Client Message: %s\n",buffer);
        }
    }
}

DWORD WINAPI SOCKTH_send(SOCKET socket_fh){
    char buffer[BUFFER_SIZE];
    while(1){
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        if(send(socket_fh, buffer, strlen(buffer), 0) < 0){
            SOCK_FlCleanUp("Unable to send data!");
            closesocket(socket_fh);
            return NULL;
        }
    }
}