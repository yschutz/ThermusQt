// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Novembre 2016
//__________________________________________________________________________
// Object for storage of experimental yields and model predictions.
//

#include <QDebug>

#include "TTMYield.h"
//__________________________________________________________________________
TTMYield::TTMYield(QObject *parent) : QObject(parent),
    mExpError(0.), mExpValue(0.), mID1(0), mID2(0),
    mModelError(0.), mModelValue(0.), mSet1(NULL), mSet2(NULL)

{
    // default ctor
    setObjectName("");
}

//__________________________________________________________________________
TTMYield::TTMYield(QString name, double exp_val, double exp_err, qint32 id1, qint32 id2, bool fit) :
    QObject(),
    mExpError(exp_err),  mExpValue(exp_val), mID1(id1), mID2(id2),mFit(fit),
    mModelError(0.), mModelValue(0.), mSet1(NULL), mSet2(NULL)
{
    // ctor
    setObjectName(name);

}

//__________________________________________________________________________
void TTMYield::list()
{
    // list the result
    qInfo() << objectName() << ":";
    if(mFit)
        qInfo() << "\t\t\t FIT YIELD   ";
    else
        qInfo() << "\t\t\t PREDICTED YIELD   ";
    qInfo() << "\t\t\t Experiment: " << mExpValue << "\t +- " << mExpError;

    if (mModelValue) {
       qInfo() << "\t\t\t" << "Model:      " << mModelValue << "\t +- " << mModelError;
       qInfo() << "\t\t\t" << "Std.Dev.:   " << getStdDev() << "  Quad.Dev.: " << getQuadDev();
    }
}

//__________________________________________________________________________
//TTMYield TTMYield::operator=(TTMYield obj)
//{
//    //assignation operator
//    setObjectName(obj.objectName());
//    mID1        = obj.getID1();
//    mID2        = obj.getID2();
//    mFit        = obj.getFit();
//    mSet1       = obj.getPartSet1();
//    mSet2       = obj.getPartSet2();
//    mExpValue   = obj.getExpValue();
//    mExpError   = obj.getExpError();
//    mModelValue = obj.getModelValue();
//    mModelError = obj.getModelError();
//    return *this;
//}
