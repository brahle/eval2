#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

int main() {
  try {
    connection C("host=localhost user=tuna password=tuna dbname=knoge");
    cout << "Connected to " << C.dbname() << endl;
    work W(C);

    result R = W.exec("SELECT name FROM system.tables;");

    cout << "Found " << R.size() << " rows:" << endl;
    string t;
    for (result::const_iterator r = R.begin();
      r != R.end(); ++r) {

      cout << r[0].size() << endl;
    }

    /*
    cout << "Doubling all employees' salaries..." << endl;
    W.exec("UPDATE employee SET salary=salary*2");

    cout << "Making changes definite: ";
    W.commit();
    cout << "ok." << endl;
    */
  } catch (const exception &e) {
    cerr << "bok" << endl;
    cerr << e.what() << endl;
    return 1;
  }
  return 0;
}

