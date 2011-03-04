typedef i32 object_id

exception TunaExp {
  1: i32 what,
  2: string why,
  3: string query,
  4: object_id oid
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

