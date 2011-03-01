#
# Copyright 2011 Matija Osrecki
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

TARGETS = c_pool work_link queue_link link_base db_row db_assoc tuna
CC = c_pool.cc work_link.cc queue_link.cc link_base.cc db_row.cc db_assoc.cc tuna.cc

CC1 = main.cc

# folders and paths

THRIFT_DIR = /usr/local/include/thrift
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = ../../../src

VPATH = src:src/db:src/db/gen-cpp:src/queue:src/queue/gen-cpp:src/worker:src/worker/gen-cpp

# compiler flags

CXXFLAGS = -O3 -Wall

all :
	g++ -obin/tuna $(CC1) $(CXXFLAGS) -I $(INC_DIR) -I $(THRIFT_DIR) -lthrift -lpq -lpqxx $^
