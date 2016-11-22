// Author: Spencer Wheaton 26 April 2010
// Adapted for Qt by Yves Schutz: 24 Octobre 2016

// Stores particle properties relevant to the thermal model.
// fName is determined from the Monte Carlo particle ID using the function
// Int_2_String(qint32 x). fName is the key used to find objects in ROOT hash
// tables (as in the TTMParticleSet class). It is important that the
// conversion from ID to fName is consistent with that in TTMParticleSet.
//

#include <QDebug>
#include <QFile>

#include "TTMParticle.h"

//__________________________________________________________________________
TTMParticle::TTMParticle(QObject* parent) : QObject(parent),
    mB(0), mBContent(0.0), mBeauty(0), mCharm(0), mCContent(0.0), mDeg(0),
    mID(0), mMass(0.0), mPartName(""), mQ(0), mRadius(0.0), mS(0), mSContent(0.0),
    mStable(false), mStat(0), mTContent(0.0), mThreshold(0.0), mThresholdCalc(0.0),
    mThresholdFlag(false), mTop(0), mWidth(0.0)
{
    // ctor
    // Note: the id is set through the function SetID() which sets fName too.

    mDecaySummary = new QList<TTMDecay*>;
    mDecayChannels = new QList<TTMDecayChannel*>;
}

//__________________________________________________________________________
TTMParticle::TTMParticle(const TTMParticle &obj, QObject* parent) : QObject(parent)
{
    //cpy ctor
    setID(obj.getID());
    mPartName      = obj.getPartName();
    mStable        = obj.getStable();
    mDeg           = obj.getDeg();
    mStat          = obj.getStat();
    mB             = obj.getB();
    mS             = obj.getS();
    mQ             = obj.getQ();
    mCharm         = obj.getCharm();
    mBeauty        = obj.getBeauty();
    mTop           = obj.getTop();
    mMass          = obj.getMass();
    mWidth         = obj.getWidth();
    mThreshold     = obj.getThreshold();
    mRadius        = obj.getRadius();
    mThresholdCalc = obj.getThresholdCalc();
    mThresholdFlag = obj.getThresholdFlag();
    mSContent      = obj.getSContent();
    mCContent      = obj.getCContent();
    mBContent      = obj.getBContent();
    mTContent      = obj.getTContent();

    mDecaySummary = new QList<TTMDecay*>;
    mDecayChannels = new QList<TTMDecayChannel*>;

    qDeleteAll(mDecaySummary->begin(), mDecaySummary->end());
    qDeleteAll(mDecayChannels->begin(), mDecayChannels->end());

    QList<TTMDecayChannel*>::iterator oldch;
    for (oldch = obj.getDecayChannels()->begin(); oldch != obj.getDecayChannels()->end(); ++oldch) {
        double bRatio = (*oldch)->getBRatio();
        QList<TTMIDObj*>::iterator oldid;
        QList<TTMIDObj*>* dlist = new QList<TTMIDObj*>;
        for (oldid = (*oldch)->getDaughterList()->begin(); oldid != (*oldch)->getDaughterList()->end(); ++oldid) {
            dlist->append(new TTMIDObj((*oldid)->getID(), obj.parent()));
        }
        mDecayChannels->append(new TTMDecayChannel(bRatio, dlist, obj.parent()));
    }
    updateDecaySummary();
}

//__________________________________________________________________________
TTMParticle::~TTMParticle()
{
    // returns the memory associated with fDecaySummary and fDecayChannels
    // to the heap
    //

    if(mDecaySummary){
        qDeleteAll(mDecaySummary->begin(), mDecaySummary->end()); // kills all heap-based entries
        delete mDecaySummary;              // returns memory to heap
    }

    if(mDecayChannels){

        QList<TTMDecayChannel*>::iterator ch;
        for (ch = mDecayChannels->begin(); ch != mDecayChannels->end(); ++ch) {
            qDeleteAll((*ch)->getDaughterList()->begin(), (*ch)->getDaughterList()->end());
            delete (*ch)->getDaughterList();
        }

        qDeleteAll(mDecayChannels->begin(), mDecayChannels->end()); // kills all heap-based entries
        delete mDecayChannels;              // returns memory to heap
    }
}

//__________________________________________________________________________
TTMDecay *TTMParticle::getDecay(qint32 daughter_id)
{
    // Access to decay object of parent involving daughter with id daughter_id.
    // Returns 0 if daughter_id is not in the list of daughter id's.
    //

    TTMDecay* rv = NULL;

    QList<TTMDecay*>::iterator decay;
    for (decay = mDecaySummary->begin(); decay != mDecaySummary->end(); ++decay) {
        if ((*decay)->getDaughterID() == daughter_id) {
            rv  = *decay;
            break;
        }
    }
    return rv;
}

