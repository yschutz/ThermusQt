// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Octobre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMDecay                                                             //
//                                                                      //
// Stores parent & daughter id's and Branching ratio in an object for   //
// storage in a ROOT container class                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMDECAY_H
#define TTMDECAY_H

#include <QObject>

class TTMDecay : public QObject
{
    Q_OBJECT
public:
    explicit TTMDecay(QObject* parent = nullptr);
    TTMDecay(qint32 mother, qint32 daughter, double fraction, QObject* parent = nullptr);
     ~TTMDecay() { }

    double getBRatio() const {return mBRatio;}
    qint32 getDaughterID() const {return mDaughterID;}
    qint32 getParentID() const {return mParentID;}
    void   setParentID(qint32 a) {mParentID = a;}
    void   setDaughterID(qint32 b) {mDaughterID = b;}
    void   setBRatio(double x) {mBRatio = x;}
    void   list();

    TTMDecay& operator=(const TTMDecay& obj);

private:
    qint32 mParentID;             // Parent ID
    qint32 mDaughterID;           // Daughter ID
    double mBRatio;               // Branching ratio (fraction i.e. NOT %!)
};

#endif // TTMDECAY_H
