// a class to mimic basic features of root TF1
// Y. Schutz Septembre 2017
#ifndef F1_H
#define F1_H

#include <QObject>

class F1 : public QObject
{
    Q_OBJECT
public:
//    explicit F1(QObject *parent = 0);
    F1(const QString& name, double(*fcn)(double*, double*), double xmin, double xmax, int par, QObject* parent = 0);
    ~F1();

    double eval(double x);
    double integral(double a, double b, double epsrel = 1e-12);
    void   setParameter(int npar, double value) { mPar[npar] = value; }
    void   setParameters(double p0, double p1,
                         double p2  = 0,
                         double p3  = 0,
                         double p4  = 0,
                         double p5  = 0,
                         double p6  = 0,
                         double p7  = 0,
                         double p8  = 0,
                         double p9  = 0,
                         double p10 = 0);

protected:
    double(*mFunction)(double*, double*);  // pointer to the function
    double  mXMax;                         // maximum x value to compute
    double  mXMin;                         // minimum x value to compute
    int     mNPar;                         // number of parameters
    double* mPar;                          // array of parameters
};

#endif // F1_H