//__________________________________________________________________________
TTMDecayChannel *TTMParticle::getDecayChannel(qint32 channel)
{
    // Provides access to the specified decay channel. Returns 0 if the
    // specified channel is not in the list.
    //
    TTMDecayChannel *rv = 0;

    for( qint32 index = 0; index <= channel; index++) {
        rv = mDecayChannels->at(index);
        if(!rv) {
            qWarning() << Q_FUNC_INFO << "WARNING: Channel #" << channel << "does not exist!";
            rv = NULL;
        }
    }
    return rv;
}

//__________________________________________________________________________
void TTMParticle::list() const
{
    // Listing for particles

    qDebug() << Q_FUNC_INFO << "INFO: ********* LISTING FOR PARTICLE " << mPartName
             << " *********";

    qDebug() << "\t ID                 = " << mID        << "    \t";
    qDebug() << "\t Deg.               = " << mDeg       << "    \t";
    qDebug() << "\t STAT               = " << mStat      << "    \t";
    qDebug() << "\t Mass               = " << mMass      << "GeV \t";
    qDebug() << "\t Width              = " << mWidth     << "GeV \t";
    qDebug() << "\t Threshold          = " << mThreshold << "GeV \t";
    if(mThresholdFlag){
        qDebug() << "\t Calculated Threshold \t= " << mThresholdCalc << "GeV \t";
    }
    qDebug() << "\t Hard sphere radius = " << mRadius    << "    \t";
    qDebug() << "\t B                  = " << mB         << "    \t";
    qDebug() << "\t S                  = " << mS         << "    \t" << "\t" << "|S| = " << mSContent;
    qDebug() << "\t Q                  = " << mQ         << "    \t";
    qDebug() << "\t Charm              = " << mCharm     << "    \t" << "\t" << "|C| = " << mCContent;
    qDebug() << "\t Beauty             = " << mBeauty    << "    \t" << "\t" << "|B| = " << mBContent;
    qDebug() << "\t Top                = " << mTop       << "    \t" << "\t" << "|T| = " << mTContent;
    if (mStable) {
        qDebug() << "\t\t STABLE";
    } else {
        qDebug() << "\t\t UNSTABLE";
        qDebug() << "\t\t Decay Channels:";

        QList<TTMDecayChannel*>::iterator nextch;
        for( nextch = mDecayChannels->begin(); nextch != mDecayChannels->end(); ++nextch)
            (*nextch)->list();
        qDebug() << "\t\t Summary of Decays: ";

        QList<TTMDecay*>::iterator d;
        for (d = mDecaySummary->begin(); d != mDecaySummary->end(); ++d) {
            qDebug() << "\t\t" << (*d)->getDaughterID() << "\t\t"
                     << (*d)->getBRatio() * 100 << "%";
        }
        qDebug() << "\t ********************************************** ";
    }
}

//__________________________________________________________________________
void TTMParticle::setDecayChannels(QString file, bool scaleBRatios)
{
    // Inputs decay channels listed in the specified file in the decay channel
    // list of the particle (the parent).
    //
    // Required file format:
    //                            BR1(%) \t d_id1(1) \t d_id2(1) ... d_idN(1)\n
    //                            BR2(%) \t d_id1(2) \t d_id2(2) ... d_idN(2)\n
    //                            etc ...
    //
    // If the file is not found, the particle is set to stable. Once the decay
    // channel list has been populated, fDecaySummary is populated with each
    // daughter appearing only once using UpdateDecaySummary.
    //

    qDeleteAll(mDecaySummary->begin(), mDecaySummary->end());
    qDeleteAll(mDecayChannels->begin(), mDecayChannels->end());

    QFile data(file);
    if (!data.open(QFile::ReadOnly)) {
        qInfo() << Q_FUNC_INFO << "INFO: Cannot open file: " << file;
        qInfo()   << "              Setting Particle " << mID << "to stable!";
        mStable = true;
    } else {
        qint32    nch = 0;
        qint32    no_daughters[500];   //# of daughters in each decay channel of parent
        double vBRatio[100];         // BR in each channel
        QList<TTMIDObj*>* ld[100];
        double totalBRatio = 0.;

        data.seek(0);
        QRegExp rx("\t");// match a tab
        while (!data.atEnd()) {
            QString line = data.readLine();
            QStringList list = line.split(rx, QString::SkipEmptyParts);
            no_daughters[nch] = list.size() - 1;
            vBRatio[nch] = list.at(0).toDouble();
            totalBRatio += list.at(0).toDouble();
            ld[nch] = new QList<TTMIDObj*>;
            for (qint32 i = 1; i < list.size(); i++) {
                TTMIDObj* id = new TTMIDObj(list.at(i).toInt());
                ld[nch]->append(id);
            }
            nch++;
        }
        data.close();
        for (qint32 ch = 0; ch < nch; ch++) {
            if (!scaleBRatios)
                vBRatio[ch] /= 100.;
            else
                vBRatio[ch] = vBRatio[ch] / 100. * 100. / totalBRatio;
            mDecayChannels->append(new TTMDecayChannel(vBRatio[ch], ld[ch]));
        }
    }
    updateDecaySummary();
}

