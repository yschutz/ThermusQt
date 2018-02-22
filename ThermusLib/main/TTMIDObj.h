// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz:  Octobre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMIDObj                                                             //
//                                                                      //
// Object containing ID for storage in container class.                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMIDOBJ_H
#define TTMIDOBJ_H

#include <QObject>

class TTMIDObj : public QObject
{
    Q_OBJECT
public:
    explicit TTMIDObj(QObject* parent = nullptr);
    TTMIDObj(int x, QObject* parent = nullptr);
    ~TTMIDObj() { }

    int    getID() const           {return mID;}
    void   setID(int val)          {mID = val;}

    TTMIDObj& operator=(const TTMIDObj& obj);

    int    mID;                // particle ID
};

#endif // TTMIDOBJ_H
