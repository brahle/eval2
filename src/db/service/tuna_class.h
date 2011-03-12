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

  simpleQ reserveFrom(const string &qname,
    const vector<string> &args);

  /*
   * these are functions for perfoming a query
   */
  simpleQ simpleQuery(const string &qname,
    const vector<string> &args);

  doubleQ doubleQuery(const string &qname,
    const vector<string> &args);

  fullQ fullQuery(
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

 public:

  /*
   * those two functions are specialized for fetching objects
   * this code is here because functions are templated
   */
  
  template<class T>
  vector<T> multiGet(const vector<object_id> &ids) {

    waitForLock();
    vector<T> sol;
    vector<object_id> _input, fromDb, input = ids;
    T proto;

    for (unsigned int i = 0; i < ids.size(); ++i) {
      if (tablename[ids[i] % TUNA_MAX_TABLES] != getTableName(proto)) {
        throw TunaException("id and object type do not match");
      }
    }
    if (input.size() >= COLLECTOR_PERIOD) {
      throw TunaException("multiGet query to large. this is "
        + "adjustable in service/tuna.h:COLLECTOR_PERIOD");
    }
    /*
      check if these ids are objects of type T
     */
    while (input.size()) {

      /* check in bigMap which of these ids are already here. */
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
          //make_log( string("OBJECT MODIFIED!") ); 
        } else if (ptr->flag_ == TUNA_OK) {
          sol.push_back( *( static_pointer_cast<T, void>(ptr->object_) ) );
        } 
      }
      input = _input;
    }

    // sol.size() == ids.size()
    return sol;
  }

  template<class T> 
  vector<T> getFrom(const string &qname,
    const vector<string> &args) {

    waitForLock();

    return multiGet<T>( simpleQuery(qname, args).nums );
  }

};

}} // eval::tuna

#endif
