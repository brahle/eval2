/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include "db/service/tuna.h"

using namespace std;

using namespace eval::tuna;

Tuna T;


void test1() {
  Task Z;
  Z.__isset.id = true;
  Z.__isset.type = true;
  Z.__isset.filePath = true;

  Z.id = 502;
  Z.type = 8;
  Z.filePath = "kenda";

  // T.destroy(502);
  for (int i = 0; i < 10; ++i) {
    cout << T.insert("tasks", serialize(Z)) << endl; 
  }
}

void test2() {
  vector< pair<string,string> > tmp;
  vector<int> v;
  vector<int> p;

  tmp.push_back(make_pair("id","903"));
  tmp.push_back(make_pair("test_case_id","0"));
  tmp.push_back(make_pair("finished","f"));
  
  p.push_back(803);

  v.push_back(803);
  v.push_back(903);

  T.reserve(v);
  T.reserve(v);
  T.reserve(v);
  T.reserve(v);

  cout << T.update("jobs", tmp) << endl;
}

void test3() {
  vector<int> pp = T.reserveFrom("get_tasks", vector<string>());

  for (int i = 0; i < (int)pp.size(); ++i)
    printf("%d ", pp[i]);
}

void test4() {
  vector<int> v;
  vector<int> p;
  p.push_back(102);
  p.push_back(202);

  for (int i = 0; i < 10; ++i) {
    v.push_back(100*(i+1)+2);
  }

  T.reserve(v);
  T.multiGet<Task>(p);
}

int main() {
  test1();
  test2();

  return 0;
}

