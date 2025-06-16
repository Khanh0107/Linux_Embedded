CUR_DIR := .
BIN_DIR := $(CUR_DIR)/bin
OBJ_DIR	:= $(CUR_DIR)/obj
SRC_DIR := $(CUR_DIR)/src
INC_DIR := $(CUR_DIR)/inc
LIB_DIR := $(CUR_DIR)/lib
LIB_SHARED := $(LIB_DIR)/shared
LIB_STATIC := $(LIB_DIR)/static

CF_FLAGS = -I $(INC_DIR)

create_objects:
	gcc -c -fPIC $(SRC_DIR)/helloKhanh.c -o $(OBJ_DIR)/helloKhanh.o -I $(INC_DIR) 
	gcc -c -fPIC $(SRC_DIR)/helloThu.c -o $(OBJ_DIR)/helloThu.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)

create_libs:
	gcc -shared $(OBJ_DIR)/helloKhanh.o $(OBJ_DIR)/helloThu.o -o $(LIB_SHARED)/libhello.so

all: create_objects create_libs
	gcc $(OBJ_DIR)/main.o -L$(LIB_SHARED) -lhello -Wl,-rpath,$(LIB_SHARED) -o $(BIN_DIR)/used-shared-library

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(LIB_SHARED)/*.so
	rm -rf $(BIN_DIR)/used-shared-library
