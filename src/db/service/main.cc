/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include "db/service/tuna.h"

int main() {
  eval::tuna::Tuna T;

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

 // 2 je task 
  return 0;
}

