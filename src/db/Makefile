#
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

SCRIPTS = ../../scripts/

# folders and paths

OBJ_DIR = obj
OBJ_TUNA_DIR = obj/tuna
OBJ_THRIFT_DIR = obj/thrift
BIN_DIR = bin

GEN_CPP_DIR = gen-cpp

INC = ../../src gen-cpp /usr/local/include/thrift service client .
DIRS = $(OBJ_DIR) $(BIN_DIR) $(GEN_CPP_DIR) $(OBJ_TUNA_DIR) $(OBJ_THRIFT_DIR)
 
LIB = thrift pq pqxx 

VPATH = src:src/db:src/db/gen-cpp:src/queue:src/queue/gen-cpp:src/worker:src/worker/gen-cpp

TUNA_OBJS = c_pool.o db_assoc.o db_row.o link_base.o query.o queue_link.o tuna_class.o work_link.o utility.o
THRIFT_OBJS = models_constants.o models_types.o tuna_constants.o Tuna.o tuna_types.o thrift-psql.o

TUNACC = service/*.cc
THRIFTCC = gen-cpp/*.cc

# compiler flags

CFLAGS = -O3 -Wall
CDBFLAGS = -g -ggdb -Wall

$(DIRS) : 
	mkdir -p $(DIRS)

thrift/tuna.thrift sql/tables.sql $(GEN_CPP_DIR)/thrift-psql.h : $(DIRS) ../models/models.thrift template/tuna.thrift
	$(SCRIPTS)thrift-psql.py ../models/models.thrift sql/tables.sql gen-cpp/thrift-psql.h template/tuna.thrift thrift/tuna.thrift

backup :
	sudo -u postgres pg_dump -C deval > sql/backup/deval_backup.sql

thrift : thrift/tuna.thrift 
	thrift --gen cpp -r thrift/tuna.thrift
	mv $(GEN_CPP_DIR)/Tuna_server.skeleton.cpp $(GEN_CPP_DIR)/server_skeleton_cpp

client/tuna_client.h : thrift/tuna.thrift 
	$(SCRIPTS)client_gen.py cpp thrift/tuna.thrift
	mv tuna_client.h client/
 
tuna_server.cc : template/skeleton.template
	$(SCRIPTS)thrift-skeleton.py ../models/models.thrift template/skeleton.template $(GEN_CPP_DIR)/server_skeleton_cpp tuna_server.cc
  
$(OBJ_DIR)/tuna/%.o : service/%.cc service/tuna.h 
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $(addprefix -l, $(LIB)) $<

$(OBJ_DIR)/thrift/%.o : $(GEN_CPP_DIR)/%.cpp 
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $(addprefix -l, $(LIB)) $<

$(OBJ_DIR)/%.o : %.cc 
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $(addprefix -l, $(LIB)) $<

server : $(DIRS) $(OBJ_DIR)/tuna_server.o $(addprefix $(OBJ_DIR)/thrift/, $(THRIFT_OBJS)) $(addprefix $(OBJ_DIR)/tuna/, $(TUNA_OBJS))
	g++ -obin/server obj/tuna_server.o $(addprefix $(OBJ_DIR)/thrift/, $(THRIFT_OBJS)) $(addprefix $(OBJ_DIR)/tuna/, $(TUNA_OBJS)) $(addprefix -l, $(LIB))

unit : gen_client skeleton
	g++ -obin/test gen-cpp/*.cpp unit_test.cc $(CFLAGS) $(addprefix -I, $(INC)) -lthrift 

create_db : sql/tables.sql 
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat sql/tables.sql >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

