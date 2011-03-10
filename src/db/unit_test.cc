#include <iostream>
#include <client_radi.h>
#include <cstdlib>

using namespace std;
using boost::shared_ptr;

int main() {
  eval::TunaClient T("localhost", 9090);
  shared_ptr<Task> A = T.getTask(201);
  cout << A->id << A->type << A->filePath << endl;
  return 0;
}

