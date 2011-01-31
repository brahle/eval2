#!/bin/bash

BIN=../../bin/queue

THRIFT_DIR=/usr/local/include/thrift

OBJS=QueueServer.o 	QueueService.o ThreadSafeQueue.o

CXXFLAGS = -O3 -Wall

QueueServer : $(addprefix ${BIN}/, ${OBJS})
	g++ $< -o $@ ${CXXFLAGS} -I ${THRIFT_DIR} -lthrift

${BIN}/%.o : %.cpp
	g++ -c $< -o $@ ${CXXFLAGS} -I ${THRIFT_DIR} -I..

thrift : queue.thrift
	thrift --gen cpp queue.thrift
