/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_EXCEPTION_H
#define EVAL_TUNA_EXCEPTION_H

namespace eval { namespace tuna {

class TunaException : public TunaExp {
 public:
  
  TunaException( const string& _why );

  TunaException( const string& _why, const int &_what );

  TunaException( const pqxx::pqxx_exception &e );

};

}} // eval::tuna 


#endif
