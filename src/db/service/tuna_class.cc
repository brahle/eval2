/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

namespace eval { namespace tuna {


Tuna::Tuna() {
  /* treba li koristiti shared_ptr??? */
  tablename[0] = "test_cases";
  tablename[1] = "submissions";
  tablename[2] = "tasks";
  tablename[3] = "jobs";
  
  bigMap_ = shared_ptr<DbAssoc> (new DbAssoc);
  connPool_ = shared_ptr<ConnectionPool> (new ConnectionPool);
}

void Tuna::reserve(const object_id &id) {
  return reserve(vector<object_id> (1,id));
}

void Tuna::reserve(vector<object_id> ids) {

  ids = bigMap_->reserve(ids);
      
//  ids = memcache_.reserve(ids);

  connPool_->getFreeQueueLink()->reserve(ids, this);

}

/* blocking function */
vector<object_id> Tuna::simpleQuery(string qname,vector<string> data) {
  return connPool_->getFreeWorkLink()->simpleQuery(qname, data);
}

vector<shared_ptr<DbRow> > Tuna::multiGet(
  const vector<object_id> &ids) {

  vector<object_id> fromDb = bigMap_->resolve(ids, this);
 
  // *(static_pointer_cast<Job, void>(bigMap_->row(id)->object_));
  
  return vector<shared_ptr<DbRow> >(0);
}


}} // eval::tuna
