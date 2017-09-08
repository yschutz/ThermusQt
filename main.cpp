// Author: Yves Schutz 24 octobre 2016
//
// The main program

#include <QHBoxLayout>

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

#include <QStandardItemModel>
#include <QTableView>
#include <QSqlQuery>

#include <gsl/gsl_sf_bessel.h>

#include "main/TTMThermalFit.h"
#include "main/TTMYield.h"
#include "external/particlesdbmanager.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    MainWindow w;
//    w.show();

    ParticlesDBManager::Instance().connect("/Users/schutz/ThermusQt/particles/ThermusParticles.db");
//    TTMThermalFit fit;
//    fit.inputExpYields("/Users/schutz/cernbox/group/Thermus/THERMUS_v3.x_191016/simpletest/prediction_yannick.txt");
//    fit.listYields();
//    fit.listMinuitInfo();

    QList<int> stableParticles;
    ParticlesDBManager::Instance().allParticles(stableParticles, ParticlesDBManager::kUNSTABLE);
    for (int pdg : stableParticles) {
        if (ParticlesDBManager::Instance().isStable(pdg))
            qDebug() << pdg << "Stable";
        else
            qDebug() << pdg <<
                        "instable";

    }
    return a.exec();
}
