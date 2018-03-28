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
    ./k $i
done
rm k
g++ code_clust/*.cpp -o clust -O2
./clust $1
rm clust
./plot.sh 1>>tmp 2>>tmp
