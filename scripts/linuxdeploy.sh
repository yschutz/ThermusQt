#!/bin/sh
# deploy thermus on linux as an AppImage
OS=`uname`
if [ ! $OS = 'Linux' ]; then
	echo "You are on $OS : this script works only for Linux"
	exit 1
fi
#check if there is a qt installation
which qmake 
if [ ! $? -eq 0 ]; then 
	echo "Qt apparently not installed ! Trying to install it" 
        sudo apt-get install qt5-default 
        if [ ! $0 -eq 0 ]; then 
           echo "no su priviledges ?" 
           exit 1
        fi
        sudo apt-get instal qt5-qmake   
fi

#check if gsl is installed 
if [ ! -d /usr/include/gsl ]; then 
	echo "gsl apparently not installed ! Trying to install it"
        sudo apt-get install libgsl2
        if [ ! $0 -eq 0 ]; then 
           echo "no su priviledges ?" 
           exit 1
        fi
        sudo apt-get install gsl-bin libgsl0-dev
fi

#check if git installed
if [ ! -e /usr/bin/git ]; then
    echo "git apparently not installed ! Trying to install it"
    sudo apt-get Install git
fi
#check if ThermusQt software present 
if [ ! -d ThermusQt ]; then
	git clone https://github.com/yschutz/ThermusQt.git
        if [ ! $0 -eq 0 ]; then
           echo "no su priviledges ?"
           exit 1
        fi
fi 

cd ThermusQt

# check if Makefile already exists and clean everything
if [ -e Makefile ]
then
    make clean 
    rm Makefile MainApp/Makefile ThermusLib/Makefile 
    if [ -d appdir ]
    then 
        rm -fR appdir
    fi
    if [ -e MainApp/ThermusQt ]
    then 
        rm MainApp/ThermusQt
    fi
    if [ -d ThermusLib/lib ]
    then 
        rm -fr ThermusLib/lib
    fi
fi
# creates the make file 
qmake CONFIG+=release PREFIX=/usr
# makes everything
make -j6
if [ ! $? -eq 0 ]; then 
	echo compilation failed ... exiting
	exit 1
fi
# recreate the Makefile ... if not bin and lib will not be installed
qmake CONFIG+=release PREFIX=/usr
make INSTALL_ROOT=appdir -j6 install 
cd appdir/usr/lib/
mv libThermusLib.so libThermusLib.so.1.0.0
ln -s libThermusLib.so.1.0.0 libThermusLib.so
ln -s libThermusLib.so.1.0.0 libThermusLib.so.1
ln -s libThermusLib.so.1.0.0 libThermusLib.so.1.0
cd - ; find appdir/
if [ ! -e linuxdeployqt-continuous-x86_64.AppImage ]
then 
    wget -c -q "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    chmod a+x linuxdeployqt*.AppImage
fi
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
./linuxdeployqt*.AppImage ./appdir/usr/share/applications/*.desktop -bundle-non-qt-libs
if [ ! $? -eq 0 ]
then 
     echo "./linuxdeployqt*.AppImage ./appdir/usr/share/applications/*.desktop -bundle-non-qt-libs" ended with error code	
fi
./linuxdeployqt*.AppImage ./appdir/usr/share/applications/*.desktop -appimage
if [ ! $? -eq 0 ]
then 
     echo "./linuxdeployqt*.AppImage ./appdir/usr/share/applications/*.desktop -appimage" ended with error code	
fi
mv ThermusQt*.AppImage* ~/Desktop
make clean 
rm Makefile MainApp/Makefile ThermusLib/Makefile 
rm -fR appdir
rm MainApp/ThermusQt
rm -fr ThermusLib/lib
rm *.AppImage
echo "*********************************************************"
echo Congratulations: ThermusQt.AppImage saved on your desktop
echo You can now use ThermusQt .... enjoy !
echo "*********************************************************"
