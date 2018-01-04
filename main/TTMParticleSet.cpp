// Author: Spencer Wheaton 28 April 2010 //
// Adapted for Qt by Yves Schutz: 24 Octobre 2016
//__________________________________________________________________________
// A collection of particles. A hash table is used for quick access to
// entries. The entries each have a unique fName (based on their id's).
// When searching for entries, the id must first be converted into a
// TString in exactly the same way as the TTMParticle fNames are set.
// The function Int_2_String(int) is used for this.
//

#include "TTMDecayChannel.h"
#include "TTMParticleSet.h"

#include "external/particlesdbmanager.h"

#include <QDebug>
#include <QFile>
//__________________________________________________________________________
TTMParticleSet::TTMParticleSet(QObject *parent) : QObject(parent)
{
    // ctor
    mParticleNumber = 0;
}

//__________________________________________________________________________
TTMParticleSet::TTMParticleSet(QString /*file*/, bool /*CB*/, QObject *parent) : QObject(parent)
{
    // Populates the hash table with particles listed in the specified file.
    // This file lists only PARTICLES. If a particle listed in the file has
    // an anti-particle (i.e. if it has any non-zero quantum numbers), then
    // the anti-particle is automatically entered. In this way, particle and
    // anti-particle are treated symmetrically.
    //
    // With CB set to true, the required file format is as follows:
    // File format: (tab separated)
    //
    // Stable Name ID Deg. Stat. Mass S  B  Q  C  B  |S| Width Threshold (*) \n
    // \n
    // Stable Name ID Deg. Stat. Mass S  B  Q  C  B  |S| Width Threshold (*) \n
    // \n
    // etc ...
    //
    // With CB set to false, the file format is as above but without the
    // C and B columns
    //
    // If the width is non-zero, the decay products in the channel which
    // determines the threshold is listed (*).
    //
    // ===> all is now in DB (check ThermusParicles.py for building the DB)

    mParticleNumber = ParticlesDBManager::instance().size();
}

//__________________________________________________________________________
TTMParticleSet::TTMParticleSet(TTMParticleSet& obj) : QObject(obj.parent())
{
    // cpy ctor
    mParticleNumber = 0;
}

//__________________________________________________________________________
TTMParticleSet::~TTMParticleSet()
{
    // Return memory to heap
    mParticleNumber = 0;
}


//__________________________________________________________________________
TTMParticleSet &TTMParticleSet::operator=(TTMParticleSet& obj)
{
    // assignation operator
    mParticleNumber = obj.mParticleNumber;
    return *this;
}

