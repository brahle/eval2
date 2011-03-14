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

  void resetLink();

  query_id push(const vector<object_id> &ids, Tuna *T);

  void reserve(const vector<object_id> &ids, Tuna *T);

  void resolveAllPendingQuerys(Tuna *T);

  int resolveResult(query_id qid, result rec, object_id id, Tuna *T);

  /*
    resolve reservations to some query, and check if
    object with id %id is resolved, and if query_id qid
    was in pipeline %uopce%?
    returns bitmap OBJ_WAS_IN, QID_WAS_IN
   */
  int resolveToQuery(query_id qid, object_id id, Tuna *T);

  unsigned int curentQueueSize();

 private:

  shared_ptr<pipeline> pipeline_;

  queue<query_id> queryQueue_;

};

}} // eval::tuna 


#endif
