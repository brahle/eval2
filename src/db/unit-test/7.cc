/*
  name: agressive random multiGet and reserve
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);
 
  for (int i = 0; i < 100; i++ ) {
    vector<int> ids;
    for (int j = 0; j < rand()%1000; ++j) {
      ids.push_back( rand()%10000 );
    }
    try {
      if (i%2) {
        T.getTasks(ids);
      } else {
        T.reserve(ids);
      }
    } catch (const TunaExp &e) {
      cout << e.why << endl;
    } catch (...) {
      return 1;
    }
  }



  return 0;
}

int main() {

  return test();
}
