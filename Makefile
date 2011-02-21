SHELL = /bin/bash

# target programs and objects

TARGETS = QueueServer
OBJS = QueueServer.o QueueService.o

# folders and paths

THRIFT_DIR = /usr/local/include/thrift
OBJ_DIR = obj
BIN_DIR = bin
VPATH = src:src/queue:src/queue/gen-cpp
INC_DIR = src

# compiler flags

CXXFLAGS = -O3 -Wall

all : thrift dirs $(TARGETS)

dirs :
	if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	if [ ! -d $(BIN_DIR) ]; then mkdir $(BIN_DIR); fi

QueueServer : $(addprefix $(OBJ_DIR)/, $(OBJS))
	g++ -o $(BIN_DIR)/$@ $(CXXFLAGS) -I $(THRIFT_DIR) -l thrift $^

$(OBJ_DIR)/%.o : %.cpp
	g++ -c -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -I $(INC_DIR) -l thrift $<

thrift : queue.thrift
	thrift --gen cpp -o src/queue src/queue/queue.thrift

clean : 
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	rm -rf src/queue/gen