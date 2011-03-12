#!/bin/bash

out=0
col=`tput cols`

for i in bin/*;
do 
  j=`echo $i | sed 's/bin\///g'`
  f=`head $j.cc -n2 | tail -n 1`

  nap=`echo $col'-10-'${#f} | bc`

  $i &> /dev/null

  if [ $? -eq 0 ]; then
    echo -ne $f "\033["$nap"C [OK]\n"
  else
    out=1
    echo -ne $f "\033["$nap"C\e[1;31m[FAIL]\e[m\n"
  fi
done

exit $out
