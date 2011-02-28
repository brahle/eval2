#
# Copyright 2011 Matija Osrecki
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

TARGETS = tuna

# folders and paths

THRIFT_DIR = /usr/local/include/thrift
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = ../../../src

VPATH = src:src/db:src/db/gen-cpp:src/queue:src/queue/gen-cpp:src/worker:src/worker/gen-cpp

# compiler flags

CXXFLAGS = -O3 -Wall

all : dirs $(addprefix $(BIN_DIR)/, $(TARGETS))

dirs :
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi

$(BIN_DIR)/tuna : $(addprefix $(OBJ_DIR)/, tuna.o)
	g++ -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -l thrift -lpq -lpqxx $^

$(OBJ_DIR)/%.o : %.cc %.h
	g++ -c -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -I $(INC_DIR) -l thrift $<

clean : 
	rm -rf $(OBJ_DIR) $(BIN_DIR)
