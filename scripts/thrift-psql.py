#!/usr/bin/env python
# 
# Copyright 2011 Matija Osrecki
# 
# This file is part of Evaluator.
# 
# Evaluator is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.
# 
# Evaluator is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
# 
# You should have received a copy of the GNU Affero General Public
# License along with Evaluator. If not, see
# <http://www.gnu.org/licenses/>.

######################################################################
#
# Script reads a thrift file, finds all structures and makes tables out
# of them. Apart from that, it creates C++ templated functions that
# convert rows from pqxx library to these structures. The script also
# takes a thrift template file by which it generates thrift db services
# for models.
#
# Usage:
# python thrift-psql.py input.thrift tables.sql convert.h template.thrift \
#        model_services.thrift
# 
# Structures must have the following format (with respect to newlines):
# 
# struct %class_name% { // %table_name%
#  %ignored_number% : %type% %att_name%, // additional information
#  ..
#  ..
# }
# 
# Whitespaces and tabs are ignored. Thrift types accepted are : i16,
#  i32, i64, bool, boolean, double, byte and string for now, enums are
#  not yet supported (** will be supported **)
# 
# If the thrift attribute type is string, then specific type must be
#  supplied as the first argument after comment start // (arguments are
#  divided with |). Types are: varchar[%len%], char[%len%] and text
# 
# One other thing is supported in the comment, and it represents what
# should be appended to the attribute (constraints).
# For example: 'UNIQUE REFERENCES %some_table%(%some_id%)'. 
# Also, it is good pratice to write tablenames with double quotes.
#   -> references "user"(id)
# 
# In conclusion, if the type is string, the first must be it's type, and
# the other one is optional. If it's not a string, the argument after
# comment is optional and it's appended to the string with basic name
# and type of the table attribute.
# 

import sys
import re

#
# **************** TYPES ****************
#

# contains ttype, tname, pqtype, pqname and constaints
class Attribute:
    
    def __init__(self):
        self.ttype = self.tname = ""
        self.pqtype = self.pqname = self.constraints = ""

    def create_db_line(self):
        return " ".join([self.pqname, self.pqtype, self.constraints])

# containts class name, table name and attributes
class Model:
    
    def __init__(self, class_name, table_name, attributes):
        self.cname = class_name
        self.tname = table_name
        self.attributes = attributes


# 
# **************** READING THRIFT FILE, GENERATING SQL FILE ****************
#


# creates a thrift - SQL type conversion map
def get_types():
    types_map = {
        'i16' : 'smallint',
        'i32' : 'integer',
        'i64' : 'bigint',
        'boolean' : 'boolean',
        'bool' : 'boolean',
        'double' : 'double',
        'byte' : '"char"',
        'string' : ('text', 'char[\d+]', 'varchar[\d+]')
        }
    
    return types_map


# adds _ in front of big letters (except for the first) and lower cases
def convert_name(name):
    new_name = re.sub(r'\B([A-Z])', r'_\1', name)
    new_name = new_name.lower()
    return new_name


# sees if the string type in comment matches any of allowed string types
def match_string_type(given_type, types_map):
    for stype in types_map['string']:
        if re.match(stype, given_type):
            return True
    
    return False



# goes through all attributes and creates their line in SQL code
# returns a list of strings (attributes)
def read_attributes(raw, types_map):
    attributes = []
    
    for ratt in raw:
        att = Attribute()
        
        att_all = re.search(r':\s*(\w+)\s*(\w+)\s*', ratt)

        att.ttype = att_all.group(1);
        att.tname = att_all.group(2);

        comment = re.search(r'//\s*([A-Za-z_0-9() ]+)$', ratt)

        if comment != None:
            comment = comment.group(1).rstrip().split('|')

        if att.ttype == 'string':
            if comment == None or len(comment) < 1 and \
            match_string_type(comment[0], types_map) == False:

                print "\nError: You need to define string type as the first" + \
                    "element in the comment in the same line.\n" + \
                    "Types are varchar[%number%], char[%number%] or text."
                print "  Line: '" + ratt + "'"
            
            att.pqtype = comment[0]
            comment = comment[1:]
        else:
            att.pqtype = types_map[att.ttype]
            
        if comment != None and len(comment) > 0:
            att.constraints = comment[0]

        att.pqname = convert_name(att.tname)

        attributes.append(att)

    return attributes


