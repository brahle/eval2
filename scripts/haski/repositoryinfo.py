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

from haskiutils import HaskiException

import os, sys, re

class RepositoryInfoException(HaskiException):
    pass

class RepositoryInfo(object):
    PATH_REGEX = '^(?P<dir>.+)\/eval2\/.*'
    PATH_PATTERN = re.compile(PATH_REGEX)

    def __init__(self):
        self._prepared = False
        self.prepare()

    def prepare(self):
        path_matches = self.PATH_PATTERN.match(sys.path[0])
        if path_matches is None:
            raise RepositoryInfoException('Unknown directory!')
        self._path = path_matches.group('dir') + '/eval2'
        self._prepared = True

    def get_path(self):
        if not self._prepared:
            self.prepare()
        return self._path

def main():
    pass

if __name__ == '__main__':
    main()

