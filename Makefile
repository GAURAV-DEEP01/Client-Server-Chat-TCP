SOCKET = ws2_32

client: Client/client.c
	gcc -o exe/client Client/client.c -l $(SOCKET) 

server: Server/server.c
	gcc -o exe/server Server/server.c -l $(SOCKET)