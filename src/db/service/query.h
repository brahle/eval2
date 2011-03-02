/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_QUERY_H_
#define EVAL_TUNA_QUERY_H_

namespace eval { namespace tuna {

class Query {

 public:

  Query();

  Query(const vector<string> &lines);
  
  string apply(const vector<string> &params, shared_ptr<connection> C);

  void debug();

  string qname_;

  string sql_;
  
  int argc_;

  vector< pair<string,int> > invalidate_;


};

}} // eval::tuna 


#endif
