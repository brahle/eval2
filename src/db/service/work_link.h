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

  vector<object_id> simpleQuery(string qname, vector<string> data);

 private:

};

}} // eval::tuna 


#endif
