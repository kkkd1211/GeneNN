#!/bin/bash
for file in ./*
do
    if [ "${file##*.}" = "txt" ]; then
        echo "row-column switching: $file"
        awk '{for(i=1;i<=NF;i=i+1){a[NR,i]=$i}}END{for(j=1;j<=NF;j++){str=a[1,j];for(i=2;i<=NR;i++){str=str " " a[i,j]}print str}}' $file >> "${file%.*}_P.txt"
        rm $file
    fi
done





