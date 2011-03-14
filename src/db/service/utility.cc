/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

#include <tuna.h>

namespace eval { namespace tuna {

string stoi(object_id id) {
  char tmp[20];
  sprintf(tmp, OBJECT_ID_PRINTF, id);
  return string(tmp);
}

string trim(const string &t) {
  string sol = t;

  while (sol.size() && sol[0] == ' ')
    sol = sol.substr(1);

  while (sol.size() && sol[sol.size()-1] == ' ')
    sol = sol.substr(0, sol.size()-1);

  return sol;
}

string join(const string &delimiter, const vector<string> v) {
  string sol = v[0];
  for (unsigned int i = 1; i < v.size(); ++i) {
    sol = sol + delimiter + v[i]; 
  }
  return sol;
}

vector<string> split(const string &delimiter, const string &t) {
  vector<string> sol;
  string curr;

  for (unsigned int i = 0; i < t.size(); ++i) {
    if (i < t.size() - delimiter.size() + 1
        && t.substr(i, delimiter.size()) == delimiter) {
      curr = trim(curr);
      if (curr.size()) {
        sol.push_back(curr);
      }
      curr = "";
      i += delimiter.size() - 1;
    } else {
      curr.push_back(t[i]);
    }
  }
  curr = trim(curr);
  if (curr.size()) {
    sol.push_back(curr);
  }
  return sol;
}

vector<string> split(const string &delimiter, const string &t,bool e) {
  vector<string> sol;
  string curr;

  for (unsigned int i = 0; i < t.size(); ++i) {
    if (i < t.size() - delimiter.size() + 1
        && t.substr(i, delimiter.size()) == delimiter) {
      sol.push_back(curr);
      curr = "";
      i += delimiter.size() - 1;
    } else {
      curr.push_back(t[i]);
    }
  }
  sol.push_back(curr);
  return sol;
}

void make_log(string log) {
  return;
  cerr << log << endl;
}

void make_log(string log, object_id id) {
  return;
  cerr << "object/" << id << ": " << log << endl;
}

string toPgArray(vector<object_id> ids) {
  char tmp[20];
  string sol = "{";
  for (unsigned int i = 0; i < ids.size(); ++i) {
    sprintf(tmp, OBJECT_ID_PRINTF, ids[i]);
    sol = sol + (i ? "," : "" ) + string(tmp);
  }
  return sol + "}";
}


string trunc( string str, int w ) {
  if (str.size() > (unsigned int) w)
    str = str.substr(0, w-3) + "...";
  return str;
}

void echo( result r ) {
  printf("fetched %ld rows.\n", r.size());
  int width = 12;
  if (!r.size()) return;

  for (unsigned int j = 0;j < r[0].size(); ++j)
    printf("%14s ", r[0][j].name());
  printf("\n");
  for (unsigned int i = 0; i < r.size(); i++) {
    for (unsigned int j = 0;j < r[i].size(); ++j)
      printf("%14s ", trunc( r[i][j].as<string>("") , width).c_str());
    printf("\n");
  }
}


void describeStlException(const exception &e) {
  make_log("! exception is not TunaException, typeid(e).name():");
  make_log( typeid(e).name() );
  make_log( e.what() );
}

void handleStlException(const exception &e) {
  describeStlException(e);
  throw;
}

void handleOtherException() {
  make_log("EXCEPTION FAULT, ABORTING");
  exit(1);
}

}}

