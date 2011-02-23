#
# Copyright 2011 Matija Osrecki
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

TARGETS = dispatcher worker tuna
DISPATCHER_OBJS = dispatcher_main.o basic_queue_dispatcher.o Dispatcher.o
WORKER_OBJS = worker_main.o worker_handler.o Worker.o Dispatcher.o
TUNA_OBJS = TunaServer.o Tuna.o

# folders and paths

THRIFT_DIR = /usr/local/include/thrift
OBJ_DIR = obj
BIN_DIR = bin
INC_DIRS = src include $(THRIFT_DIR)

# folders to look for souce code

VPATH = src:src/db:src/db/gen-cpp:src/dispatcher:src/dispatcher/gen-cpp: \
	src/worker:src/worker/gen-cpp

# compiler flags

CXXFLAGS = -O3 -Wall

# create dirs and compile binaries

all : dirs $(addprefix $(BIN_DIR)/, $(TARGETS))

# create directories if necessary

dirs :
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi

# link object files into target binaries

$(BIN_DIR)/dispatcher : $(addprefix $(OBJ_DIR)/, $(DISPATCHER_OBJS))
	g++ -o $@ $(CXXFLAGS) -l thrift $^

$(BIN_DIR)/worker : $(addprefix $(OBJ_DIR)/, $(WORKER_OBJS))
	g++ -o $@ $(CXXFLAGS) -l thrift $^

$(BIN_DIR)/tuna : $(addprefix $(OBJ_DIR)/, $(TUNA_OBJS))
	g++ -o $@ $(CXXFLAGS) -l thrift $^

# compile C++ files into object files

$(OBJ_DIR)/%.o : %.cpp
	g++ -c -o $@ $(CXXFLAGS) $(addprefix -I , $(INC_DIRS)) $<

$(OBJ_DIR)/%.o : %.cc
	g++ -c -o $@ $(CXXFLAGS) $(addprefix -I , $(INC_DIRS)) $<

# generate thrift code

thrift : dispatcher.thrift worker.thrift tuna.thrift
	thrift --gen cpp -o src/dispatcher src/dispatcher/dispatcher.thrift
	thrift --gen cpp -o src/worker src/worker/worker.thrift
	thrift --gen cpp -o src/db src/db/tuna.thrift

# delete targets, object files and thrift generated files

clean : 
	rm -rf $(OBJ_DIR) $(addprefix $(BIN_DIR)/, $(TARGETS))
	rm -rf src/{dispatcher,worker,db}/gen-cpp
