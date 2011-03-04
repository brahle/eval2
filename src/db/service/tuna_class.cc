/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

namespace eval { namespace tuna {

Tuna::Tuna() {
  {
    Guard g(lock_);
    bigMap_ = shared_ptr<DbAssoc> (new DbAssoc);
    connPool_ = shared_ptr<ConnectionPool> (new ConnectionPool);
  }

  cerr << loadQuerys() << " querys loaded.\n";
  cerr << loadTables() << " tables loaded.\n";

}

/*
  ideja ove funkcije jest da mogu
  zakljucat sve, i onda na miru reload query-e i tabele
  kad bude trebalo. ..malo je skrbavo to za sad, mislit cu
  o tome sutra.
*/

void Tuna::waitForLock() {
  Guard g(lock_);
}

int Tuna::loadTables() {
  Guard g(lock_);

  result rec = 
    connPool_->getFreeWorkLink()->_execQuery("_get_tables", this);
  
  for (unsigned int i = 0; i < rec.size(); ++i) {
    cerr << "  table " <<
      rec[i][0].as<string>() << " has mod " << 
      rec[i][1].as<int>() << ".\n";

    tablename[ rec[i][1].as<int>() ] = rec[i][0].as<string>();
  }

  return rec.size();
}

/*
  this is ugly. maybe rewrite or move out.
 */
int Tuna::loadQuerys() {
  Guard g(lock_);

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
        new Query(oneQuery, this);
        sol++;
      }
      oneQuery.clear();
    }
    oneQuery.push_back(line);
  }
  if (oneQuery.size()) {
    new Query(oneQuery, this);
    sol++;
  }
  return sol;
}

vector<object_id> Tuna::simpleQuery(const string &qname,
  const vector<string> &args) {

  waitForLock();

  vector<object_id> sol;

  result r =
    connPool_->getFreeWorkLink()->_execQuery(qname, args, this);


  for (unsigned int i = 0; i < r.size(); ++i) {
    if (!r[i][0].is_null()) {
      sol.push_back( r[i][0].as<int>() );
    }
  }

  return sol;  
}

vector< pair<int, string> > Tuna::doubleQuery(const string &qname,
  const vector<string> &args) {

  waitForLock();
 
  vector< pair<int, string> > sol;

  result r =
    connPool_->getFreeWorkLink()->_execQuery(qname, args, this);

  for (unsigned int i = 0; i < r.size(); ++i) {
    if (!r[i][0].is_null()) {
      sol.push_back(
        make_pair(r[i][0].as<int>(), r[i][1].as<string>()) );
    }
  }

  return sol;  
}

pair< vector<string>, vector< vector<string> > > Tuna::fullQuery(
  const string &qname, const vector<string> &args) {

  waitForLock();
 
  vector<string> col, tmp;
  vector< vector<string> > data;

  result r =
    connPool_->getFreeWorkLink()->_execQuery(qname, args, this);

  if (!r.size()) {
    return make_pair(vector<string>(),vector< vector<string> >());
  }

  for (unsigned int i = 0; i < r[0].size(); ++i) {
    col.push_back( string(r[0][i].name()) );
  }

  for (unsigned int i = 0; i < r.size(); ++i) {
    tmp.clear();
    for (unsigned int j = 0; j < r[i].size(); ++j) {
      tmp.push_back( r[i][j].as<string>() );
    }
    data.push_back( tmp );
  }

  return make_pair(col, data);
}

vector<object_id> Tuna::reserveFrom(const string &qname,
  const vector<string> &args) {

  waitForLock();

  vector<object_id> sol;

  result r =
    connPool_->getFreeWorkLink()->_execQuery(qname, args, this);

  for (unsigned int i = 0; i < r.size(); ++i) {
    if (!r[i][0].is_null()) {
      sol.push_back( r[i][0].as<int>() );
    }
  }

  reserve(sol);

  return sol;  
}

