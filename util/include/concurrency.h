#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<winsock2.h>

#define BUFFER_SIZE 1000

// integrating...

// socket error status & makes WSA cleanup 
extern void SOCK_FlCleanUp(char* err);

// thread funciton for receving param typecasted to (SOCKET)  
extern DWORD WINAPI SOCKTH_receive(LPVOID socket_fh);

// thread funciton for sending param typecasted to (SOCKET) 
extern DWORD WINAPI SOCKTH_send(LPVOID socket_fh);

#endif