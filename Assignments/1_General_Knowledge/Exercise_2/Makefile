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
	gcc -c $(SRC_DIR)/helloKhanh.c -o $(OBJ_DIR)/helloKhanh.o -I $(INC_DIR) 
	gcc -c $(SRC_DIR)/helloThu.c -o $(OBJ_DIR)/helloThu.o -I $(INC_DIR)
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)

create_libs:
	ar rcs $(LIB_STATIC)/libhello.a $(OBJ_DIR)/helloKhanh.o $(OBJ_DIR)/helloThu.o 

all: create_objects create_libs
	gcc  $(OBJ_DIR)/main.o -o $(BIN_DIR)/used-static-library -L$(LIB_STATIC) -lhello

clean:
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(LIB_STATIC)/*.a
	rm -rf $(BIN_DIR)/used-static-library
