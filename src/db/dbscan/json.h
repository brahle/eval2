#ifndef EVAL_JSON_H_
#define EVAL_JSON_H_

#include "json_spirit/json_spirit_reader_template.h"
#include "json_spirit/json_spirit_writer_template.h"

json_spirit::Value file2json( const char* file_name );

#endif // EVAL_JSON_H_
