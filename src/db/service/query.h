/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_QUERY_H_
#define EVAL_TUNA_QUERY_H_

namespace eval { namespace tuna {

class Query {

 public:
  
  Query(vector<string> lines, Tuna *T);
  
  Query(const string &action, const string &tb, Tuna *T);

  Query(const string &action, const string &tb, 
    vector<string> col, Tuna *T);

  string apply(const vector<string> &params, unsigned int part,
    shared_ptr<connection> C);

  void invokeInvalidation(const vector<string> &params, Tuna *T);

  void debug();

  string qname_;

  vector<string> sql_;

  int argc_;

  /*
    ponekad izvrsavanje query-a invalidira neke objekte iz cahce-a.
    pair<tb, redni_br>
    tb je ime tabele iz koje ce se objekt invalidirat
    ako je redni_br = -1 onda se invalidiraju svi objekti iz tablice.
    ako nije, onda se invalidira onaj objekt koji ima id
    redni_br-og parametra pri pozivu querya.
   */
  vector< pair<string,int> > invalidate_;

 private:
  Query();

};

}} // eval::tuna 


#endif
