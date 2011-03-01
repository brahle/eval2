
CREATE TABLE "tasks" (
  id integer primary key,
  title varchar(128) ,
  text text 
);

CREATE TABLE "users_suf" (
  id integer primary key,
  ime varchar(128) ,
  prezime varchar(128) 
);

CREATE TABLE "solutions" (
  id integer primary key,
  task_id integer references tasks(id),
  user_id integer references users_suf(id),
  programing_language_id integer ,
  source text ,
  time_submited integer 
);
