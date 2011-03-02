/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_DB_ASSOC_H_
#define EVAL_TUNA_DB_ASSOC_H_

namespace eval { namespace tuna {

class DbAssoc {
 public:

  DbAssoc() {}

  /*
    Checks if object with id already exists in map,
    if not -> create him and return true
    if exists -> return false (no need to reserve him again)
   */
  bool touch(object_id id);

  /*
    Checks which ids are already available in assoc_,
    and returns only those who needs to be reserved
   */
  vector<object_id> reserve(const vector<object_id> &ids);

  /*
    when some of the QueueLink successfuly submits a query
    into the pipeline, this function will be invoked.
   */
  void reserveSent(const vector<object_id> &ids, QueueLink *ln, query_id qid);


  /*
    when request to fetch obejcts is made, i must check 
    for each object in assoc_ and see if he is OK or RESERVED
   */
  vector<object_id> resolve(const vector<object_id> &ids, Tuna *T);

  shared_ptr<DbRow> row(object_id id);

  friend class DbRow;
  friend class Tuna;
  friend class QueueLink;
  friend class WorkLink;

 private:

  Mutex lock_;

  std::map<object_id, shared_ptr<DbRow> > assoc_;

};

}} // eval::tuna 


#endif
