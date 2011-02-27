/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_WORK_LINK__HPP_
#define EVAL_TUNA_WORK_LINK__HPP_

using std::string;
using std::queue;
using std::vector;

namespace eval { namespace tuna {

WorkLink::WorkLink(string dsn) {
  dsn_ = dsn;
}

vector<object_id> WorkLink::simpleQuery(string qname,
  vector<string> data) {

  return vector<object_id>(0);
}


}} // eval::tuna 


#endif
