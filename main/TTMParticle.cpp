// Author: Spencer Wheaton 26 April 2010
// Adapted for Qt by Yves Schutz: 24 Octobre 2016

// Stores particle properties relevant to the thermal model.
// fName is determined from the Monte Carlo particle ID using the function
// Int_2_String(int x). fName is the key used to find objects in ROOT hash
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

//    mDecaySummary = new QList<TTMDecay*>;
//    mDecayChannels = new QList<TTMDecayChannel*>;
}

//__________________________________________________________________________
TTMParticle::TTMParticle(TTMParticle &/*obj*/, QObject* parent) : QObject(parent)
{
    //cpy ctor
//    setID(obj.getID());
//    mPartName      = obj.getPartName();
//    mStable        = obj.getStable();
//    mDeg           = obj.getDeg();
//    mStat          = obj.getStat();
//    mB             = obj.getB();
//    mS             = obj.getS();
//    mQ             = obj.getQ();
//    mCharm         = obj.getCharm();
//    mBeauty        = obj.getBeauty();
//    mTop           = obj.getTop();
//    mMass          = obj.getMass();
//    mWidth         = obj.getWidth();
//    mThreshold     = obj.getThreshold();
//    mRadius        = obj.getRadius();
//    mThresholdCalc = obj.getThresholdCalc();
//    mThresholdFlag = obj.getThresholdFlag();
//    mSContent      = obj.getSContent();
//    mCContent      = obj.getCContent();
//    mBContent      = obj.getBContent();
//    mTContent      = obj.getTContent();

//    mDecaySummary = new QList<TTMDecay*>;
//    mDecayChannels = new QList<TTMDecayChannel*>;

//    qDeleteAll(mDecaySummary.begin(), mDecaySummary.end());
//    qDeleteAll(mDecayChannels.begin(), mDecayChannels.end());

//    QList<TTMDecayChannel*>::const_iterator oldch;
//    for (oldch = (obj.getDecayChannels()).begin(); oldch != (obj.getDecayChannels()).end(); ++oldch) {
//        double bRatio = (*oldch)->getBRatio();
//        QList<TTMIDObj*>::iterator oldid;
//        QList<TTMIDObj*>* dlist = new QList<TTMIDObj*>;
//        for (oldid = (*oldch)->getDaughterList()->begin(); oldid != (*oldch)->getDaughterList()->end(); ++oldid) {
//            dlist->append(new TTMIDObj((*oldid)->getID(), obj.parent()));
//        }
//        mDecayChannels.append(new TTMDecayChannel(bRatio, dlist, obj.parent()));
//    }
//    updateDecaySummary();
}

//__________________________________________________________________________
TTMParticle::~TTMParticle()
{
    // returns the memory associated with fDecaySummary and fDecayChannels
    // to the heap
    //

    qDeleteAll(mDecaySummary.begin(), mDecaySummary.end()); // kills all heap-based entries

        QList<TTMDecayChannel*>::const_iterator ch;
        for (ch = mDecayChannels.begin(); ch != mDecayChannels.end(); ++ch) {
            qDeleteAll((*ch)->getDaughterList()->begin(), (*ch)->getDaughterList()->end());
            delete (*ch)->getDaughterList();
        }

        qDeleteAll(mDecayChannels.begin(), mDecayChannels.end()); // kills all heap-based entries
}

//__________________________________________________________________________
//TTMDecay *TTMParticle::getDecay(int daughter_id)
//{
//    // Access to decay object of parent involving daughter with id daughter_id.
//    // Returns 0 if daughter_id is not in the list of daughter id's.
//    //

//    TTMDecay* rv = nullptr;

//    QList<TTMDecay*>::iterator decay;
//    for (decay = mDecaySummary.begin(); decay != mDecaySummary.end(); ++decay) {
//        if ((*decay)->getDaughterID() == daughter_id) {
//            rv  = *decay;
//            break;
//        }
//    }
//    return rv;
//}

//__________________________________________________________________________
//TTMDecayChannel *TTMParticle::getDecayChannel(int channel)
//{
//    // Provides access to the specified decay channel. Returns 0 if the
//    // specified channel is not in the list.
//    //
//    TTMDecayChannel *rv = 0;

//    for( int index = 0; index <= channel; index++) {
//        rv = mDecayChannels.at(index);
//        if(!rv) {
//            qWarning() << Q_FUNC_INFO << "WARNING: Channel #" << channel << "does not exist!";
//            rv = nullptr;
//        }
//    }
//    return rv;
//}

//__________________________________________________________________________
//void TTMParticle::list() const
//{
//    // Listing for particles

