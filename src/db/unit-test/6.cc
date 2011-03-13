/*
  name: insert 20 tasks, reserve, multi get (check if equal) and destroy them
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

    T.reserve(ids);
    vector<Task> v = *T.getTasks(ids);
    for (int i = 0; i < v.size(); ++i) {
      if (org[i] != v[i]) return 1;
    }

    for(int i = 0; i < ids.size(); ++i) {
      T.destroy( ids[i] );
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
