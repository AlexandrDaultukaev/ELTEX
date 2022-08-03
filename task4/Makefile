.PHONY: build run

OPERATION_DIR = functional/operation
BUILD_OBJ_DIR = build/obj
BUILD_LIB_DIR = build/libs
BIN_DIR       = build/bin
FUNCS_OBJ     = $(BUILD_OBJ_DIR)/addition.o $(BUILD_OBJ_DIR)/subtraction.o $(BUILD_OBJ_DIR)/division.o $(BUILD_OBJ_DIR)/multiplication.o
MENU_OBJ      = $(BUILD_OBJ_DIR)/menu.o
LIBCALC       = $(BUILD_LIB_DIR)/libmycalc.a
CC            = gcc
I_FLAG        = -I./functional -I./menu
AR_FLAGS      = -rcs


build: $(BIN_DIR)/main

$(BIN_DIR)/main: $(BUILD_OBJ_DIR)/main.o $(LIBCALC)
	gcc -o $(BIN_DIR)/main $(BUILD_OBJ_DIR)/main.o -L./$(BUILD_LIB_DIR) -lmycalc
	
$(LIBCALC): $(MENU_OBJ) $(FUNCS_OBJ)
	ar $(AR_FLAGS) $(LIBCALC) $(MENU_OBJ) $(FUNCS_OBJ)

$(BUILD_OBJ_DIR)/main.o: main.c
	gcc $(I_FLAG) -c main.c -o $(BUILD_OBJ_DIR)/main.o -g

$(BUILD_OBJ_DIR)/addition.o: $(OPERATION_DIR)/addition.c
	gcc -c $(OPERATION_DIR)/addition.c -o $(BUILD_OBJ_DIR)/addition.o

$(BUILD_OBJ_DIR)/subtraction.o: $(OPERATION_DIR)/subtraction.c
	gcc -c $(OPERATION_DIR)/subtraction.c -o $(BUILD_OBJ_DIR)/subtraction.o
$(BUILD_OBJ_DIR)/division.o: $(OPERATION_DIR)/division.c
	gcc -c $(OPERATION_DIR)/division.c -o $(BUILD_OBJ_DIR)/division.o

$(BUILD_OBJ_DIR)/multiplication.o: $(OPERATION_DIR)/multiplication.c
	gcc -c $(OPERATION_DIR)/multiplication.c -o $(BUILD_OBJ_DIR)/multiplication.o

$(BUILD_OBJ_DIR)/menu.o: menu/menu.c
	gcc $(I_FLAG) -c menu/menu.c -o $(MENU_OBJ)

run:
	./$(BIN_DIR)/main
