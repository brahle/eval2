#
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

OBJ_DIR = ../obj
INC = ../../../src ../gen-cpp /usr/local/include/thrift ../service ../client ..
DIRS = $(OBJ_DIR) $(BIN_DIR) $(OBJ_TUNA_DIR) $(OBJ_THRIFT_DIR)
 
LIB = thrift pq pqxx

# compiler flags

CFLAGS = -O3 -Wall
#CFLAGS = -g -ggdb -Wall

bin/% : %.cc
	mkdir -p bin
	g++ -o$@ $(OBJ_DIR)/thrift/*.o $< $(CFLAGS) $(addprefix -I, $(INC)) $(addprefix -l, $(LIB));

test : run.sh 
	./run.sh
  
run : run.sh 
	./run.sh
  
compile :
	for i in *.cc; do j=`echo $$i | sed 's/.cc//g'`; make bin/$$j; done

recompile :
	rm bin/*
	for i in *.cc; do j=`echo $$i | sed 's/.cc//g'`; make bin/$$j; done
