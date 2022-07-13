.PHONY: build clean

build: t1.o t2.o t3.o t4.o
	gcc -o t1 t1.o
	gcc -o t2 t2.o
	gcc -o t3 t3.o
	gcc -o t4 t4.o

t1.o: t1.c
	gcc -c t1.c

t2.o: t2.c
	gcc -c t2.c

t3.o: t3.c
	gcc -c t3.c

t4.o: t4.c
	gcc -c t4.c

clean:
	rm t1 t2 t3 t4 t1.o t2.o t3.o t4.o