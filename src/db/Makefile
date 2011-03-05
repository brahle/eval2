#
# Copyright 2011 Matija Osrecki
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

SCRIPTS=../../scripts/

# folders and paths

THRIFT_DIR = /usr/local/include/thrift
OBJ_DIR = obj
BIN_DIR = bin
INC_DIR = ../../src

VPATH = src:src/db:src/db/gen-cpp:src/queue:src/queue/gen-cpp:src/worker:src/worker/gen-cpp

# compiler flags

CXXFLAGS = -O3 -Wall

gen_dirs :
	mkdir -p gen-cpp
	mkdir -p bin

service : gen_dirs 
	g++ -obin/tuna service/main.cc $(CC1) $(CXXFLAGS) -I $(INC_DIR) -I $(THRIFT_DIR) -lthrift -lpq -lpqxx

server :
	g++ -obin/server gen-cpp/*.cpp -I $(THRIFT_DIR) -lthrift 

types : gen_dirs
	$(SCRIPTS)thrift-psql.py ../models/models.thrift sql/tables.sql gen-cpp/thrift-psql.h thrift/tuna.skeleton.thrift thrift/tuna.thrift

backup :
	sudo -u postgres pg_dump -C deval > sql/backup/deval_backup.sql

thrift : types
	thrift --gen cpp -r thrift/tuna.thrift

create_db : types
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat sql/tables.sql >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

