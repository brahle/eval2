/*
  name: check if tableMod(tableName(x)) == x
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);
 
  for (int i = 0; i < 100; i++ ) {
    try {
      if (T.tableMod(*T.tableName(i)) != i) {
        cout << *T.tableName(i) << ' ' <<
          T.tableMod(*T.tableName(i)) << ' ' << i << endl;
        return 1;
      }
      cout << "passed: " << *T.tableName(i) << endl;
    } catch (const TunaExp &e) {
      if (e.why != "unknown table.") {
        cout << "unexpected exception: " << endl;
        cout << e.why << endl;
        return 1;
      }
    } catch (...) {
      cout << "other exception thrown." << endl;
    }
  }

  return 0;
}

int main() {

  return test();
}

