#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T17:22:55
#
#-------------------------------------------------

QT       += core widgets sql

TEMPLATE = app

include("../QFontIcon/QFontIcon.pri")

INCLUDEPATH += ../QFontIcon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ThermusLib/release/ -lThermusLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ThermusLib/debug/ -lThermusLib
else:!mac: LIBS += -L$$OUT_PWD/../ThermusLib/lib/ -L../lib -lThermusLib

INCLUDEPATH += $$PWD/ThermusLib $$PWD/../ThermusLib/external $$PWD/../ThermusLib/macros \
               $$PWD/../ThermusLib/main
DEPENDPATH += $$PWD/../ThermusLib

unix:!mac {
    QMAKE_CXXFLAGS += --std=c++11
}

ICON = ../images/ThermusQt.icns

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS  += mainwindow.h \

RESOURCES += \
    ../images/images.qrc

DISTFILES += \
    .travis.yml \
    .appveyor.yml \
    innosetup.iss \
    ThermusQt.desktop \
    ../scripts/macdeploy.sh \
    ../scripts/linuxdeploy.sh

# Installation

DISTFILES += \
    external/particles_mass.txt \
    external/pdg_table.txt


TARGET = ThermusQt

mac {
    LIBS           += -L$$OUT_PWD/../ThermusLib/lib/ -lThermusLib
    PRE_TARGETDEPS += $$OUT_PWD/../ThermusLib/lib/libThermusLib.1.0.0.dylib
    images/ThermusQt.icns
    APP_PD.files      = ../particles/ThermusParticles.db ../particles/PDGParticles.db
    APP_PD.files     += ../particles/particles.tar.gz
    APP_PD.path       = Contents/Resources/particles
    APP_PY.files     += ../python/PDGParticles.py ../python/ThermusParticles.py
    APP_PY.path       = Contents/Resources/python
    APP_MAC.files     = ../macrotemplate/makelibrary.sh \
                        ../macrotemplate/plugintemplate.cpp ../macrotemplate/plugintemplate.h ../macrotemplate/plugintemplate.json ../macrotemplate/plugintemplate.pro \
                        ../macrotemplate/plugin_global.h \
                        ../ThermusLib/external/macrointerface.h
    APP_MAC.path      = Contents/Resources/plugintemplate
    APP_H.files       = ../ThermusLib/external/parasel.h ../ThermusLib/external/macroparasel.h ../ThermusLib/external/thermuswiz.h \
                        ../ThermusLib/main/TTMParameter.h ../ThermusLib/main/TTMParameterSet.h ../ThermusLib/main/TTMParameterSetBSQ.h

    APP_H.path        = Contents/include
    APP_LIB.files     = ../ThermusLib/lib/libThermusLib.1.0.0.dylib
    APP_LIB.path      = Contents/lib

    QMAKE_BUNDLE_DATA += APP_PD APP_PY APP_MAC APP_H APP_LIB
}
