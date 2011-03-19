#!/bin/bash
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
# License along with Evaluator.  If not, see
# <http://www.gnu.org/licenses/>.

# Install required packages

function assert {
    if [[ $1 -ne 0 ]]
    then
        echo "Error!!! Aborting."
        exit 0
    fi
}

sudo apt-get install gcc g++
assert $?
sudo apt-get install python2.6 python-django
assert $?
sudo apt-get install git-core
assert $?
sudo apt-get install postgresql libpqxx-3.0 libpqxx3-dev
assert $?

