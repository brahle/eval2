#
# Copyright 2011 Matija Osrecki
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

SCRIPTS=../../scripts/

# folders and paths

OBJ_DIR = obj
BIN_DIR = bin

INC = ../../src gen-cpp /usr/local/include/thrift service
LIB = thrift pq pqxx 

VPATH = src:src/db:src/db/gen-cpp:src/queue:src/queue/gen-cpp:src/worker:src/worker/gen-cpp

# compiler flags

CXXFLAGS = -O3 -Wall

gen_dirs : 
	mkdir -p bin
	mkdir -p gen-cpp 

types : gen_dirs
	$(SCRIPTS)thrift-psql.py ../models/models.thrift sql/tables.sql gen-cpp/thrift-psql.h template/tuna.thrift thrift/tuna.thrift

backup :
	sudo -u postgres pg_dump -C deval > sql/backup/deval_backup.sql

thrift : types
	thrift --gen cpp -r thrift/tuna.thrift

skeleton : thrift 
	$(SCRIPTS)thrift-skeleton.py ../models/models.thrift template/skeleton.template gen-cpp/Tuna_server.skeleton.cpp TunaServer.cpp
	rm gen-cpp/Tuna_server.skeleton.cpp
  
tuna_test : thrift
	g++ -obin/tuna service/main.cc $(CXXFLAGS) $(addprefix -I, $(INC)) $(addprefix -l, $(LIB))

server : skeleton
	g++ -obin/server gen-cpp/*.cpp TunaServer.cpp $(CXXFLAGS) $(addprefix -I, $(INC)) $(addprefix -l, $(LIB))

create_db : types
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat sql/tables.sql >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

