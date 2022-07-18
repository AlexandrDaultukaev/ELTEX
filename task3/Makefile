.PHONY: build run clear

build: padding.o
	gcc -o padding padding.o

padding.o: padding.c
	gcc -c padding.c

run:
	./padding

clear:
	rm *.o padding	