/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_TUNA_CLASS_H_
#define EVAL_TUNA_TUNA_CLASS_H_

namespace eval { namespace tuna {

class Tuna {
 public:
  
  Tuna();

  int loadQuerys();

  int loadTables();

  void waitForLock();

  /*
   * those two functions are specialized for reserving objects
   */
  void reserve(vector<object_id> ids);

  vector<object_id> reserveFrom(const string &qname,
    const vector<string> &args);

  /*
   * those two functions are specialized for fetching objects
   */
  vector<shared_ptr<void> > multiGet(const vector<object_id> &ids);

  vector<shared_ptr<void> > getFrom(const string &qname,
    const vector<string> &args);

  /*
   * these are functions for perfoming a query
   */
  vector<object_id> simpleQuery(const string &qname,
    const vector<string> &args);

  vector< pair<int, string> > doubleQuery(const string &qname,
    const vector<string> &args);

  pair< vector<string>, vector< vector<string> > > fullQuery(
    const string &qname, const vector<string> &args);

  /* fullQuery data structure explain:
   * pair <
   *   vector<string>    : column names
   *   vector<           : rows
   *     vector<string>  : fields in row 
   *   > fullQuery(...);
   */

  /*
   * these are hibernate functions
   */
  bool destroy(object_id id);

  object_id insert(const string &tb, vector<pair<string,string> >data);

  bool update(const string &tb, vector<pair<string,string> > data);


  /*
   * utility func
   */
  int tableMod(const string &tb);

  // variables:

  string tablename[TUNA_MAX_TABLES];

  shared_ptr<DbAssoc> bigMap_;

  shared_ptr<ConnectionPool> connPool_;

  map < string, shared_ptr<Query> > querys_;


 private:
  
  Mutex lock_;

};

}} // eval::tuna

#endif
