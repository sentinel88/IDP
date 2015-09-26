#!/bin/bash
LOG0_PATH=/home/nishanth/IDP/results/Sioux_falls_network/default_selection
LOG1_PATH=/home/nishanth/IDP/results/Sioux_falls_network/rank_based_selection
LOG2_PATH=/home/nishanth/IDP/results/Sioux_falls_network/tournament_selection

rm -rf $LOG0_PATH/*
rm -rf $LOG1_PATH/*
rm -rf $LOG2_PATH/*

let a=1
let b=1
let i=2
let k=2
for (( i=2; i<=7; i++));
do
b=$i
a=$(($b * 10))
echo "*******Budget=$a********"
   for (( j=1; j<=7; j++));
   do
      echo "********Experiment $j********"
      #valgrind --leak-check=yes ./xbdndp $a 0 2 > $LOG0_PATH/try0'_'$a'_'$j'.'txt 2>&1 
      ./xbdndp $a 0 2 > $LOG0_PATH/try0'_'$a'_'$j'.'txt 
      sleep 3
      #valgrind --leak-check=yes ./xbdndp $a 1 2 > $LOG1_PATH/try1'_'$a'_'$j'.'txt 2>&1
      ./xbdndp $a 1 2 > $LOG1_PATH/try1'_'$a'_'$j'.'txt 
      sleep 3
      #valgrind --leak-check=yes ./xbdndp $a 2 2 > $LOG2_PATH/try2'_'$a'_'$j'.'txt 2>&1
      ./xbdndp $a 2 2 > $LOG2_PATH/try2'_'$a'_'$j'.'txt
      sleep 3
   done
done
