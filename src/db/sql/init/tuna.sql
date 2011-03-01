-- THIS FILE MUST BE CONECATED WITH CREATE_DB.SQL 
-- WHILE SENDING TO PSQL!

--
-- fetch all visible tables (not from system schema!)
--
create or replace function tuna_get_tables() returns setof name as $tuna$
  begin
    return query select c.relname
      from pg_catalog.pg_class c
      left join pg_catalog.pg_namespace n on n.oid = c.relnamespace
      where c.relkind in ('r','') and n.nspname not in ('pg_catalog', 'pg_toast')
        and pg_catalog.pg_table_is_visible(c.oid);
  end;
$tuna$ language plpgsql;

--
-- prepare a tuna_pget_%name% prepared procedure for fast executing multigets
--
create or replace function tuna_create_prep(tb name) returns boolean as $tuna$
  begin
    begin
      execute 'deallocate tuna_pget_' || tb;
    exception
      when others then
        null;
    end;
    begin  
      execute 'prepare tuna_pget_' || tb ||
              ' as select ' || tb || 
              $$.*, '$$ || tb || $$' $$ || 
              ' as _tablename from ' || tb ||
              ' where id = any($1);';
    exception
      when others then
        return false;
    end;
    return true;
  end
$tuna$ language plpgsql;


--
-- this is trigger function for insert on system.tables
-- alters newly added table with new sequence
--
create or replace function tuna_new_table() returns trigger as $tuna$
  begin
    if (tg_op = 'DELETE') then
      raise notice 'nesmijes brisat redove iz ove tablice!';
      return null; 
    end if;

    raise notice 'creating sequence for table %.', new.name;

    execute 'alter table ' || new.name || 
            ' alter column id set default 1;';

    begin
      execute 'drop sequence tuna_seq_' || new.name || ';';
    exception when others then null;
    end;

    begin
    execute 'create sequence tuna_seq_' || new.name ||
            ' increment 100 minvalue ' || new.mod ||
            ' start with ' || new.mod || ';';
    exception when others then null;
    end;

    raise notice 'emptying table %.', new.name;
    execute 'delete from ' || new.name || ';'; 

    execute 'alter table ' || new.name || 
            ' alter column id set default nextval(' ||
            $$ 'tuna_seq_$$ || new.name || $$ '::regclass) $$;

    return new;
  end;
$tuna$ language plpgsql;

--
-- this function checks for potentialy new added tables and
-- update theirs id sequence
--
create or replace function tuna_refresh() returns boolean as $tuna$
  declare
  begin
    perform tuna_create_prep(tb) from tuna_get_tables() as tb;
    insert into system.tables (name)
      (select tb from tuna_get_tables() as tb
        where not tb in (select name from system.tables));
    return true;
  end;
$tuna$ language plpgsql;

--
-- trigger for insert ont system.tables
--
create trigger tuna_tg_new_table
  before insert or delete or update
  on system.tables
  for each row execute procedure tuna_new_table();
