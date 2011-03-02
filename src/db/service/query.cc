/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

namespace eval { namespace tuna {

Query::Query() {}

/*
  Constructs query from something like this:

    :_get_tables:::
      select name, mod from system.tables; 
 */
Query::Query(const vector<string> &lines) {
  assert(lines[0][0] == ':');
  vector<string> params = split(":", lines[0].substr(1), true);

  qname_ = params[0];
  argc_ = params[1].size() ? atoi(params[1].c_str()) : 0;

  /* parses query header */ 
  if (params[2].size()) {
    vector<string> tmp = split(",", params[2]); 
    for (unsigned int i = 0; i < tmp.size(); ++i) {
      string::size_type loc = tmp[i].find("(", 0);
      if (loc != string::npos) {
        // this is disgusting!
        string tb = tmp[i].substr(0,loc);
        int parm = atoi(
          tmp[i].substr(loc + 1, tmp[i].size() - loc - 2).c_str());
        invalidate_.push_back( make_pair(tb, parm) ); 
      } else {
        invalidate_.push_back( make_pair(tmp[i], -1) );
      }
    }
  }

  qname_ = params[0];
  sql_ = "";
  for (unsigned int i = 1; i < lines.size(); ++i) {
    sql_ = sql_ + lines[i];
  }

}

string Query::apply(const vector<string> &params, 
  shared_ptr<connection> C) {

  vector<string> tmp = split("?", sql_, true);
  string sol = tmp[0]; 
  assert( tmp.size() - 1 == params.size() );

  for (unsigned int i = 1; i < tmp.size(); ++i) {
    sol = sol + "'" + C->esc(params[i-1]) + "'" + tmp[i];
  }
  return sol;
}

/*
  this code changes ? into $1, $2, $3, etc..
  sql_ = "";
  int n = 0;
  for (unsigned int i = 1; i < lines.size(); ++i) {
    vector<string> tmp = split("?", lines[i], true);
    for (unsigned int j = 0; j < tmp.size(); ++j) {
      sql_ = sql_ + tmp[j];
      if (j < tmp.size() - 1) {
        char parmFlag[10];
        sprintf(parmFlag,"$%d", ++n);
        sql_ = sql_ + string(parmFlag);
      }
    }
    sql_ = sql_ + " ";
  }
*/

void Query::debug() {
  cout << "name: " << qname_ << endl;
  cout << "argc: " << argc_ << endl;
  cout << "sql: " << sql_ << endl;
  if (invalidate_.size()) {
    cout << "invalidates:" << endl;
    for (unsigned int i = 0; i < invalidate_.size(); ++i) {
      cout << "table " << invalidate_[i].first << " on parameter "
        << invalidate_[i].second << endl;
    }
  }
  cout << endl;
}


}} // eval::tuna 


