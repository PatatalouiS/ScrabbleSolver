#!/bin/bash

expect -c 'spawn ssh -N -L 8080:192.168.77.13:3389 p1710179@linuxetu.univ-lyon1.fr; expect "assword:"; send "24Juin1995\r"; interact'
