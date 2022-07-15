.PHONY: build run clear

build: byte.o
	gcc -o byte byte.o

byte.o: byte.c
	gcc -c byte.c

run:
	./byte

clear:
	rm *.o byte	