.PHONY: all clear

all: main

main: main.o
	gcc -o main main.o -lpthread

main.o: main.c
	gcc -c main.c

clear:
	rm main main.o