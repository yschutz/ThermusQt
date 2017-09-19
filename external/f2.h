// a class to mimic basic features of root TF2
// Y. Schutz Septembre 2017
#ifndef F2_H
#define F2_H

#include "external/f1.h"

class F2 : public F1
{
public:
    F2(const QString& name, double(*fcn)(double*, double*), double xmin, double xmax,
       double ymin, double ymay, int par, QObject* parent = 0);

    double eval(double x, double y);
    double integral(double ax, double bx, double ay, double by, double epsrel = 1e-12);

protected:
    double mYmax;    // maximum y value to compute
    double mYMin;    // minimum y value to compute
};

#endif // F2_H
