/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include "db/service/tuna.h"

using namespace std;

using namespace eval::tuna;

int main() {
  Tuna T;
  Task Z;

  Z.__isset.id = true;
  Z.__isset.type = true;
  Z.__isset.filePath = true;

  Z.id = 502;
  Z.type = 8;
  Z.filePath = "kenda";

  T.destroy(502);
  cout << T.update("tasks", serialize(Z)) << endl;

 /* 
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

  T.reserve(v);
  T.multiGet(v);

  vector<int> pp = T.reserveFrom("get_tasks", vector<string>());

  for (int i = 0; i < pp.size(); ++i)
    printf("%d ", pp[i]);

*/

  /*
  vector<int> v;
  vector<int> p;
  p.push_back(2);
  p.push_back(1);
  p.push_back(0);
  p.push_back(100);

  v.push_back(2);
  v.push_back(103);
  v.push_back(203);
  v.push_back(200);
  v.push_back(300);
  v.push_back(300);


  T.reserve(v);
  T.multiGet(p);
  */
  return 0;
}

