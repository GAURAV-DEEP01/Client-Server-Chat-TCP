#include "../include/concurrency.h"

void SOCK_FlCleanUp(char *err){
    fprintf(stderr,"\nError : %s\nErrCode : %d\n",err, WSAGetLastError());
    WSACleanup();
    return;
}

DWORD WINAPI SOCKTH_receive(LPVOID socket_fh){
    char buffer[BUFFER_SIZE];
    char *exit_cmd = "leave";

    while(1){
        memset(buffer, 0, sizeof(buffer));
        size_t bytes_read;
        if((bytes_read  = recv((SOCKET)socket_fh, buffer, sizeof(buffer),0)) < 0 ){
            SOCK_FlCleanUp("Unable to revieve data!");
            closesocket((SOCKET)socket_fh);
            return 0;
        }else if((bytes_read == 0) || !strncmp(buffer, exit_cmd, strlen(exit_cmd))){
            fprintf(stdout,"\nConnection closed End Of Chat\n");        
            closesocket((SOCKET)socket_fh);
            return 0;
        }
        fprintf(stdout, "Client Message: %s\n",buffer);
    }
}

DWORD WINAPI SOCKTH_send(LPVOID socket_fh){
    char buffer[BUFFER_SIZE];
    while(1){
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        if(send((SOCKET)socket_fh, buffer, strlen(buffer), 0) < 0){
            SOCK_FlCleanUp("Unable to send data!");
            closesocket((SOCKET)socket_fh);
            return 0;
        }
    }
}