#!/usr/bin/env python

import re, sys

class CommitMessage(object):
    """This class is used to parse the commit message.
    TODO: write an example
    """
    ALLOWED_FIELDS = ['title', 'summary', 'tags', 'reviewers',
                      'groups', 'blamed revisions', 'tickets',
                      'reviewboardid', 'skip reviewboard']

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

        field_pattern_str = '^('
        for field in allowed_fields:
            field_pattern_str = field_pattern_str + field + '|'
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
                    current_field = match.group('field').lower()
                    line = match.group('value').strip()

            if current_field in self.fields:
                 new_value = self.fields[current_field] + '\n' + line.strip()
            else:
                 new_value = line.strip()
            self.fields[current_field] = new_value

        for field in self.fields:
            self.fields[field] = self.fields[field].strip()

        if 'reviewers' not in self.fields or len(self.fields['reviewers'])==0:
            print '[ABORTING] Please specify reviewers!'
            sys.exit(1)

        return self.fields


def main():
    message = """This is a sample message

              Summary: This is the summary of the sample message.
              This is a multi line one!
              Hoax field: just testing it.

              Tags: tag1, tag2

              Reviewers: john, paul, peter

              CCs: mark, lucas

              Blamed Revisions: r8, r12

              Tickets: 12341124, 2412341234151, 123412,
              """

    commit_message = CommitMessage(message)
    for field in commit_message.fields:
        print '%s: %s\n' % (field, commit_message.fields[field])

if __name__ == '__main__':
    main()
