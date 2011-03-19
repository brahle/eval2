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

from abc import ABCMeta, abstractmethod
import subprocess
import tempfile

class BaseLinter(object):
    """This class is used to model the linting classes.
    """
    __metaclass__ = ABCMeta
    BASE_SUFFIX = '.txt'
    BUFFER_SIZE = 10240
    NOP = ['/bin/true']

    GIT_LIST_FILES = 'git diff --name-only {0} {0}^'
    GIT_GET_CONTENTS = 'git cat-file blob {0}:{1}'

    def __init__(self, commit, repository_location, results_file=None):
        """
        """
        self.commit = commit
        self._prepare_files()
        self.repository_location = repository_location
        if results_file is None:
            self.results = tempfile.NamedTemporaryFile(mode='w+', delete=False)
        elif isinstance(results_file, str):
            self.results = open(results_file, 'w+')
        else:
            self.results = results_file
        self._process = None

    @abstractmethod
    def lint(self):
        """Used to start the linting process. It is asynchronous, which
        means the linting won't end when the method itslef termintes.
        IMPLEMENTATION NOTE: attach the corresponing process to
        self._process. The process must have a method wait() simillar
        to subprocess.wait() implemented.
        """
        pass

    def _prepare_files(self):
        """Prepares the files for linting. It gets all files from the
        selected commit and moves their original contents to a
        temporary location while trying to preserve as much as possible
        of the original name.
        """
        cmd = self.GIT_LIST_FILES.format(self.commit).split()
        raw_output = subprocess.check_output(cmd, stderr=subprocess.STDOUT)
        files = raw_output.decode().split('\n')
        files = self._filter_files(files)
        new_files = []
        for name in files:
            dummy_name = '.' + name.replace('/', '.')
            new = tempfile.NamedTemporaryFile(prefix='', suffix=dummy_name,
                                              delete=False)
            cmd = self.GIT_GET_CONTENTS.format(self.commit, name).split()
            subprocess.call(cmd, stdout=new)
            new.close()
            new_files.append(new.name)
        self.files = new_files

    def _filter_files(self, files):
        """Returns the list of files upon which the linting work should
        be done.
        Default version just return all files.
        """
        return files

    def get_results(self):
        """Used to return the results as string.
        """
        if self._process is None:
            self.lint()
        self._process.wait()
        self.results.seek(0)
        return self.results.read()

    def append_results(self, destination):
        """Appends the results to destination.
        """
        if self._process is None:
            self.lint()
        self._process.wait()
        self.results.seek(0)
        buff = self.results.read(self.BUFFER_SIZE)
        while buff != '':
            destination.write(buff)
            buff = self.results.read(self.BUFFER_SIZE)


def main():
    pass

if __name__ == '__main__':
    main()

