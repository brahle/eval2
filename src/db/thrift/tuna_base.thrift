include "types.thrift"

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

  void reserve(1:list<i32> ids)

  bool destroy(1:i32 id)
  
  /*
   * these are per-model methods 
   */

}
