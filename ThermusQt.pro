TEMPLATE = subdirs

SUBDIRS = PlotLib ThermusLib MainApp \


CONFIG += ordered

PlotLib.subdir    = PlotLib
ThermusLib.subdir = ThermusLib
MainApp.subdir    = MainApp

MainApp.depends = PlotLib ThermusLib

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
                     $$OUT_PWD/ThermusLib/external/fittingthread.h $$OUT_PWD/ThermusLib/external/QMinuit.h \
                     $$OUT_PWD/ThermusLib/main/TTMParameter.h $$OUT_PWD/ThermusLib/main/TTMParameterSet.h $$OUT_PWD/ThermusLib/main/TTMParameterSetBSQ.h \
                     $$OUT_PWD/ThermusLib/main/TTMThermalFit.h $$OUT_PWD/ThermusLib/main/TTMThermalFitBSQ.h $$OUT_PWD/ThermusLib/main/TTMYield.h \
                     $$OUT_PWD/ThermusLib/main/TTMParticleSet.h $$OUT_PWD/ThermusLib/main/TTMParticle.h \
                     $$OUT_PWD/PlotLib/plot.h
    header.path    = $$PREFIX/include

    lib.files      = $$OUT_PWD/ThermusLib/lib/libThermusLib.so.1.0.0 $$OUT_PWD/PlotLib/lib/libPlotLib.so.1.0.0
    lib.path       = $$PREFIX/lib

    app.files      = $$OUT_PWD/MainApp/ThermusQt
    app.path       = $$PREFIX/bin

    INSTALLS += icons desktop data python macro header lib app

    lib.target     = $$PREFIX/lib/libThermusLib.so.1.0 $$PREFIX/lib/libThermusLib.so.1
    lib.command    = ln -si  $$PREFIX/lib/libThermusLib.so.1.0.0 $$lib.target

    QMAKE_EXTRA_TARGETS += lib
    QMAKE_POST_LINK     += lib
}
