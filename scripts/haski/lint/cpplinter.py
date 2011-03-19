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

from lint.baselinter import BaseLinter

import re
import subprocess

class CppLinter(BaseLinter):
    BASE_SUFFIX = '.cc'
    CPP_LINT_PATH = '/scripts/bin/cpplint'
    CPP_FILES_REGEX_STR = r'.*\.(cpp|cc|h|hpp)$'
    CPP_FILES_REGEX = re.compile(CPP_FILES_REGEX_STR)
    ADDITIONAL_OPTIONS = []

    def lint(self):
        if len(self.files) == 0:
            self._process = subprocess.Popen(self.NOP)
            return
        cmd = ['{}/{}'.format(self.repository_location, self.CPP_LINT_PATH)]
        cmd.extend(self.ADDITIONAL_OPTIONS)
        cmd.extend(self.files)
        self._process = subprocess.Popen(cmd, stdout=self.results,
                                         stderr=subprocess.STDOUT)

    def _filter_files(self, files):
        cpp_files = []
        for f in files:
            if self.CPP_FILES_REGEX.match(f):
                cpp_files.append(f)
        return cpp_files


def main():
    pass

if __name__ == '__main__':
    main()

