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
  v.push_back(8);
  v.push_back(109);
  v.push_back(209);
  v.push_back(208);
  v.push_back(308);
  v.push_back(308);

  p.push_back(8);
  p.push_back(209);
  p.push_back(208);
  p.push_back(309);

  T.reserve(v);
  T.multiGet( p );

  return 0;
}