vector<shared_ptr<void> > Tuna::getFrom(const string &qname,
  const vector<string> &args) {

  waitForLock();

  vector<object_id> sol;

  result r =
    connPool_->getFreeWorkLink()->_execQuery(qname, args, this);

  for (unsigned int i = 0; i < r.size(); ++i) {
    if (!r[i][0].is_null()) {
      sol.push_back( r[i][0].as<int>() );
    }
  }

  return multiGet(sol);
}

/*
 * insert, update and destroy
 */

bool Tuna::destroy(object_id id) {
  string &tb = tablename[id % TUNA_MAX_TABLES];
 
  result r = connPool_->getFreeWorkLink()->_execQuery(
    shared_ptr<Query> (new Query("delete", tb, this)),
    vector<string>(1, stoi(id)), this);

  return !!(r.affected_rows());
}

object_id Tuna::insert(const string &tb,
  vector<pair<string,string> > data) {

  vector<string> keys, vals;
  
  for (unsigned int i = 0; i < data.size(); ++i) {
    if (data[i].first == "id") {
      make_log("insert: poslao si mi ID kojeg cu izignorirat.");
      continue;
    }
    keys.push_back(data[i].first);
    vals.push_back(data[i].second);
  } 

  result r = connPool_->getFreeWorkLink()->_execQuery(
    shared_ptr<Query> (new Query("insert", tb, keys, this)),
    vals, this);
  
  object_id sol = r[0][0].as<int>();
  bigMap_->invalidateObject(sol); 

  return sol;
}


bool Tuna::update(const string &tb, vector<pair<string,string> > data) {
  vector<string> keys, vals;
  
  for (unsigned int i = 0; i < data.size(); ++i) {
    keys.push_back(data[i].first);
    vals.push_back(data[i].second);
  } 

  if (keys[0] != "id") {
    throw "first column must be id.";
  }

  vals.push_back( vals[0] );

  vals.erase(vals.begin());
  keys.erase(keys.begin());

  result r = connPool_->getFreeWorkLink()->_execQuery(
    shared_ptr<Query> (new Query("update", tb, keys, this)),
    vals, this);
  
  return !!r.affected_rows();
}

/* 
  send async querys to fetch requested objects.
*/
void Tuna::reserve(vector<object_id> ids) {

  waitForLock();

  ids = bigMap_->reserve(ids);
      
  //memcache!;

  connPool_->getFreeQueueLink()->reserve(ids, this);
}

/*  
  blocking function
  send querys to fetch objects and wait for the result.
*/
vector<shared_ptr<void> > Tuna::multiGet(
  const vector<object_id> &ids) {

  waitForLock();
  vector< shared_ptr<void> > sol;
  vector<object_id> _input, fromDb, input = ids;

  while (input.size()) {
    /*
      check in bigMap which of these ids are already here.
     */
    fromDb = bigMap_->resolve(input, this);
    /*
      memcache check should go here.
      --
      sync request only those we don't have.
     */
    connPool_->getFreeWorkLink()->resolve(fromDb, this);

    Guard g(bigMap_->lock_);
    _input.clear();
    for (unsigned int i = 0; i < input.size(); ++i) {
      object_id id = input[i];
      shared_ptr<DbRow> ptr = bigMap_->get(id);
      /*
        if requested object isn't in bigMap at all?
        there is posibility that some other thread changed
        one of our objects after we made
        sure that this object is in our bigMap. if this is
        so, I will do my while loop one more time with modified
        objects in my input.
       */
      if (ptr->flag_ != TUNA_OK && ptr->flag_ != TUNA_NON_EXISTENT) {
        _input.push_back(id);
        make_log("OBJECT MODIFIED!"); 
        continue;
      }

      sol.push_back(ptr->object_);
    }
    input = _input;
  }

  // sol.size() == ids.size()
  return sol;
}


int Tuna::tableMod(const string &tb) {
  for (unsigned int i = 0; i < TUNA_MAX_TABLES; ++i) {
    if (tablename[i] == tb)
      return i;
  }
  throw("unknown table.");
}

}} // eval::tuna
