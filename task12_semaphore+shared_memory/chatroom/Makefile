.PHONY: all


all: client

client: client.o
	gcc -o client client.o -lrt -pthread

client.o: client.c
	gcc -c client.c