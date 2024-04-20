SOCKET = ws2_32
GXX = gcc 

client: Client/client.c
	$(GXX) -o exe/client Client/client.c -l $(SOCKET) 

server: Server/server.c
	$(GXX) -o exe/server Server/server.c -l $(SOCKET)