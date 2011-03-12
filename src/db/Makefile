#
#
# TODO copy notice
#

SHELL = /bin/bash

# target programs (components)

SCRIPTS = ../../scripts/

# folders and paths

GEN_DIR = _gen

OBJ = $(GEN_DIR)/obj
OBJ_TUNA = $(OBJ)/tuna
OBJ_THRIFT = $(OBJ)/thrift
BIN = $(GEN_DIR)/bin
GEN_CPP_DIR = $(GEN_DIR)/gen-cpp

TUNA_THRIFT = $(GEN_DIR)/thrift/tuna.thrift
TABLES_SQL = $(GEN_DIR)/sql/tables.sql
THRIFT_PSQL = $(GEN_CPP_DIR)/thrift-psql
THRIFT_PSQL_CC = $(GEN_CPP_DIR)/thrift-psql.cc
THRIFT_PSQL_H = $(GEN_CPP_DIR)/thrift-psql.h
THRIFT_PSQL_CCH = $(THRIFT_PSQL_H) $(THRIFT_PSQL_CC)
CLIENT_H = $(GEN_DIR)/client/tuna_client.h
SERVER_SKELETON_CC = $(GEN_CPP_DIR)/server_skeleton_cc

INC = ../../src $(GEN_CPP_DIR) $(GEN_DIR)/client /usr/local/include/thrift service .
DIRS = $(OBJ) $(BIN) $(OBJ_TUNA) $(OBJ_THRIFT) 
 
LIB = thrift pq pqxx 

TUNA_OBJS = c_pool.o db_assoc.o db_row.o link_base.o query.o queue_link.o tuna_class.o work_link.o utility.o tuna_exception.o
THRIFT_OBJS = models_constants.o models_types.o tuna_constants.o Tuna.o tuna_types.o thrift-psql.o

# compiler flags

CFLAGS = -O3 -Wall
#CFLAGS = -g -ggdb -Wall

dirs : 
	mkdir -p $(DIRS)

# u readmeu je ovo skripta 1)
$(TUNA_THRIFT) $(TABLES_SQL) $(THRIFT_PSQL_CCH) : ../models/models.thrift template/tuna.thrift.template
	mkdir -p $(GEN_CPP_DIR) $(GEN_DIR)/thrift $(GEN_DIR)/sql
	$(SCRIPTS)thrift-psql.py ../models/models.thrift $(TABLES_SQL) $(THRIFT_PSQL) template/tuna.thrift.template $(TUNA_THRIFT) 

db_backup :
	sudo -u postgres pg_dump -C deval > sql/backup/deval_backup.sql

# u readmeu je ovo skripta 2)
$(SERVER_SKELETON_CC) $(GEN_CPP_DIR)/Tuna.h $(GEN_CPP_DIR) : $(TUNA_THRIFT)
	thrift --gen cpp -r -o $(GEN_DIR) $(TUNA_THRIFT) 
	rename 's/\.cpp$$/.cc/' -f $(GEN_CPP_DIR)/*.cpp
	mv $(GEN_CPP_DIR)/Tuna_server.skeleton.cc $(SERVER_SKELETON_CC) 

# u readmeu je ovo skripta 3)
$(CLIENT_H) : $(TUNA_THRIFT)
	mkdir -p $(GEN_DIR)/client
	$(SCRIPTS)client_gen.py cpp $(TUNA_THRIFT)
	mv tuna_client.h $(CLIENT_H)
 
# u readmeu je ovo skripta 4)
tuna_server.cc : $(SERVER_SKELETON_CC) ../models/models.thrift template/skeleton.template
	$(SCRIPTS)thrift-skeleton.py ../models/models.thrift template/skeleton.template $(SERVER_SKELETON_CC) tuna_server.cc
  


$(OBJ)/tuna/%.o : service/%.cc service/%.h service/tuna.h 
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $<

$(OBJ)/thrift/%.o : $(GEN_CPP_DIR)/%.cc $(GEN_CPP_DIR)/*.h
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $<

$(OBJ)/%.o : %.cc client/* $(GEN_CPP_DIR)/* service/* 
	g++ -c -o $@ $(CFLAGS) $(addprefix -I , $(INC)) $(addprefix -l, $(LIB)) $<

$(BIN)/tnserver : dirs $(OBJ)/tuna_server.o $(THRIFT_PSQL_H) $(addprefix $(OBJ)/thrift/, $(THRIFT_OBJS)) $(addprefix $(OBJ)/tuna/, $(TUNA_OBJS))
	g++ -o $(BIN)/tnserver $(OBJ)/tuna_server.o $(addprefix $(OBJ)/thrift/, $(THRIFT_OBJS)) $(addprefix $(OBJ)/tuna/, $(TUNA_OBJS)) $(addprefix -l, $(LIB))

generate :
	rm -rf $(GEN_DIR) 
	make tuna_server.cc $(CLIENT_H) &> /dev/null

run : $(BIN)/tnserver stop
	./scripts/run 

stop :
	./scripts/stop

create_db : sql/tables.sql
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat $(TABLES_SQL) >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

update_db : sql/tables.sql db_backup
	cat sql/init/create_db.sql > /tmp/temp.sql
	cat sql/init/tuna.sql >> /tmp/temp.sql
	cat $(TABLES_SQL) >> /tmp/temp.sql
	cat sql/init/refresh.sql >> /tmp/temp.sql
	sudo -u postgres psql -f /tmp/temp.sql 
	rm /tmp/temp.sql

