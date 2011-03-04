/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_QUEUE_LINK_H_
#define EVAL_TUNA_QUEUE_LINK_H_

namespace eval { namespace tuna {

class QueueLink : public Link_base {

 public:

  QueueLink();

  query_id push(const vector<object_id> &ids, Tuna *T);

  void reserve(const vector<object_id> &ids, Tuna *T);

  void resolveAllPendingQuerys(Tuna *T);

  bool resolveResult(query_id qid, result rec, object_id id, Tuna *T);

  /*
    resolve reservations to some query, and check if
    object with id %id is resolved!
   */
  bool resolveToQuery(query_id qid, object_id id, Tuna *T);

  unsigned int curentQueueSize();

 private:

  shared_ptr<pipeline> pipeline_;

  queue<query_id> queryQueue_;

};

}} // eval::tuna 


#endif
