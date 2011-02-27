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
  
using namespace pqxx;

using apache::thrift::concurrency::Guard;
using apache::thrift::concurrency::Mutex;

using std::map;
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
}}

#include "db/service/queue_link.h"
#include "db/service/work_link.h"
#include "db/service/link_base.h"
#include "db/service/db_row.h"
#include "db/service/db_assoc.h"
#include "db/service/c_pool.h"

