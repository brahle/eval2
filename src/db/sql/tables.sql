\c deval

CREATE TABLE solutions (
    id bigint NOT NULL,
    task_id bigint NOT NULL,
    user_id bigint NOT NULL,
    programming_language_id bigint NOT NULL,
    source text NOT NULL,
    time_submited bigint NOT NULL
);

ALTER TABLE public.solutions OWNER TO deval;


CREATE TABLE tasks (
    id bigint NOT NULL,
    title character varying(128),
    text text,
    user_id bigint[]
);

ALTER TABLE public.tasks OWNER TO deval;
