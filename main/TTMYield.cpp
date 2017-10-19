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
    mModelError(0.), mModelValue(0.)//, mSet1(nullptr), mSet2(nullptr)

{
    // default ctor
    setObjectName("");
}

//__________________________________________________________________________
TTMYield::TTMYield(QString name, double exp_val, double exp_err, int id1, int id2, bool fit) :
    QObject(),
    mExpError(exp_err),  mExpValue(exp_val), mID1(id1), mID2(id2),mFit(fit),
    mModelError(0.), mModelValue(0.)//, mSet1(nullptr), mSet2(nullptr)
{
    // ctor
    setObjectName(name);

}

//__________________________________________________________________________
TTMYield::TTMYield(const TTMYield &yield)
{
    // copy ctor
    setObjectName(yield.objectName());
    mID1        = yield.getID1();
    mID2        = yield.getID2();
    mFit        = yield.getFit();
    mExpValue   = yield.getExpValue();
    mExpError   = yield.getExpError();
    mModelValue = yield.getModelValue();
    mModelError = yield.getModelError();
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
TTMYield &TTMYield::operator=(const TTMYield &obj)
{
    // assignation operator

    if (this != &obj) {
        setObjectName(obj.objectName());
        mID1        = obj.getID1();
        mID2        = obj.getID2();
        mFit        = obj.getFit();
        mExpValue   = obj.getExpValue();
        mExpError   = obj.getExpError();
        mModelValue = obj.getModelValue();
        mModelError = obj.getModelError();
    }
    return *this;
}
