/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

TunaException::TunaException(const string &_why) {
  this->why = _why;
  make_log("exception thrown: " + why);
}

TunaException::TunaException(const string &_why, const int &_what) {
  this->why = _why;
  this->what = _what;
  make_log("exception thrown: " + stoi(what) + " " + why);
}

TunaException::TunaException(const pqxx::pqxx_exception &e) {
  this->why = e.base().what();

  const pqxx::sql_error *s = 
    dynamic_cast<const pqxx::sql_error*>(&e.base());

  if (s) {
    this->query = s->query();
  }

  make_log("exception thrown: " + stoi(what) + " " + why);
}

}} // eval::tuna 


