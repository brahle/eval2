/*
  name: insert 20 tasks, multi get them, and destroy
 */

#include <cstdlib>
#include <iostream>

#include "tuna_client.h"

using namespace std;
using boost::shared_ptr;

int test() {
  eval::TunaClient T("localhost", 9090);
 
  try {
    vector<int> ids;
    vector<Task> org;
    for (int i = 0; i < 20; i++) {
      Task tmp;
      tmp.type = rand()%100;
      tmp.filePath = "sdlfkj";
      tmp.id = T.insertTask(tmp);
      ids.push_back(tmp.id);
      org.push_back(tmp);
    }

    vector<Task> v = *T.getTasks(ids);
    for (int i = 0; i < v.size(); ++i) {
      if (org[i] != v[i]) return 1;
    }

    for(int i = 0; i < ids.size(); ++i) {
      T.destroy( ids[i] );
    }

  } catch (...) {
    return 1;
  }

  return 0;
}

int main() {

  return test();
}

