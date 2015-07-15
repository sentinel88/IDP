#!/bin/bash
let a=1
let b=1
let i=3
for (( i=3; i<=3; i++));
do
b=$i
a=$(($b * 10))
echo "*******Budget=$a********"
   for (( j=1; j<=7; j++));
   do
      echo "********Experiment $j********"
      ./xbdndp $a > try$a'_'$j'.'txt 
   done
done
