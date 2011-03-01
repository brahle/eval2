--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- Name: deval; Type: DATABASE; Schema: -; Owner: deval
--

CREATE DATABASE deval WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';


ALTER DATABASE deval OWNER TO deval;

\connect deval

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- Name: system; Type: SCHEMA; Schema: -; Owner: deval
--

CREATE SCHEMA system;


ALTER SCHEMA system OWNER TO deval;

--
-- Name: plpgsql; Type: PROCEDURAL LANGUAGE; Schema: -; Owner: deval
--

CREATE PROCEDURAL LANGUAGE plpgsql;


ALTER PROCEDURAL LANGUAGE plpgsql OWNER TO deval;

SET search_path = public, pg_catalog;

--
-- Name: tuna_create_prep(name); Type: FUNCTION; Schema: public; Owner: deval
--

CREATE FUNCTION tuna_create_prep(tb name) RETURNS boolean
    LANGUAGE plpgsql
    AS $_$
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
$_$;


ALTER FUNCTION public.tuna_create_prep(tb name) OWNER TO deval;

--
-- Name: tuna_get_tables(); Type: FUNCTION; Schema: public; Owner: deval
--

CREATE FUNCTION tuna_get_tables() RETURNS SETOF name
    LANGUAGE plpgsql
    AS $$
  begin
    return query select c.relname
      from pg_catalog.pg_class c
      left join pg_catalog.pg_namespace n on n.oid = c.relnamespace
      where c.relkind in ('r','') and n.nspname not in ('pg_catalog', 'pg_toast')
        and pg_catalog.pg_table_is_visible(c.oid);
  end;
$$;


ALTER FUNCTION public.tuna_get_tables() OWNER TO deval;

--
-- Name: tuna_new_table(); Type: FUNCTION; Schema: public; Owner: deval
--

CREATE FUNCTION tuna_new_table() RETURNS trigger
    LANGUAGE plpgsql
    AS $_$
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
$_$;


ALTER FUNCTION public.tuna_new_table() OWNER TO deval;

--
-- Name: tuna_refresh(); Type: FUNCTION; Schema: public; Owner: deval
--

CREATE FUNCTION tuna_refresh() RETURNS boolean
    LANGUAGE plpgsql
    AS $$
  declare
  begin
    perform tuna_create_prep(tb) from tuna_get_tables() as tb;
    insert into system.tables (name)
      (select tb from tuna_get_tables() as tb
        where not tb in (select name from system.tables));
    return true;
  end;
$$;


ALTER FUNCTION public.tuna_refresh() OWNER TO deval;

--
-- Name: array_accum(anyelement); Type: AGGREGATE; Schema: public; Owner: deval
--

CREATE AGGREGATE array_accum(anyelement) (
    SFUNC = array_append,
    STYPE = anyarray,
    INITCOND = '{}'
);


ALTER AGGREGATE public.array_accum(anyelement) OWNER TO deval;

--
-- Name: tuna_seq_solutions; Type: SEQUENCE; Schema: public; Owner: deval
--

CREATE SEQUENCE tuna_seq_solutions
    START WITH 1
    INCREMENT BY 100
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.tuna_seq_solutions OWNER TO deval;

--
-- Name: tuna_seq_solutions; Type: SEQUENCE SET; Schema: public; Owner: deval
--

SELECT pg_catalog.setval('tuna_seq_solutions', 1, false);


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: solutions; Type: TABLE; Schema: public; Owner: deval; Tablespace: 
--

CREATE TABLE solutions (
    id integer DEFAULT nextval('tuna_seq_solutions'::regclass) NOT NULL,
    task_id integer,
    user_id integer,
    programing_language_id integer,
    source text,
    time_submited integer
);


ALTER TABLE public.solutions OWNER TO deval;

--
-- Name: tuna_seq_tasks; Type: SEQUENCE; Schema: public; Owner: deval
--

CREATE SEQUENCE tuna_seq_tasks
    START WITH 0
    INCREMENT BY 100
    NO MAXVALUE
    MINVALUE 0
    CACHE 1;


ALTER TABLE public.tuna_seq_tasks OWNER TO deval;

--
-- Name: tuna_seq_tasks; Type: SEQUENCE SET; Schema: public; Owner: deval
--

SELECT pg_catalog.setval('tuna_seq_tasks', 0, false);


--
-- Name: tasks; Type: TABLE; Schema: public; Owner: deval; Tablespace: 
--

