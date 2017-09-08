// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: August 2017
//__________________________________________________________________________
// Density object for storage in ROOT hash table. Inherits from TNamed with
// fName determined from the particle ID.

#include <QStandardItem>

#include "TTMDensobj.h"

//__________________________________________________________________________
TTMDensObj::TTMDensObj(QObject *parent) : QObject(parent),
   mDecayDensity(0.0), mDensity(0.0), mEnergy(0.0), mEntropy(0.0), mID(0), mPressure(0.0)
{
    // ctor
}

//__________________________________________________________________________
TTMDensObj::TTMDensObj(int pdg, QObject *parent) : QObject(parent),
    mDecayDensity(pdg), mDensity(0.0), mEnergy(0.0), mEntropy(0.0), mPressure(0.0)
{
    // ctor
    setID(pdg);
}

//__________________________________________________________________________
QList<QStandardItem *> TTMDensObj::list() const
{
    // makes one row for a view in a table of the data memmbers
    QList<QStandardItem*> rv;
    rv << new QStandardItem(QString::number(mID)) <<
          new QStandardItem(QString::number(mDensity)) <<
          new QStandardItem(QString::number(mDecayDensity)) <<
          new QStandardItem(QString::number(mEnergy)) <<
          new QStandardItem(QString::number(mEntropy)) <<
          new QStandardItem(QString::number(mPressure));
    return rv;
}

//__________________________________________________________________________
QStringList TTMDensObj::members() const
{
    // returns the list of data members
    QStringList rv;
    rv << "Particle ID" <<
          "Primary Density" <<
          "Decay Density" <<
          "Primary Energy" <<
          "Primary Entropy" <<
          "Primary Pressure";
    return rv;
}

//__________________________________________________________________________
void TTMDensObj::setID(int pdg)
{
    // Sets density ID and determines fName (required for retrieval of
     // objects from a ROOT hash table) based on the ID.

    mID = pdg;
    setObjectName(QString::number(pdg));
}

//__________________________________________________________________________
TTMDensObj &TTMDensObj::operator=(const TTMDensObj &obj)
{
    // assign operator
    if (this == &obj)
        return *this;

    setID(obj.getID());
    mEnergy       = obj.getPrimaryEnergy();
    mEntropy      = obj.getPrimaryEntropy();
    mPressure     = obj.getPrimaryPressure();
    mDensity      = obj.getPrimaryDensity();
    mDecayDensity = obj.getDecayDensity();
    return *this;
}