# reads all the structures from the file data and returns them
def read_models(data):
    types_map = get_types()
    
    raw_structs = re.findall('struct *(\w+) *{ *// *(\w+) *\n([^}]*)}', 
                             data, re.DOTALL)

    models = []
    
    for struct in raw_structs:
        class_name = struct[0]
        table_name = struct[1]

        raw_attributes = [p.strip() for p in struct[2].rstrip().split("\n")]
        attributes = read_attributes(raw_attributes, types_map)
    
        models.append(Model(class_name, table_name, attributes))

    return models


def table_to_string(model):
    table = "CREATE TABLE \"%s\" (\n" % model.tname
    table += ",\n".join([("  " + a.create_db_line()) for a in model.attributes])
    table += "\n);\n"
    return table


# 
# **************** GENERATING PSQL <==> C++ CONVERSION ****************
#

# creats a thrift - c++ type conversion map
def create_tcpp_typemap():
    type_map = {
        'i16' : 'short int',
        'i32' : 'int',
        'i64' : 'bigint',
        'boolean' : 'bool',
        'bool' : 'bool',
        'double' : 'double',
        'byte' : 'char',
        'string' : 'std::string'
        }

    return type_map

# generates conversion functions
def generate_conv(out, models):
    tab = 2 * " "

    fun_name = 'convertFrom'
    aname = 't'
    arg = 'const result::tuple &%s' % aname
    throw = 'throw(int) '
    typemap = create_tcpp_typemap()

    # prepare includes, namespace

    includes = ['models_types.h', 'string', 'vector', 'sstream', 'pqxx/result']
    usings = ['std::pair', 'std::string', 'std::vector', \
      'std::stringstream', 'pqxx::result', 'boost::shared_ptr', \
      'boost::static_pointer_cast', 'namespace eval::model']
    namespace = ['eval', 'tuna']

    out.write("/*\n * this is generated by scripts/thrift-psql.py\n");
    out.write(" * don't change unless you know what are you doing.\n");
    out.write(" * changes will be rewriten on thrift-pqsl call.\n\n");
    out.write(" */\n")

    out.write("".join(['#include <'+i+'>\n' for i in includes]))
    out.write("\n")

    out.write("".join(['using ' + sth + ';\n' for sth in usings]))
    out.write("\n")

    out.write("".join(["namespace %s { " % ns for ns in namespace]))
    out.write("\n" * 2)

    # templates
    
    out.write("template <class T>\n")
    out.write("shared_ptr<T> %s(%s) %s{\n" % (fun_name, arg, throw))
    out.write(tab + "throw 1;\n}")
    out.write("\n"*2)

    for model in models:
        cname = model.cname
        oname = convert_name(model.cname)

        out.write("template<>\n")
        out.write("shared_ptr<%s> %s(%s) %s{\n" % \
                      (cname, fun_name, arg, throw))
        
        out.write(tab + "shared_ptr<%s> %s(new %s());\n\n" % \
                      (cname, oname, cname))

        counter = 0
        for a in model.attributes:
            out.write(tab + "%s->%s = %s[%d].as<%s>();\n" % \
                          (oname, a.tname, aname, counter, typemap[a.ttype]))

            counter += 1

        out.write("\n")
        out.write(tab + "return %s;\n" % oname)

        out.write("}")
        out.write("\n"*2)

    # added by anton

    # convertGeneric:
    out.write("shared_ptr<void> convertGeneric\n")
    out.write(tab + "(string tb, %s" % (arg))
    out.write(") throw(int) {\n\n")

    for model in models:
        tname = model.tname
        cname = model.cname

        out.write(tab + "if (tb==\"%s\") {\n" % (tname)) 
        out.write(tab*2 + "return static_pointer_cast<")
        out.write("%s, void>(\n" % (cname)) 
        out.write(tab*3 + "convertFrom<%s>(t));\n" % cname)
        out.write(tab + "}\n") 

    out.write("\n" + tab + "throw(2);\n")
    out.write("}\n\n")
    

    # getTableName:
    for model in models:
        cname = model.cname
        oname = convert_name(model.cname)
        out.write("string getTableName")
        out.write("(const %s &%s) {\n" % (cname,oname))
        out.write(tab + "return \"%s\";\n" % (model.tname));
        out.write("}\n\n")

    # toQuery:
    for model in models:
        cname = model.cname
        oname = convert_name(model.cname)
        out.write("vector< pair<string,string> > serialize")

        out.write("(const %s &%s) {\n" % (cname,oname))
        out.write(tab + "vector< pair<string,string> > sol;\n\n")

        for a in model.attributes:
            out.write(tab + "if (%s.__isset.%s) {\n" % (oname, a.tname))
            out.write(tab*2 + "stringstream out;\n")
            out.write(tab*2 + "out << %s.%s;\n" % (oname, a.tname))
            out.write(tab*2 + "sol.push_back( make_pair(\"")
            out.write("%s\", out.str() ) );\n" % (a.pqname))
            out.write(tab + "}\n");

        out.write("\n" + tab + "return sol;\n")
        out.write("}\n\n")

    # end anton

    out.write("} " * len(namespace) + " // namespace\n")
    

