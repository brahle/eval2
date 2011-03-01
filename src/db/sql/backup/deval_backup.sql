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

SET search_path = public, pg_catalog;

--
-- Name: array_accum(anyelement); Type: AGGREGATE; Schema: public; Owner: deval
--

CREATE AGGREGATE array_accum(anyelement) (
    SFUNC = array_append,
    STYPE = anyarray,
    INITCOND = '{}'
);


ALTER AGGREGATE public.array_accum(anyelement) OWNER TO deval;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: solutions; Type: TABLE; Schema: public; Owner: deval; Tablespace: 
--

CREATE TABLE solutions (
    id integer NOT NULL,
    task_id integer,
    user_id integer,
    programing_language_id integer,
    source text,
    time_submited integer
);


ALTER TABLE public.solutions OWNER TO deval;

--
-- Name: tasks; Type: TABLE; Schema: public; Owner: deval; Tablespace: 
--

CREATE TABLE tasks (
    id integer NOT NULL,
    title character varying(128),
    text text
);


ALTER TABLE public.tasks OWNER TO deval;

--
-- Name: users_suf; Type: TABLE; Schema: public; Owner: deval; Tablespace: 
--

CREATE TABLE users_suf (
    id integer NOT NULL,
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

SELECT pg_catalog.setval('tuna_mod', 0, false);


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

