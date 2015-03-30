#!/bin/bash
echo "removing old files"
rm -f libHHKinFit.so run

echo "creating shared library"
g++ -fPIC -shared src/*.cpp `root-config --cflags --glibs` -I ./include -o libHHKinFit.so

echo "creating executable"
g++ main.C `root-config --cflags --glibs` -I ./include -L . -lHHKinFit  -o runHHKinFit
