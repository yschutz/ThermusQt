TEMPLATE = subdirs

SUBDIRS = ThermusLib MainApp

CONFIG += ordered

ThermusLib.subdir = ThermusLib
MainApp.subdir    = MainApp

MainApp.depends = ThermusLib

unix:!mac {
    isEmpty(PREFIX) {
        PREFIX = /usr/local
    }
    target.path = $$PREFIX/bin
    INSTALLS += target

    desktop.files += $$OUT_PWD/ThermusQt.desktop
    desktop.path   = $$PREFIX/share/applications

    icons.files   += $$OUT_PWD/images/ThermusQt.png
    icons.path     = $$PREFIX/share/icons/hicolor/48x48/apps

    data.files     = $$OUT_PWD/particles/ThermusParticles.db $$OUT_PWD/particles/PDGParticles.db \
                     $$OUT_PWD/particles/particles.tar.gz
    data.path      = $$PREFIX/Resources/particles

    python.files   = $$OUT_PWD/python/PDGParticles.py $$OUT_PWD/python/ThermusParticles.py
    python.path    = $$PREFIX/Resources/python

    macro.files    = $$OUT_PWD/macrotemplate/* $$OUT_PWD/ThermusLib/external/macrointerface.h
    macro.path     = $$PREFIX/Resources/plugintemplate

    header.files   = $$OUT_PWD/ThermusLib/external/parasel.h $$OUT_PWD/ThermusLib/external/macroparasel.h \
                     $$OUT_PWD/ThermusLib/main/TTMParameter.h $$OUT_PWD/ThermusLib/main/TTMParameterSet.h $$OUT_PWD/ThermusLib/main/TTMParameterSetBSQ.h
    header.path    = $$PREFIX/include

    lib.files      = $$OUT_PWD/ThermusLib/lib/libThermusLib.so
    lib.path       = $$PREFIX/lib

    app.files      = $$OUT_PWD/MainApp/ThermusQt
    app.path       = $$PREFIX/bin

    INSTALLS += icons desktop data python macro header lib app
}
