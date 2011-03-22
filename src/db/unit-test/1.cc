/*
  name: insert, get and destroy Task
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);

  try {
    Task a;
    a.type = 5;
    a.filePath = "test";
    int id = T.insertTask(a);
    cout << "inserted with id " << id << endl;
    shared_ptr<Task> A = T.getTask(id);
    
    if (A->filePath != a.filePath) {
      return 1;
    }
    T.destroy(id);

  } catch (const TunaExp &e) {
    cerr << e.why << endl;
    return 1;
  } catch (...) {
    return 1;
  }

  return 0;
}

int main() {

  return test();
}