//    QString text = QString("********* LISTING FOR PARTICLE %1 *********").arg(mPartName);
//    qInfo() << text;
//    qInfo() << QString("%1 = %2").arg("ID",  -20, QChar('.')).arg(mID, 5);
//    qInfo() << QString("%1 = %2").arg("Deg.", -20, QChar('.')).arg(mDeg, 5);
//    qInfo() << QString("%1 = %2").arg("STAT",  -20, QChar('.')).arg(mStat, 5);
//    qInfo() << QString("%1 = %2 GeV").arg("Mass",  -20, QChar('.')).arg(mMass, 5);
//    qInfo() << QString("%1 = %2 GeV").arg("Width",  -20, QChar('.')).arg(mWidth, 5);
//    qInfo() << QString("%1 = %2 GeV").arg("Threshold",  -20, QChar('.')).arg(mThreshold, 5);
//    if(mThresholdFlag)
//        qInfo() << QString("%1 = %2 GeV").arg("Calculated Threshold",  -20, QChar('.')).arg(mThresholdCalc, 5);
//    qInfo() << QString("%1 = %2").arg("Hard sphere radius",  -20, QChar('.')).arg(mRadius);
//    qInfo() << QString("%1 = %2").arg("B",  -20, QChar('.')).arg(mB);
//    qInfo() << QString("%1 = %2  %3 = %4").arg("S",  -20, QChar('.')).arg(mS).arg("|S|", -5, QChar('.')).arg(mSContent, 5);
//    qInfo() << QString("%1 = %2").arg("ID",  -10, QChar('.')).arg(mQ);
//    qInfo() << QString("%1 = %2  %3 = %4").arg("Charm",  -20, QChar('.')).arg(mCharm).arg("|C|", -5, QChar('.')).arg(mCContent, 5);
//    qInfo() << QString("%1 = %2  %3 = %4").arg("Beauty", -20, QChar('.')).arg(mBeauty).arg("|B|", -5, QChar('.')).arg(mBContent, 5);
//    qInfo() << QString("%1 = %2  %3 = %4").arg("Top",    -20, QChar('.')).arg(mTop).arg("|T|", -5, QChar('.')).arg(mTContent, 5);
//    qInfo() << text;
//    if (mStable) {
//        text  = "STABLE";
//    } else {
//        text = "UNSTABLE";
//        qInfo() << "Decay Channels:";

//        qInfo() << text;

//        for (TTMDecayChannel * dc : mDecayChannels)
//            dc->list();

//        qInfo() << "Summary of Decays: ";

//        for (TTMDecay * decay : mDecaySummary)
//            qInfo() << QString("%1 %2 %").arg(decay->getDaughterID()).arg(decay->getBRatio() * 100);

//        qInfo() << "\t ********************************************** ";
//    }
//}

//__________________________________________________________________________
//void TTMParticle::setDecayChannels(QString file, bool scaleBRatios)
//{
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

//    qDeleteAll(mDecaySummary.begin(), mDecaySummary.end());
//    qDeleteAll(mDecayChannels.begin(), mDecayChannels.end());

//    QFile data(file);
//    if (!data.open(QFile::ReadOnly)) {
//        QString text = QString("Cannot open file: %1 ☛ Setting Particle %2 to stable").arg(file).arg(mID);
//        qInfo() << text;
//        mStable = true;
//    } else {
//        int    nch = 0;
//        int    no_daughters[500];   //# of daughters in each decay channel of parent
//        double vBRatio[100];         // BR in each channel
//        QList<TTMIDObj*>* ld[100];
//        double totalBRatio = 0.;

//        data.seek(0);
//        QRegExp rx("\t");// match a tab
//        while (!data.atEnd()) {
//            QString line = data.readLine();
//            QStringList list = line.split(rx, QString::SkipEmptyParts);
//            no_daughters[nch] = list.size() - 1;
//            vBRatio[nch] = list.at(0).toDouble();
//            totalBRatio += list.at(0).toDouble();
//            ld[nch] = new QList<TTMIDObj*>;
//            for (int i = 1; i < list.size(); i++) {
//                TTMIDObj* id = new TTMIDObj(list.at(i).toInt());
//                ld[nch]->append(id);
//            }
//            nch++;
//        }
//        data.close();
//        for (int ch = 0; ch < nch; ch++) {
//            if (!scaleBRatios)
//                vBRatio[ch] /= 100.;
//            else
//                vBRatio[ch] = vBRatio[ch] / 100. * 100. / totalBRatio;
//            mDecayChannels.append(new TTMDecayChannel(vBRatio[ch], ld[ch]));
//        }
//    }
//    updateDecaySummary();
//}

