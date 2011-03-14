/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

DbAssoc::DbAssoc() {
}

void DbAssoc::invokeCollector() {
  collectorIter_ = 0;
  vector< pair<long long, object_id> > sortedByTime;
  sortedByTime.reserve(timestamp_.size());

  for (map<object_id, long long>::iterator it = timestamp_.begin();
       it != timestamp_.end(); ++it) {
    sortedByTime.push_back(make_pair(it->second, it->first));    
  }

  sort(sortedByTime.rbegin(), sortedByTime.rend());
  for (int i = COLLECTOR_PERIOD; i < (int)sortedByTime.size(); ++i) {
    timestamp_.erase(sortedByTime[i].second);
    assoc_.erase(sortedByTime[i].second);
  }
}

/*
  check if object with given id exists in map, if not
  create him with flag EMPTY

  returns true if object don't exists
 */
bool DbAssoc::touch(object_id id) {
  timestamp_[id] = time(0);
  ++collectorIter_;

  if (collectorIter_ == COLLECTOR_PERIOD){
    invokeCollector();
  }
  // must be locked here.
  if (!assoc_.count(id)) {
    assoc_[id] = shared_ptr<DbRow>(new DbRow(id));
    return true;
  } else {
    return (assoc_[id]->flag_ == TUNA_EMPTY);
  }
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
    if (!this->assoc_.count(ids[i])) {
      make_log("OBJECT MODIFIED (reservation finished, \
but object is not in bigMap (garbage collector | thread-modified)!");
      continue;
    }

    shared_ptr<DbRow> ptr = get(ids[i]);
    ptr->flag(TUNA_RESERVED);
    ptr->callBack(ln, qid);
  }
} 

vector<object_id> DbAssoc::resolve(
  const vector<object_id> &ids, Tuna *T) {

  vector<object_id> sol;

  for (unsigned int i = 0; i < ids.size(); ++i) {
    bool touched; 
    int currentFlag, currentQid;
    object_id id = ids[i];
    QueueLink *currentLn;
    shared_ptr<DbRow> ptr;

    {
      Guard g(lock_);
      touched = touch(id); 
      ptr = assoc_[id];
      currentFlag = ptr->flag_;
      currentLn = ptr->ln_;
      currentQid = ptr->qid_;
      // trenutak A
    }

    if (touched) {

      sol.push_back( id );
      make_log("fetch: objekt nije u mapi [NOT RESERVED]!");

    } else {
      /*
        ako je u trenutku A objekt rezerviran ja mogu krenut
        u resolvanje pending query-a. dok to radim, moguce je
        da netko invalidira objekt ili napravi nesto s njim,
        no onda ce se to skuzit kad probam upisat rezultat
        u objekt. ako u resolvanju ne uspijem naic na tocno ovaj
        objekt, oznacujem ga sa NON_EXISTENT.

        stvar dodatno komplicira i mogucnost rusenja pipeline-a
        u tom slucaju resloveToQuery ce vratiti true, kao da je
        nasao objekt, no taj objekt nece imati flag TUNA_OK
        tada invalidiramo objekt (maknemo mu RESERVED flag)
       */
      if (currentFlag == TUNA_RESERVED) {
        int stat = currentLn->resolveToQuery(currentQid, id, T);
        /*
          nasao sam trazeni query, no objekt nije resolvan
          dakle sigurno ne postoji
        */
        if ( stat == TUNA_QID_WAS_IN_PIPELINE ) {
          Guard g(lock_);
          ptr->flag(TUNA_NON_EXISTENT); 
        } 
        /*
          nisam nasao ni query ni objekt
        */
        if ( !stat ) {
          Guard g(lock_);
          make_log("! invalidiranje uzrokovano greskom u pipeline-u",id);
          assoc_.erase(id); 
        }
      }
    }
  }

  return sol;
}

shared_ptr<DbRow> DbAssoc::get(object_id id) {
  touch(id);
  return assoc_[id]; 
}

void DbAssoc::invalidateObject(object_id id) {
  Guard g(lock_);
  make_log("object invalidated", id);
  assoc_.erase(id); 
}

void DbAssoc::invalidateTable(int tableMod) {
  Guard g(lock_);
  vector<object_id> sched;
  map < object_id, shared_ptr<DbRow> >::iterator i;

  make_log("invalidate table " + stoi(tableMod));

  for (i = assoc_.begin(); i != assoc_.end(); ++i) {
    if (i->first % TUNA_MAX_TABLES == tableMod) {
      sched.push_back(i->first);
    }
  }

  for (unsigned int i = 0; i < sched.size(); ++i) {
    assoc_.erase(sched[i]);
  }
}

}} // eval::tuna 
