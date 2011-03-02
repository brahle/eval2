
CREATE TABLE "tasks" (
  id integer PRIMARY KEY,
  type integer ,
  file_path varchar(150) 
);

CREATE TABLE "test_cases" (
  id integer PRIMARY KEY,
  task_id integer REFERENCES tasks(id),
  file_path varchar(100) 
);

CREATE TABLE "submissions" (
  id integer PRIMARY KEY,
  task_id integer REFERENCES tasks(id),
  code_path varchar(150) 
);

CREATE TABLE "jobs" (
  id integer PRIMARY KEY,
  test_case_id integer REFERENCES test_cases(id),
  finished boolean 
);
