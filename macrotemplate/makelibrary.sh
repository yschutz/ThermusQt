#!/bin/sh
export PATH=$PATH:$QTDIR
os=`uname`
if [ "$os" = "Darwin" ]; then
        echo DARWIN
        lib=lib$1.dylib
elif [ "$os" = "Linux" ]; then
        lib=lib$1.so
else
        echo OS $os not found
        exit
fi
if [ -f "Makefile" ]; then
        make clean
        rm MakeFile
fi
qmake $1.pro
make clean
if [ -f $lib ]; then
   rm $lib
fi
make
