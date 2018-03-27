#!/bin/bash
echo '' > tmp
cd .
for file in testoutput/*/*
do
    cd $file
    rm ./*.txt 1>>../../../tmp 2>>../../../tmp
    rm ./*.gif 1>>../../../tmp 2>>../../../tmp
    rm ./*.png 1>>../../../tmp 2>>../../../tmp
    cd -
done
g++ code/*.cpp -o k -lm -O2
./k 99
rm k
./plot.sh 1>>tmp 2>>tmp
##./switch.sh
##gnuplot makegif.plt
