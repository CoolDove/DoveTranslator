#!/bin/bash
cd /home/dove/workspace/dotrans

#tokens
PATH_ROOT=`pwd`
PATH_INT_BIN=${PATH_ROOT}/bin_int
OUTPUT_NAME=dt

echo building...

cd ${PATH_INT_BIN}

echo compiling...
g++ \
-c ${PATH_ROOT}/DoveTranslator/src/main.cpp ${PATH_ROOT}/DoveTranslator/vendor/src/json/*.cpp \
-I ${PATH_ROOT}/DoveTranslator/vendor/include 

echo linking...
g++ -g *.o -o ${PATH_ROOT}/bin/linux/${OUTPUT_NAME} -lcurl -lcrypto

cd ${PATH_ROOT}
