.PHONY: all


all: client server

client: client.o
	gcc -o client client.o -lrt

server: server.o
	gcc -o server server.o -lrt

server.o: server.c
	gcc -c server.c

client.o: client.c
	gcc -c client.c