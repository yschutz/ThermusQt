// Author: Spencer Wheaton 28 April 2010 //
// Adapted for Qt by Yves Schutz: 24 Octobre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMParticleSet                                                       //
//                                                                      //
// A collection of particles.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMPARTICLESET_H
#define TTMPARTICLESET_H

#include <QObject>

#include "TTMParticle.h"

class TTMParticleSet : public QObject
{
    Q_OBJECT
public:
    explicit TTMParticleSet(QObject* parent = nullptr);
    TTMParticleSet(QString file, bool CB = true, QObject *parent = nullptr); // BH 26/04/2014
    TTMParticleSet(TTMParticleSet &obj);
    ~TTMParticleSet();

    TTMParticleSet& operator=(TTMParticleSet& obj);

private:
    int                        mParticleNumber; // No. of particles in the set
};
#endif // TTMPARTICLESET_H
