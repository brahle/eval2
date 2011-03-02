#ifndef EVAL_TUNA_TUNA_H
#define EVAl_TUNA_TUNA_H

#include <map>
#include <queue>
#include <string>
#include <iostream>

#include <protocol/TBinaryProtocol.h>
#include <server/TThreadedServer.h>
#include <thrift/concurrency/Mutex.h>
#include <transport/TServerSocket.h>
#include <transport/TBufferTransports.h>


#include <pqxx/pqxx>

#define MAX_TABLES 100  
#define OBJECT_ID_PRINTF "%d"

#include <db/gen-cpp/models_types.h>
#include <db/gen-cpp/thrift-psql.h>

using namespace pqxx;

using apache::thrift::concurrency::Guard;
using apache::thrift::concurrency::Mutex;

using std::map;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;
using std::queue;
using std::string;
using std::vector;

using boost::shared_ptr;

namespace eval { namespace tuna {
  typedef pipeline::query_id query_id; // from pqxx/pipeline.hxx
  typedef int object_id; // object from db (int64_t)

  const int TUNA_EMPTY = 0;
  const int TUNA_RESERVED = 1;
  const int TUNA_NON_EXISTENT = 2;

  const int TUNA_OK = 3;
  const int TUNA_MODIFIED = 4;

  string ime_flag[5] =
    {"EMPTY","RESERVED","NON_EXISTENT","OK","MODIFIED"};

  class Tuna;
  class DbRow;
  class DbAssoc;
  class QueueLink;
  class WorkLink;
  class ConnectionPool;
}}

#include "db/service/link_base.h"
#include "db/service/utility.h"
#include "db/service/db_row.h"
#include "db/service/db_assoc.h"
#include "db/service/queue_link.h"
#include "db/service/work_link.h"
#include "db/service/c_pool.h"
#include "db/service/tuna_class.h"

#include "db/service/link_base.cc"
#include "db/service/db_row.cc"
#include "db/service/db_assoc.cc"
#include "db/service/queue_link.cc"
#include "db/service/work_link.cc"
#include "db/service/c_pool.cc"
#include "db/service/tuna_class.cc"

#endif
