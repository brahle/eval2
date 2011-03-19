#!/bin/bash
# Copyright 2011 Matija Osrecki
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

sudo apt-get install libboost-dev libboost-test1.42-dev
assert $?
sudo apt-get install libevent-dev automake libtool flex bison pkg-config
assert $?
sudo apt-get install php5-dev python-dev ruby1.8-dev sun-java6-jdk
assert $?
sudo apt-get install python-twisted
assert $?

wget http://ftp.carnet.hr/misc/apache//incubator/thrift/0.5.0-incubating/thrift-0.5.0.tar.gz
assert $?

echo 'Extracting..'
tar xvvf thrift-0.5.0.tar.gz
assert $?

cd thrift-0.5.0
export JAVA_HOME=/usr/lib/jvm/java-6-sun
sudo ./configure
assert $?

sudo make
assert $?

sudo make install
assert $?

cd ..
sudo rm -rf thrift*
sudo mv /usr/local/lib/libthrift* /lib
