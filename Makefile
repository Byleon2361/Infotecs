CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Ilib -fPIC

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

SRC_PROG1 = $(SRC_DIR)/prog1.cpp
SRC_PROG2 = $(SRC_DIR)/prog2.cpp
SRC_LIB = $(LIB_DIR)/lib.cpp

OBJ_PROG1 = $(OBJ_DIR)/prog1.o
OBJ_PROG2 = $(OBJ_DIR)/prog2.o
OBJ_LIB = $(OBJ_DIR)/lib.o

PROG1 = $(BIN_DIR)/prog1
PROG2 = $(BIN_DIR)/prog2

LIB_NAME = libmylib.so
LIB_TARGET = $(LIB_DIR)/$(LIB_NAME)

all: $(PROG1) $(PROG2)

$(PROG1): $(OBJ_PROG1) $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_PROG1) -L$(LIB_DIR) -lmylib -Wl,-rpath,$(LIB_DIR)

$(PROG2): $(OBJ_PROG2) $(LIB_TARGET)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ_PROG2) -L$(LIB_DIR) -lmylib -Wl,-rpath,$(LIB_DIR)

$(OBJ_PROG1): $(SRC_PROG1)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(OBJ_PROG2): $(SRC_PROG2)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(LIB_TARGET): $(OBJ_LIB)
	$(CXX) -shared -o $@ $<

$(OBJ_LIB): $(SRC_LIB)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/* $(LIB_DIR)/*.so

.PHONY: all clean