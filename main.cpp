// Author: Yves Schutz 24 octobre 2016
//
// The main program

#include <QHBoxLayout>

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QtWidgets>

#include "external/particlesdbmanager.h"

#include <gsl/gsl_sf_bessel.h>

#include <QtSql>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();
    ParticlesDBManager* db = new ParticlesDBManager("/Users/schutz/work/ThermusQt/particles/Particles.db");
    db->listDecays(9000111, 0.);

//        QSqlQuery q;
//        q.exec("SELECT * FROM particle");
//        while (q.next()) {
//            QString name = q.record().value("name").toString();

//            qDebug() << name;
//        }
        return a.exec();
}
