#!/bin/bash

# these goes without saying

sudo apt-get install libboost-dev libboost-test1.42-dev libevent-dev automake libtool flex bison pkg-config g++

# also, install java-jdk & python

sudo apt-get install sun-java6-jdk python-twisted python-dev

wget http://ftp.carnet.hr/misc/apache//incubator/thrift/0.5.0-incubating/thrift-0.5.0.tar.gz

echo 'Extracting..'
tar -xvz thrift-0.5.0.tar.gz

cd thrift-0.5.0

export JAVA_HOME=/usr/lib/jvm/java-6-sun

make

sudo make install

