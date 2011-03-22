/*
  name: create and destroy task (simple example)
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);
 
  try {

    Task tmp;
    tmp.type = rand()%100;
    tmp.filePath = "bla bla bla";
    tmp.id = T.insertTask(tmp);

    Task tmp2 = *T.getTask( tmp.id );

    if (tmp != tmp2) {  
      cout << "nisu jednaki" << endl;
      return 1;
    }

    T.destroy( tmp.id );

    if (*T.getTask(tmp.id) != Task()) {
      cout << "trebao bi biti unisten" << endl;
      return 1;
    }
  } catch (...) {
    return 1;
  }

  return 0;
}

int main() {

  return test();
}