# generates thrift service methods for tuna (dbapi)
def generate_thrift(inp, out, models):
    exp = 'TunaExp e'

    mcode = []

    for m in models:
	lines = []

        cname = m.cname
        mcname = 'models.%s' % cname
        cnames = "".join([w[:1].upper() + w[1:] for w in m.tname.split("_")])
        oname = convert_name(cname)
        
        lines += [
            '  // *** %s ***' % cname,
            '',
            '%s get%s(1: i32 id),' % (mcname, cname),
            'list<%s> get%s(1: list<i32> ids),' % (mcname, cnames),
            'list<%s> get%sFrom(' % (mcname, cnames),
            '  1: string qname,',
            '  2: list<string> data',
            ') throws(1: %s),' % (exp),
            '',
            'bool update%s(1: %s %s) throws(1: %s),' % (cname, mcname, oname, exp),
            'i32 insert%s(1: %s %s) throws (1: %s)' % (cname, mcname, oname, exp)
            ]

	mcode.append("\n  	".join(lines))


    for line in inp.readlines():
        if line.find(r'%MODEL_FUNCTIONS%') == -1:
            out.write(line)
        else:
            out.write("" + ",\n\n".join(mcode))




#
# **************** MAIN ****************
#

def main():
    input = open(sys.argv[1], 'r')
    table_out = open(sys.argv[2], 'w')
    convert_out = open(sys.argv[3], 'w')
    thrift_temp = open(sys.argv[4], 'r')
    thrift_out = open(sys.argv[5], 'w')

    data_list = input.readlines()
    data = ''.join(data_list)
    
    input.close()

    models = read_models(data)

    print 'Found 4 structures:'

    for model in models:
        print "Class name '%s', table name '%s':" % (model.cname, model.tname)
        stable = "\n" + table_to_string(model)
#        print stable
        table_out.write(stable)
        

    table_out.close()

    generate_conv(convert_out, models)
    generate_thrift(thrift_temp, thrift_out, models)

    convert_out.close()
    thrift_temp.close()
    thrift_out.close()


if __name__ == '__main__':    
    main()
