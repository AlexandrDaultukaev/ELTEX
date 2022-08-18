.PHONY: all clear

all: main

main: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c main.c

clear:
	rm main main.o