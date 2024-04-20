#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

#define SERVER_ADDRESS "127.0.0.1"
#define BUFFER_SIZE 1000

void SOCK_FlCleanUp(char* err){
    fprintf(stderr,"\nError : %s\nErrCode : %d\n",err, WSAGetLastError());
    WSACleanup();
    return;
}


int main(int argc, char* argv[]){
    WORD WSA_vRequested = MAKEWORD(2,2);
    WSADATA WSAData;
    short port_no = 9090;

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


    // setting up sever port and address family for binding 
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_no);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);


    // bind 
    if(bind(socket_fh,(struct sockaddr*)&server, sizeof(server)) < 0){
        SOCK_FlCleanUp("Failed to bind!");
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Binding Successfull...\n");

     
    // listening..
    
    if(listen(socket_fh, 5) < 0){
        SOCK_FlCleanUp("Failed to listen!");
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Listen to client on port '%hd'successfully...\n",port_no); 

    // accept 
    int server_len = sizeof(server);
    SOCKET new_socket_fh;
    if((new_socket_fh = accept(socket_fh,(struct sockaddr*)&server, &server_len)) < 0){
        SOCK_FlCleanUp("Failed to Accept Requests!");
        closesocket(socket_fh);
        return 1;
    }
    char buffer[BUFFER_SIZE] = {0};
    do{
        memset(buffer, 0, sizeof(buffer));
        size_t bytes_read;
        if((bytes_read  = recv(new_socket_fh, buffer, sizeof(buffer),0)) < 0 ){
            SOCK_FlCleanUp("Unable to revieve data!");
            closesocket(new_socket_fh);
            return 1;
        }else if(bytes_read == 0){
            fprintf(stdout,"Connection closed by Client\n");
            closesocket(new_socket_fh);
        }else{
            fprintf(stdout, "Client Message: %s\n",buffer);
        }

        // sending
        memset(buffer, 0, sizeof(buffer));
        printf("Server: ");
        fgets(buffer, sizeof(buffer), stdin);
        size_t bytes_sent;
        if(bytes_sent = send(new_socket_fh, buffer, strlen(buffer), 0) < 0){
            SOCK_FlCleanUp("Unable to send data!");
            closesocket(new_socket_fh);
            return 1;
        }

    }while(1);

    
    closesocket(socket_fh);
    WSACleanup();
}