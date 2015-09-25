#!/bin/bash
LOG0_PATH=/home/nishanth/IDP/results/example_network/default_selection
LOG1_PATH=/home/nishanth/IDP/results/example_network/rank_based_selection
LOG2_PATH=/home/nishanth/IDP/results/example_network/tournament_selection
let a=1
let b=1
let i=2
let k=2
for (( i=2; i<=2; i++));
do
b=$i
a=$(($b * 10))
echo "*******Budget=$a********"
   for (( j=1; j<=7; j++));
   do
      echo "********Experiment $j********"
      ./xbdndp $a 0 1 > $LOG0_PATH/try0'_'$a'_'$j'.'txt 
      sleep 3
      ./xbdndp $a 1 1 > $LOG1_PATH/try1'_'$a'_'$j'.'txt 
      sleep 3
      ./xbdndp $a 2 1 > $LOG2_PATH/try2'_'$a'_'$j'.'txt 
      sleep 3
   done
done
