/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

using std::string;
using std::queue;
using std::vector;

namespace eval { namespace tuna {

WorkLink::WorkLink() {
}

vector<object_id> WorkLink::simpleQuery(string qname,
  const vector<string> &data) {

  return vector<object_id>(0);
}

void WorkLink::resetWork() {
  work_->commit();
  work_.reset();
  work_ = shared_ptr<work>(new work(*conn_));
}

void WorkLink::resolveOneTable(const vector<object_id> &ids, Tuna *T) {
  string query = makePGet(ids, T);
  result rec = work_->exec(query);
  make_log("BLOCKING query: " + query);

  for (unsigned int i = 0; i < rec.size(); ++i) {
    Guard g(T->bigMap_->lock_);
    object_id id = rec[i][0].as<int>();
    string tablename = rec[i][ rec[i].size()-1 ].as<string>();

    make_log("fetch: resolved from worker", id);
    shared_ptr<DbRow> ptr = T->bigMap_->assoc_[id];

    ptr->object_ = convertGeneric(tablename, rec[i]);

    ptr->flag(TUNA_OK);
  }

}

void WorkLink::resolve(const vector<object_id> &ids, Tuna *T) {
  vector<object_id> by_tables[MAX_TABLES];
  
  Guard g(lock_);
  /*
    dispatch to tables
   */
  for (unsigned int i = 0; i < ids.size(); ++i) {
    by_tables[ ids[i] % MAX_TABLES ].push_back(ids[i]);
  } 

  for (int i = 0; i < MAX_TABLES; ++i) {
    if (by_tables[i].size()) {
      resolveOneTable(by_tables[i], T);
    }
  }

  resetWork();
}


}} // eval::tuna 

