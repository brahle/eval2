/*
  name: getTask
 */

#include <cstdlib>
#include <iostream>

#include "../client/client_radi.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);

  try {
    shared_ptr<Task> A = T.getTask(202);
  } catch (const TunaExp &e) {
    cout << e.why << endl;
    return 1;
  }

  return 0;
}

int main() {

  return test();
}

