// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: 27 Octobre 2016
//__________________________________________________________________________
//
// TTMParameter stores the name, value and error of a variable as well as the
// "variable type" (i.e. whether it is to be fixed, constrained or fit).
// When of "fit-type", the fit parameters can be set
//

#ifndef TTMPARAMETER_H
#define TTMPARAMETER_H

#include <QObject>

class TTMParameter : public QObject
{
    Q_OBJECT

public:
    explicit TTMParameter(QObject* parent = nullptr);
    TTMParameter(QString name, double value, double error = 0.);
    ~TTMParameter() {}

    void    constrain();
    void    fit(double start, double min, double max, double step);
    void    fix(double value, double error = 0.);
    double  getError() const {return mError;}
    qint32  getFlag() const {return mFlag;}
    double  getMax() const {return mMax;}
    double  getMin() const {return mMin;}
    double  getStart() const {return mStart;}
    QString getStatus() const {return mStatus;}
    double  getStep() const {return mStep;}
    double  getValue() const {return mValue;}
    void    list();
    void    setError(double x) {mError = x;}
    void    setParameter(QString name, double value, double error = 0.);
    void    setStatus(QString x) {mStatus = x;}
    void    setTMName(QString x) {setObjectName(x);}
    void    setValue(double x) {mValue = x;}

    TTMParameter& operator=(const TTMParameter& obj);

private:
    double  mError;   // error in variable
    qint32  mFlag;    // -1:constrained ; 0:fitted ; 1:fixed ; 2:uninitialised
    double  mMax;     // upper bound of fit range
    double  mMin;     // lower bound of fit range
    QString mStatus;  // reflects qint32ended treatment or action taken
    double  mStart;   // initial value used in fit
    double  mStep;    // step size used in fit
    double  mValue;   // value of variable
};

#endif // TTMPARAMETER_H
