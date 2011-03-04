include "../../models/models.thrift"
include "types.thrift"
include "tuna_base.thrift"

namespace cpp eval.tuna

typedef i32 object_id

service Tuna extends tuna_base.Tuna_base {

  /*
   * these are per-model methods 
   */

  models.Task getTask(1:object_id id);
  list<models.Task> getTasks(1:list<object_id> ids);
  list<models.Task> getTasksFrom(
    1: string qname,
    2: list<string> data 
  ) throws (1:types.TunaExp e);

  bool updateTask(1:models.Task task) throws (1:types.TunaExp e);
  object_id insertTask(1:models.Task task) throws (1:types.TunaExp e);

}


