.PHONY: build run all

OBJ_DIR = build/obj
BIN_DIR = build/bin
CC      = gcc

all: $(OBJ_DIR)/t1.o $(OBJ_DIR)/t2.o $(OBJ_DIR)/write_to_file.o
	$(CC) -Wall -o $(BIN_DIR)/t1 $(OBJ_DIR)/t1.o $(OBJ_DIR)/write_to_file.o
	$(CC) -Wall -o $(BIN_DIR)/t2 $(OBJ_DIR)/t2.o $(OBJ_DIR)/write_to_file.o
	

$(OBJ_DIR)/t1.o: t1.c
	$(CC) -I./write_to_file -Wall -c -o $(OBJ_DIR)/t1.o t1.c

$(OBJ_DIR)/t2.o: t2.c
	$(CC) -I./write_to_file -Wall -c -o $(OBJ_DIR)/t2.o t2.c

$(OBJ_DIR)/write_to_file.o: write_to_file/write_to_file.c
	$(CC) -Wall -c -o $(OBJ_DIR)/write_to_file.o write_to_file/write_to_file.c

run_t1:
	./build/bin/t1

run_t2:
	./build/bin/t2

build_clear:
	rm ./build/bin/* && rm ./build/obj/*