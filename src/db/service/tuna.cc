/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_CPP_
#define EVAL_TUNA_CPP_

#include "db/service/tuna.h"

namespace eval { namespace tuna {

class Tuna {
 public:
  
  Tuna() {
    /* treba li koristiti shared_ptr??? */
    bigMap = new DbAssoc;
    connPool = new ConnectionPool;
  }

  void reserve(object_id id) {
    return reserve(vector<object_id> (1,id));
  }
  
  void reserve(vector<object_id> ids) {
    ids = bigMap->reserve(ids);
        
    // ids = memcache_.reserve(ids);
    connPool_->getFreeQueueLink()->reserve(ids);

    return true;
  }


  /* blocking function */
  vector<object_id> simpleQuery(string qname, vector<string> data) {
    return connPool_->getFreeWorkLink()->simpleQuery(qname, data);
  }
  
} T;

}}

// eval::tuna
using namespace std;

int main() {
  vector<int> v;
  v.push_back(0);
  v.push_back(101);
  v.push_back(200);
  v.push_back(201);
  v.push_back(300);
  v.push_back(301);
  eval::tuna::T.reserve(v);
  return 0;
}

#endif
