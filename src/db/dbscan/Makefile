SHELL = /bin/bash
CPPFLAGS = -O2 -Wall -I ../../../include -lpq -lpqxx

dbscan : dbscan.o json.o
	g++ -o $@ $(CPPFLAGS) $^ 

%.o : %.cpp
	g++ -c -o $@ $(CPPFLAGS) $<


