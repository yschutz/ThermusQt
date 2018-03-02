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
else:unix: LIBS += -L$$OUT_PWD/../ThermusLib/lib/ -l$$qtLibraryTarget(ThermusLib)

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

# Installation

DISTFILES += \
    external/particles_mass.txt \
    external/pdg_table.txt


TARGET = ThermusQt
unix:!mac {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    target.path = $$PREFIX/bin
    INSTALLS += target
}
