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
#include <QHash>
#include <QVector>

#include <gsl/gsl_sf_bessel.h>

#include "main/TTMThermalFit.h"
#include "main/TTMYield.h"
#include "main/TTMThermalModel.h"
#include "external/particlesdbmanager.h"
#include "main/TTMParameter.h"
#include "main/TTMDensObj.h"

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

//    QList<int> stableParticles;
//    ParticlesDBManager::Instance().allParticles(stableParticles, ParticlesDBManager::kUNSTABLE);
//    for (int pdg : stableParticles) {
//        if (ParticlesDBManager::Instance().isStable(pdg))
//            qDebug() << pdg << "Stable";
//        else
//            qDebug() << pdg <<
//                        "instable";

//    }

    QVector<QVector<double>> vec(5);
    vec[0].resize(5);
    vec[1].resize(5);
    vec[2].resize(5);
    vec[3].resize(5);
    vec[4].resize(5);
    vec[0][0] = 10.1;
    vec[0][1] = 10.2;
    vec[0][2] = 10.3;
    qDebug() << vec[0][2];
    qDebug() << vec[1][2];

    double result = gsl_sf_bessel_In(1, 10.);
    qDebug() << result;


    return a.exec();
}
