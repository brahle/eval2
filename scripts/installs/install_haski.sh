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

function assert {
    if [[ $1 -ne 0 ]]
    then
        echo "Error!!! Aborting."
        exit 0
    fi
}

./install_python3-2.sh
assert $?

dir=`git rev-parse --show-toplevel`
haski_dir=$dir'/scripts/haski'
haski=$haski_dir'/haski.py'

echo "Installing python setup tools..."
sudo apt-get install python-setuptools
assert $?

echo "Installing RBTools (for post-review)..."
sudo easy_install -U RBTools
assert $?

echo "Creating a symbolic link in /usr/local/bin/..."
sudo ln -s $haski /usr/local/bin/haski
assert $?

echo "Seting the reviewboard url to your git configuration..."
git config reviewboard.url http://evaluator.skole.hr:8080/reviews/
assert $?

echo "Success! Haski should now be working!"
