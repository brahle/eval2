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

class ReviewAction(BaseHaskiAction):
    """This class is the class that does linting work.
    """
    RB_ID_STR = 'reviewboard id'

    def __call__(self, params):
        """Fetches the desired revision and then sends it to reviewboard.
        """
        commit = self.get_commit(params)
        if not params.skip_lint:
            commit.lint(params)
        rb_id = commit.review()
        if params.revision != 'HEAD':
            if self.RB_ID_STR not in commit.message.fields:
                print('[WARNING] Please edit the message to incorporate',
                      '`ReviewBoardID` field.')
        else:
            commit.message.set_field(self.RB_ID_STR, rb_id)
            commit.amend()

def main():
    pass

if __name__ == '__main__':
    main()

