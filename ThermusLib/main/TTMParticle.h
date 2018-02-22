// Author: Spencer Wheaton 26 April 2010
// Adapted for Qt by Yves Schutz: 24 Octobre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMParticle                                                          //
//                                                                      //
// Object containing particle properties relevant to thermal model.     //
// Inherits from TNamed to allow TTMParticle objects to be stored in    //
// ROOT hash tables. The particle's Monte Carlo ID is used to set       //
// fName.                                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMPARTICLE_H
#define TTMPARTICLE_H

#include <QObject>

class TTMParticle : public QObject
{
    Q_OBJECT

public:
    explicit TTMParticle(QObject* parent = nullptr);
    TTMParticle(QString name, int pdg, int spin, int statistic, double mass,
                int strange, int baryon, int charge, int charm, int beauty, int top,
                int sContent, int cContent, int bContent, int tContent,
                double width, double lifetime, double radius, double threshold, int ndecay,
                QObject* parent = nullptr);

    TTMParticle(TTMParticle &obj, QObject* parent);
    ~TTMParticle(){;}

    int                       getBaryon() const                              { return mBaryon; }
    int                       getBContent() const                            { return mBC; }
    int                       getBeauty() const                              { return mB; }
    int                       getCContent() const                            { return mCC; }
    double                    getCharge() const                              { return mCharge; }
    int                       getCharm() const                               { return mC; }
    double                    getLifetime() const                            { return mLifetime; }
    double                    getMass() const                                { return mMass; }
    const QString&            getName() const                                { return mName; }
    int                       getPDG() const                                 { return mPDG; }
    double                    getRadius() const                              { return mRadius; }
    int                       getS() const                                   { return mS; }
    int                       getSContent() const                            { return mSC; }
    int                       getSpin() const                                { return mSpin; }
    int                       getStat() const                                { return mStatistic;}
    int                       getTContent() const                            { return mTC; }
    double                    getThreshold() const                           { return mThreshold; }
    int                       getTop() const                                 { return mT; }
    double                    getWidth() const                               { return mWidth; }
    bool                      isStable() const                               { return mNdecay == 0 ? true : false; }
    void                      list() const;

    TTMParticle& operator=(TTMParticle& obj);

private:
    QString                   mName;           // particle name
    int                       mPDG;            // PDG ID
    int                       mSpin;           // spin degeneracy
    int                       mStatistic;      // 0:Boltzmann +1:Fermi-Dirac -1:Bose-Einstein
    double                    mMass;           // mass
    int                       mS;              // strangeness
    int                       mBaryon;         // baryon number
    double                    mCharge;         // charge
    int                       mC;              // charm
    int                       mB;              // |b_i|= #b + #b-bar quarks
    int                       mT;              // |T_i|= #t + #t-bar quarks
    int                       mSC;             // |S_i|= #s + #s-bar quarks
    int                       mCC      ;       // |C_i|= #c + #c-bar quarks
    int                       mBC;             // |S_i|= #s + #s-bar quarks
    int                       mTC;             // |S_i|= #s + #s-bar quarks
    double                    mWidth;          // width
    double                    mLifetime;       // lifetime
    double                    mRadius;         // hard    sphere excluded volume radius
    double                    mThreshold;      // threshold
    int                       mNdecay;         // number of decays
};

#endif // TTMPARTICLE_H