//__________________________________________________________________________
//void TTMParticle::setDecayChannels(QList<TTMDecayChannel *> &x)
//{
//    // Sets decay channel list and generates fDecaySummary using
//    // UpdateDecaySummary
//    //

//    qDeleteAll(mDecayChannels.begin(), mDecayChannels.end());
//    mDecayChannels = x;

//    updateDecaySummary();
//}

//__________________________________________________________________________
//void TTMParticle::setDecayChannelEfficiency(int channel, double eff)
//{
//    // Modifies the branching ratio of the specified channel according to the
//    // given efficiency (given as a percentage).
//    //

//    TTMDecayChannel *ch = getDecayChannel(channel);

//    if(ch){
//        ch->setBRatio(ch->getBRatio() * eff/100.);
//        updateDecaySummary();
//    }
//}

//__________________________________________________________________________
//void TTMParticle::setDecaySummary(QList<TTMDecay*>& x)
//{
//    // Sets fDecaySummary-- allows for incompatibility between channel and
//    // summary lists.
//    //

//    qDeleteAll(mDecaySummary.begin(), mDecaySummary.end());

//    mDecaySummary = x;
//}

//__________________________________________________________________________
//void TTMParticle::setID(int x)
//{
//    // Sets fName for retrieval from Hash Table (fName based on particle ID)

//    mID = x;
//    setObjectName(QString::number(x));
//}

//__________________________________________________________________________
//void TTMParticle::updateDecaySummary()
//{
//    // Updates the fDecaySummary summary list from the fDecayChannels list by
//    // iterating over these channels. Uses GetDecay().
//    //

//    qDeleteAll(mDecaySummary.begin(), mDecaySummary.end());

//    QList<TTMDecayChannel*>::iterator ch;
//    for( ch = mDecayChannels.begin(); ch != mDecayChannels.end(); ++ch) {
//        double BRatio = (*ch)->getBRatio();
//        QList<TTMIDObj*>::iterator daughter;
//        for( daughter = (*ch)->getDaughterList()->begin(); daughter != (*ch)->getDaughterList()->end(); ++daughter) {
//            int d_id = (*daughter)->getID();
//            if (getDecay(d_id)) {   // daughter already in summary decay list of parent
//                TTMDecay* decay = getDecay(d_id);
//                decay->setBRatio(decay->getBRatio() + BRatio);
//            } else {             // daughter not yet in summary decay list of parent
//                mDecaySummary.append(new TTMDecay(mID, d_id, BRatio, parent()));
//            }
//        }
//    }
//}

//__________________________________________________________________________
//TTMParticle &TTMParticle::operator=(TTMParticle &obj)
//{
//    // assignation operator

//    if (this == &obj)
//        return *this;

//    setID(obj.getID());
//    mPartName      = obj.getPartName();
//    mStable        = obj.getStable();
//    mDeg           = obj.getDeg();
//    mStat          = obj.getStat();
//    mB             = obj.getB();
//    mS             = obj.getS();
//    mQ             = obj.getQ();
//    mCharm         = obj.getCharm();
//    mBeauty        = obj.getBeauty();
//    mTop           = obj.getTop();
//    mMass          = obj.getMass();
//    mWidth         = obj.getWidth();
//    mThreshold     = obj.getThreshold();
//    mRadius        = obj.getRadius();
//    mThresholdCalc = obj.getThresholdCalc();
//    mThresholdFlag = obj.getThresholdFlag();
//    mSContent      = obj.getSContent();
//    mCContent      = obj.getCContent();
//    mBContent      = obj.getBContent();
//    mTContent      = obj.getTContent();

//    qDeleteAll(mDecaySummary.begin(), mDecaySummary.end());
//    qDeleteAll(mDecayChannels.begin(), mDecayChannels.end());

//    QList<TTMDecayChannel*>::const_iterator oldch;
////    for (TTMDecayChannel* it : obj.getDecayChannels()) {
//    for (oldch = obj.getDecayChannels().begin(); oldch != obj.getDecayChannels().end(); ++oldch) {
//        double  BRatio = (*oldch)->getBRatio();
//        QList<TTMIDObj*>::iterator oldid;
//        QList<TTMIDObj*>* dlist = new QList<TTMIDObj*>;
//        for (oldid = (*oldch)->getDaughterList()->begin(); oldid != (*oldch)->getDaughterList()->end(); oldid++) {
//            dlist->append(new TTMIDObj((*oldid)->getID()));
//        }
//        mDecayChannels.append(new TTMDecayChannel(BRatio, dlist));
//    }

//    updateDecaySummary();

//    return *this;
//}
