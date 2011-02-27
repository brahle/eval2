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
# of them. Usage:
#
# python thrift-to-psql.py input.thrift output.sql
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
#  not yet supported
# 
# If the thrift attribute type is string, then specific type must be
#  supplied as the first argument after comment start // (arguments are
#  divided with |). Types are: varchar[%len%], char[%len%] and text
# 
# One other thing is supported in the comment, and it represents what
# should be appended to the attribute type. 
# For example: 'UNIQUE REFERENCES %some_table%(%some_id%)'. 
# 
# In conclusion, if the type is string, the first must be it's type, and
# the other one is optional. If it's not a string, the argument after
# comment is optional and it's appended to the string with basic name
# and type of the table attribute.
# 

import sys
import re

# creates a type conversion map
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
    
    return types_map;


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
def create_attributes(raw, types_map):
    attributes = []
    
    for ratt in raw:
        att_all = re.search(r':\s*(\w+)\s*(\w+)\s*', ratt)

        thr_type = att_all.group(1)
        att_name = att_all.group(2)

        comment = re.search(r'//\s*([A-Za-z_0-9() ]+)$', ratt)

        if comment != None:
            comment = comment.group(1).rstrip().split('|')

        if thr_type == 'string':
            if comment == None or len(comment) < 1 and \
            match_string_type(comment[0], types_map) == False:

                print "\nError: You need to define string type as the first" + \
                    "element in the comment in the same line.\n" + \
                    "Types are varchar[%number%], char[%number%] or text."
                print "  Line: '" + ratt + "'"
            
            new_type = comment[0]
            comment = comment[1:]
        else:
            new_type = types_map[thr_type]
            
        if comment != None and len(comment) > 0:
            new_type += " " + comment[0]

        att = convert_name(att_name) + ' ' + new_type
        attributes.append(att)

    return attributes


# goes through all the sections and parses struct title
def main():
    input = open(sys.argv[1], 'r')
    output = open(sys.argv[2], 'w')

    
    data_list = input.readlines()
    data = ''.join(data_list);
    
    types_map = get_types()
    
    raw_structs = re.findall('struct *(\w+) *{ *// *(\w+) *\n([^}]*)}', 
                             data, re.DOTALL)

    print 'Found %d structures:' % len(raw_structs)
    
    for struct in raw_structs:
        class_name = struct[0]
        table_name = struct[1]

        print "\nClass name '%s', table name '%s':\n" % (class_name, table_name)

        raw_attributes = [p.strip() for p in struct[2].rstrip().split("\n")]
        attributes = create_attributes(raw_attributes, types_map);

        table = "CREATE TABLE %s (\n" % table_name
        table += ",\n".join(["  " + a for a in attributes]);
        table += "\n);\n"

        print table

        output.write(table + "\n")


if __name__ == '__main__':    
    main()
