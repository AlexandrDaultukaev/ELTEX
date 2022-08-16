.PHONY: all clear run

all: main

main: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c main.c -g

run:
	./main

clear:
	rm *.o main