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

    explicit TTMParameterSet(QObject *parent = nullptr);
    ~TTMParameterSet() {qDeleteAll(mPar.begin(), mPar.end()); mPar.clear();}
    TTMParameterSet(const TTMParameterSet& set);

    double           get(ParameterType type) const          { return mPar[type]->getValue(); }
    double           getB2Q() const                         { return mB2Q; }
    double           getCanRadius() const                   { return mPar[kCRADIUS]->getValue(); }
    TTMParameter*    getCanRadiusPar()                      { return mPar[kCRADIUS]; }
    QString          getConstraintInfo() const              { return mConstraintInfo; }
    double           getGammab() const                      { return mPar[kGAMMABEAUTY]->getValue(); }
    TTMParameter*    getGammabPar()                         { return mPar[kGAMMABEAUTY]; }
    double           getGammac() const                      { return mPar[kGAMMAC]->getValue(); }
    TTMParameter*    getGammacPar()                         { return mPar[kGAMMAC]; }
    double           getGammas() const                      { return mPar[kGAMMAS]->getValue(); }
    TTMParameter*    getGammasPar()                         { return mPar[kGAMMAS]; }
    double           getMuB() const                         { return mPar[kMUB]->getValue(); }
    TTMParameter*    getMuBPar()                            { return mPar[kMUB]; }
    double           getMuBeauty() const                    { return mPar[kMUBEAUTY]->getValue(); }
    TTMParameter*    getMuBeautyPar()                       { return mPar[kMUBEAUTY]; }
    double           getMuC() const                         { return mPar[kMUC]->getValue(); }
    TTMParameter*    getMuCPar()                            { return mPar[kMUC]; }
    double           getMuQ() const                         { return mPar[kMUQ]->getValue(); }
    TTMParameter*    getMuQPar()                            { return mPar[kMUQ]; }
    double           getMuS() const                         { return mPar[kMUS]->getValue(); }
    TTMParameter*    getMuSPar()                            { return mPar[kMUS]; }
    TTMParameter*    getParameter(ParameterType type) const { return mPar[type]; }
    double           getRadius() const                      { return mPar[kRADIUS]->getValue(); }
    TTMParameter*    getRadiusPar()                         { return mPar[kRADIUS]; }
    double           getT() const                           { return mPar[kT]->getValue(); }
    TTMParameter*    getTPar()                              { return mPar[kT]; }
    double           getVolume() const                      { return 4. * M_PI / 3. * qPow(getRadius(), 3); }
    static QString   name(ParameterType type);
    void             set(ParameterType type, double x)      { mPar[type]->setValue(x); }
    void             setB2Q(double x)                       { mB2Q = x; }
    void             setCanRadius(double x)                 { mPar[kCRADIUS]->setValue(x); }
    void             setConstraintInfo(QString x)           { mConstraintInfo = x; }

protected:
    double               mB2Q;             // the initial B/2Q ratio
    QList<TTMParameter*> mPar;             // Parameters objects list
    QString              mConstraintInfo;  // Constraint information
};

#endif // TTMPARAMETERSET_H
