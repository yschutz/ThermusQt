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
#include "main/TTMDecay.h"
#include "main/TTMDecayChannel.h"


class TTMParticle : public QObject
{
    Q_OBJECT

    friend class TTMParticleSet;

public:
    explicit TTMParticle(QObject* parent = nullptr);
    TTMParticle(TTMParticle &obj, QObject* parent);
    ~TTMParticle();

//    int                       getB() const {return mB;}
//    double                    getBContent() const {return mBContent;}
//    int                       getBeauty() const {return mBeauty;}
//    int                       getCharm() const {return mCharm;}
//    double                    getCContent() const {return mCContent;}
//    TTMDecay*                 getDecay(int daughter_id);
//    TTMDecayChannel*          getDecayChannel(int channel);
//    QList<TTMDecayChannel*>&  getDecayChannels() {return mDecayChannels;}
//    QList<TTMDecay*>&         getDecaySummary() {return mDecaySummary;}
//    int                       getDeg() const {return mDeg;}
//    int                       getID() const {return mID;}
//    double                    getMass() const {return mMass;}
//    const QString&            getPartName() const {return mPartName;}
//    int                       getQ() const {return mQ;}
//    double                    getRadius() const {return mRadius;}
//    int                       getS() const {return mS;}
//    double                    getSContent() const {return mSContent;}
//    bool                      getStable() const {return mStable;}
//    int                       getStat() const {return mStat;}
//    double                    getTContent() const {return mTContent;}
//    double                    getThreshold() const {return mThreshold;}
//    double                    getThresholdCalc() const {return mThresholdCalc;}
//    bool                      getThresholdFlag() const {return mThresholdFlag;}
//    int                       getTop() const {return mTop;}
//    double                    getWidth() const {return mWidth;}
//    void                      list() const;
//    void                      setB(int x) {mB = x;}
//    void                      setBContent(double x) {mBContent = x;}
//    void                      setBeauty(int x) {mB = x;}
//    void                      setCharm(int x) {mCharm = x;}
//    void                      setCContent(double x) {mCContent = x;}
//    void                      setDecayChannels(QString file, bool scaleBRatios = false);
//    void                      setDecayChannels(QList<TTMDecayChannel *> &x);
//    void                      setDecayChannelEfficiency(int channel, double eff);
//    void                      setDeg(int x) {mDeg = x;}
//    void                      setID(int x);
//    void                      setMass(double x) {mMass = x;}
//    void                      setPartName(QString x) {mPartName = x;}
//    void                      setQ(int x) {mQ = x;}
//    void                      setS(int x) {mS = x;}
//    void                      setSContent(double x) {mSContent = x;}
//    void                      setStable() {mStable = true;}
//    void                      setStat(int x) {mStat = x;}
//    void                      setTop(int x) {mTop = x;}
//    void                      setThreshold(double x) {mThreshold = x;}
//    void                      setThresholdFlag(bool x) {mThresholdFlag = x;}
//    void                      setUnstable() {mStable = false;}
//    void                      setWidth(double x) {mWidth = x;}
//    void                      setRadius(double x) {mRadius = x;}
//    void                      setTContent(double x) {mTContent = x;}
    void                        updateDecaySummary();

    TTMParticle& operator=(TTMParticle& obj);

private:
//    void                      setDecaySummary(QList<TTMDecay *> &x);
//    void                      setThresholdCalc(double x) {mThresholdCalc = x;}

    int                       mB;              // baryon number
    double                    mBContent;       // |b_i|= #b + #b-bar quarks
    int                       mBeauty;         // beauty
    int                       mCharm;          // charm
    double                    mCContent;       // |C_i|= #c + #c-bar quarks
    QList<TTMDecay*>          mDecaySummary;   // pointer to summary list of decays
    QList<TTMDecayChannel*>   mDecayChannels;  // pointer to list of decay channels
    int                       mDeg;            // spin degeneracy
    int                       mID;             // Monte Carlo ID
    double                    mMass;           // mass
    QString                   mPartName;       // particle name
    int                       mQ;              // charge
    double                    mRadius;         // hard    sphere excluded volume radius
    int                       mS;              // strangeness
    double                    mSContent;       // |S_i|= #s + #s-bar quarks
    bool                      mStable;         // true if stable
    int                       mStat;           // 0:Boltzmann +1:Fermi-Dirac -1:Bose-Einstein
    double                    mTContent;       // |T_i|= #t + #t-bar quarks
    double                    mThreshold;      // threshold
    double                    mThresholdCalc;  // calculated threshold
    bool                      mThresholdFlag;  // true if threshold has been calculated
    int                       mTop;            // top
    double                    mWidth;          // width
};

#endif // TTMPARTICLE_H
