/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

namespace eval { namespace tuna {

QueueLink::QueueLink() {
  pipeline_ = shared_ptr<pipeline>(
    new pipeline(*work_));
  make_log("pipeline initialized!");
}

query_id QueueLink::push(const vector<object_id> &ids, Tuna *T) {
  string arr = toPgArray(ids); 
  int table_id = ids[0] % MAX_TABLES;
  string query = "execute tuna_pget_" + T->tablename[table_id];
  query = query + "('" + conn_->esc(arr) + "');";

  make_log("ubacujem u pipeline: " + query);
  return pipeline_->insert(query);
}


/*
  divide ids into groups where every group has ids
  with same mod with MAX_TABLES.
  this could be done with map also, which is better?
 */
void QueueLink::reserve(
  const vector<object_id> &ids, Tuna *T) {
  
  vector<object_id> by_tables[MAX_TABLES];
  
  /*
    dispatcher by tables, connectionPool is locked
   */
  for (unsigned int i = 0; i < ids.size(); ++i) {
    by_tables[ ids[i] % MAX_TABLES ].push_back(ids[i]);
  } 

  for (int i = 0; i < MAX_TABLES; ++i) {
    if (by_tables[i].size()) {
      query_id qid = push(by_tables[i], T);
      queryQueue_.push(qid);

      T->bigMap_->reserveSent(by_tables[i], this, qid);
    }
  }
}

// TODO 
// resolveResult nije dobar, tuple se dealocira

void QueueLink::resolveResult(result rec, Tuna *T) {

  for (unsigned int i = 0; i < rec.size(); ++i) {
    Guard g(T->bigMap_->lock_);
    object_id id = rec[i][0].as<int>();

    make_log("fetch: resolved from pipeline", id);
    shared_ptr<DbRow> ptr = T->bigMap_->assoc_[id];

    ptr->tup_ = rec[i]; 

    ptr->flag(TUNA_OK);
  }

}

void QueueLink::resolveAllPendingQuerys(Tuna *T) {
  Guard g(lock_);

  while(queryQueue_.size()) {
    pair<query_id, result> ret = pipeline_->retrieve();
    assert(ret.first == queryQueue_.front()); 
    queryQueue_.pop();

    resolveResult(ret.second, T);
  }


}

void QueueLink::resolveToQuery(query_id qid, Tuna *T) {
  Guard g(lock_);

  while(queryQueue_.size()) {
    pair<query_id, result> ret = pipeline_->retrieve();
    assert(ret.first == queryQueue_.front()); 
    queryQueue_.pop();
    
    resolveResult(ret.second, T);
    if (ret.first == qid) break;
  }

}

unsigned int QueueLink::curentQueueSize() {
  Guard g(lock_);
  return queryQueue_.size();
} 


}} // eval::tuna 


