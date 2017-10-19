#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T17:22:55
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ThermusQt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    main/TTMParticleSet.cpp \
    main/TTMParticle.cpp \
    main/TTMDecay.cpp \
    main/TTMDecayChannel.cpp \
    main/TTMIDObj.cpp \
    parasel.cpp \
    main/TTMParameter.cpp \
    thermuswiz.cpp \
    main/TTMParameterSet.cpp \
    main/TTMParameterSetBSQ.cpp \
    main/TTMThermalFit.cpp \
    main/TTMYield.cpp \
    external/QMinuit.cpp \
    summary.cpp \
    macros/runmacro.cpp \
    logger.cpp \
    external/particlesdbmanager.cpp \
    external/finddialog.cpp \
    external/listdialog.cpp \
    external/selectdialog.cpp \
    external/newdecaydialog.cpp \
    external/newparticledialog.cpp \
    main/TTMThermalFitBQ.cpp \
    main/TTMParameterSetBQ.cpp \
    main/TTMDensObj.cpp \
    main/TTMThermalModel.cpp \
    main/TTMThermalModelBQ.cpp \
    functions/BQConstrainQ.cpp \
    main/TTMThermalParticle.cpp \
    functions/Functions.cpp \
    external/f1.cpp \
    functions/IntegrationRoutines.cpp \
    main/TTMThermalParticleBQ.cpp \
    main/TTMThermalParticleBSQ.cpp \
    external/f2.cpp \
    functions/InteggrationRoutines2D.cpp \
    functions/BQConstrainEN.cpp \
    functions/BQfuncQ.cpp \
    functions/BQConstrainQEN.cpp \
    functions/BQConstrainQNetBDens.cpp \
    functions/BQConstrainQPercolation.cpp \
    functions/BQfuncQPercolation.cpp \
    functions/BQConstrainQST3.cpp \
    functions/BQConstrainST3.cpp \
    functions/BQfuncST3.cpp \
    functions/BQConstrainBDens.cpp \
    functions/BQfuncBDens.cpp \
    functions/BQConstrainQBDens.cpp \
    functions/BQfuncQBDens.cpp \
    functions/BQfuncQEN.cpp \
    functions/BQfuncEN.cpp \
    functions/BQfuncQNetBDens.cpp \
    functions/BQfuncQST3.cpp \
    functions/functest.cpp \
    functions/broyden.cpp \
    main/TTMThermalModelBSQ.cpp \
    functions/BSQConstrainQ.cpp \
    functions/BSQfuncQ.cpp \
    functions/broydens.cpp \
    functions/BSQConstrainS.cpp \
    functions/BSQfuncS.cpp \
    functions/BSQConstrainQQ.cpp \
    functions/BSQfuncQQ.cpp \
    functions/BSQConstrainSQ.cpp \
    functions/BSQfuncSQ.cpp \
    functions/BSQConstrainBSQ.cpp \
    functions/BSQfuncBSQ.cpp \
    functions/BSQConstrainSEN.cpp \
    functions/BSQfuncSEN.cpp \
    functions/BSQConstrainSQC.cpp \
    functions/BSQfuncSQC.cpp \
    functions/BSQConstrainSQCb.cpp \
    functions/BSQfuncSQCb.cpp \
    functions/BSQConstrainSQEN.cpp \
    functions/BSQfuncSQEN.cpp \
    functions/BSQConstrainSST3.cpp \
    functions/BSQfuncSST3.cpp \
    functions/BSQConstrainSQST3.cpp \
    functions/BSQfuncSQST3.cpp \
    functions/BSQConstrainSBDens.cpp \
    functions/BSQfuncSBDens.cpp \
    functions/BSQConstrainSQBDens.cpp \
    functions/BSQfuncSQBDens.cpp \
    functions/FindExclVolPressure.cpp \
    functions/brent.cpp \
    functions/ExclVolPressureFunc.cpp \
    functions/BSQConstrainSQNetBDens.cpp \
    functions/BSQfuncSQNetBDens.cpp \
    functions/BSQConstrainSPercolation.cpp \
    functions/BSQfuncSPercolation.cpp \
    functions/BSQConstrainSQPercolation.cpp \
    functions/BSQfuncSQPercolation.cpp \
    main/TTMThermalFitBSQ.cpp

HEADERS  += mainwindow.h \
    main/TTMParticleSet.h \
    main/TTMParticle.h \
    main/TTMDecay.h \
    main/TTMDecayChannel.h \
    main/TTMIDObj.h \
    parasel.h \
    main/TTMParameter.h \
    thermuswiz.h \
    main/TTMParameterSet.h \
    main/TTMParameterSetBSQ.h \
    main/TTMThermalFit.h \
    main/TTMYield.h \
    external/QMinuit.h \
    summary.h \
    macros/runmacro.h \
    logger.h \
    external/particlesdbmanager.h \
    external/finddialog.h \
    external/listdialog.h \
    external/selectdialog.h \
    external/newdecaydialog.h \
    external/newparticledialog.h \
    main/TTMThermalFitBQ.h \
    main/TTMParameterSetBQ.h \
    main/TTMDensObj.h \
    main/TTMThermalModel.h \
    main/TTMThermalModelBQ.h \
    functions/FncsConstrain.h \
    functions/FncsThermalModel.h \
    main/TTMThermalParticle.h \
    external/f1.h \
    main/TTMThermalParticleBQ.h \
    main/TTMThermalParticleBSQ.h \
    external/f2.h \
    main/TTMThermalModelBSQ.h \
    main/TTMThermalFitBSQ.h


RESOURCES += \
    particles/particles.qrc \
    images/images.qrc

unix:LIBS += -L/usr/local/lib -lgsl
INCLUDEPATH += /usr/local/include

DISTFILES += \
    external/ManageParticleDB.py \
    data/prediction_yannick.txt
