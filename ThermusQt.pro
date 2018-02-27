TEMPLATE = subdirs

SUBDIRS = ThermusLib 

CONFIG += ordered

ThermusLib.subdir = ThermusLib


MainApp.subdir    = MainApp

SUBDIRS += MainApp

MainApp.depends = ThermusLib

mac {
    images/ThermusQt.icns
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
    desktop.files += ThermusQt.desktop
    desktop.path   = $$PREFIX/share/applications

    icons.path     = $$PREFIX/share/icons/hicolor/48x48/apps
    icons.files   += images/ThermusQt.png

    data.files     = particles/ThermusParticles.db particles/PDGParticles.db
    data.files    += particles/particles.tar.gz
    data.path      = $$PREFIX/Resources/particles

    python.files   = python/PDGParticles.py python/ThermusParticles.py
    python.path    = $$PREFIX/Resources/python

    macro.files    = macrotemplate/* ThermusLib/external/macrointerface.h
    macro.path     = $$PREFIX/Resources/plugintemplate

    header.files   = ThermusLib/external/parasel.h ThermusLib/external/macroparasel.h \
                     ThermusLib/main/TTMParameter.h ThermusLib/main/TTMParameterSet.h ThermusLib/main/TTMParameterSetBSQ.h
    header.path    = $$PREFIX/include

    INSTALLS += icons desktop data python macro header
}
