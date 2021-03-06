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
for ((i=0;i<$1;i++))
do
    pkurun-cnlong 1 1 ./k $i
done
