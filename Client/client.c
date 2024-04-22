#include "../util/include/concurrency.h"
#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<WinSock2.h>

#define SERVER_ADDRESS "127.0.0.1"

int main(int argc, char* argv[]){
    WORD WSA_vRequested = MAKEWORD(2,2);
    WSADATA WSAData;
    short port_no = 9090;
    
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


    // setting up port, AF and addr
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port_no);
    server.sin_addr.S_un.S_addr = inet_addr(SERVER_ADDRESS);

    if(connect(socket_fh, (struct sockaddr*)&server, sizeof(server)) < 0){
        SOCK_FlCleanUp("Unable to connect!");
        closesocket(socket_fh);
        return 1;
    }

    // sending/receiving 
    HANDLE senderThread, receiverThread;
    DWORD senderThreadID, receiverThreadId;
    
    senderThread = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)SOCKTH_send, (LPVOID)socket_fh, 0, &senderThreadID);
    if(senderThread == NULL){
        fprintf(stderr, "Sender thread Failed!\n");
        WSACleanup();
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Sender threadID : %X\n", senderThreadID);

    receiverThread = CreateThread(NULL, 0 , (LPTHREAD_START_ROUTINE)SOCKTH_receive, (LPVOID)socket_fh, 0, &receiverThreadId);
    if(!receiverThread){
        fprintf(stderr, "Recevier thread Failed!\n");
        WSACleanup();
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Receiver threadID : %X\n", receiverThreadId);
    

    WaitForSingleObject(receiverThread, INFINITE);

    closesocket(socket_fh);
    WSACleanup();
}