CREATE TABLE tasks (
    id integer DEFAULT nextval('tuna_seq_tasks'::regclass) NOT NULL,
    title character varying(128),
    text text
);


ALTER TABLE public.tasks OWNER TO deval;

--
-- Name: tuna_seq_users_suf; Type: SEQUENCE; Schema: public; Owner: deval
--

CREATE SEQUENCE tuna_seq_users_suf
    START WITH 2
    INCREMENT BY 100
    NO MAXVALUE
    MINVALUE 2
    CACHE 1;


ALTER TABLE public.tuna_seq_users_suf OWNER TO deval;

--
-- Name: tuna_seq_users_suf; Type: SEQUENCE SET; Schema: public; Owner: deval
--

SELECT pg_catalog.setval('tuna_seq_users_suf', 2, false);


--
-- Name: users_suf; Type: TABLE; Schema: public; Owner: deval; Tablespace: 
--

CREATE TABLE users_suf (
    id integer DEFAULT nextval('tuna_seq_users_suf'::regclass) NOT NULL,
    ime character varying(128),
    prezime character varying(128)
);


ALTER TABLE public.users_suf OWNER TO deval;

SET search_path = system, pg_catalog;

--
-- Name: tuna_mod; Type: SEQUENCE; Schema: system; Owner: deval
--

CREATE SEQUENCE tuna_mod
    START WITH 0
    INCREMENT BY 1
    MAXVALUE 99
    MINVALUE 0
    CACHE 1
    CYCLE;


ALTER TABLE system.tuna_mod OWNER TO deval;

--
-- Name: tuna_mod; Type: SEQUENCE SET; Schema: system; Owner: deval
--

SELECT pg_catalog.setval('tuna_mod', 2, true);


--
-- Name: tables; Type: TABLE; Schema: system; Owner: deval; Tablespace: 
--

CREATE TABLE tables (
    name name NOT NULL,
    mod integer DEFAULT nextval('tuna_mod'::regclass) NOT NULL
);


ALTER TABLE system.tables OWNER TO deval;

SET search_path = public, pg_catalog;

--
-- Data for Name: solutions; Type: TABLE DATA; Schema: public; Owner: deval
--

COPY solutions (id, task_id, user_id, programing_language_id, source, time_submited) FROM stdin;
\.


--
-- Data for Name: tasks; Type: TABLE DATA; Schema: public; Owner: deval
--

COPY tasks (id, title, text) FROM stdin;
\.


--
-- Data for Name: users_suf; Type: TABLE DATA; Schema: public; Owner: deval
--

COPY users_suf (id, ime, prezime) FROM stdin;
\.


SET search_path = system, pg_catalog;

--
-- Data for Name: tables; Type: TABLE DATA; Schema: system; Owner: deval
--

COPY tables (name, mod) FROM stdin;
tasks	0
solutions	1
users_suf	2
\.


SET search_path = public, pg_catalog;

--
-- Name: solutions_pkey; Type: CONSTRAINT; Schema: public; Owner: deval; Tablespace: 
--

ALTER TABLE ONLY solutions
    ADD CONSTRAINT solutions_pkey PRIMARY KEY (id);


--
-- Name: tasks_pkey; Type: CONSTRAINT; Schema: public; Owner: deval; Tablespace: 
--

ALTER TABLE ONLY tasks
    ADD CONSTRAINT tasks_pkey PRIMARY KEY (id);


--
-- Name: users_suf_pkey; Type: CONSTRAINT; Schema: public; Owner: deval; Tablespace: 
--

ALTER TABLE ONLY users_suf
    ADD CONSTRAINT users_suf_pkey PRIMARY KEY (id);


SET search_path = system, pg_catalog;

--
-- Name: tuna_tg_new_table; Type: TRIGGER; Schema: system; Owner: deval
--

CREATE TRIGGER tuna_tg_new_table
    BEFORE INSERT OR DELETE OR UPDATE ON tables
    FOR EACH ROW
    EXECUTE PROCEDURE public.tuna_new_table();


SET search_path = public, pg_catalog;

--
-- Name: solutions_task_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: deval
--

ALTER TABLE ONLY solutions
    ADD CONSTRAINT solutions_task_id_fkey FOREIGN KEY (task_id) REFERENCES tasks(id);


--
-- Name: solutions_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: deval
--

ALTER TABLE ONLY solutions
    ADD CONSTRAINT solutions_user_id_fkey FOREIGN KEY (user_id) REFERENCES users_suf(id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

