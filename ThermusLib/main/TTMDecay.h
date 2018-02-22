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
    TTMDecay(int mother, int daughter, double fraction, QObject* parent = nullptr);
    TTMDecay(const TTMDecay& decay);
     ~TTMDecay() { }

    double getBRatio() const {return mBRatio;}
    int    getDaughterID() const {return mDaughterID;}
    int    getParentID() const {return mParentID;}
    void   setParentID(int a) {mParentID = a;}
    void   setDaughterID(int b) {mDaughterID = b;}
    void   setBRatio(double x) {mBRatio = x;}
    void   list();

    TTMDecay& operator=(const TTMDecay& obj);

private:
    int mParentID;             // Parent ID
    int mDaughterID;           // Daughter ID
    double mBRatio;               // Branching ratio (fraction i.e. NOT %!)
};

#endif // TTMDECAY_H
