/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

WorkLink::WorkLink() {
}

result WorkLink::_execQuery(const string &qname, Tuna *T) {
  return _execQuery(qname, vector<string>(0), T);
}

result WorkLink::_execQuery(const string &qname,
  const vector<string> &data, Tuna *T) {

  return _execQuery(T->querys_[qname], data, T);
}

result WorkLink::_execQuery(shared_ptr<Query> Q,
  const vector<string> &data, Tuna *T) {

  result ret;
  bool invalidate = false;

  {
    Guard g(lock_);

    for (unsigned int i = 0; i < Q->sql_.size(); ++i) {
      make_log( "QUERY: " + Q->apply(data, i, conn_) );
      ret = work_->exec( Q->apply(data, i, conn_) );  
      if (ret.affected_rows()) {
        invalidate = true;
      }
    }
  }

  resetWork();

  if (invalidate) {
    Q->invokeInvalidation(data, T);
  }

  return ret;
}

void WorkLink::resetWork() {
  Guard g(lock_);
  work_->commit();
  work_.reset();
  work_ = shared_ptr<work>(new work(*conn_));
}

void WorkLink::resolveOneTable(const vector<object_id> &ids, Tuna *T) {
  string query = makePGet(ids, T);
  result rec = work_->exec(query);
  // ovdje sam zakljucan od resolve.
  make_log("BLOCKING query: " + query);

  Guard g(T->bigMap_->lock_);

  for (unsigned int i = 0; i < rec.size(); ++i) {
    object_id id = rec[i][0].as<int>();
    string tablename = rec[i][ rec[i].size()-1 ].as<string>();

    make_log("fetch: resolved from worker", id);
    shared_ptr<DbRow> ptr = T->bigMap_->assoc_[id];

    ptr->object_ = convertGeneric(tablename, rec[i]);

    ptr->flag(TUNA_OK);
  }

  for (unsigned int i = 0; i < ids.size(); ++i) {
    shared_ptr<DbRow> ptr = T->bigMap_->assoc_[ids[i]];
    if (ptr->flag_ != TUNA_OK) {
      ptr->flag(TUNA_NON_EXISTENT);
    }
  }
}

void WorkLink::resolve(const vector<object_id> &ids, Tuna *T) {
  vector<object_id> by_tables[TUNA_MAX_TABLES];
  
  Guard g(lock_);
  /*
    dispatch to tables
   */
  for (unsigned int i = 0; i < ids.size(); ++i) {
    by_tables[ ids[i] % TUNA_MAX_TABLES ].push_back(ids[i]);
  } 

  for (int i = 0; i < TUNA_MAX_TABLES; ++i) {
    if (by_tables[i].size()) {
      resolveOneTable(by_tables[i], T);
    }
  }
}


}} // eval::tuna 

