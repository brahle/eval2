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

python3.2 -V >/dev/null 2>/dev/null
if [[ $? -ne 0 ]]
then
    echo "You don't have python 3.2 installed!"
    echo "Downloading python 3.2..."
    wget "http://www.python.org/ftp/python/3.2/Python-3.2.tgz"
    assert $?

    sudo mkdir -p /usr/local/src/python3.2
    assert $?

    sudo tar xvvf Python-3.2.tgz -C /usr/local/src/python3.2
    assert $?

    rm Python-3.2.tgz*
    echo "Python extracted!"

    echo "Installing dependencies..."
    sudo apt-get install libreadline6-dev
    echo "Done!"

    echo "Making..."
    cd /usr/local/src/python3.2/Python-3.2
    sudo ./configure
    assert $?

    sudo make
    assert $?

    sudo make install
    assert $?

    echo "Success!"
fi

