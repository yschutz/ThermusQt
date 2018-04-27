#include "plot.h"
#include <QApplication>
#include <QVector>
#include <QVector3D>
#include <QtCore/qmath.h>

#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int nDimX = 50, nDimY = 50;
    double startX = -8.0, startY = -8.0;
    double endX   =  8.0, endY   =  8.0;
    double stepX = (endX - startX) / double(nDimX - 1.0);
    double stepY = (endY - startY) / double(nDimY - 1.0);

    Plot pl3("This is a test of a 3D Graph", startX, endX, nDimX, startY, endY, nDimY, 0, 2);

    for (int iy = 0; iy < nDimY; iy++) {
        double y = qMin(endY, (iy * stepY + startY));
        for (int ix = 0; ix < nDimX; ix++) {
            double x = qMin(endX, (ix * stepX + startX));
            double R = qSqrt(y *y + x * x) + 0.01;
            double z = (qSin(R) / R + 0.24) * 1.61;
            pl3.fill(x, y, z);
        }
    }
    pl3.setAxisTitle("testX", "testY", "testZ");
    pl3.draw();
//    pl3.proX();
//    pl3.proY();
//    pl3.contour();

    startX = 0.;
    endX   = 250.;
    nDimX = 251;
    stepX = (endX - startX) / double(nDimX - 1.0);
    Plot pl1("This is a test of a 1D Graph", startX, endX, nDimX);
    pl1.setAxisTitle("testX", "testY", "testZ");
    for (int ix = 0; ix < nDimX; ix++) {
        double x = qMin(endX, (ix * stepX + startX));
        double y = qExp(-x/150.0)*qCos(x/10.0);
        pl1.fill(x, y);
    }
    pl1.setAxisTitle("testX", "testY", "testZ");
    pl1.draw();

//    QVector<double> x( {2.91204, 2.91973, 2.92742, 2.93512, 2.94281, 2.95051, 2.9582, 2.9659,
//                        2.97359, 2.98129, 2.98898, 2.99668, 3.00437, 3.01207, 3.01977, 3.02746,
//                        3.03516, 3.05055, 3.06594, 3.08133, 3.09673, 3.11212, 3.12751, 3.1429,
//                        3.1583, 3.16137, 3.15367, 3.14596, 3.13826, 3.13055, 3.12284, 3.11514,
//                        3.10743, 3.09973, 3.09202, 3.08431, 3.07661, 3.0689, 3.06119, 3.05349,
//                        3.04578, 3.03807, 3.02266, 3.00724, 2.99183, 2.97642, 2.961, 2.94559,
//                        2.93017, 2.91476, 2.91204} );
//    QVector<double> y( {0.160625, 0.160301, 0.160085, 0.159888, 0.159702, 0.159508, 0.159351,
//                        0.15917, 0.159019, 0.158847, 0.158701, 0.158536, 0.158395, 0.158235,
//                        0.158099, 0.157945, 0.157812, 0.157535, 0.157268, 0.15701, 0.156763,
//                        0.156529, 0.156312, 0.15612, 0.156, 0.156065, 0.156359, 0.156561,
//                        0.156745, 0.15692, 0.157102, 0.157252, 0.157423, 0.15757, 0.157733,
//                        0.157878, 0.158036, 0.158178, 0.158332, 0.158471, 0.158621, 0.158758,
//                        0.159039, 0.159314, 0.159582, 0.159842, 0.160092, 0.160329, 0.160541,
//                        0.160678, 0.160625} );
//    startX = *std::min_element(x.begin(), x.end()) * 0.8 - 1.;
//    endX   = *std::max_element(x.begin(), x.end()) * 1.2 + 1.;
//    startY = *std::min_element(y.begin(), y.end()) * 0.8 - 1.;
//    endY   = *std::max_element(y.begin(), y.end()) * 0.1 + 1.;

// qDebug() << startX << endX << startY << endY;
//    Plot pl4("This is a test of a graph", nDimX, 2, 5, 0.12, 0.25);

//    QVector<double> xx, yy;
//    for (int ix = 0; ix < nDimX; ix++) {
//            xx.append(x[ix] * 1.2);
//            yy.append(y[ix] * 1.2);
//        }
//    pl4.addGraph(x, y);
//    pl4.addGraph(xx, yy);
//    pl4.draw();

    return a.exec();
}
