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
from repositoryinfo import RepositoryInfo
from reviewboardinvoker import ReviewBoardInvoker

import os, string, sys, re, tempfile

class CommitException(HaskiException):
    pass

class Commit(object):
    CPP_LINT_PATH = '/scripts/bin/cpplint'
    CPP_FILES_REGEX = r'.*\.(cpp|cc|h|hpp)$'
    CPP_FILES_REGEX_C = re.compile(CPP_FILES_REGEX)

    AMEND_STR = 'git commit --amend --file={0} --reset-author --no-verify'

    VERIFY_CMD = 'git log {0}'

    def __init__(self, commit_hash, author, time, message, files):
        self.commit_hash = commit_hash
        self.author = author
        self.time = time
        self.message = CommitMessage(message)
        self.files = files
        self._ri = RepositoryInfo()
        self._ri.prepare()
        self.verify()

    def verify(self):
        """Raises exception if something isn't as expected."""
        invoker = Invoker(self.VERIFY_CMD.format(self.commit_hash))
        invoker.invoke()
        if invoker.status != 0:
            raise CommitException('Revision ' + self.commit_hash +
                                  ' does not exist!')
        return True

    def get_cpp_files(self):
        """Returns all cpp files from this commit."""
        cpp_files = []
        for f in self.files:
            if self.CPP_FILES_REGEX_C.match(f):
                cpp_files.append(f)
        return cpp_files

    def lint(self, save_to=None):
        """Lints all files in this commit.
        BUG: Lints the file from the cache, not the one from the index.
        TODO: Create lint class
        """
        if save_to is not None:
            if isinstance(save_to, str):
                f = open(save_to, 'a+')
                f.close()
        cpp_lint_cmd = self._ri.get_path() + self.CPP_LINT_PATH
        cpp_files = self.get_cpp_files()
        if len(cpp_files) == 0:
            print('[SUCCESS] Nothing to lint!')
            return
        cmd = [cpp_lint_cmd]
        cmd.extend(cpp_files)
        print('Linting', len(cpp_files), ' cpp files...')
        invoker = Invoker(cmd)
        invoker.invoke()
        print(invoker.stdout)
        print('[SUCCESS] Lint complete!\n')
        if save_to is not None:
            if isinstance(save_to, file):
                save_to.write(invoker.stdout)
            else:
                f = open(save_to, "w")
                f.write(invoker.stdout)

    def review(self, tests_filename=None):
        """Sends this commit to the reviewboard."""
        rbi = ReviewBoardInvoker(self.message, tests_filename=tests_filename,
                                 revision=self.commit_hash)
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

