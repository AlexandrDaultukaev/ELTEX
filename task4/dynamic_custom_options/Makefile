.PHONY: build run

OPERATION_DIR = functional/operation
BUILD_OBJ_DIR = build/obj
BUILD_LIB_DIR = build/libs
BIN_DIR       = build/bin
FUNCS_OBJ     = $(BUILD_OBJ_DIR)/addition.o $(BUILD_OBJ_DIR)/subtraction.o $(BUILD_OBJ_DIR)/division.o $(BUILD_OBJ_DIR)/multiplication.o
MENU_OBJ      = $(BUILD_OBJ_DIR)/menu.o
CC            = gcc
I_FLAG        = -I./functional -I./menu
AR_FLAGS      = -rcs


build: $(BIN_DIR)/main 

$(BIN_DIR)/main: $(BUILD_OBJ_DIR)/main.o libmyadd.so libmysub.so libmymul.so libmydiv.so libmymenu.so
	$(CC) -o $(BIN_DIR)/main $(BUILD_OBJ_DIR)/main.o -L./$(BUILD_LIB_DIR) -lmymenu -ldl


# ====----------BUILD LIBRARIES---------------------------
libmyadd.so: $(BUILD_OBJ_DIR)/addition.o
	$(CC) -shared $(BUILD_OBJ_DIR)/addition.o -o $(BUILD_LIB_DIR)/libmyadd.so

libmysub.so: $(BUILD_OBJ_DIR)/subtraction.o
	$(CC) -shared $(BUILD_OBJ_DIR)/subtraction.o -o $(BUILD_LIB_DIR)/libmysub.so

libmymul.so: $(BUILD_OBJ_DIR)/multiplication.o
	$(CC) -shared $(BUILD_OBJ_DIR)/multiplication.o -o $(BUILD_LIB_DIR)/libmymul.so

libmydiv.so: $(BUILD_OBJ_DIR)/division.o
	$(CC) -shared $(BUILD_OBJ_DIR)/division.o -o $(BUILD_LIB_DIR)/libmydiv.so

libmymenu.so: $(MENU_OBJ)
	$(CC) -shared $(MENU_OBJ) -o $(BUILD_LIB_DIR)/libmymenu.so
# ====----------------------------------------------------

$(BUILD_OBJ_DIR)/main.o: main.c
	$(CC) $(I_FLAG) -c main.c -o $(BUILD_OBJ_DIR)/main.o -g

$(BUILD_OBJ_DIR)/addition.o: $(OPERATION_DIR)/addition.c
	$(CC) -fPIC -c $(OPERATION_DIR)/addition.c -o $(BUILD_OBJ_DIR)/addition.o -g

$(BUILD_OBJ_DIR)/subtraction.o: $(OPERATION_DIR)/subtraction.c
	$(CC) -fPIC -c $(OPERATION_DIR)/subtraction.c -o $(BUILD_OBJ_DIR)/subtraction.o -g
	
$(BUILD_OBJ_DIR)/division.o: $(OPERATION_DIR)/division.c
	$(CC) -fPIC -c $(OPERATION_DIR)/division.c -o $(BUILD_OBJ_DIR)/division.o -g

$(BUILD_OBJ_DIR)/multiplication.o: $(OPERATION_DIR)/multiplication.c
	$(CC) -fPIC -c $(OPERATION_DIR)/multiplication.c -o $(BUILD_OBJ_DIR)/multiplication.o -g

$(BUILD_OBJ_DIR)/menu.o: menu/menu.c
	$(CC) -fPIC $(I_FLAG) -c menu/menu.c -o $(MENU_OBJ) -g

run:
	export LD_LIBRARY_PATH=LD_LIBRARY_PATH:$(PWD)/build/libs && ./$(BIN_DIR)/main

build_clear:
	rm ./build/bin/* && rm ./build/libs/* && rm ./build/obj/*
