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

from repository import Repository

from abc import ABCMeta, abstractmethod

class BaseHaskiAction(object):
    """This class is the abstract base class for Haski tasks.
    """
    __metaclass__ = ABCMeta

    def __init__(self):
        self._repository = Repository()

    @abstractmethod
    def __call__(self, namespace):
        """This method is invoked by argparse and should do the actuall work.
        By default, it is abstract and does nothing, so it forces you to
        implement it.
        """
        pass

    def get_commit(self, namespace):
        """This function gets the commit wanted commit on which an action is to
        be performed.
        """
        commit = self._repository.get_commit(namespace.revision,
                                             namespace.ignore_message)
        return commit

    def get_repository_location(self):
        """Returns the directory where this repository is located.
        """
        return self._repository.get_path()


def main():
    pass

if __name__ == '__main__':
    main()

