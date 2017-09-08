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
    explicit TTMParticle(QObject* parent = 0);
    TTMParticle(TTMParticle &obj, QObject* parent);
    ~TTMParticle();

//    qint32                    getB() const {return mB;}
//    double                    getBContent() const {return mBContent;}
//    qint32                    getBeauty() const {return mBeauty;}
//    qint32                    getCharm() const {return mCharm;}
//    double                    getCContent() const {return mCContent;}
//    TTMDecay*                 getDecay(qint32 daughter_id);
//    TTMDecayChannel*          getDecayChannel(qint32 channel);
//    QList<TTMDecayChannel*>&  getDecayChannels() {return mDecayChannels;}
//    QList<TTMDecay*>&         getDecaySummary() {return mDecaySummary;}
//    qint32                    getDeg() const {return mDeg;}
//    qint32                    getID() const {return mID;}
//    double                    getMass() const {return mMass;}
//    const QString&            getPartName() const {return mPartName;}
//    qint32                    getQ() const {return mQ;}
//    double                    getRadius() const {return mRadius;}
//    qint32                    getS() const {return mS;}
//    double                    getSContent() const {return mSContent;}
//    bool                      getStable() const {return mStable;}
//    qint32                    getStat() const {return mStat;}
//    double                    getTContent() const {return mTContent;}
//    double                    getThreshold() const {return mThreshold;}
//    double                    getThresholdCalc() const {return mThresholdCalc;}
//    bool                      getThresholdFlag() const {return mThresholdFlag;}
//    qint32                    getTop() const {return mTop;}
//    double                    getWidth() const {return mWidth;}
//    void                      list() const;
//    void                      setB(qint32 x) {mB = x;}
//    void                      setBContent(double x) {mBContent = x;}
//    void                      setBeauty(qint32 x) {mB = x;}
//    void                      setCharm(qint32 x) {mCharm = x;}
//    void                      setCContent(double x) {mCContent = x;}
//    void                      setDecayChannels(QString file, bool scaleBRatios = false);
//    void                      setDecayChannels(QList<TTMDecayChannel *> &x);
//    void                      setDecayChannelEfficiency(qint32 channel, double eff);
//    void                      setDeg(qint32 x) {mDeg = x;}
//    void                      setID(qint32 x);
//    void                      setMass(double x) {mMass = x;}
//    void                      setPartName(QString x) {mPartName = x;}
//    void                      setQ(qint32 x) {mQ = x;}
//    void                      setS(qint32 x) {mS = x;}
//    void                      setSContent(double x) {mSContent = x;}
//    void                      setStable() {mStable = true;}
//    void                      setStat(qint32 x) {mStat = x;}
//    void                      setTop(qint32 x) {mTop = x;}
//    void                      setThreshold(double x) {mThreshold = x;}
//    void                      setThresholdFlag(bool x) {mThresholdFlag = x;}
//    void                      setUnstable() {mStable = false;}
//    void                      setWidth(double x) {mWidth = x;}
//    void                      setRadius(double x) {mRadius = x;}
//    void                      setTContent(double x) {mTContent = x;}
    void                      updateDecaySummary();

    TTMParticle& operator=(TTMParticle& obj);

private:
//    void                      setDecaySummary(QList<TTMDecay *> &x);
//    void                      setThresholdCalc(double x) {mThresholdCalc = x;}

    qint32                    mB;              // baryon number
    double                    mBContent;       // |b_i|= #b + #b-bar quarks
    qint32                    mBeauty;         // beauty
    qint32                    mCharm;          // charm
    double                    mCContent;       // |C_i|= #c + #c-bar quarks
    QList<TTMDecay*>          mDecaySummary;   // pointer to summary list of decays
    QList<TTMDecayChannel*>   mDecayChannels;  // pointer to list of decay channels
    qint32                    mDeg;            // spin degeneracy
    qint32                    mID;             // Monte Carlo ID
    double                    mMass;           // mass
    QString                   mPartName;       // particle name
    qint32                    mQ;              // charge
    double                    mRadius;         // hard    sphere excluded volume radius
    qint32                    mS;              // strangeness
    double                    mSContent;       // |S_i|= #s + #s-bar quarks
    bool                      mStable;         // true if stable
    qint32                    mStat;           // 0:Boltzmann +1:Fermi-Dirac -1:Bose-Einstein
    double                    mTContent;       // |T_i|= #t + #t-bar quarks
    double                    mThreshold;      // threshold
    double                    mThresholdCalc;  // calculated threshold
    bool                      mThresholdFlag;  // true if threshold has been calculated
    qint32                    mTop;            // top
    double                    mWidth;          // width
};

#endif // TTMPARTICLE_H
