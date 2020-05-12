#!/bin/bash

echo "Now you can use the --suzette_check option in the ScrabbleSolver !"
echo "Don't kill this process as long as you use --suzette-check"

{
	expect -c 'spawn ssh -N -L 8080:192.168.77.13:3389 p1710179@linuxetu.univ-lyon1.fr; expect "assword:"; send "24Juin1995\r"; interact'
} > /dev/null
