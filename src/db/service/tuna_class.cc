/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

namespace eval { namespace tuna {


Tuna::Tuna() {
  /* treba li koristiti shared_ptr??? */
  tablename[8] = "solutions";
  tablename[9] = "tasks";
  
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
 
  cout << "tuna class output" << endl;
  for (unsigned int i = 0; i < ids.size(); ++i) {
    cout << ids[i] << ' ' << bigMap_->row(ids[i])->echo() << endl;
  }

  return vector<shared_ptr<DbRow> >(0);
}


}} // eval::tuna
