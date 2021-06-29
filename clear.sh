#!/bin/bash

PATH_ROOT=`pwd`

cd /home/dove/workspace/dotrans
chmod 777 bin_int
chmod 777 bin

rm bin_int/*
rm bin/*

cd ${PATH_ROOT}
