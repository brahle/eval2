#!/usr/bin/env python3.2
# Copyright 2011 Bruno Rahle
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

from fields import IntegerField, StringField, ListField
from haskiutils import HaskiException

import re, sys

class CommitMessageException(HaskiException):
    pass

class CommitMessage(object):
    """This class is used to parse the commit message.
    """
    # Each element of ALLOWED_FIELDS has four fields:
    #    * name
    #    * type
    #    * required flag
    #    * print field name flag
    # A 'print field field' is field whose name is written in the first line of
    # it's description.
    ALLOWED_FIELDS = [
        ('title',               StringField,    True,   False),
        ('summary',             StringField,    True,   True),
        ('tags',                ListField,      False,  True),
        ('reviewers',           ListField,      True,   True),
        ('groups',              ListField,      False,  True),
        ('ccs',                 ListField,      False,  True),
        ('blamed revisions',    ListField,      False,  True),
        ('tickets',             ListField,      False,  True),
        ('reviewboard id',      IntegerField,   False,  True),
        ('skip reviewboard',    StringField,    False,  True),
    ]
    FIELD_GROUP_NAME = 'field'
    VALUE_GROUP_NAME = 'value'
    FIELD_REGEX = ('^(?P<' + FIELD_GROUP_NAME + '>[\w\s]+):(?P<' +
                   VALUE_GROUP_NAME + '>.*)')
    FIELD_REGEX_C = re.compile(FIELD_REGEX)

    def __init__(self, message, allowed_fields=None):
        if allowed_fields is None:
            self.allowed_fields = self.ALLOWED_FIELDS
        else:
            self.allowed_fields = allowed_fields
        self.message = message
        self.parse()

    def parse(self):
        """This function is used to parse the components from a message.
        First element of allowed_fields is the default field in which all data
        is saved until another field is encountered.

        Returns: a dictionary of fields
        """
        message = self.message
        allowed_fields = self.allowed_fields
        message = message.split('\n')
        self.fields = dict()

        allowed_regex = '^\W*('
        for field in allowed_fields:
            allowed_regex = allowed_regex + field[0] + '|'
        allowed_regex = allowed_regex.strip('|') + ')$'
        allowed_regex_c = re.compile(allowed_regex, re.IGNORECASE)

        current_field = allowed_fields[0][0]
        for line in message:
            line = line.rstrip()
            match = self.FIELD_REGEX_C.match(line)
            if line.lstrip().startswith('#'): continue
            if match is not None:
                field = match.group(self.FIELD_GROUP_NAME)
                field_match = allowed_regex_c.match(field)
                if field_match is not None:
                    current_field = match.group(self.FIELD_GROUP_NAME).lower().strip()
                    line = match.group(self.VALUE_GROUP_NAME).strip()
            if current_field in self.fields:
                 new_value = self.fields[current_field] + '\n' + line.rstrip()
            else:
                 new_value = line.rstrip()
            self.fields[current_field] = new_value

        for field in allowed_fields:
            if field[0] in self.fields:
                self.fields[field[0]] = field[1](self.fields[field[0]])
                if field[2] and not self.fields[field[0]].nonempty():
                    raise CommitMessageException('[ABORT] Field ' +
                                                 field[0].upper() +
                                                 ' is required!')
            elif field[2]:
                raise CommitMessageException('[ABORT] Field ' +
                                             field[0].upper() + 'is required!')
        return self.fields

    def get_raw_message(self):
        """Returns the message as a string. The fields are ordered the in same
        ordered they are listed in self.allowed_fields.
        """
        message = ''
        for allowed_field in self.allowed_fields:
            field = self.get_field(allowed_field[0])
            if field is not None:
                message = message
                if allowed_field[3]:
                    message = message + '\n{0}: '.format(allowed_field[0])
                message = message + '{0}\n'.format(field.stringify())
        return message

    def get_field(self, field):
        """Returns the requested field or `None` if it doesn't exists/is empty.
        """
        field = field.lower()
        if field not in self.fields:
            return None
        if not self.fields[field].nonempty():
            return None
        return self.fields[field]

    def set_field(self, field, value):
        field = field.lower()
        for allowed_field in self.allowed_fields:
            if allowed_field[0] == field:
                self.fields[field] = allowed_field[1](value)
                return
        raise CommitMessageException("Field " + field + " isn't allowed!")

def main():
    pass

if __name__ == '__main__':
    main()

