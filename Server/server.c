#include "../util/include/concurrency.h"
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

// Any intereface
#define SERVER_ADDRESS " "
#define DEFAULT_PORT 9090

#define SERVER_NAME "SERVER"

int main(int argc, char* argv[]){
    WORD WSA_vRequested = MAKEWORD(2,2);
    WSADATA WSAData;
    short port_no = argc > 1 ? atoi(argv[1]) : DEFAULT_PORT;

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


    // setting up sever port ,address family and addr  
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
    fprintf(stdout, "Listen to client on port '%hd'...\n",port_no); 

    // accept 
    int server_len = sizeof(server);
    SOCKET new_socket_fh;
    if((new_socket_fh = accept(socket_fh,(struct sockaddr*)&server, &server_len)) < 0){
        SOCK_FlCleanUp("Failed to Accept Requests!");
        closesocket(socket_fh);
        return 1;
    }

    // sender / receiver
    HANDLE senderThread, receiverThread;
    DWORD senderThreadID, receiverThreadId;

    SendRecvDescipt sendRecvDesc;
    sendRecvDesc.sockfh = new_socket_fh;
    strncpy(sendRecvDesc.application, SERVER_NAME, strlen(SERVER_NAME));

    senderThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SOCKTH_send, (LPVOID)&sendRecvDesc, 0, &senderThreadID);
    if(!senderThread){
        fprintf(stderr, "Sender thread Failed!\n");
        WSACleanup();
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Sender threadID : %X\n", senderThreadID);

    receiverThread = CreateThread(NULL, 0 , (LPTHREAD_START_ROUTINE)SOCKTH_receive, (LPVOID)&sendRecvDesc, 0, &receiverThreadId);
    if(!receiverThread){
        fprintf(stderr, "Receiver thread Failed!\n");
        WSACleanup();
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Receiver threadID : %X\n", receiverThreadId);
    fprintf(stdout, "\n\t-Chat Here-\n");

    WaitForSingleObject(receiverThread, INFINITE);

    closesocket(socket_fh);
    WSACleanup();
}