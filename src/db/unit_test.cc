#include <iostream>
#include <tuna_client_radi.h>


eval::TunaClient T("localhost", 9090);

int main() {
  T.destroy(202);
  return 0;
}

