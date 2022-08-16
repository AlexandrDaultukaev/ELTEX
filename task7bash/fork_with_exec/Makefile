.PHONY: all clear run

all: main

main: main.o
	gcc -o main main.o

main.o: main.c
	gcc -c main.c -g

run:
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(PWD) && ./main

clear:
	rm *.o main