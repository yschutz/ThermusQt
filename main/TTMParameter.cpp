// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: 27 Octobre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMParameter                                                         //
//                                                                      //
// A Parameter Object                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <QDebug>

#include "TTMParameter.h"

//__________________________________________________________________________
TTMParameter::TTMParameter(QObject *parent) : QObject(parent)
{
    // default ctor
     setObjectName("unnamed");
     mValue = 0.;
     mError = 0.;
     mFlag = 2;
     mStatus = "(UNINITIALISED)";
     mStart = 0.;
     mMin = 0.;
     mMax = 0.;
     mStep = 0.;
}

//__________________________________________________________________________
TTMParameter::TTMParameter(QString name, double value, double error)
{
    // Simple constructor: sets the name, value and error of a variable,
     // making it of "FIXED" type. Use Constrain() or Fit(...) to change
     // its type

     setParameter(name, value, error);
}

//__________________________________________________________________________
void TTMParameter::constrain()
{
    // Changes the variable type to "CONSTRAIN" and sets error to 0.

     mFlag = -1;
     mStatus = "(to be CONSTRAINED)";
     mError = 0.;
}

//__________________________________________________________________________
void TTMParameter::fit(double start, double min, double max, double step)
{
    // Changes the variable type to "FIT", sets error to 0 and sets
    // the fit parameters:
    //            start: initial value for fit
    //            min & max : bounds for fit
    //            step    : step size

    mFlag = 0;
    mStatus = "(to be FITTED)";
    mStart = start;
    mMin = min;
    mMax = max;
    mStep = step;
    mValue = start;
    mError = 0.;
}

//__________________________________________________________________________
void TTMParameter::fix(double value, double error)
{
    // Changes the variable type to "FIXED" and sets its value and error.

     mFlag = 1;
     mStatus = "(FIXED)";
     mValue = value;
     mError = error;
}

//__________________________________________________________________________
void TTMParameter::list()
{
    // Outputs the variable's properties

    if (mError != 0.)
        qDebug() << Q_FUNC_INFO << "INFO: " << objectName() << mValue << "+-" << mError;
    else
        qDebug() << Q_FUNC_INFO << "INFO: " << objectName() << mValue;

    if (mFlag == 0) {
      qDebug() << mStatus;
      qDebug() << "      " << " start: " << mStart;
      qDebug() << "      " << " range: " << mMin << " -- " << mMax;
      qDebug() << "      " << " step:  " << mStep;
    } else {
      qDebug() << mStatus;
    }
}

//__________________________________________________________________________
void TTMParameter::setParameter(QString name, double value, double error)
{
    // Sets the name, value and error of a variable, making it of
    // "FIXED" type. Use Constrain() or Fit(...) to change its
    // type

    setObjectName(name);
    mValue = value;
    mError = error;
    mFlag = 1;

    mStatus = "(FIXED)";

    mStart = 0.;
    mMin = 0.;
    mMax = 0.;
    mStep = 0.;
}

//__________________________________________________________________________
TTMParameter &TTMParameter::operator=(const TTMParameter &obj)
{
    // assignation operator

    if (this == &obj) return *this;

     setObjectName(obj.objectName());
     mValue  = obj.getValue();
     mError  = obj.getError();
     mFlag   = obj.getFlag();
     mStatus = obj.getStatus();
     mStart  = obj.getStart();
     mMin    = obj.getMin();
     mMax    = obj.getMax();
     mStep   = obj.getStep();

     return *this;
}
