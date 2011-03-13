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
 
  try {
    for (int i = 0; i < 100; i++ ) {
      vector<int> ids;
      for (int j = 0; j < rand()%1000; ++j) {
        ids.push_back( rand()%10000 );
      }
      if (i%2) {
        T.getTasks(ids);
      } else {
        T.reserve(ids);
      }
    }

  } catch (const TunaExp &e) {
    if (e.why != "id and object type do not match") {
      cout << e.why << ' ' << e.what << endl;
      return 1;
    }
  } catch (const exception &e) {
    cout << e.what() << endl;
    return 1;
  } catch (...) {
    return 1;
  }

  return 0;
}

int main() {

  return test();
}
