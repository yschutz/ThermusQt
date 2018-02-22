#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T17:22:55
#
#-------------------------------------------------

QT       += core widgets sql

include("../QFontIcon/QFontIcon.pri")

INCLUDEPATH += ../QFontIcon

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ThermusLib/release/ -lThermusLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ThermusLib/debug/ -lThermusLib
else:unix: LIBS += -L$$OUT_PWD/../libs/ -lThermusLib

INCLUDEPATH += $$PWD/../ThermusLib $$PWD/../ThermusLib/external $$PWD/../ThermusLib/macros \
               $$PWD/../ThermusLib/main
DEPENDPATH += $$PWD/../ThermusLib

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

unix:!mac {
    QMAKE_CXXFLAGS += --std=c++11
}

mac {
    images/ThermusQt.icns
    TARGET = ThermusQt
    TEMPLATE          = app
    APP_PD.files      = ../particles/ThermusParticles.db ../particles/PDGParticles.db
    APP_PD.files     += ../particles/particles.tar.gz
    APP_PD.path       = Contents/Resources/particles
    APP_PY.files     += ../python/PDGParticles.py ../python/ThermusParticles.py
    APP_PY.path       = Contents/Resources/python
    APP_MAC.files     = ../macrotemplate/plugintemplate.h ../macrotemplate/plugintemplate.cpp ../macrotemplate/plugintemplate.json \
                        ../macrotemplate/plugintemplate.pro ../macrotemplate/makelibrary.sh \
                        ../macrotemplate/plugin_global.h ../ThermusLib/external/macrointerface.h
    APP_MAC.path      = Contents/Resources/plugintemplate
    APP_H.files       = ../ThermusLib/external/parasel.h ../ThermusLib/external/macroparasel.h \
                        ../ThermusLib/main/TTMParameter.h ../ThermusLib/main/TTMParameterSet.h ../ThermusLib/main/TTMParameterSetBSQ.h
    APP_H.path        = Contents/Resources/thermusinclude
    APP_LIB.files     = $$OUT_PWD/../libs/libThermusLib.dylib
    APP_LIB.path      = Contents/libs

    QMAKE_BUNDLE_DATA += APP_PD APP_PY APP_MAC APP_H APP_LIB
}

unix:!mac {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    target.path    = $$PREFIX/bin
    desktop.path   = $$PREFIX/share/applications
    desktop.files += ThermusQt.desktop
    icons.path     = $$PREFIX/share/icons/hicolor/48x48/apps
    icons.files   += ../images/ThermusQt.png
    data.files     = ../particles/ThermusParticles.db ../particles/PDGParticles.db
    data.files    += ../particles/particles.tar.gz
    data.path      = $$PREFIX/Resources/particles
    python.files   = ../python/PDGParticles.py ../python/ThermusParticles.py
    python.path    = $$PREFIX/Resources/python
    macro.files    = ../macrotemplate/plugintemplate.cpp ../macrotemplate/plugintemplate.h ../macrotemplate/plugintemplate.json  \
                     ../macrotemplate/plugintemplate.pro ../macrotemplate/makelibrary.sh\
                     ../macrotemplate/plugin_global.h ../ThermusLib/external/macrointerface.h
    macro.path     = $$PREFIX/Resources/plugintemplate
    header.files   = ../ThermusLib/external/parasel.h ../ThermusLib/external/macroparasel.h \
                     ../ThermusLib/main/TTMParameter.h ../ThermusLib/main/TTMParameterSet.h ../ThermusLib/main/TTMParameterSetBSQ.h
    header.path    = $$PREFIX/Resources/thermusinclude
    libs.files     = ../libs/libThermusLib.so
    libs.path      = $$PREFIX/libs

    INSTALLS += target icons desktop data python macro header
}
