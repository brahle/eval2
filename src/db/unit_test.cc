#include <iostream>
#include <tuna_client_radi.h>
#include <cstdlib>



int main() {
  eval::TunaClient T("localhost", 9090);
  system("sleep 5");
  T.ping();
  system("sleep 5");
  return 0;
}

