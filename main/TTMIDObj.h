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
    explicit TTMIDObj(QObject* parent = 0);
    TTMIDObj(qint32 x, QObject* parent = 0);
    ~TTMIDObj() { }

    qint32  getID() const {return mID;}
    void setID(qint32 x) {mID = x;}


    TTMIDObj& operator=(const TTMIDObj& obj);

  qint32 mID;                   // particle ID

};

#endif // TTMIDOBJ_H
