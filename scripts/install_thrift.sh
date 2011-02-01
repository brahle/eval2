#!/bin/bash

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
