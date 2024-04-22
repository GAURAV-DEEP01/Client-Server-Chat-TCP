#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<winsock2.h>

#define BUFFER_SIZE 1000

// integrating...

typedef struct{
    SOCKET sockfh;
    char application[10];
}SendRecvDescipt;

// socket error status & makes WSA cleanup 
extern void SOCK_FlCleanUp(char* err);

// thread funciton for receving, param changed to senderReceverDescription  
extern DWORD WINAPI SOCKTH_receive(SendRecvDescipt *sendRecvDesc);

// thread funciton for sending, param changed to senderReceverDescription
extern DWORD WINAPI SOCKTH_send(SendRecvDescipt *sendRecvDesc);

#endif