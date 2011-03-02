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

  cerr << loadQuerys() << " querys loaded.\n";
}

int Tuna::loadQuerys() {
  ifstream inSql(QUERY_FILE); 
  vector<string> oneQuery;
  string line;
  int sol = 0;

  while (getline(inSql, line)) {
    string::size_type loc = line.find("--", 0);
    if (loc != string::npos) {
      line = line.substr(0, loc);
    }
    line = trim(line);
    if (!line.size()) continue;
    if (line[0] == ':') {
      if (oneQuery.size()) {
        Query tmp(oneQuery);
        querys_[tmp.qname_] = tmp;
        sol++;
      }
      oneQuery.clear();
    }
    oneQuery.push_back(line);
  }
  if (oneQuery.size()) {
    Query tmp(oneQuery);
    querys_[tmp.qname_] = tmp;
    sol++;
  }
  return sol;
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

/* blocking function */
vector<shared_ptr<DbRow> > Tuna::multiGet(
  const vector<object_id> &ids) {

  vector<object_id> fromDb = bigMap_->resolve(ids, this);
 
  // tu fali memcache_->resolve(ids, this)

  connPool_->getFreeWorkLink()->resolve(fromDb, this);

  vector<shared_ptr<DbRow> > sol;
  Guard g(bigMap_->lock_);

  for (unsigned int i = 0; i < ids.size(); ++i) {
    assert( 
      (bigMap_->assoc_[ids[i]]->flag_ == TUNA_OK) ||
      (bigMap_->assoc_[ids[i]]->flag_ == TUNA_NON_EXISTENT)
    );
    sol.push_back( bigMap_->assoc_[ids[i]] );
  }

  return sol;
}


}} // eval::tuna
