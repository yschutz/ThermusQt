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
    filesel.cpp \
    external/particlesdbmanager.cpp \
    external/finddialog.cpp \
    external/listdialog.cpp \
    external/selectdialog.cpp

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
    filesel.h \
    external/particlesdbmanager.h \
    external/finddialog.h \
    external/listdialog.h \
    external/selectdialog.h


RESOURCES += \
    particles/particles.qrc \
    images/images.qrc

unix:LIBS += -L/usr/local/lib -lgsl
INCLUDEPATH += /usr/local/include

DISTFILES += \
    external/ManageParticleDB.py
