#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

#define SERVER_ADDRESS "127.0.0.1"


void SOCK_FlCleanUp(char* err){
    fprintf(stderr,"Error : %s\nErrCode : %d\n",err, WSAGetLastError());
    WSACleanup();
    return;
}


int main(int argc, char* argv[]){
    WORD WSA_vRequested = MAKEWORD(2,2);
    WSADATA WSAData;
    short port_no = 443;

    // startup 
    if(WSAStartup(WSA_vRequested,&WSAData) != 0){
        SOCK_FlCleanUp("WSA Startup failed");
        return 1;
    }    
    fprintf(stdout, "WSAStartup Successfull...\n"); 

    
    // socket  
    SOCKET socket_fh;
    if((socket_fh  = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        SOCK_FlCleanUp("Socket creation failed!");
        return 1;   
    }        
    fprintf(stdout, "Socket creation Successfull...\n"); 


    // setting up port and af
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_no);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);

    if(connect(socket_fh, (struct sockaddr*)&server, sizeof(server)) < 0){
        SOCK_FlCleanUp("Unable to connect!");
        closesocket(socket_fh);
        return 1;
    }

    // sending
    char buffer[500] = {0};
    do{
        memset(buffer, 0, sizeof(buffer));
        printf("Client: ");
        fgets(buffer, 500, stdin);
        size_t bytes_sent;
        if((bytes_sent = send(socket_fh, buffer, strlen(buffer), 0)) < 0){
            SOCK_FlCleanUp("Unable to send data!");
            closesocket(socket_fh);
            return 1;
        }

        size_t bytes_read;
        memset(buffer, 0, sizeof(buffer));
        if((bytes_read  = recv(socket_fh, buffer, sizeof(buffer),0)) < 0 ){
            SOCK_FlCleanUp("Unable to revieve data!");
            closesocket(socket_fh);
            return 1;
        }else if(bytes_read == 0){
            fprintf(stdout,"Connection closed by Client\n");
            WSACleanup();
            closesocket(socket_fh);
            return 1;
        }else
            fprintf(stdout, "Server Message: %s\n",buffer);
    }while(1);

    
    closesocket(socket_fh);
    WSACleanup();
}