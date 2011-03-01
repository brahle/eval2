/**
* Copyright 2011 Anton Grbin 
*
* TODO copy notice
*/

namespace eval { namespace tuna {

DbRow::DbRow(object_id id) {
  // bigMap must be locked
  id_ = id; 
  flag(TUNA_EMPTY);
}

void DbRow::flag(int flag) {
  // bigMap must be locked
  flag_ = flag;
  make_log( "oznacen kao " + ime_flag[flag], id_);
}

void DbRow::callBack(QueueLink *ln, query_id qid) {
  // bigMap must be locked
  ln_ = ln;
  qid_ = qid;
}

void DbRow::resolvePending(Tuna *T) {
  // bigMap must NOT be locked
  int currentFlag;
  QueueLink *currentLn;

  {
    Guard g(T->bigMap_->lock_);
    currentFlag = flag_;
    currentLn = ln_;
  }

  if (currentFlag == TUNA_RESERVED) {
    currentLn->resolveToQuery(qid_, T);
  }
  
  {
    Guard g(T->bigMap_->lock_);
    if (flag_ != TUNA_OK) {
      flag(TUNA_NON_EXISTENT);
    }
  }
}

string DbRow::echo() {
  if (flag_ != TUNA_OK) return "nije ok";
  string sol;

  result::tuple t = *tuple_; 

  printf("\n************%d*********\n", t.size());
  return "tu";

  for (unsigned int i = 0; i < t.size(); i++) {
    sol = sol + "   " + t[i].as<string>("<null>");
  }
  
  return sol;
}

/*result DbRow::getResult() {
  if (flag_ == TUNA_NON_EXISTENT) {
    throw("red ne postoji.");
  }

  if (flag_ == TUNA_OK || flag_ == TUNA_MODIFIED) {
    return result_;
  }

  throw("neznam");
}*/

}} // eval::tuna 


