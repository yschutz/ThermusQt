#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T15:01:06
#
#-------------------------------------------------

QT       += widgets sql

TARGET = $$qtLibraryTarget(ThermusLib)
TEMPLATE = lib

DEFINES += THERMUSLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += main external functions macros
INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib -lgsl -lgslcblas -lm

unix:!mac {
    QMAKE_CXXFLAGS += --std=c++11
}

SOURCES += \
        thermuslib.cpp \
    external/editorwindow.cpp \
    external/f1.cpp \
    external/f2.cpp \
    external/finddialog.cpp \
    external/highlighter.cpp \
    external/listdialog.cpp \
    external/macroeditor.cpp \
    external/macrointerface.cpp \
    external/newdecaydialog.cpp \
    external/newparticledialog.cpp \
    external/particlesdbmanager.cpp \
    external/QMinuit.cpp \
    external/parasel.cpp \
    external/macroparasel.cpp \
    functions/BQConstrainBDens.cpp \
    functions/BQConstrainEN.cpp \
    functions/BQConstrainQ.cpp \
    functions/BQConstrainQBDens.cpp \
    functions/BQConstrainQEN.cpp \
    functions/BQConstrainQNetBDens.cpp \
    functions/BQConstrainQPercolation.cpp \
    functions/BQConstrainQST3.cpp \
    functions/BQConstrainST3.cpp \
    functions/BQfuncBDens.cpp \
    functions/BQfuncEN.cpp \
    functions/BQfuncQ.cpp \
    functions/BQfuncQBDens.cpp \
    functions/BQfuncQEN.cpp \
    functions/BQfuncQNetBDens.cpp \
    functions/BQfuncQPercolation.cpp \
    functions/BQfuncQST3.cpp \
    functions/BQfuncST3.cpp \
    functions/brent.cpp \
    functions/broyden.cpp \
    functions/broydens.cpp \
    functions/BSQConstrainBSQ.cpp \
    functions/BSQConstrainQ.cpp \
    functions/BSQConstrainQQ.cpp \
    functions/BSQConstrainS.cpp \
    functions/BSQConstrainSBDens.cpp \
    functions/BSQConstrainSEN.cpp \
    functions/BSQConstrainSPercolation.cpp \
    functions/BSQConstrainSQ.cpp \
    functions/BSQConstrainSQBDens.cpp \
    functions/BSQConstrainSQC.cpp \
    functions/BSQConstrainSQCb.cpp \
    functions/BSQConstrainSQEN.cpp \
    functions/BSQConstrainSQNetBDens.cpp \
    functions/BSQConstrainSQPercolation.cpp \
    functions/BSQConstrainSQST3.cpp \
    functions/BSQConstrainSST3.cpp \
    functions/BSQfuncBSQ.cpp \
    functions/BSQfuncQ.cpp \
    functions/BSQfuncQQ.cpp \
    functions/BSQfuncS.cpp \
    functions/BSQfuncSBDens.cpp \
    functions/BSQfuncSEN.cpp \
    functions/BSQfuncSPercolation.cpp \
    functions/BSQfuncSQ.cpp \
    functions/BSQfuncSQBDens.cpp \
    functions/BSQfuncSQC.cpp \
    functions/BSQfuncSQCb.cpp \
    functions/BSQfuncSQEN.cpp \
    functions/BSQfuncSQNetBDens.cpp \
    functions/BSQfuncSQPercolation.cpp \
    functions/BSQfuncSQST3.cpp \
    functions/BSQfuncSST3.cpp \
    functions/ExclVolPressureFunc.cpp \
    functions/FindExclVolPressure.cpp \
    functions/fit_function.cpp \
    functions/functest.cpp \
    functions/Functions.cpp \
    functions/InteggrationRoutines2D.cpp \
    functions/IntegrationRoutines.cpp \
    functions/minuit_fcn.cpp \
    macros/fitmacro.cpp \
    macros/macro.cpp \
    macros/predictionmacro.cpp \
    external/logger.cpp \
    external/thermuswiz.cpp \
    external/fittingthread.cpp \
    external/summary.cpp \
    main/TTMDecay.cpp \
    main/TTMDecayChannel.cpp \
    main/TTMDensObj.cpp \
    main/TTMIDObj.cpp \
    main/TTMParameter.cpp \
    main/TTMParameterSet.cpp \
    main/TTMParameterSetBQ.cpp \
    main/TTMParameterSetBSQ.cpp \
    main/TTMParticle.cpp \
    main/TTMParticleSet.cpp \
    main/TTMThermalFit.cpp \
    main/TTMThermalFitBQ.cpp \
    main/TTMThermalFitBSQ.cpp \
    main/TTMThermalModel.cpp \
    main/TTMThermalModelBQ.cpp \
    main/TTMThermalModelBSQ.cpp \
    main/TTMThermalParticle.cpp \
    main/TTMThermalParticleBQ.cpp \
    main/TTMThermalParticleBSQ.cpp \
    main/TTMYield.cpp

HEADERS += \
        thermuslib.h \
        thermuslib_global.h \ 
    external/editorwindow.h \
    external/f1.h \
    external/f2.h \
    external/finddialog.h \
    external/highlighter.h \
    external/listdialog.h \
    external/macroeditor.h \
    external/macrointerface.h \
    external/newdecaydialog.h \
    external/newparticledialog.h \
    external/particlesdbmanager.h \
    external/QMinuit.h \
    external/parasel.h \
    external/macroparasel.h \
    functions/FncsConstrain.h \
    functions/FncsThermalModel.h \
    macros/fitmacro.h \
    macros/macro.h \
    macros/predictionmacro.h \
    external/logger.h \
    external/thermuswiz.h \
    external/fittingthread.h \
    external/summary.h \
    main/TTMDecay.h \
    main/TTMDecayChannel.h \
    main/TTMDensObj.h \
    main/TTMIDObj.h \
    main/TTMParameter.h \
    main/TTMParameterSet.h \
    main/TTMParameterSetBQ.h \
    main/TTMParameterSetBSQ.h \
    main/TTMParticle.h \
    main/TTMParticleSet.h \
    main/TTMThermalFit.h \
    main/TTMThermalFitBQ.h \
    main/TTMThermalFitBSQ.h \
    main/TTMThermalModel.h \
    main/TTMThermalModelBQ.h \
    main/TTMThermalModelBSQ.h \
    main/TTMThermalParticle.h \
    main/TTMThermalParticleBQ.h \
    main/TTMThermalParticleBSQ.h \
    main/TTMYield.h

DESTDIR = ../lib
