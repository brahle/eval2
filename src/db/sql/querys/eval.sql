-- this is query file for project evaluator
-- query names starting with _ are of internal use

:_get_tables:::
  select mod, name
  from system.tables; 

:get_tasks:::
  select * from tasks;

:get_jobs:::
  select * from jobs;

:filip:::
  select * from govno;


:wrong_double:::
  select 'anton', 2;

:ok_double:::
  select 2, 'anton';

