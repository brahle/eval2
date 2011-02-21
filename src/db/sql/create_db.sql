drop database "deval";

create database "deval" encoding 'UTF8';
create role "deval" login password 'deval';
grant all privileges on database "deval" to "deval";
alter database "deval" owner to "deval";

\c deval

create aggregate array_accum (anyelement) (
    sfunc = array_append,
    stype = anyarray,
    initcond = '{}'
);

create schema system;
alter schema system owner to "deval";

create sequence system.tuna_mod minvalue 0 maxvalue 99 cycle;
alter sequence system.tuna_mod owner to "deval";

create table system.tables (
    name name NOT NULL,
    mod integer DEFAULT nextval('system.tuna_mod'::regclass) NOT NULL
);

alter table system.tables owner to "deval";

create procedural language plpgsql;
