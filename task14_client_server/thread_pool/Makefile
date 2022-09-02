.PHONY: all

CLIENT = tp_client
SERVER = tp_server

all: client server

client: $(CLIENT).o
	gcc $(CLIENT).o -o client -lpthread

$(CLIENT).o: $(CLIENT).c
	gcc -c $(CLIENT).c -g

server: $(SERVER).o
	gcc $(SERVER).o -o server -lpthread

$(SERVER).o: $(SERVER).c
	gcc -c $(SERVER).c -g