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

from commitmessage import CommitMessage
from fields import Field
from invoker import Invoker

import os
import re
import sys
import tempfile

class ReviewBoardInvoker(object):
    """This class is used to invoke ReviewBoard tool post-review safely.
    TODO: example, more detailed description
    """
    POST_REVIEW_CMD_STR = 'post-review --parent={0}^'

    def __init__(self, commit, **kwargs):
        self.commit = commit
        self.revision = 'HEAD'
        self.tests_filename = None
        self.publish = True
        if 'revision' in kwargs:
            self.revision = kwargs['revision']
        if 'tests_filename' in kwargs:
            self.tests_filename = kwargs['tests_filename']
        if 'publish' in kwargs:
            self.publish = kwargs['publish']

    def write_to_file(self, f, text):
        if isinstance(f, str):
            f = open(f, 'w')
        f.write(text)
        f.close()

    def prepare(self):
        msg = self.commit.message.fields
        self.command = self.POST_REVIEW_CMD_STR.format(self.revision)

        def exists(dictionary, field):
            if field not in dictionary:
                return False
            elem = dictionary[field]
            if elem == None:
                return False
            if isinstance(elem, Field) and not elem.nonempty():
                return False
            return True

        if self.publish:
            self.command = self.command + ' -p'
        if exists(msg, 'title'):
            self.command = self.command + ' --summary=' + str(msg['title'])
        if exists(msg, 'summary'):
            file = tempfile.NamedTemporaryFile('w+', delete=False)
            self.write_to_file(file, msg['summary'].get_value())
            self.command = self.command + ' --description-file=' + file.name
        if exists(msg, 'reviewers'):
            self.command = (self.command + ' --target-people=' +
                            str(msg['reviewers']))
        if exists(msg, 'tickets'):
            self.command = (self.command + ' --bugs-closed=' +
                            str(msg['tickets']))
        if exists(msg, 'reviewboard id') and msg['reviewboard id'].get_value() != -1:
            self.command = self.command + ' -r' + str(msg['reviewboard id'])
        if self.tests_filename is not None:
            self.command = (self.command + ' --testing-done-file="' +
                            self.tests_filename + '"')

    def invoke(self):
        self.prepare()
        invoker = Invoker(self.command)
        invoker.invoke()
        print(invoker.stdout)
        print(invoker.stderr)

        if invoker.status != 0:
            print("[WARNING] post-review failed!!!")
            self.reviewboard_id = -1
            return

        # Get the ReviewBoard ID from the message
        pattern = re.compile('Review request #(?P<id>\d+) posted.')
        match = pattern.match(invoker.stdout)
        if match is None:
            print("[ABORT] post-review failed!!!")
            sys.exit(1)
        self.reviewboard_id = int(match.group('id'))

    def get_reviewboard_id(self):
        return self.reviewboard_id

def main():
    pass

if __name__ == '__main__':
    main()

