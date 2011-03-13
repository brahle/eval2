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

class LicenseAction(BaseHaskiAction):
    """This class is the class that does linting work.
    """
    LICENSE_FILE = '/COPYING'

    def __call__(self, params):
        """Fetches the desired revision and then sends it to reviewboard.
        """
        location = self.get_repository_location() + self.LICENSE_FILE
        license_file = open(location, 'r')
        for line in license_file:
            print(line, end='')
        license_file.close()


def main():
    pass

if __name__ == '__main__':
    main()

