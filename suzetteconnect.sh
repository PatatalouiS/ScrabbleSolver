#!/bin/bash

if [ ${#1} -lt 8 ]; then
  echo "Wrong username UCBL : pass a valid UCBL login ! [p1234567] form."
  exit 2
fi

echo "UCBL1 Login : "$1 
echo "Warning : After entering the password Don't kill this process as long as you use --suzette-check"
ssh -N -L 8080:192.168.77.13:3389 $1@linuxetu.univ-lyon1.fr
