/*
  name: non-existent query
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);

  try {
    T.simpleQuery("1filip", vector<string>());
  } catch (const TunaExp &e) {
    cout << e.why << endl;
    cout << e.query << endl;
    return 0;
  }

  return 1;
}

int main() {

  return test();
}
