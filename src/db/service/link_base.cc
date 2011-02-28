/**
 * Copyright 2011 Anton Grbin 
 *
 * TODO copy notice
 */

#ifndef EVAL_TUNA_LINK_BASE__HPP_
#define EVAL_TUNA_LINK_BASE__HPP_

using std::cerr;
using std::string;
using std::queue;
using std::vector;

namespace eval { namespace tuna {

Link_base::Link_base() {
  Guard g(lock_);
  dsn_ = "host=localhost user=deval password=deval dbname=deval";
  init();
}

void Link_base::init() {
  conn_ = shared_ptr<connection>(new connection(dsn_));
  work_ = shared_ptr<work>(new work(*conn_));
  work_->exec("select tuna_refresh();");
  cerr << "starting connection!" << std::endl;
}


}} // eval::tuna 


#endif
