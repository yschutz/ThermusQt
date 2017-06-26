// Author: Yves Schutz 24 octobre 2016
//
// The main program

#include <QHBoxLayout>

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QtWidgets>

#include <QtSql>

#include <gsl/gsl_sf_bessel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//        db.setDatabaseName("/Users/schutz/group/Qt/ThermusQt/particles/Thermus.db");
//        bool ok = db.open();
//        qDebug() << ok;

//        QSqlQueryModel  *model = new QSqlQueryModel;
//        QTableView *view = new QTableView;
//        view->setModel(model);

//        model->setQuery("SELECT * FROM particle");
//        view->show();

//        QSqlQuery q;
//        q.exec("SELECT * FROM particle");
//        while (q.next()) {
//            QString name = q.record().value("name").toString();

//            qDebug() << name;
//        }
        return a.exec();
}
