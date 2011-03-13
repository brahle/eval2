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

from commit import Commit
from invoker import Invoker
from haskiutils import HaskiException
from repositoryinfo import RepositoryInfo

import re

class RepositoryException(HaskiException):
    pass

class Repository(object):
    GIT_LOG_STR = 'git log -r {0} --format="%H%n%an%n%at%n###%n%s%n%n%b%n###%n" -n1'
    HASH_REGEX = r'(?P<hash>[0-9a-f]+)'
    AUTHOR_REGEX = r'(?P<author>[\w]+)'
    TIME_REGEX = r'(?P<time>[\d]+)'
    MESSAGE_REGEX = r'###\n(?P<message>.+)\n###'
    LOG_REGEX = (HASH_REGEX + r'\n' + AUTHOR_REGEX + r'\n' + TIME_REGEX +
                 r'\n' + MESSAGE_REGEX)
    LOG_REGEX_C = re.compile(LOG_REGEX, re.DOTALL)

    GIT_FILES_STR = 'git diff --name-only {0} {0}^'

    def __init__(self):
        self.info = RepositoryInfo()
        self.info.prepare()

    def get_commit(self, commit_id):
        # Get the data from git log
        log_cmd = self.GIT_LOG_STR.format(commit_id)
        invoker = Invoker(log_cmd)
        invoker.invoke()
        if invoker.status != 0:
            raise RepositoryException('Something bad happened!\n' +
                                      'Command = ' + str(invoker.cmd))
        match = self.LOG_REGEX_C.match(invoker.stdout)
        if match is None:
            raise RepositoryException('Unknown log format!\n' +
                                      'Command = ' + str(invoker.cmd))
        commit_hash = match.group('hash')
        author = match.group('author')
        time = match.group('time')
        message = match.group('message')

        # Get the files from git diff
        files_cmd = self.GIT_FILES_STR.format(commit_id)
        invoker = Invoker(files_cmd)
        invoker.invoke()
        if invoker.status != 0:
            raise RepositoryException('Something bad happened!\n' +
                                      'Command = ' + str(invoker.cmd))
        files = [self.info.get_path() + '/' + x
                     for x in invoker.stdout.split('\n')]

        return Commit(commit_hash, author, time, message, files)

    def get_last_commit(self):
        return self.get_commit('HEAD')

    def get_path(self):
        return self.info.get_path()

def main():
    pass

if __name__ == '__main__':
    main()

