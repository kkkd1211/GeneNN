#!/bin/bash
rm *.txt
gcc code/*.cpp -o k -lm -O2
./k
./switch.sh
gnuplot makegif.plt
