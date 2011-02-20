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

# these go without saying

sudo apt-get install libboost-dev libboost-test1.42-dev libevent-dev automake libtool flex bison pkg-config g++ php5-dev python-dev ruby1.8-dev

# also, install java-jdk & python

sudo apt-get install sun-java6-jdk python-twisted python-dev

wget http://ftp.carnet.hr/misc/apache//incubator/thrift/0.5.0-incubating/thrift-0.5.0.tar.gz

echo 'Extracting..'
tar xvvf thrift-0.5.0.tar.gz

cd thrift-0.5.0

export JAVA_HOME=/usr/lib/jvm/java-6-sun

sudo ./configure

sudo make

sudo make install

cd ..

sudo rm -rf thrift*
