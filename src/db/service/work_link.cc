/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

WorkLink::WorkLink() {}

result WorkLink::_execQuery(const string &qname, Tuna *T) {
  return _execQuery(qname, vector<string>(0), T);
}

result WorkLink::_execQuery(const string &qname,
  const vector<string> &data, Tuna *T) {
  
  if (!(T->querys_.count(qname))) {
    throw TunaException("no such query: " + qname);
  }

  return _execQuery(T->querys_[qname], data, T);
}

/*
  every, every query which goes to postgres,
  except those from pipeline will be called
  from this function.
 */
result WorkLink::_execQuery(shared_ptr<Query> Q,
  const vector<string> &data, Tuna *T) {

  result ret;
  bool invalidate = false;

  {
    Guard g(lock_);

    try {
      for (unsigned int i = 0; i < Q->sql_.size(); ++i) {
        make_log( "QUERY: " + Q->apply(data, i, conn_) );
        ret = work_->exec( Q->apply(data, i, conn_) );  
        if (ret.affected_rows()) {
          invalidate = true;
        }
      }
    } catch (const pqxx::pqxx_exception &e) {
      throw TunaException(e);
    }
  }

  resetWork();

  if (invalidate) {
    Q->invokeInvalidation(data, T);
  }

  return ret;
}

void WorkLink::resetWork() {
  try {
    Guard g(lock_);
    work_->commit();
    work_.reset();
    work_ = shared_ptr<work>(new work(*conn_));
  } catch (const pqxx::pqxx_exception &e) {
    throw TunaException(e);
  }
}

//result WorkLink::_execQuery(shared_ptr<Query> Q,
//  const vector<string> &data, Tuna *T) {

void WorkLink::resolveOneTable(const vector<object_id> &ids, Tuna *T) {

  result rec = _execQuery(
    shared_ptr<Query>(new Query(ids, T)),
    vector<string>(),
    T
  );

  Guard g(T->bigMap_->lock_);

  for (unsigned int i = 0; i < rec.size(); ++i) {
    object_id id = rec[i][0].as<int>();
    string tablename = rec[i][ rec[i].size()-1 ].as<string>();

    make_log("fetch: resolved from worker", id);
    shared_ptr<DbRow> ptr = T->bigMap_->assoc_[id];

    ptr->object_ = convertGeneric(tablename, rec[i]);
    ptr->flag(TUNA_OK);
  }

  // stil has the lock..
  for (unsigned int i = 0; i < ids.size(); ++i) {
    shared_ptr<DbRow> ptr = T->bigMap_->assoc_[ids[i]];
    if (ptr->flag_ != TUNA_OK) {
      ptr->flag(TUNA_NON_EXISTENT);
    }
  }
}

void WorkLink::resolve(const vector<object_id> &ids, Tuna *T) {

  vector<object_id> by_tables[TUNA_MAX_TABLES];
  
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

