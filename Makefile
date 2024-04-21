GXX = gcc 
SOCKET = ws2_32
SRC = util/src/

all: client server

client: Client/client.c
	$(GXX) -o exe/client Client/client.c $(SRC)concurrency.c -l $(SOCKET) 

server: Server/server.c
	$(GXX) -o exe/server Server/server.c $(SRC)concurrency.c -l $(SOCKET)