#!/bin/bash
for i in {1..7}
do
   echo "********Experiment $i********\n\n"
   ./xbdndp > try10_$i.txt
done
