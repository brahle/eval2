/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_WORK_LINK_H_
#define EVAL_TUNA_WORK_LINK_H_

using std::string;
using std::queue;
using std::vector;

namespace eval { namespace tuna {

class WorkLink : public Link_base {

 public:

  WorkLink();

  void resolve(const vector<object_id> &ids, Tuna *T);
 
  vector<object_id> simpleQuery(string qname,
    const vector<string> &data);

 private:

  void resolveOneTable(const vector<object_id> &ids, Tuna *T);

  void resetWork();

};

}} // eval::tuna 


#endif
