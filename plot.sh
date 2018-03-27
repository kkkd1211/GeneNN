#!/bin/bash
for file in testoutput/*/*
do
    cd $file
    sh ../../../switch.sh
    gnuplot ../../../makegif.plt
    cd ../../..
done
