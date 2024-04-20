#ifndef CONCURRENCY_H
#define CONCURRENCY_H

#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<winsock2.h>

#define BUFFER_SIZE 1000

// not integrated to client-server until completion 

extern void SOCK_FlCleanUp(char* err);

extern DWORD WINAPI SOCKTH_revieve(SOCKET socket_fh);

extern DWORD WINAPI SOCKTH_send(SOCKET socket_fh);

#endif