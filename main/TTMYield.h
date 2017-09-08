// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Novembre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMYield                                                             //
//                                                                      //
// Object for storage of experimental yields and model predictions      //
// together with the decay chain relevant to the measurement in the     //
// form of a particle set                                               //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMYIELD_H
#define TTMYIELD_H

#include "TTMParticleSet.h"

#include <QObject>

class TTMYield : public QObject
{
    Q_OBJECT
public:
    explicit TTMYield(QObject *parent = 0);
    TTMYield(QString name, double exp_val, double exp_err,
             qint32 id1, qint32 id2 = 0, bool fit = true);

    void            fit()     {mFit = true;}
    double          getExpError() const {return mExpError;}
    double          getExpValue() const {return mExpValue;}
    bool            getFit() const {return mFit;}
    qint32          getID1() const {return mID1;}
    double          getModelError() const {return mModelError;}
    double          getModelValue() const {return mModelValue;}
    qint32          getID2() const {return mID2;}
//    TTMParticleSet* getPartSet1() const {return mSet1;}
//    TTMParticleSet* getPartSet2() const {return mSet2;}
    double          getQuadDev() const {return (mModelValue - mExpValue) / mModelValue;}
    double          getStdDev() const {return (mModelValue - mExpValue) / mExpError;}
    QString         getTMName() {return objectName();}
    bool            isFitted() const {return mFit;}
    void            list();
    void            predict() {mFit = false;}
    void            setExpError(double x) {mExpError = x;}
    void            setExpValue(double x) {mExpValue = x;}
    void            setID(qint32 x, qint32 y) {mID1 = x; mID2 = y;}
    void            setModelError(double x) {mModelError = x;}
    void            setModelValue(double x) {mModelValue = x;}
//    void            setPartSet(TTMParticleSet *x, TTMParticleSet *y = (TTMParticleSet *) 0) { mSet1 = x; mSet2 = y;}
    void            setTMName(QString x) {setObjectName(x);}
//    TTMYield operator=(TTMYield obj);

private:
    double          mExpError;   // experimental error
    double          mExpValue;   // experimental value
    qint32          mID1;        // id for yield; numerator id for ratio
    qint32          mID2;        // denominator id for ratio
    bool            mFit;        // true if yield is to be fitted (predicted if false)
    double          mModelError; // model error
    double          mModelValue; // model value
//    TTMParticleSet* mSet1;       // particle set relevant to yield (numerator)
//    TTMParticleSet* mSet2;       // particle set relevant to denominator
};

#endif // TTMYIELD_H