//__________________________________________________________________________
void TTMParticle::setDecayChannels(QList<TTMDecayChannel *> *x)
{
    // Sets decay channel list and generates fDecaySummary using
    // UpdateDecaySummary
    //

    if(mDecayChannels){
        qDeleteAll(mDecayChannels->begin(), mDecayChannels->end());
        delete mDecayChannels;
    }

    mDecayChannels = x;

    updateDecaySummary();
}

//__________________________________________________________________________
void TTMParticle::setDecayChannelEfficiency(qint32 channel, double eff)
{
    // Modifies the branching ratio of the specified channel according to the
    // given efficiency (given as a percentage).
    //

    TTMDecayChannel *ch = getDecayChannel(channel);

    if(ch){
        ch->setBRatio(ch->getBRatio() * eff/100.);
        updateDecaySummary();
    }
}

//__________________________________________________________________________
void TTMParticle::setDecaySummary(QList<TTMDecay*>* x)
{
    // Sets fDecaySummary-- allows for incompatibility between channel and
    // summary lists.
    //

    if(mDecaySummary){
        qDeleteAll(mDecaySummary->begin(), mDecaySummary->end());
        delete mDecaySummary;
    }

    mDecaySummary = x;
}

//__________________________________________________________________________
void TTMParticle::setID(qint32 x)
{
    // Sets fName for retrieval from Hash Table (fName based on particle ID)

    mID = x;
    setObjectName(QString::number(x));
}

//__________________________________________________________________________
void TTMParticle::updateDecaySummary()
{
    // Updates the fDecaySummary summary list from the fDecayChannels list by
    // iterating over these channels. Uses GetDecay().
    //

    qDeleteAll(mDecaySummary->begin(), mDecaySummary->end());

    QList<TTMDecayChannel*>::iterator ch;
    for( ch = mDecayChannels->begin(); ch != mDecayChannels->end(); ++ch) {
        double BRatio = (*ch)->getBRatio();
        QList<TTMIDObj*>::iterator daughter;
        for( daughter = (*ch)->getDaughterList()->begin(); daughter != (*ch)->getDaughterList()->end(); ++daughter) {
            qint32 d_id = (*daughter)->getID();
            if (getDecay(d_id)) {   // daughter already in summary decay list of parent
                TTMDecay* decay = getDecay(d_id);
                decay->setBRatio(decay->getBRatio() + BRatio);
            } else {             // daughter not yet in summary decay list of parent
                mDecaySummary->append(new TTMDecay(mID, d_id, BRatio, parent()));
            }
        }
    }
}

//__________________________________________________________________________
TTMParticle &TTMParticle::operator=(const TTMParticle &obj)
{
    // assignation operator

    if (this == &obj)
        return *this;

    setID(obj.getID());
    mPartName      = obj.getPartName();
    mStable        = obj.getStable();
    mDeg           = obj.getDeg();
    mStat          = obj.getStat();
    mB             = obj.getB();
    mS             = obj.getS();
    mQ             = obj.getQ();
    mCharm         = obj.getCharm();
    mBeauty        = obj.getBeauty();
    mTop           = obj.getTop();
    mMass          = obj.getMass();
    mWidth         = obj.getWidth();
    mThreshold     = obj.getThreshold();
    mRadius        = obj.getRadius();
    mThresholdCalc = obj.getThresholdCalc();
    mThresholdFlag = obj.getThresholdFlag();
    mSContent      = obj.getSContent();
    mCContent      = obj.getCContent();
    mBContent      = obj.getBContent();
    mTContent      = obj.getTContent();

    qDeleteAll(mDecaySummary->begin(), mDecaySummary->end());
    qDeleteAll(mDecayChannels->begin(), mDecayChannels->end());

    QList<TTMDecayChannel*>::iterator oldch;
    for (oldch = obj.getDecayChannels()->begin(); oldch != obj.getDecayChannels()->end(); ++oldch) {
        double  BRatio = (*oldch)->getBRatio();
        QList<TTMIDObj*>::iterator oldid;
        QList<TTMIDObj*>* dlist = new QList<TTMIDObj*>;
        for (oldid = (*oldch)->getDaughterList()->begin(); oldid != (*oldch)->getDaughterList()->end(); oldid++) {
            dlist->append(new TTMIDObj((*oldid)->getID()));
        }
        mDecayChannels->append(new TTMDecayChannel(BRatio, dlist));
    }

    updateDecaySummary();

    return *this;
}
