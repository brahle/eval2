/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

namespace eval { namespace tuna {

Link_base::Link_base() {
  Guard g(lock_);
  dsn_ = "host=localhost user=deval password=deval dbname=deval";
  init();
}

void Link_base::init() {
  conn_ = shared_ptr<connection>(new connection(dsn_));
  
  {
    work W(*conn_);
    W.exec("select tuna_refresh();");
    W.commit();
  }

  work_ = shared_ptr<work>(new work(*conn_));

  cerr << "starting connection!" << std::endl;
}


}} // eval::tuna 


