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

from tempfile import TemporaryFile
import os, shlex, subprocess

class InvokerException(HaskiException):
    pass

class Invoker(object):
    def __init__(self, cmd, display_output=False):
        if isinstance(cmd, str):
            self.cmd = shlex.split(cmd)
        elif isinstance(cmd, list):
            self.cmd = cmd
        elif isinstance(cmd, tuple):
            self.cmd = list(cmd)
        else:
            raise InvokerException('Unknown cmd type (%s)!' %
                                    (self.cmd.__class__.__name__))
        self.display_output = display_output
        self.stdout = ''
        self.stderr = ''
        self.status = 0
        self.invoked = 0

    def invoke(self):
        stdout_file = TemporaryFile()
        stderr_file = TemporaryFile()
        if self.display_output:
            p = subprocess.Popen(self.cmd)
        else:
            p = subprocess.Popen(self.cmd, stdout=stdout_file,
                                 stderr=stderr_file)
        p.wait()
        stdout_file.seek(0, os.SEEK_SET)
        stderr_file.seek(0, os.SEEK_SET)

        self.stdout = stdout_file.read().decode()
        self.stderr = stderr_file.read().decode()
        self.status = int(p.returncode)
        self.invoked = 1

def main():
    pass

if __name__ == '__main__':
    main()

