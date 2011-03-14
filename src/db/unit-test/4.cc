/*
  name: random getTask
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);

  for (int i = 0; i < 200; ++i) {
    int id = rand()%1000000;
    try {
      try {
        T.getTask(id);
      } catch (const TunaExp &e) {
      }
    } catch(...) {
      return 1;
    }
  }

  return 0;
}

int main() {

  return test();
}
