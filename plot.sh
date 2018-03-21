#!/bin/bash
for file in output/*
do
    cd $file
    sh ../../switch.sh
    gnuplot ../../makegif.plt
    cd ../../
done
