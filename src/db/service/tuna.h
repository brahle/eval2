#ifndef EVAL_TUNA_TUNA_H
#define EVAl_TUNA_TUNA_H

#include <map>
#include <queue>
#include <ctime>
#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <thrift/concurrency/Mutex.h>

#include <pqxx/pqxx>

#define TUNA_MAX_TABLES 100  
#define OBJECT_ID_PRINTF "%d"

#include <tuna_types.h>
#include <thrift-psql.h>

using namespace pqxx;

using apache::thrift::concurrency::Guard;
using apache::thrift::concurrency::Mutex;

using std::map;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::sort;
using std::queue;
using std::string;
using std::vector;
using std::ifstream;
using std::make_pair;
using std::exception;

using boost::shared_ptr;

namespace eval { namespace tuna {
  typedef pipeline::query_id query_id; // from pqxx/pipeline.hxx
  typedef int object_id; // object from db (int64_t)

  const int TUNA_EMPTY = 0;
  const int TUNA_RESERVED = 1;
  const int TUNA_NON_EXISTENT = 2;
  const int TUNA_OK = 3;

  /*
    bigMap will store no more than 2*COLLECTOR_PERIOD
    elements.
   */
  const int COLLECTOR_PERIOD = 100;

  const string QUERY_FILE = "sql/querys/eval.sql";

  const string ime_flag[5] =
    {"EMPTY","RESERVED","NON_EXISTENT","OK","MODIFIED"};

  class Tuna;
  class DbRow;
  class DbAssoc;
  class QueueLink;
  class WorkLink;
  class ConnectionPool;
}}

#include <service/tuna_exception.h>
#include <service/link_base.h>
#include <service/query.h>
#include <service/utility.h>
#include <service/db_row.h>
#include <service/db_assoc.h>
#include <service/queue_link.h>
#include <service/work_link.h>
#include <service/c_pool.h>
#include <service/tuna_class.h>

#endif
