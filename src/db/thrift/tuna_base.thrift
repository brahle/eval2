include "types.thrift"

typedef i32 object_id

service Tuna_base {

  bool ping(),

  types.simpleQ simpleQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:types.TunaExp e)

  types.doubleQ doubleQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:types.TunaExp e)

  types.fullQ fullQuery(
    1: string qname,
    2: list<string> data 
  ) throws (1:types.TunaExp e)

  types.simpleQ reserveFrom(
    1: string qname,
    2: list<string> data 
  ) throws (1:types.TunaExp e)

  void reserve(1:list<object_id> ids)

  bool destroy(1:object_id id)
  
  /*
   * these are per-model methods 
   */

}
