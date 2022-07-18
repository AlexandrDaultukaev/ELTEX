.PHONY: build clear build_pad run_pad run_dict

build: dict.o
	gcc -o dict dict.o
	./dict

build_pad: padding.o
	gcc -o padding padding.o
	
padding.o: padding.c
	gcc -c padding.c

dict.o: dict.c
	gcc -c dict.c

run_pad:
	./padding

run_dict:
	./dict

clear:
	rm *.o padding dict