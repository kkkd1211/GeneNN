#!/bin/bash
echo '' > tmp
for file in output/*
do
    cd $file
    rm ./*.txt
    rm ./*.gif
    rm ./*.png
    cd ../..
done
rm *.txt
rm k
gcc code/*.cpp -o k -lm -O2
./k
./plot.sh 1>tmp 2>tmp
##./switch.sh
##gnuplot makegif.plt
