SHELL = /bin/bash

OBJ_DIR = ../../bin/queue

THRIFT_DIR = /usr/local/include/thrift

OBJS = QueueServer.o QueueService.o

CXXFLAGS = -O3 -Wall

QueueServer : $(addprefix $(OBJ_DIR)/, $(OBJS))
	g++ $^ -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -lthrift

$(OBJ_DIR)/%.o : %.cpp
	g++ -c $< -o $@ $(CXXFLAGS) -I $(THRIFT_DIR) -I.. -lthrift

thrift : queue.thrift
	thrift --gen cpp queue.thrift
