/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_QUEUE_LINK__HPP_
#define EVAL_TUNA_QUEUE_LINK__HPP_

namespace eval { namespace tuna {

class QueueLink : public Link_base {

 public:

  QueueLink();

  query_id push(vector<object_id> ids);

  void reserve(vector<object_id> ids);

  void resolveResult(result rec);

  void resolveAllPendingQuerys();

  void resolveToQuery(query_id qid);

  unsigned int curentQueueSize();

 private:

  shared_ptr<pipeline> pipeline_;

  queue<query_id> queryQueue_;

};

}} // eval::tuna 


#endif
