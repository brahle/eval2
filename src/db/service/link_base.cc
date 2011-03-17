/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#include <tuna.h>

namespace eval { namespace tuna {

Link_base::Link_base() {
  Guard g(lock_);
  dsn_ = "host=localhost user=deval password=deval dbname=deval";
  init();
}

void Link_base::init() {
  conn_ = shared_ptr<connection>(new connection(dsn_));
  
  /*
    ovo je poseban slucaj za query koji nemoze proci
    kroz work_link jer je ovo konstruktor od work_link-a.
    potrebno je napravit transactor i biti siguran
    da je query prosao.
  */
  try {
    work W(*conn_);
    /*
      tuna_refresh() will prepare all the tuna_pget_%tablename%
      on current connection.
    */
    W.exec("select tuna_refresh();");
    W.commit();
  } catch (const exception &e) {
    make_log("call to tuna_refresh() failed.");
    throw;
  }

  work_ = shared_ptr<work>(new work(*conn_));

  make_log("starting connection!");
}

void Link_base::resetConnection() {
  // moram bit lockan
  make_log("resetConnection requested."); 
  work_.reset();
  conn_.reset();
  init();
}

}} // eval::tuna 


