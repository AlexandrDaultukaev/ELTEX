.PHONY: all

CLIENT = pc_client
SERVER = pc_server

all: client server

client: $(CLIENT).o
	gcc $(CLIENT).o -o client -lrt

$(CLIENT).o: $(CLIENT).c
	gcc -c $(CLIENT).c

server: $(SERVER).o
	gcc $(SERVER).o -o server -lrt

$(SERVER).o: $(SERVER).c
	gcc -c $(SERVER).c