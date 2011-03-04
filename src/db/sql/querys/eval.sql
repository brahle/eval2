-- this is query file for project evaluator
-- query names starting with _ are of internal use

:_get_tables:::
  select name, mod
  from system.tables; 

:get_tasks:::
  select * from tasks;

:get_jobs:::
  select * from jobs;
