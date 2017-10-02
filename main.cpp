// Author: Yves Schutz 24 octobre 2016
//
// The main program

#include <QHBoxLayout>

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QScopedPointer>
#include <QStandardItemModel>
#include <QTableView>
#include <QSqlQuery>
#include <QHash>
#include <QVector>
#include <QtMath>

#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_multiroots.h>

#include "main/TTMThermalFit.h"
#include "main/TTMYield.h"
#include "main/TTMThermalModel.h"
#include "external/particlesdbmanager.h"
#include "main/TTMParameter.h"
#include "main/TTMDensObj.h"

#include "functions/FncsThermalModel.h"
#include "external/f2.h"
#include "functions/FncsConstrain.h"




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

//    QVector<QVector<double>> vec(5);
//    vec[0].resize(5);
//    vec[1].resize(5);
//    vec[2].resize(5);
//    vec[3].resize(5);
//    vec[4].resize(5);
//    vec[0][0] = 10.1;
//    vec[0][1] = 10.2;
//    vec[0][2] = 10.3;
//    qDebug() << vec[0][2];
//    qDebug() << vec[1][2];

//    double result = gsl_sf_bessel_In(1, 10.);
//    qDebug() << result << M_PI << qPow(M_PI, 2) << M_PI_2;


//    double (*ptr)(double*, double*);
//    ptr = &Test;
//    QScopedPointer<F2> test(new F2("test", ptr, 0, 10, 0, 10, 2));
//    test->setParameters(2, 5);
//    qDebug() <<  test->eval(4, 3);

//    qDebug() << test->integral(0, 10, 0, 10, 1e-10);

//    qDebug() << gsl_sf_bessel_Kn(2, 10);

//     qDebug() << Integrate2DLaguerre32Legendre32(test.data(), 0, 2);

//    const size_t n = 2;
//    struct papa p = {1, 10};
//    gsl_multiroot_function f;
//    f.f = &funcTest;
//    f.n = n;
//    f.params = &p;
//    double x_init[2] = {-10.0, -5.0};
//    gsl_vector_set (x, 0, x_init[0]);
//    gsl_vector_set (x, 1, x_init[1]);

//    const gsl_multiroot_fsolver_type *T;
//    T = gsl_multiroot_fsolver_broyden;

//    gsl_multiroot_fsolver *s;
//    s = gsl_multiroot_fsolver_alloc(T, n);
//    gsl_multiroot_fsolver_set (s, &f, x);

//    size_t iter = 0;
//    int status;
//    do
//      {
//        iter++;
//         status = gsl_multiroot_fsolver_iterate (s);

////        print_state (iter, s);

//        if (status)   /* check if solver is stuck */
//          break;

//        status =
//          gsl_multiroot_test_residual (s->f, 1e-7);
//      }
//    while (status == GSL_CONTINUE && iter < 1000);

//    qDebug() << "status = " << gsl_strerror (status);
//    qDebug() << gsl_vector_get(s->x, 0) << gsl_vector_get(s->x, 1);
//    qDebug() << gsl_vector_get(s->f, 0) << gsl_vector_get(s->f, 1);

    //     gsl_multiroot_fsolver_free (s);
    //     gsl_vector_free (x);
    const size_t ndim = 2;
    gsl_vector *x = gsl_vector_alloc(ndim);
    gsl_vector_set (x, 0, 0.);
    gsl_vector_set (x, 1, 0.);
    int check = 0;
    PARAMETERS p;
    p.p0 = new TTMThermalModelBQ();
    p.p1 = 333.;
    broyden(x, 2, check, p, funcTest);

    double x0 = gsl_vector_get(x, 0);
    double x1 = gsl_vector_get(x, 1);
    gsl_vector_free(x);
    qDebug() << x0 << x1;
    qDebug() << "status = " << gsl_strerror (check) << check;

    return a.exec();
}
