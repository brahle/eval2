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
  int mod = -1; // Task ima neki mod od 0..99

  for (int i = 0; i < 200; ++i) {
      bool ok = 0;
      try {
        T.getTask(i);
      } catch (const TunaExp &e) {
        if (e.why == "id and object type do not match"
          || e.why == "object id don't match any table"
          ) {
          ok = 1;
        }
      } catch (...) {
        cout << "uhvatio sam drugi exception.." << endl;
        return 1;
      }
      /*
        dakle, ocekivano je da cu dobit jedan od ova dva exception-a
        ili, cu tocno ubost pravi mod za Task. ako Task ne postoji,
        dobit cu prazan objekt Task().
       */
      if (!ok && i != mod && mod != -1) {
        mod = i;
        return 1;
      }
  }
  return 0;
}

int main() {

  return test();
}
