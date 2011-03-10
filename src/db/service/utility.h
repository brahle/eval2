#ifndef EVAL_TUNA_UTILITY_H_
#define EVAL_TUNA_UTILITY_H_

namespace eval { namespace tuna {

string stoi(object_id id);

string trim(const string &t);

string join(const string &delimiter, const vector<string> v);

vector<string> split(const string &delimiter, const string &t);

vector<string> split(const string &delimiter, const string &t,bool e);

void make_log(string log);

void make_log(string log, object_id id);

string toPgArray(vector<object_id> ids);

string trunc( string str, int w );

void echo( result r );

} } // namespace

#endif
