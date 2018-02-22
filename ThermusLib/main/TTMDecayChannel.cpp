// Author: Spencer Wheaton 1 July 2004 //
// Adapted for Qt by Yves Schutz: Octobre 2016

//__________________________________________________________________________
// Stores daughter id's as well as the branching ratio as a
// fraction
//
#include <QDebug>

#include "TTMDecayChannel.h"

//__________________________________________________________________________
TTMDecayChannel::TTMDecayChannel(QObject* parent) : QObject(parent),
    mBRatio(0.0), mDaughters(nullptr)
{
    //ctor
}

//__________________________________________________________________________
TTMDecayChannel::TTMDecayChannel(double fraction, QList<TTMIDObj *> *list, QObject* parent) :
    QObject(parent),
    mBRatio(fraction), mDaughters(list)
{
    //ctor

}

//__________________________________________________________________________
void TTMDecayChannel::list()
{
    // list all daughters

    QString text = QString("BRatio: %1 ☛ ").arg(mBRatio);

    if (mDaughters) {
        QList<TTMIDObj*>::iterator i;
        for (i = mDaughters->begin(); i != mDaughters->end(); ++i)            
            text.append(QString("%1; ").arg((*i)->getID()));
        qInfo() << text;
    } else
        qWarning("WARNING: No daughters list defined");
}

//__________________________________________________________________________
TTMDecayChannel &TTMDecayChannel::operator=(const TTMDecayChannel &obj)
{
    // assignation operator
    if (this != &obj) {
        mBRatio    = obj.getBRatio();
        mDaughters = obj.getDaughterList();
    }
    return *this;
}
