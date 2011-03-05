include "../../models/models.thrift"
include "types.thrift"
include "tuna_base.thrift"

namespace cpp eval.tuna

service Tuna extends tuna_base.Tuna_base {

  /*
   * these are per-model methods 
   */

  models.Task getTask(1:i32 id);
  list<models.Task> getTasks(1:list<i32> ids);
  list<models.Task> getTasksFrom(
    1: string qname,
    2: list<string> data 
  ) throws (1:types.TunaExp e);

  bool updateTask(1:models.Task task) throws (1:types.TunaExp e);
  i32 insertTask(1:models.Task task) throws (1:types.TunaExp e);

}


