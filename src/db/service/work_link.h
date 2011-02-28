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

class WorkLink : public Link_base {

 public:

  WorkLink(string dsn);

  vector<object_id> execSimpleQuery(string qname, vector<string> data);

 private:

};

}} // eval::tuna 


#endif
