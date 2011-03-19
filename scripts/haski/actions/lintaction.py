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

from actions.baseaction import BaseHaskiAction

import argparse

class LintAction(BaseHaskiAction):
    """This class is the class that does linting work.
    """
    def __call__(self, params):
        """Fetches the desired revision and invokes lint action upon it.
        """
        commit = self.get_commit(params)
        commit.lint()

def main():
    pass

if __name__ == '__main__':
    main()

