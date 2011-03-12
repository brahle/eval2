/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

#include <tuna.h>

namespace eval { namespace tuna {

DbRow::~DbRow() { make_log("OSLOBADJAM DBROW!", id_); }

DbRow::DbRow(object_id id) {
  // bigMap must be locked
  id_ = id; 
  flag(TUNA_EMPTY);
}

void DbRow::flag(int flag) {
  // bigMap must be locked
  flag_ = flag;
  make_log( "oznacen kao " + ime_flag[flag], id_);
}

void DbRow::callBack(QueueLink *ln, query_id qid) {
  // bigMap must be locked
  ln_ = ln;
  qid_ = qid;
}

void DbRow::resolvePending(Tuna *T) {
  /*
    only damage I can do here is to flag object as NON_EXISTENT
    when he exists. this will not happen because:

    if somebody has modified this object before reservation
    is resolved, then this object would be destroyed. so this
    function can't be called if object is invalidated.

    there is a problem if object is destroyed, and then 
    immediatly reserved, but in that case link and query_id will not
    match in function QueueLink::resolveResult() so it's OK.

    THIS CODE IS MOVED TO DbAssoc::resolve(...)
   */
}

}} // eval::tuna 


