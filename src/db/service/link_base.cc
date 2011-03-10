/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

Link_base::Link_base() {
  Guard g(lock_);
  dsn_ = "host=localhost user=deval password=deval dbname=deval";
  init();
}

void Link_base::init() {
  conn_ = shared_ptr<connection>(new connection(dsn_));
  
  try {
    work W(*conn_);
    // ovo tu preparea tuna_pget_%tablename%
    W.exec("select tuna_refresh();");
    W.commit();
  } catch (const exception &e) {
    make_log("call to tuna_refresh() failed.");
    throw;
  }

  work_ = shared_ptr<work>(new work(*conn_));

  cerr << "starting connection!" << std::endl;
}


string Link_base::makePGet(const vector<object_id> &ids, Tuna *T) {
  string arr = toPgArray(ids); 
  int table_id = ids[0] % TUNA_MAX_TABLES;
  
  if (!T->tablename[table_id].size()) {
    throw(
      "object_id is invalid. no table has mod: "+stoi(table_id));
  }

  string query = "execute tuna_pget_" + T->tablename[table_id];
  query = query + "('" + conn_->esc(arr) + "');";
  return query;
}

}} // eval::tuna 


