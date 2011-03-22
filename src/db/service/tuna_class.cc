/**
/ Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

#include <tuna.h>

namespace eval { namespace tuna {

Tuna::Tuna() {
  {
    Guard g(lock_);
    bigMap_ = shared_ptr<DbAssoc> (new DbAssoc);
    connPool_ = shared_ptr<ConnectionPool> (new ConnectionPool);
  }

  cerr << loadQuerys() << " querys loaded.\n";
  cerr << loadTables() << " tables loaded.\n";

  make_log("initialization complete.");
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
  doubleQ tb = doubleQuery("_get_tables", vector<string>());

  Guard g(lock_);

  for (int i = 0; i < tb.size; ++i) {
    cerr << "  table " << tb.strs[i] <<
      " has mod " << tb.nums[i] << ".\n";

    tablename[ tb.nums[i] ] = tb.strs[i];
  }

  return tb.size;
}

/*
  this is ugly. maybe rewrite or move out.
 */
int Tuna::loadQuerys() {
  Guard g(lock_);

  ifstream inSql(QUERY_FILE.c_str()); 
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

simpleQ Tuna::simpleQuery(const string &qname,
  const vector<string> &args) {

  waitForLock();
  simpleQ sol;

  try {
    result r =
      connPool_->getFreeWorkLink()->_execQuery(qname, args, this);


    for (unsigned int i = 0; i < r.size(); ++i) {
      if (!r[i][0].is_null()) {
        sol.nums.push_back( r[i][0].as<int>() );
        ++sol.size;
      }
    }
  }
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

  return sol;  
}

doubleQ Tuna::doubleQuery(const string &qname,
  const vector<string> &args) {

  waitForLock();
  doubleQ sol; 

  try {
    result r =
      connPool_->getFreeWorkLink()->_execQuery(qname, args, this);

    for (unsigned int i = 0; i < r.size(); ++i) {
      if (!r[i][0].is_null()) {
        sol.nums.push_back( r[i][0].as<int>() );
        sol.strs.push_back( r[i][1].as<string>() );
        ++sol.size;
      }
    }
  }
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

  return sol;  
}

fullQ Tuna::fullQuery(const string &qname,
  const vector<string> &args) {

  waitForLock();
  fullQ sol;

  try {
    result r =
      connPool_->getFreeWorkLink()->_execQuery(qname, args, this);

    if (!r.size()) {
      return sol;
    }

    for (unsigned int i = 0; i < r[0].size(); ++i) {
      sol.col_name.push_back( string(r[0][i].name()) );
    }

    for (unsigned int i = 0; i < r.size(); ++i) {
      vector<string> row(r[i].size());
      for (unsigned int j = 0; j < r[i].size(); ++j) {
        row.push_back( r[i][j].as<string>() );
      }
      sol.data.push_back( row );
    }
  }
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

  return sol;
}

simpleQ Tuna::reserveFrom(const string &qname,
  const vector<string> &args) {

  waitForLock();

  simpleQ sol = simpleQuery(qname, args);
  reserve(sol.nums);

  return sol;
}

/*
 * insert, update and destroy
 */

bool Tuna::destroy(object_id id) {
  string &tb = tablename[id % TUNA_MAX_TABLES];
 
  result r;

  try {
    r = connPool_->getFreeWorkLink()->_execQuery(
      shared_ptr<Query> (new Query("delete", tb, this)),
      vector<string>(1, stoi(id)), this);
  }
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

  return !!(r.affected_rows());
}

object_id Tuna::insert(const string &tb,
  vector<pair<string,string> > data) {

  vector<string> keys, vals;
  object_id sol = -1;
 
  try {
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
    
    sol = r[0][0].as<int>();
    bigMap_->invalidateObject(sol); 

  }
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

  return sol;
}


bool Tuna::update(const string &tb, vector<pair<string,string> > data) {
  vector<string> keys, vals;
  
  result r;

  try {
    for (unsigned int i = 0; i < data.size(); ++i) {
      keys.push_back(data[i].first);
      vals.push_back(data[i].second);
    } 

    if (keys[0] != "id") {
      throw TunaException("first column must be id.");
    }

    vals.push_back( vals[0] );

    vals.erase(vals.begin());
    keys.erase(keys.begin());

    r = connPool_->getFreeWorkLink()->_execQuery(
      shared_ptr<Query> (new Query("update", tb, keys, this)),
      vals, this);

  }
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

  return !!r.affected_rows();
}

/* 
  send async querys to fetch requested objects.
*/
void Tuna::reserve(vector<object_id> ids) {

  waitForLock();

  try {

    checkIds(ids);

    ids = bigMap_->reserve(ids);
        
    //memcache!;

    connPool_->getFreeQueueLink()->reserve(ids, this);
  
  } 
  /*
    is it OK for reserve to throw exceptions?
    we will enable this for now.. 
    catch(...) {}
    if you decide to disable exceptions for reserve()
    you must change template/tuna.thrift.template
    on line 53. -> remove throws (1:TunaExp e)
   */
  catch (const TunaException &e) { throw; }
  catch (const exception &e) { handleStlException(e); }
  catch (...) { handleOtherException(); }

}

int Tuna::tableMod(const string &tb) {
  for (unsigned int i = 0; i < TUNA_MAX_TABLES; ++i) {
    if (tb.size() && tablename[i] == tb)
      return i;
  }
  throw TunaException("unknown table.");
}

void Tuna::checkIds(const vector<object_id> &ids) {
  for (unsigned int i = 0; i < ids.size(); ++i) {
    if (!tablename[ids[i] % TUNA_MAX_TABLES].size()) {
      throw TunaException("object id don't match any table");
    }
  }

  if (ids.size() >= (unsigned int) COLLECTOR_PERIOD) {
    throw TunaException((string)"multiGet query to large." + 
      "this is adjustable in service/tuna.h:COLLECTOR_PERIOD");
  }
}

}} // eval::tuna
