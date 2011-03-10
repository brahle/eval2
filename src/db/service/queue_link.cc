/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

QueueLink::QueueLink() {
  pipeline_ = shared_ptr<pipeline>(
    new pipeline(*work_));
  make_log("pipeline initialized!");
}

query_id QueueLink::push(const vector<object_id> &ids, Tuna *T) {
  string arr = toPgArray(ids); 
  int table_id = ids[0] % TUNA_MAX_TABLES;
  string query = "execute tuna_pget_" + T->tablename[table_id];
  query = query + "('" + conn_->esc(arr) + "');";

  make_log("ubacujem u pipeline: " + query);
  return pipeline_->insert(query);
}


/*
  divide ids into groups where every group has ids
  with same mod with TUNA_MAX_TABLES.
  this could be done with map also, which is better?
 */
void QueueLink::reserve(
  const vector<object_id> &ids, Tuna *T) {
  
  vector<object_id> by_tables[TUNA_MAX_TABLES];
  
  /*
    dispatch to tables
   */
  for (unsigned int i = 0; i < ids.size(); ++i) {
    by_tables[ ids[i] % TUNA_MAX_TABLES ].push_back(ids[i]);
  } 

  Guard g(lock_);

  for (int i = 0; i < TUNA_MAX_TABLES; ++i) {
    if (by_tables[i].size()) {
      query_id qid = push(by_tables[i], T);
      queryQueue_.push(qid);

      T->bigMap_->reserveSent(by_tables[i], this, qid);
    }
  }
}

void QueueLink::resolveAllPendingQuerys(Tuna *T) {
  Guard g(lock_);

  while(queryQueue_.size()) {
    pair<query_id, result> ret = pipeline_->retrieve();

    assert(ret.first == queryQueue_.front()); 

    queryQueue_.pop();
    resolveResult(ret.first, ret.second, -1, T);
  }
}

bool QueueLink::resolveResult(query_id qid,
  result rec, object_id id, Tuna *T) {

  bool sol = false;

  for (unsigned int i = 0; i < rec.size(); ++i) {
    Guard g(T->bigMap_->lock_);
    object_id oid = rec[i][0].as<int>();
    string tablename = rec[i][ rec[i].size()-1 ].as<string>();

    make_log("fetch: resolved from pipeline", oid);
    shared_ptr<DbRow> ptr = T->bigMap_->get(oid);

    /*
      ovaj sol mi znaci u bigMap::resolve jesam li ovim pozivom
      resolveToQuery uspio dobit objekt sa ide-m id.
      ako odgovor bude false, onda cu ga oznacit kao NOT EXISTENT.

      ako je u medjuvremenu objekt invalidiran a ja ga nadjem,
      moram vratit true jer ce se inace oznaci kao NON_EXISTENT,
      sto nije istina.
    */ 
    if (oid == id) {
      sol = true;
    }

    if (ptr->flag_ != TUNA_RESERVED
      || ptr->ln_ != this || ptr->qid_ != qid ) {
      make_log("flag link and qid do not match, OBJECT MODIFIED!", oid);
      continue;
    }

    ptr->object_ = convertGeneric(tablename, rec[i]);
    ptr->flag(TUNA_OK);
  }

  return sol;
}

bool QueueLink::resolveToQuery(query_id qid, object_id id, Tuna *T) {
  Guard g(lock_);
  bool sol = false;

  while(queryQueue_.size()) {
    pair<query_id, result> ret = pipeline_->retrieve();
    assert(ret.first == queryQueue_.front()); 
    queryQueue_.pop();
    
    if (resolveResult(ret.first, ret.second, id, T)) {
      sol = true;
    }
    if (ret.first == qid) break;
  }

  return sol;
}

unsigned int QueueLink::curentQueueSize() {
  Guard g(lock_);
  return queryQueue_.size();
} 


}} // eval::tuna 


