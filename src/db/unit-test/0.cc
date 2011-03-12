/*
  name: is tuna server up and running?
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  try {
    eval::TunaClient T("localhost", 9090);
    T.ping();
  } catch(...) {
    return 1;
  }

  return 0;
}

int main() {

  return test();
}

