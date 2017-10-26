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

//#include <QHash>
#include <QObject>

#include "main/TTMParticle.h"

class TTMParticleSet : public QObject
{
    Q_OBJECT
public:
    explicit TTMParticleSet(QObject* parent = nullptr);
    TTMParticleSet(QString file, bool CB = true, QObject *parent = nullptr); // BH 26/04/2014
//    TTMParticleSet(TDatabasePDG *pdg);
    TTMParticleSet(TTMParticleSet &obj);
    ~TTMParticleSet();

//    void                          addParticle(TTMParticle* part);
//    void                          calculateThresholds();
//    void                          generateBRatios();
//    QString&                      getFilename() {return mFilename;}
//    void                          getParents(QList<TTMDecay *> *parents, int id) const;
//    TTMParticle*                  getParticle(int id) const;
//    int                        getParticleNumber() const {return mParticleNumber;}
//    QHash<QString, TTMParticle*>* getParticleTable() const {return mPartTable;}
//    void                          inputDecays(QString dir, bool scaleBRatios = false);
//    void                        inputDecays(TDatabasePDG *pdg);
//    void                          listParents(int id) const;
//    void                          listParticle(int id) const;
//    void                          listParticles(bool full = true) const;
//    void                          listStableParticles();
//    void                          massCut(double max);
//    void                          removeParticle(int id);
//    void                          setDecayEfficiency(int p_id, int d_id, double eff);
//    void                          setRadii(double radius); // BH 24/04/2014

    TTMParticleSet& operator=(TTMParticleSet& obj);

private:
//    void generateBRatios(TTMParticle* parent);
//    void calculateThreshold(TTMParticle* part);

//    QString                       mFilename;       // Input file
    int                        mParticleNumber; // No. of particles in the set
//    QHash<QString, TTMParticle*>* mPartTable;      // hash table of TTMParticle objects
};
#endif // TTMPARTICLESET_H
