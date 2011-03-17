/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

/*
  plain sql query with no parameters.
 */
Query::Query(const string &sql) {
  argc_ = 0; 
  qname_ = "_plain_sql__" + sql; 
  sql_.push_back(sql);
}


/*
  query which calls tuna_pget_%tablename% with
  array of object_ids. tablename is excracted from
  first id in array.
 */
Query::Query(const vector<object_id> &ids, Tuna *T) {
  string &tb = T->tablename[ ids[0] % TUNA_MAX_TABLES ];
  
  if (!tb.size()) {
    throw TunaException( "id is invalid. no table has mod: " +
      stoi( ids[0] % TUNA_MAX_TABLES ) );
  }

  argc_ = 0;
  qname_ = "_tuna_pget_" + toPgArray(ids);
  sql_.push_back(
    "execute tuna_pget_" + tb + "('" + toPgArray(ids) + "');"
  );
}

/*
  actions with no parametars: delete
 */
Query::Query(const string &action, const string &tb, Tuna *T) {
  if (action=="delete") {
    sql_.push_back("delete from " + tb + " where id = ?;");
    argc_ = 1;
    qname_ = "_" + action + "_" + tb;
    invalidate_.push_back( make_pair(tb, 1) );
  }
}

/*
  actions with parametars: insert, update
 */
Query::Query(const string &action, const string &tb, 
  vector<string> col, Tuna *T) {

  vector<string> upitnici(col.size(), "?");
  string sql;

  if (action=="insert") {
    sql = "insert into " + tb + " (" + join(",", col) + ") ";
    sql = sql + "values(" + join(",", upitnici) + ");";

    argc_ = col.size();
    qname_ = "_" + action + "_" + tb + "-" + join("_", col);
    sql_.push_back(sql);
    sql_.push_back("select currval('tuna_seq_" + tb + "');");
  }

  if (action=="update") {
    sql = "update " + tb + " set ";
    sql = sql + join(" = ?, ", col) + " = ? where id = ?;";

    sql_.push_back(sql);
    argc_ = col.size() + 1;
    qname_ = "_" + action + "_" + tb + "-" + join("_", col);

    invalidate_.push_back( make_pair(tb, argc_) );
  }
}

/*
  Constructs query from something like this:

    :_get_tables:::
      select name, mod from system.tables; 
 */
Query::Query(vector<string> lines, Tuna *T) {
  if (lines[0][0] != ':') {
    throw TunaException("query:: Parse error.");
  }

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

  lines.erase( lines.begin(), 1 + lines.begin() );
  
  sql_ = split(";", join(" ", lines));
  T->querys_[qname_] = shared_ptr<Query>(this);
}

/*
  replace '?' with params
 */
string Query::apply(const vector<string> &params,
  unsigned int part, shared_ptr<connection> C) {

  vector<string> tmp = split("?", sql_[part], true);
  string sol = tmp[0]; 

  if (tmp.size() - 1 > params.size()) {
    throw TunaException("query :: not enough params.");
  }

  for (unsigned int i = 1; i < tmp.size(); ++i) {
    sol = sol + "'" + C->esc(params[i-1]) + "'" + tmp[i];
  }

  return sol;
}

/*
  this is called after executing a query if
  affected rows != 0. invokeInvalidation will invalidate
  all objs that query changed.
 */
void Query::invokeInvalidation(const vector<string> &params, Tuna *T) {

  for (unsigned int i = 0; i < invalidate_.size(); ++i) {
    pair <string, int> pr = invalidate_[i];

    if (pr.second == -1) {
      T->bigMap_->invalidateTable( T->tableMod(pr.first) );
    } else {
      T->bigMap_->invalidateObject(atoi(params[pr.second-1].c_str()));
    }
  }
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


}} // eval::tuna 
