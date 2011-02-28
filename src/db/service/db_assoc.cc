/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_DB_ASSOC__HPP_
#define EVAL_TUNA_DB_ASSOC__HPP_

namespace eval { namespace tuna {

DbAssoc::DbAssoc() {}

bool DbAssoc::touch(object_id id) {
  // must be locked here.
  if (!assoc_.count(id)) {
    assoc_[id] = shared_ptr<DbRow>(new DbRow(id));
    return true;
  }
  return false;
}

vector<object_id> DbAssoc::reserve(vector<object_id> ids) {
  Guard g(lock_);
  vector<object_id> sol;

  for (unsigned int i = 0; i < ids.size(); ++i) {
    if (touch(ids[i])) {
      sol.push_back(ids[i]);
      make_log("rezervacija: nije u bigMap.", ids[i]);
    } else {
      make_log("rezervacija: nasao u bigMap, ignoriram.", ids[i]);
    }
  }

  return sol;
}

void DbAssoc::reserveSent(vector<object_id> ids, 
  shared_ptr<QueueLink> ln, query_id qid) {

  Guard g(lock_);
  for (unsigned int i = 0; i < ids.size(); ++i) {
    assert( assoc_.count(ids[i]) );

    shared_ptr<DbRow> ptr = assoc_[ids[i]];

    make_log("rezervacija: submited!", ids[i]);
    ptr->flag(TUNA_RESERVED);
    ptr->callBack(ln, qid);
  } 
} 


}} // eval::tuna 


#endif
