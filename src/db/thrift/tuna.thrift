include "../../models/models.thrift"

namespace cpp eval.tuna

exception TunaExp {
  1: i32 what,
  2: string why,
  3: string query,
  4: i64 oid
}

struct simpleQ {
  1: i32 size,
  2: list<i32> nums 
}

struct doubleQ {
  1: i32 size,
  2: list<i32> nums,
  3: list<string> strs
}

struct fullQ {
  1: i32 size,
  2: list<string> col_name,
  3: list< list<string> > data 
}

service Tuna {

  bool ping(),

  simpleQ simpleQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  doubleQ doubleQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  fullQ fullQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  simpleQ reserveFrom(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e)

  void reserve(1:list<i32> ids)

  bool destroy(1:i32 id)

//%MODEL_FUNCTIONS%

  /*
   * these are per-model methods 
   */

  models.Task getTask(1:i32 id);
  list<models.Task> getTasks(1:list<i32> ids);
  list<models.Task> getTasksFrom(
    1: string qname,
    2: list<string> data 
  ) throws (1:TunaExp e);

  bool updateTask(1:models.Task task) throws (1:TunaExp e);
  i32 insertTask(1:models.Task task) throws (1:TunaExp e);

}


