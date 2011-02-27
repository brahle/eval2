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

  /*
    obavezno ovo pokrenut query prije i skuzit koje su
    select * from system.tables;
  */
  string tablename[2] = {"solutions","tasks"};

  class DbRow;
  class DbAssoc;
  class QueueLink;
  class WorkLink;
  class ConnectionPool;

  void make_log(string log) {
    cerr << log << endl;
  }

  void make_log(string log, object_id id) {
    cerr << "object/" << id << ": " << log << endl;
  }
}}

namespace eval { namespace tuna {
  DbAssoc *bigMap;
  ConnectionPool *connPool;
}}

#include "db/service/utility.h"

#include "db/service/link_base.h"

#include "db/service/db_row.h"
#include "db/service/db_assoc.h"

#include "db/service/queue_link.h"
#include "db/service/work_link.h"

#include "db/service/c_pool.h"




