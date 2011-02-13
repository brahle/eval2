#!/usr/bin/env python
#!/usr/bin/env python3.1
# Copyright 2011 Bruno Rahle
#
# TODO: paste copy statement
# TODO: documentation cleanup

from Fields import IntegerField, StringField, ListField

import re, sys

class CommitMessage(object):
    """This class is used to parse the commit message.
    TODO: write an example
    """
    ALLOWED_FIELDS = [
        ('title',               StringField,    True),
        ('summary',             StringField,    True),
        ('tags',                ListField,      False),
        ('reviewers',           ListField,      True),
        ('groups',              ListField,      False),
        ('ccs',                 ListField,      False),
        ('blamed revisions',    ListField,      False),
        ('tickets',             ListField,      False),
        ('reviewboardid',       IntegerField,   False),
        ('skip reviewboard',    StringField,    False),
    ]

    def __init__(self, message, allowed_fields=None):
        if allowed_fields is None:
            self.parse(message, self.ALLOWED_FIELDS)
        else:
            self.parse(message, allowed_fields)

    def parse(self, message, allowed_fields):
        """This function is used to parse the components from a message.
        Returns: a dictionary of fields
        TODO: example
        """

        message = message.split('\n')
        self.fields = dict()
        pattern = re.compile('^(?P<field>[\w\s]+):(?P<value>.*)')

        field_pattern_str = '^\W*('
        for field in allowed_fields:
            field_pattern_str = field_pattern_str + field[0] + '|'
        field_pattern_str = field_pattern_str.strip('|') + ')$'
        field_pattern = re.compile(field_pattern_str, re.IGNORECASE)

        current_field = 'title'

        for line in message:
            line = line.rstrip()
            match = pattern.match(line)
            if line.lstrip().startswith('#'): continue

            if match is not None:
                field = match.group('field')
                field_match = field_pattern.match(field)
                if field_match is not None:
                    current_field = match.group('field').lower().strip()
                    line = match.group('value').strip()

            if current_field in self.fields:
                 new_value = self.fields[current_field] + '\n' + line.rstrip()
            else:
                 new_value = line.rstrip()
            self.fields[current_field] = new_value

        for field in allowed_fields:
            if field[0] in self.fields:
                self.fields[field[0]] = field[1](self.fields[field[0]])
                if field[2]:
                    if not self.fields[field[0]].nonempty():
                        print('[ABORT] Field', field[0].upper(), 'is required!')
                        sys.exit(1)
            elif field[2]:
                print('[ABORT] Field', field[0].upper(), 'is required!')
                sys.exit(1)

        return self.fields


def main():
    message = """This is a sample message

              Summary: This is the summary of the sample message.
              This is a multi line one!

                Some empty lines...
                # and some this be comment

              Hoax field: just testing it.

              Tags: tag1, tag2

              Reviewers: john, paul, peter,

                somebodyelse

              CCs: mark, lucas

              Blamed Revisions: r8, r12

              Tickets: 12341124, 2412341234151, 123412,
              """

    commit_message = CommitMessage(message)
    for field in commit_message.fields:
        print('%s: %s' % (field, commit_message.fields[field]))

if __name__ == '__main__':
    main()

