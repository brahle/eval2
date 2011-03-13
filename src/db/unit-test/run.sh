#!/bin/bash

out=0
col=`tput cols`
argc=$#
make compile &> /tmp/compilation_out
if [ $? -ne 0 ]; then
  cat /tmp/compilation_out
  rm /tmp/compilation_out
  exit 1
fi
rm /tmp/compilation_out

if [ $argc -eq 0 ]; then
  for i in bin/*;
  do 
    j=`echo $i | sed 's/bin\///g'`
    f=`head $j.cc -n2 | tail -n 1`

    nap=`echo $col'-15-'${#f} | bc`

    $i &> /dev/null

    if [ $? -eq 0 ]; then
      echo -ne $j: $f "\033["$nap"C [OK]\n"
    else
      out=1
      echo -ne $j: $f "\033["$nap"C\e[1;31m[FAIL]\e[m\n"
    fi
  done
else
  i=bin/$1
  j=`echo $i | sed 's/bin\///g'`
  f=`head $j.cc -n2 | tail -n 1`

  nap=`echo $col'-15-'${#f} | bc`

  $i

  if [ $? -eq 0 ]; then
    echo -ne $j: $f "\033["$nap"C [OK]\n"
  else
    out=1
    echo -ne $j: $f "\033["$nap"C\e[1;31m[FAIL]\e[m\n"
  fi
fi

exit $out
