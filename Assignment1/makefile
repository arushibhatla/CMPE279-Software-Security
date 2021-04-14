# use command: "make all" for compiling client and server
all:
	gcc client.c -o client
	gcc server.c -o server

# use command: "make client" for compiling client
client: client.c
	gcc client.c -o client

# use command: "make server" for compiling server
server: server.c
	gcc server.c -o server

# use command: "make clean" for removing the compiled client and server binary files
clean:
	rm client server