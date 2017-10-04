// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: August 2017
//////////////////////////////////////////////////////////////////////////
//                                                                   	//
// TTMDensObj                                          			        //
//                                                                  	//
// Object containing densities for storage in container class. 		    //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////

#ifndef TTMDENSOBJ_H
#define TTMDENSOBJ_H

#include <QObject>

class QStandardItem;

class TTMDensObj : public QObject
{
    Q_OBJECT
public:
    explicit TTMDensObj(QObject* parent = nullptr);
    TTMDensObj(int pdg, QObject* parent = nullptr);
    TTMDensObj(const TTMDensObj& dens);

    double                getDecayDensity() const    { return mDecayDensity; }
    double                getFinalDensity() const    { return mDensity + mDecayDensity; }
    int                   getID() const              { return mID; }
    double                getPrimaryDensity() const  { return mDensity; }
    double                getPrimaryEnergy() const   { return mEnergy; }
    double                getPrimaryEntropy() const  { return mEntropy; }
    double                getPrimaryPressure() const { return mPressure; }
    QList<QStandardItem*> list() const;
    QStringList           members() const ;
    void                  setDecayDensity(double value)     { mDecayDensity = value; }
    void                  setID(int pdg);
    void                  setPrimaryDensity(double value)   { mDensity = value; }
    void                  setPrimaryEnergy(double value)    { mEnergy = value; }
    void                  setPrimaryEntropy(double value)   { mEntropy = value; }
    void                  setPrimaryPressure(double value)  { mPressure = value; }

    TTMDensObj& operator=(const TTMDensObj& obj);


private:
    double mDecayDensity;      // decay density contribution
    double mDensity;           // primordial particle density
    double mEnergy;            // primordial energy density
    double mEntropy;           // primordial entropy density
    int    mID;                // particle ID
    double mPressure;          // primordial pressure
};

#endif // TTMDENSOBJ_H
