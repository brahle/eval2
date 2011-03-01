/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_TUNA_CLASS_H_
#define EVAL_TUNA_TUNA_CLASS_H_

namespace eval { namespace tuna {

class Tuna {
 public:
  
  Tuna();

  void reserve(const object_id &id);

  /*
    ids is not reference, becouse i need to copy whole vector and
    change it. 
   */
  void reserve(vector<object_id> ids);

  string tablename[10];

  vector<shared_ptr<DbRow> > multiGet(const vector<object_id> &ids);

  /* blocking function */
  vector<object_id> simpleQuery(string qname, vector<string> data);
  
  shared_ptr<DbAssoc> bigMap_;

  shared_ptr<ConnectionPool> connPool_;

};

}} // eval::tuna

#endif
