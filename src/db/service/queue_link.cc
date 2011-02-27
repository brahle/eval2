/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_QUEUE_LINK__HPP_
#define EVAL_TUNA_QUEUE_LINK__HPP_

namespace eval { namespace tuna {

QueueLink::QueueLink() {
  pipeline_ = shared_ptr<pipeline>(
    new pipeline(*work_));
  make_log("initing pipeline!");
}

query_id QueueLink::push(vector<object_id> ids) {
  string arr = toPgArray(ids); 
  int table_id = ids[0] % MAX_TABLES;
  string query = "execute tuna_pget_" + tablename[table_id];
  query = query + "('" + conn_->esc(arr) + "');";

  make_log("ubacujem u pipeline: " + query);
  return pipeline_->insert(query);
}


/*
  divide ids into groups where every group has ids
  with same mod with MAX_TABLES.
  this could be done with map also, which is better?
 */
void QueueLink::reserve(vector<object_id> ids) {
  
  vector<object_id> by_tables[MAX_TABLES];
  
  /*
    dispatcher by tables, connectionPool is locked
   */
  for (unsigned int i = 0; i < ids.size(); ++i) {
    by_tables[ ids[i] % MAX_TABLES ].push_back(ids[i]);
  } 

  for (int i = 0; i < MAX_TABLES; ++i) {
    if (by_tables[i].size()) {
      query_id qid = push(by_tables[i]);
      queryQueue_.push(qid);

      bigMap->reserveSent
        (by_tables[i], shared_ptr<QueueLink>(this), qid);
    }
  }

}

void QueueLink::resolveResult(result rec) {
  echo(rec); 
}

void QueueLink::resolveAllPendingQuerys() {
  Guard g(lock_);

  while(queryQueue_.size()) {
    pair<query_id, result> ret = pipeline_->retrieve();
    assert(ret.first == queryQueue_.front()); 
    queryQueue_.pop();

    resolveResult(ret.second);
  }

}

void QueueLink::resolveToQuery(query_id qid) {
  Guard g(lock_);

  while(queryQueue_.size()) {
    pair<query_id, result> ret = pipeline_->retrieve();
    assert(ret.first == queryQueue_.front()); 
    queryQueue_.pop();
    
    resolveResult(ret.second);
    if (ret.first == qid) break;
  }
}

unsigned int QueueLink::curentQueueSize() {
  Guard g(lock_);
  return queryQueue_.size();
} 


}} // eval::tuna 


#endif
