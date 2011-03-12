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
DIRS = $(OBJ_DIR) $(BIN_DIR) $(OBJ_TUNA_DIR) $(OBJ_THRIFT_DIR)
 
LIB = thrift pq pqxx 

TUNA_OBJS = c_pool.o db_assoc.o db_row.o link_base.o query.o queue_link.o tuna_class.o work_link.o utility.o tuna_exception.o
THRIFT_OBJS = models_constants.o models_types.o tuna_constants.o Tuna.o tuna_types.o thrift-psql.o

# compiler flags

CFLAGS = -O3 -Wall
#CFLAGS = -g -ggdb -Wall

$(DIRS) : 
	mkdir -p $(DIRS)

thrift/tuna.thrift sql/tables.sql $(GEN_CPP_DIR)/thrift-psql.cc $(GEN_CPP_DIR)/thrift-psql.h : ../models/models.thrift template/tuna.thrift
	mkdir -p $(GEN_CPP_DIR)
	$(SCRIPTS)thrift-psql.py ../models/models.thrift sql/tables.sql $(GEN_CPP_DIR)/thrift-psql template/tuna.thrift thrift/tuna.thrift

db_backup :
	sudo -u postgres pg_dump -C deval > sql/backup/deval_backup.sql

$(GEN_CPP_DIR)/server_skeleton_cc $(GEN_CPP_DIR)/Tuna.h $(GEN_CPP_DIR) : thrift/tuna.thrift 
	thrift --gen cpp -r thrift/tuna.thrift
	rename 's/\.cpp$$/.cc/' -f $(GEN_CPP_DIR)/*.cpp
	mv $(GEN_CPP_DIR)/Tuna_server.skeleton.cc $(GEN_CPP_DIR)/server_skeleton_cc

client/tuna_client.h : thrift/tuna.thrift 
	$(SCRIPTS)client_gen.py cpp thrift/tuna.thrift
	mv tuna_client.h client/
 
tuna_server.cc : $(GEN_CPP_DIR)/server_skeleton_cc ../models/models.thrift template/skeleton.template
	$(SCRIPTS)thrift-skeleton.py ../models/models.thrift template/skeleton.template $(GEN_CPP_DIR)/server_skeleton_cc tuna_server.cc
  
$(OBJ_DIR)/tuna/%.o : service/%.cc service/%.h service/tuna.h
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $<

$(OBJ_DIR)/thrift/%.o : $(GEN_CPP_DIR)/%.cc $(GEN_CPP_DIR)/*.h
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $<

$(OBJ_DIR)/%.o : %.cc client/* $(GEN_CPP_DIR)/* service/* 
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $(addprefix -l, $(LIB)) $<

bin/server : $(DIRS) $(GEN_CPP_DIR) $(OBJ_DIR)/tuna_server.o $(addprefix $(OBJ_DIR)/thrift/, $(THRIFT_OBJS)) $(addprefix $(OBJ_DIR)/tuna/, $(TUNA_OBJS))
	g++ -obin/server obj/tuna_server.o $(addprefix $(OBJ_DIR)/thrift/, $(THRIFT_OBJS)) $(addprefix $(OBJ_DIR)/tuna/, $(TUNA_OBJS)) $(addprefix -l, $(LIB))

run : bin/server
	./scripts/run 

stop :
	./scripts/stop

create_db : sql/tables.sql
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat sql/tables.sql >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

update_db : sql/tables.sql db_backup
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat sql/tables.sql >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

