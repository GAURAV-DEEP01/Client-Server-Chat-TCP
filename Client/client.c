#include "../util/include/concurrency.h"
#include "../util/include/ipPortParser.h"

#define CLIENT_NAME "CLIENT"


int main(int argc, char* argv[]){
    IpPort ipPort;
    WORD WSA_vRequested = MAKEWORD(2,2);
    WSADATA WSAData;
    
    if(SOCK_HandleArgs(argc, argv, &ipPort) == ARG_ERROR){
        fprintf(stderr,"Invalid arguments!\n");
        return 1;
    }

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
    server.sin_port = htons(ipPort.port);
    server.sin_addr.S_un.S_addr = inet_addr(ipPort.ip);

    if(connect(socket_fh, (struct sockaddr*)&server, sizeof(server)) < 0){
        SOCK_FlCleanUp("Unable to connect!");
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Connected...\n");

    // sending/receiving 
    HANDLE senderThread, receiverThread;
    DWORD senderThreadID, receiverThreadId;
    
    SendRecvDescipt sendRecvDesc;
    sendRecvDesc.sockfh = socket_fh;
    strncpy(sendRecvDesc.application, CLIENT_NAME, strlen(CLIENT_NAME));

    senderThread = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)SOCKTH_send, (LPVOID)&sendRecvDesc, 0, &senderThreadID);
    if(senderThread == NULL){
        fprintf(stderr, "Sender thread Failed!\n");
        WSACleanup();
        closesocket(socket_fh);
        return 1;
    }
    fprintf(stdout, "Sender threadID : %X\n", senderThreadID);

    receiverThread = CreateThread(NULL, 0 , (LPTHREAD_START_ROUTINE)SOCKTH_receive, (LPVOID)&sendRecvDesc, 0, &receiverThreadId);
    if(!receiverThread){
        fprintf(stderr, "Recevier thread Failed!\n");
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