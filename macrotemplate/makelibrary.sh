#!/bin/sh
export PATH=$PATH:$QTDIR
os=`uname`
if [ "$os" = "Darwin" ]; then
        lib=lib$1.dylib
elif [ "$os" = "Linux" ]; then
        lib=lib$1.so
else
        echo OS $os not found
        exit
fi
if [ -f "Makefile" ]; then
        make clean
        rm Makefile
fi
qmake $1.pro
if [ "$os" = "Linux" ]; then
    sed -i '/CXXFLAGS      =/a CXXFLAGS      += --std=c++11' Makefile
fi
make clean
if [ -f $lib ]; then
   rm $lib
fi
make
