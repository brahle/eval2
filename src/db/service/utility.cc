
namespace eval { namespace tuna {

string toPgArray(vector<object_id> ids) {
  char tmp[20];
  string sol = "{";
  for (unsigned int i = 0; i < ids.size(); ++i) {
    sprintf(tmp, OBJECT_ID_PRINTF, ids[i]);
    sol = sol + (i ? "," : "" ) + string(tmp);
  }
  return sol + "}";
}


string trunc( string str, int w ) {
  if (str.size() > w)
    str = str.substr(0, w-3) + "...";
  return str;
}

void echo( result r ) {
  printf("fetched %ld rows.\n", r.size());
  int width = 12;
  if (!r.size()) return;

  for (int j = 0;j < r[0].size(); ++j)
    printf("%14s ", r[0][j].name());
  printf("\n");
  for (int i = 0; i < r.size(); i++) {
    for (int j = 0;j < r[i].size(); ++j)
      printf("%14s ", trunc( r[i][j].as<string>() , width).c_str());
    printf("\n");
  }
}

}}
