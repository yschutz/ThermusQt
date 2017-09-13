// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: November 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMParameterSet                                                      //
//                                                                      //
// Base class for parameter set objects. All derived classes must       //
// define Double_t GetRadius().                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


#ifndef TTMPARAMETERSET_H
#define TTMPARAMETERSET_H

#include <QtMath>
#include <QObject>

#include "TTMParameter.h"

class TTMParameterSet : public QObject
{
    Q_OBJECT

public:
    enum ParameterType {kT, kMUB, kMUQ, kGAMMAS, kCRADIUS, kRADIUS, kGAMMAC, kGAMMABEAUTY,
                        kMUS, kMUC, kMUBEAUTY, kPARTYPES};

    explicit TTMParameterSet(QObject *parent = 0);
    ~TTMParameterSet() {qDeleteAll(mPar.begin(), mPar.end()); mPar.clear();}

    virtual double   getB2Q() const = 0;
    QString          getConstraintInfo() const {return mConstraintInfo;}
    TTMParameter*    getParameter(ParameterType type) const {return mPar[type];}
    virtual double   getRadius() const = 0 ;
    double           getVolume() const {return 4. * M_PI / 3. * qPow(getRadius(), 3);}
    static QString   name(ParameterType type);
    void             setConstraintInfo(QString x) {mConstraintInfo = x;}

protected:
    QList<TTMParameter*> mPar;             // Parameters objects list
    QString              mConstraintInfo;  // Constraint information
};

#endif // TTMPARAMETERSET_H
