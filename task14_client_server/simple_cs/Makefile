.PHONY: all

CLIENT = simple_cs_client
SERVER = simple_cs_server

all: client server

client: $(CLIENT).o
	gcc $(CLIENT).o -o client

$(CLIENT).o: $(CLIENT).c
	gcc -c $(CLIENT).c

server: $(SERVER).o
	gcc $(SERVER).o -o server

$(SERVER).o: $(SERVER).c
	gcc -c $(SERVER).c