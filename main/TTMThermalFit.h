// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Novembre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMThermalFit                                                        //
//                                                                      //
// Base class for thermal fit objects.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#ifndef TTMTHERMALFIT_H
#define TTMTHERMALFIT_H

//#include "TTMParticleSet.h"
#include "TTMYield.h"
#include "external/QMinuit.h"

#include <QObject>

class TTMThermalFit : public QObject
{
    Q_OBJECT
public:
    explicit TTMThermalFit(QObject *parent = 0);
    ~TTMThermalFit();

    void      addYield(TTMYield *yield);
    void      generateYields();
    QString   getTMName(int id1, int id2, const QString& descr) const;
    TTMYield *getYield(int id1, int id2, const QString& descr) const;
    void      inputExpYields(const QString& fileName);
    void      listMinuitInfo() const;
    void      listYields() const;
    void      removeYield(int id1, int id2, const QString& descr);

protected:

//     TTMParticleSet*  mPartSet;   // pointer to BASE Particle Set
     double           mChiSquare;   // chi-squared
     QString          mDescriptor;  // string describing fit
     QMinuit*         mMinuit;      // pointer to QMinuit obj
     double           mQuadDev;     // quadratic deviation
     QList<TTMYield*> mYields;      // Container for yields of interest
};

#endif // TTMTHERMALFIT_H
