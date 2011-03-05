-- this is query file for project evaluator
-- query names starting with _ are of internal use

:_get_tables:::
  select name, mod
  from system.tables; 

:get_tasks:::
  select * from tasks;

:get_jobs:::
  select * from jobs;

:filip:2:users,task(2),solution(1):
  select * from govno;
  delete from users;
  delete from solution where id = ?;
  delete from task where id = ?;

