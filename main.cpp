// Author: Yves Schutz 24 octobre 2016
//
// The main program

#include <QApplication>

#include "mainwindow.h"

#include "QFontIcon/qfonticon.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("ThermusQt");
    a.setOrganizationName("IPHC/IN2P3");
    a.setOrganizationDomain("in2p3.fr");
    a.setApplicationVersion("1.0");
    a.setApplicationDisplayName("ThermusQt");

//    QFontIcon::addFont(":/fontawesome.ttf");

    MainWindow w;
    w.show();
    return a.exec();
}
