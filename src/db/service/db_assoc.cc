/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

namespace eval { namespace tuna {

bool DbAssoc::touch(object_id id) {
  // must be locked here.
  if (!assoc_.count(id)) {
    assoc_[id] = shared_ptr<DbRow>(new DbRow(id));
    return true;
  }
  return false;
}

vector<object_id> DbAssoc::reserve(const vector<object_id> &ids) {
  Guard g(lock_);
  vector<object_id> sol;

  for (unsigned int i = 0; i < ids.size(); ++i) {
    if (touch(ids[i])) {
      sol.push_back(ids[i]);
    } else {
      make_log("rezervacija: nasao u bigMap, ignoriram.", ids[i]);
    }
  }

  return sol;
}

void DbAssoc::reserveSent(const vector<object_id> &ids, 
  QueueLink *ln, query_id qid) {

  Guard g(lock_);
  // sad je zakljucan i bigmap i connection pool
  for (unsigned int i = 0; i < ids.size(); ++i) {
    assert( this->assoc_.count(ids[i]) );

    shared_ptr<DbRow> ptr = assoc_[ids[i]];

    ptr->flag(TUNA_RESERVED);
    ptr->callBack(ln, qid);
  }
} 

vector<object_id> DbAssoc::resolve(
  const vector<object_id> &ids, Tuna *T) {

  vector<object_id> sol;

  for (unsigned int i = 0; i < ids.size(); ++i) {
    bool touched; 
    {
      Guard g(lock_);
      touched = touch(ids[i]); 
    }
    if (touched) {
      sol.push_back( ids[i] );
      make_log("fetch: objekt nije u mapi [NOT RESERVED]!");
    } else {
      assoc_[ids[i]]->resolvePending(T);
    }
  }

  return sol;
}

shared_ptr<DbRow> DbAssoc::row(object_id id) {
  Guard g(lock_);
  touch(id);
  return assoc_[id]; 
}


}} // eval::tuna 


