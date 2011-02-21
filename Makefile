#
# Copyright 2011 Matija Osrecki
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

TARGETS = Queue Worker Tuna

# folders and paths

THRIFT_DIR = /usr/local/include/thrift
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = src

VPATH = src:src/db:src/db/gen-cpp:src/queue:src/queue/gen-cpp:src/worker:src/worker/gen-cpp

# compiler flags

CXXFLAGS = -O3 -Wall

all : dirs thrift $(addprefix $(BIN_DIR)/, $(TARGETS))

dirs :
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi

$(BIN_DIR)/Queue : $(addprefix $(OBJ_DIR)/, QueueServer.o QueueService.o)
	g++ -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -l thrift $^

$(BIN_DIR)/Worker : $(addprefix $(OBJ_DIR)/, WorkerServer.o WorkerService.o QueueService.o)
	g++ -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -l thrift $^

$(BIN_DIR)/Tuna : $(addprefix $(OBJ_DIR)/, TunaServer.o Tuna.o)
	g++ -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -l thrift $^

$(OBJ_DIR)/%.o : %.cpp
	g++ -c -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -I $(INC_DIR) -l thrift $<

thrift : queue.thrift worker.thrift tuna.thrift
	thrift --gen cpp -o src/queue src/queue/queue.thrift
	thrift --gen cpp -o src/worker src/worker/worker.thrift
	thrift --gen cpp -o src/db src/db/tuna.thrift

clean : 
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -rf src/queue/gen
