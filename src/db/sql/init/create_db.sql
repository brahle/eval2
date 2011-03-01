drop database "deval";
drop role "deval";

create database "deval" encoding 'UTF8';
create role "deval" login password 'deval';
grant all privileges on database "deval" to "deval";
alter database "deval" owner to "deval";

--
-- I am connectiong to "deval" database
--

\c deval

-- this will set current user to deval so i
-- don't need to write alter %object% owner to "deval'
-- after every statement;

set session authorization "deval";

create procedural language plpgsql;

create aggregate array_accum (anyelement) (
    sfunc = array_append,
    stype = anyarray,
    initcond = '{}'
);

create schema system;

create sequence system.tuna_mod minvalue 0 maxvalue 99 cycle;

create table system.tables (
    name name NOT NULL,
    mod integer DEFAULT nextval('system.tuna_mod'::regclass) NOT NULL
);



