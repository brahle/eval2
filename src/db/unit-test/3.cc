/*
  name: id and object don't match
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);

  for (int i = 0; i < 2000; ++i) {
    if (i%100) {
      bool ok = 0;
      try {
        T.getTask(i);
      } catch (const TunaExp &e) {
        if (e.why == "id and object type do not match") {
          ok = 1;
        }
      }
      if (!ok) return 1;
    }
  }
  return 0;
}

int main() {

  return test();
}
