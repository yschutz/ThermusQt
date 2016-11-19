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

#include "TTMParticleSet.h"
#include "TTMYield.h"
#include "external/QMinuit.h"

#include <QObject>

class TTMThermalFit : public QObject
{
    Q_OBJECT
public:
    explicit TTMThermalFit(QObject *parent = 0);


protected:
     TTMParticleSet*  mPartSet;   // pointer to BASE Particle Set
     QList<TTMYield*> mYields;    // Container for yields of interest
     double           mChiSquare; // chi-squared
     double           mQuadDev;   // quadratic deviation
     QMinuit*         mMinuit;    // pointer to QMinuit obj
};

#endif // TTMTHERMALFIT_H
