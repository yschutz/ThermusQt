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
    explicit TTMParameterSet(QObject *parent = 0);
    ~TTMParameterSet() {}

    QString          getConstraintInfo() const {return mConstraintInfo;}
    TTMParameter*    getParameter(qint32 i) const {return &(mPar[i]);}
    virtual double   getRadius() const = 0 ;
    double           getVolume() const {return 4. * M_PI / 3. * qPow(getRadius(), 3);}
    void             setConstraintInfo(QString x) {mConstraintInfo = x;}

protected:
    TTMParameter* mPar;             // Pointer to first Parameter Object
    QString       mConstraintInfo;  // Constraqint32 information
};

#endif // TTMPARAMETERSET_H
