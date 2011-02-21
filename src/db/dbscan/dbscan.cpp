/*
  Scans the databases for dumb table objects, and
  outputs .thrift configuration file with structures.

  Translates postgreSql data types to thrift simple types.
 */

#include <iostream>
#include <string>
#include <pqxx/pqxx>

#include "json.h"

using namespace std;
using namespace pqxx;
using namespace json_spirit;

string type_convert( string pg_type ) {
  string names[] = {"bool","byte","string","i64","double"};

  string i64[] = {"smallint", "integer", "bigint", "serial", "bigserial"};
  string dbl[] = {"real", "double precision"};
  string str[] = {"name", "text", "character varying", 
    "varchar", "character", "char"};
  string byte[] = {"\"char\""};
  string boolean[] = {"boolean"};

  vector<string> thrift_names(names, names + 5);
  vector<string> pg_types[5];

  pg_types[4] = vector<string>(dbl, dbl + 2);
  pg_types[3] = vector<string>(i64, i64 + 5);
  pg_types[2] = vector<string>(str, str + 6);
  pg_types[1] = vector<string>(byte, byte + 1);
  pg_types[0] = vector<string>(boolean, boolean + 1);

  for (int i = 0; i < 5; i++ ) {
    for (int j = 0; j < pg_types[i].size(); ++j) {
      if( pg_type == pg_types[i][j] ) {
        return names[i];
      }
    }
  }
  cerr << "failed to convert type:" << pg_type << endl;
  return "exception";
}

string pg_type2thrift_type( string pg_type ) {
  if (pg_type.find('[') != -1) {
    return "list<" + pg_type2thrift_type(
      pg_type.substr(0, pg_type.size() - 2)) + ">";
  }

  if (pg_type.find('(') != -1) {
    pg_type = pg_type.substr(0, pg_type.find('('));
  }

  return type_convert(pg_type);
}

void getFields( string tablename, work &W ) {
  string query = "SELECT"
      " a.attname as \"column\","
      " pg_catalog.format_type(a.atttypid, a.atttypmod) as \"type\""
      " FROM"
      " pg_catalog.pg_attribute a"
      " WHERE"
      " a.attnum > 0"
      " AND NOT a.attisdropped"
      " AND a.attrelid = ("
          " SELECT c.oid"
          " FROM pg_catalog.pg_class c"
              " LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace"
          " WHERE c.relname = '" + tablename + "'"
              " AND pg_catalog.pg_table_is_visible(c.oid)"
      ");";

  result R = W.exec(query);

  cout << "struct " << tablename << endl;
  cout << "{" << endl;

  int index = 1;

  for (result::const_iterator r = R.begin();
      r != R.end(); ++r, ++index) {

    if (r != R.begin()) {
      cout << "," << endl;
    }
    cout << "  " << index << ": " << pg_type2thrift_type(r[1].c_str()) 
      << " " << r[0].c_str();
  }

  cout << endl << "}" << endl;
}

string getDsn() {
  Value val = file2json(
    "../../../configs/db/dsn.json"
  );
  string sol;

  const Object& dsn = val.get_obj();

  for (int i = 0; i < dsn.size(); i++) {
    sol += dsn[i].name_ + "=" +  dsn[i].value_.get_str() + " ";
  }

  return sol;
}


int main() {

  try {
    connection C(getDsn());

    work W(C);
    W.exec("select tuna_refresh();");
    result R = W.exec("select name from system.tables;");

    for (result::const_iterator r = R.begin();
      r != R.end(); ++r) {
      getFields( r[0].c_str(), W );
    }
  } catch (exception &e) {
    cerr << "database error" << endl;
    return 1;
  }

  return 0;
}

