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

from actions.licenseaction import LicenseAction
from actions.lintaction import LintAction
from actions.reviewaction import ReviewAction
from haskiutils import HaskiException
from repositoryinfo import RepositoryInfo

import argparse, sys
from abc import ABCMeta, abstractmethod

def print_header():
    license_info = """\
Haski     Copyright (C) 2011 Bruno Rahle
This program comes with ABSOLUTELY NO WARRANTY.
This is free software, and you are welcome to redistribute it
under certain conditions.
"""
    print(license_info)


def main():
    """ TODO: docs
    """
    print_header()

    HASKI_DESCRIPTION = """\
This program is used for integration with ReviewBoard, our code-review system.

Any problems should be reported to brahle+haski@gmail.com
"""
    LICENSE_HELP = """\
Prints the license and exits.
"""
    LINT_HELP = """\
Lints the files from selected revision. Currently only works for C++.
"""
    REVIEW_HELP = """\
Sends the revision to ReviewBoard.
"""

    LINT_LOG_FILE = RepositoryInfo().get_path() + '/logs/haski-lint.log'
    HEAD = 'HEAD'

    LINT_DESCRIPTION = """\
File where you want to save your lint log. It defaults to {0}.
""".format(LINT_LOG_FILE)
    REVISION_DESCRIPTION = """\
Revision you want to perform the task upon. It defaults to {0}.
""".format(HEAD)
    SKIP_LINT_DESCRIPTION = """\
Flag that tels haski to skip linting and kill kittens. Do not use.
"""

    parser = argparse.ArgumentParser(description=HASKI_DESCRIPTION)
    subparsers = parser.add_subparsers()

    parser_license = subparsers.add_parser('license', help=LICENSE_HELP,
                                           description=LICENSE_HELP)
    parser_license.set_defaults(action=LicenseAction())

    parser_lint = subparsers.add_parser('lint', help=LINT_HELP,
            description=LINT_HELP)
    parser_lint.add_argument('-ll', '--lint-log', help=LINT_DESCRIPTION)
    parser_lint.add_argument('-r', '--revision', help=REVISION_DESCRIPTION)
    parser_lint.set_defaults(action=LintAction())

    parser_rb = subparsers.add_parser('review', help=REVIEW_HELP,
            description=REVIEW_HELP)
    parser_rb.add_argument('-r', '--revision', help=REVISION_DESCRIPTION)
    parser_rb.add_argument('--skip-lint', action='store_true',
            help=SKIP_LINT_DESCRIPTION)
    parser_rb.set_defaults(action=ReviewAction())

    parser.set_defaults(
        lint_log=LINT_LOG_FILE,
        revision=HEAD,
        skip_lint=False
    )

    args = parser.parse_args()

    try:
        args.action(args)
    except HaskiException as e:
        print('{0}: {1}'.format(type(e).__name__, e.message))



if __name__ == '__main__':
    main()

