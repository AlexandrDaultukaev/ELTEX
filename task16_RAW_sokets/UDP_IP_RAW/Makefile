.PHONY: all

CLIENT = RAW_client
SERVER = RAW_server

all: client server

client: $(CLIENT).o
	gcc $(CLIENT).o -o client

$(CLIENT).o: $(CLIENT).c
	gcc -c $(CLIENT).c

server: $(SERVER).o
	gcc $(SERVER).o -o server

$(SERVER).o: $(SERVER).c
	gcc -c $(SERVER).c