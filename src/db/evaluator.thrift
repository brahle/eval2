struct Task {         // tasks
  1 : i32 id,         // primary key
  2 : string title,   // varchar(128)
  3 : string text     // text
}

struct User {         // users_suf
  1: i32 id,          // primary key
  2: string ime,      // varchar(128)
  3: string prezime   // varchar(128)
}

struct Solution {     // solutions 
  1 : i32 id,         // primary key
  2 : i32 taskId,     // references tasks(id)
  3 : i32 userId,     // references users_suf(id)
  4 : i32 programingLanguageId,
  5 : string source,  // text
  6 : i32 timeSubmited 
}
