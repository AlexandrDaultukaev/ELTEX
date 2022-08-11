.PHONY: all

all: main

main: main.o
	gcc -o main main.o -lncurses -lm

main.o: main.c
	gcc -c main.c -g