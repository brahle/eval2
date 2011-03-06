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
# Script reads a thrift file, finds all structures and implements 
# thrift skeleton from template.
#
# In template file you can add INSERT AFTER directive to insert a block of code
# after line in which given string is found. 
#
# Usage:
# python thrift-skeleton.py input.thrift skeleton.template input.skeleton.cpp \
#        out.skeleton.cpp 
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

def generate_skeleton(template, inp, out, models):
    tab = 2 * " "
    tmp_line = 0
    loop_back = 0
    cmodel = 0
    rules = []
    replaces = []
 
    #load INSERT AFTER commands
    while not re.search("###", template[tmp_line]):

        if re.search("INSERT AFTER ", template[tmp_line]):
            rules.append( template[tmp_line].split( \
              "INSERT AFTER ")[-1][:-1] )
            code = ""
            tmp_line += 1

            while not re.search("END", template[tmp_line]):
                code += template[tmp_line]
                tmp_line += 1

            replaces.append(code);
        else:
            tmp_line += 1

    tmp_line = 0
    poslije = 0
 
    for line in inp:
        if poslije:
            out.write(poslije)
            poslije = 0

        for i in range(0, len(rules)):
            if re.search(rules[i], line):
                poslije = replaces[i]

        if re.search("// Your implementation", line):
            while not re.search("###", template[tmp_line]):
                tmp_line = tmp_line + 1
            # ako sam dosao do 'models' onda zabiljezim to 
            if re.search("models", template[tmp_line]):
                loop_back = tmp_line
            tmp_line += 1
            code = ""
            # skupljam kod dok ne izadnjem iz ### scop-a 
            while not re.search("###", template[tmp_line]):
                code = code + template[tmp_line]
                tmp_line = tmp_line + 1
            # promjenim sve tagove 
            if loop_back > 0:
                mdl = models[cmodel]
                code = code.replace("%className%", mdl.cname)
                code = code.replace("%tableName%", mdl.tname)
                code= code.replace("%varName%",convert_name(mdl.cname))
            # print code to out

            out.write(code)

            if tmp_line + 1 == len(template):
                tmp_line = loop_back
                cmodel += 1
            else:
                tmp_line += 1
        else:
            out.write(line) 
      
#
# **************** MAIN ****************
#

def main():
    
# python thrift-psql.py input.thrift skeleton.template input.skeleton.cpp \
#        out.skeleton.cpp 
    if len(sys.argv) != 5:
        print "usage: python thrift-psql.py input.thrift " +  \
          "skeleton.template input.skeleton.cpp out.skeleton.cpp"
        sys.exit() 

    thrift = open(sys.argv[1], 'r')
    template = open(sys.argv[2], 'r')
    input_skeleton = open(sys.argv[3], 'r')
    out_skeleton = open(sys.argv[4], 'w')

    data = ''.join(thrift.readlines())
    thrift.close()

    models = read_models(data)

    print 'Found 4 structures:'

    for model in models:
        print "Class name '%s', table name '%s'." % (model.cname, model.tname)
        

    generate_skeleton( \
      template.readlines(), \
      input_skeleton.readlines(),\
      out_skeleton, \
      models)


if __name__ == '__main__':    
    main()
