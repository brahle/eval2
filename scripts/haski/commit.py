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
from haskiutils import HaskiException
from invoker import Invoker
from lint.cpplinter import CppLinter
from repositoryinfo import RepositoryInfo
from reviewboardinvoker import ReviewBoardInvoker

import os
import re
import string
import subprocess
import sys
import tempfile

class CommitException(HaskiException):
    pass

class Commit(object):
    LINTERS = [
        CppLinter,
    ]

    AMEND_STR = 'git commit --amend --file={0} --reset-author --no-verify'

    VERIFY_CMD = 'git log {0}'

    def __init__(self, commit_hash, author, time, message, files,
                 ignore_message=False):
        self.commit_hash = commit_hash
        self.author = author
        self.time = time
        self.ignore_message = ignore_message
        self.message = CommitMessage(message, ignore_message)
        self.files = files

        self._ri = RepositoryInfo()
        self._ri.prepare()
        self._lint_log = tempfile.NamedTemporaryFile('w+', delete=False)
        self._linters = []
        for linter in self.LINTERS:
            self._linters.append(
                linter(self.commit_hash, self._ri.get_path()))
        self.verify()

    def verify(self):
        """Raises exception if something isn't as expected."""
        invoker = Invoker(self.VERIFY_CMD.format(self.commit_hash))
        invoker.invoke()
        if invoker.status != 0:
            raise CommitException('Revision ' + self.commit_hash +
                                  ' does not exist!')
        return True

    def lint(self, print_results=True):
        """Lints all files in this commit.
        """
        for linter in self._linters:
            linter.append_results(self._lint_log)
        if print_results:
            self._print_lint()

    def get_lint_log_name(self):
        """Returns the lint log name.
        """
        return self._lint_log.name

    def _print_lint(self):
        """Prints the contents of the lint log file.
        """
        self._lint_log.seek(0)
        for line in self._lint_log:
            print(line, end='')

    def review(self, tests_filename=None):
        """Sends this commit to the reviewboard.
        """
        rbi = ReviewBoardInvoker(self, revision=self.commit_hash,
                                 test_filename=self.get_lint_log_name())
        rbi.invoke()
        rb_id = rbi.get_reviewboard_id()
        return rb_id

    def amend(self):
        """This amends the current commit.
        WARNING: current commit should be the last one.
        """
        f = tempfile.NamedTemporaryFile()
        f.write(self.message.get_raw_message().encode())
        f.seek(0, os.SEEK_SET)
        cmd = self.AMEND_STR.format(f.name)
        invoker = Invoker(cmd)
        invoker.invoke()
        if invoker.status != 0:
            message = '{0}\n{1}\n{0}\n{2}\n{0}\n[FAIL] Amending failed!'.format(
                '-'*80, invoker.stdout, invoker.stderr)
            raise CommitException(message)

    def get_field(self, field):
        if field not in self.message.fields:
            return None
        if not self.message.fields[field].nonempty():
            return None
        return self.message.fields[field].get_value()


def main():
    pass

if __name__ == '__main__':
    main()

