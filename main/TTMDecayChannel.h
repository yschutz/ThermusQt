// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Octobre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMDecayChannel                                                      //
//                                                                      //
// Stores daughter id's and Branching ratio in an object for            //
// storage in a ROOT container class                                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMDECAYCHANNEL_H
#define TTMDECAYCHANNEL_H

#include <QObject>

#include "TTMIDObj.h"

class TTMDecayChannel : public QObject
{
    Q_OBJECT
public:
    explicit TTMDecayChannel(QObject* parent = nullptr);
    TTMDecayChannel(double fraction, QList<TTMIDObj*>* list, QObject* parent = nullptr);
     ~TTMDecayChannel() { }

     double            getBRatio() const {return mBRatio;}
     QList<TTMIDObj*>* getDaughterList() const {return mDaughters;}
     void              list();
     void              setBRatio(double x) {mBRatio = x;}
     void              setDaughterList(QList<TTMIDObj*>* x) {mDaughters = x;}

     TTMDecayChannel& operator=(const TTMDecayChannel& obj);

private:
    double            mBRatio;            // Branching ratio (fraction i.e. NOT %!)
    QList<TTMIDObj*>* mDaughters;           // list of daughter id's
};

#endif // TTMDECAYCHANNEL_H
