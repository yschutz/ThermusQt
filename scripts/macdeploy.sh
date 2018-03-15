#!/bin/sh
# deploy thermus on Mac as an app
if [ ! $# -eq 1 ]; then
	echo "usage: macdeploy.sh APPDIR" 
	exit 1
fi

OS=`uname`
if [ ! $OS = "Darwin" ]; then 
	echo "You are on $OS : this script only works for Darwin (MacOS)"
	exit 1
fi

if [ ! $QTDIR ]; then 
	echo "You do not seem to have Qt installed or QTDIR is not set"
	exit 1
fi

APPDIR=$1
macdeployqt $APPDIR -dmg
APPNAME=ThermusQt
APPLIB=libThermusLib
APPLIBDIR=$APPDIR/Contents/lib
ln -s $APPLIBDIR/$APPLIB.1.0.0.dylib $APPLIBDIR/$APPLIB.1.0.dylib 
ln -s $APPLIBDIR/$APPLIB.1.0.0.dylib $APPLIBDIR/$APPLIB.1.dylib 
ln -s $APPLIBDIR/$APPLIB.1.0.0.dylib $APPLIBDIR/$APPLIB.dylib 
# otool -L $APPDIR/Contents/MacOS/$APPNAME
cp $APPLIBDIR/$APPLIB.1.0.0.dylib $APPDIR/Contents/Frameworks/$APPLIB.1.dylib
install_name_tool -change $APPLIB.1.dylib @executable_path/../Frameworks/$APPLIB.1.dylib $APPDIR/Contents/MacOS/$APPNAME
rm $APPDIR/../$APPNAME.dmg
macdeployqt $APPDIR -dmg