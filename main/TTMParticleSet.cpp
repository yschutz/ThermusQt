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

#include <QDebug>
#include <QFile>
//__________________________________________________________________________
TTMParticleSet::TTMParticleSet(QObject *parent) : QObject(parent)
{
    // ctor
    mPartTable = new QHash<QString, TTMParticle*>;
    mFilename = QString("");
    mParticleNumber = 0;
}

//__________________________________________________________________________
TTMParticleSet::TTMParticleSet(QString file, bool CB, QObject *parent)  : QObject(parent)
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

    mPartTable      = new QHash<QString, TTMParticle*>;
    mFilename       = file;
    mParticleNumber = 0;

    QFile data(file);
    if (!data.open(QFile::ReadOnly)) {
        qWarning() << Q_FUNC_INFO << "WARNING: Cannot open file " << file;
        exit(1);
    }
    data.seek(0);
    while (!data.atEnd()) {
        TTMParticle* part = new TTMParticle();
        QString line = data.readLine();

        QRegExp rx("\t");// match a tab
        QStringList list = line.split(rx, QString::SkipEmptyParts);

        if (list.at(0).toInt()) {
            part->setStable();
        } else {
            part->setUnstable();
        }
        part->setPartName(list.at(1));
        part->setID(list.at(2).toInt());
        part->setDeg(list.at(3).toInt());
        part->setStat(list.at(4).toInt());
        part->setMass(list.at(5).toDouble());
        part->setS(list.at(6).toInt());
        part->setB(list.at(7).toInt());
        part->setQ(list.at(8).toInt());
        if(CB) {
            part->setCharm(list.at(9).toInt());
            part->setBeauty(list.at(10).toInt());
        } else {
            part->setCharm(0);
            part->setBeauty(0);
        }
        part->setTop(0);
        if (list.size() > 11) {
            part->setSContent(list.at(11).toDouble());
            part->setCContent(qAbs(part->getCharm()));
            part->setBContent(qAbs(part->getBeauty()));
            part->setTContent(qAbs(part->getTop()));
        }
        if (list.size() > 12) {
            part->setWidth(list.at(12).toDouble());
        }
        if (list.size() > 13) {
            part->setThreshold(list.at(13).toDouble());
        }
        part->setRadius(0.);

        mPartTable->insert(part->objectName(), part);

        part->list();

        mParticleNumber++;
        if (part->getB() != 0 || part->getS() != 0 || part->getQ() != 0 || part->getCharm() != 0 || part->getBeauty() != 0 || part->getTop() != 0) {
            TTMParticle* apart = new TTMParticle();
            if (part->getStable()) {
                apart->getStable();
            } else {
                apart->setUnstable();
            }
            QString temp;
            temp = part->getPartName();
            temp = temp.prepend("anti-");
            apart->setPartName(temp);
            apart->setID(-part->getID());
            apart->setDeg(part->getDeg());
            apart->setStat(part->getStat());
            apart->setMass(part->getMass());
            apart->setS(-part->getS());
            apart->setB(-part->getB());
            apart->setQ(-part->getQ());
            apart->setCharm(-part->getCharm());
            apart->setBeauty(-part->getBeauty());
            apart->setTop(-part->getTop());
            apart->setSContent(part->getSContent());
            apart->setCContent(part->getCContent());
            apart->setBContent(part->getBContent());
            apart->setTContent(part->getTContent());
            apart->setWidth(part->getWidth());
            apart->setThreshold(part->getThreshold());

            apart->setRadius(part->getRadius());

            mPartTable->insert(apart->objectName(), apart);
            mParticleNumber++;

            apart->list();
        }
        data.readLine(); // skip empty lines
    }
    data.close();
}

//__________________________________________________________________________
//TTMParticleSet::TTMParticleSet(TDatabasePDG *pdg)
//{

//}

//__________________________________________________________________________
TTMParticleSet::TTMParticleSet(TTMParticleSet &obj)
{
    // cpy ctor

    setParent(obj.parent());

    mPartTable      = new QHash<QString, TTMParticle*>;
    mParticleNumber = 0;

    QHash<QString, TTMParticle*>::iterator a;
    for (a = obj.getParticleTable()->begin(); a != obj.getParticleTable()->end(); ++a) {
        TTMParticle *part = new TTMParticle();
        part->setPartName((*a)->getPartName());
        qDebug() << Q_FUNC_INFO << part->getPartName();
        part->setID((*a)->getID());
        if((*a)->getStable())
            part->setStable();
        else
            part->setUnstable();
        part->setDeg((*a)->getDeg());
        part->setStat((*a)->getStat());
        part->setB((*a)->getB());
        part->setS((*a)->getS());
        part->setQ((*a)->getQ());
        part->setCharm((*a)->getCharm());
        part->setBeauty((*a)->getBeauty());
        part->setTop((*a)->getTop());
        part->setMass((*a)->getMass());
        part->setWidth((*a)->getWidth());
        part->setThreshold((*a)->getThreshold());
        part->setSContent((*a)->getSContent());
        part->setCContent((*a)->getCContent());
        part->setBContent((*a)->getBContent());
        part->setTContent((*a)->getTContent());

        part->setThresholdCalc((*a)->getThresholdCalc());
        part->setThresholdFlag((*a)->getThresholdFlag());
        part->setRadius((*a)->getRadius());

        QList<TTMDecayChannel*>::const_iterator old;
        for (old = (*a)->getDecayChannels().begin(); old != (*a)->getDecayChannels().end(); ++old) {
            QList<TTMIDObj*>* ndlist = new QList<TTMIDObj*>;
            QList<TTMIDObj*>::const_iterator oldid;
            for (oldid = (*old)->getDaughterList()->begin(); oldid != (*old)->getDaughterList()->end(); ++oldid)
                ndlist->append(new TTMIDObj((*oldid)->getID()));
            (part->getDecayChannels()).append(new TTMDecayChannel((*old)->getBRatio(),ndlist));
        }
        part->updateDecaySummary();
        mPartTable->insert(part->objectName(), part);
        mParticleNumber++;
    }
    generateBRatios();
    mFilename = obj.getFilename();
}

//__________________________________________________________________________
TTMParticleSet::~TTMParticleSet()
{
    // Return memory to heap
    //

    if(mPartTable){
        qDeleteAll(mPartTable->begin(), mPartTable->end()); // deletes heap-based entries in Hash Table
        delete mPartTable;           // returns memory to heap
    }
}

//__________________________________________________________________________
void TTMParticleSet::addParticle(TTMParticle *part)
{
    // Add new particle to set if not already in. Corresponding anti-particles
    // are added if they exist and are not already in the set.
    //

    if(!getParticle(part->getID())){
        mPartTable->insert(part->objectName(), part);
        mParticleNumber++;

        if (part->getB() != 0 || part->getS() != 0 || part->getQ() != 0 || part->getCharm() != 0 || part->getBeauty() != 0 || part->getTop() != 0) {
            if(!getParticle(-part->getID())){
                TTMParticle* apart = new TTMParticle();
                if (part->getStable()) {
                    apart->setStable();
                } else {
                    apart->setUnstable();
                }
                QString temp;
                temp = part->getPartName();
                temp = temp.prepend("anti-");
                apart->setPartName(temp);
                apart->setID(-part->getID());
                apart->setDeg(part->getDeg());
                apart->setStat(part->getStat());
                apart->setMass(part->getMass());
                apart->setS(-part->getS());
                apart->setB(-part->getB());
                apart->setQ(-part->getQ());
                apart->setCharm(-part->getCharm());
                apart->setBeauty(-part->getBeauty());
                apart->setTop(-part->getTop());
                apart->setSContent(part->getSContent());
                apart->setCContent(part->getCContent());
                apart->setBContent(part->getBContent());
                apart->setTContent(part->getTContent());
                apart->setWidth(part->getWidth());
                apart->setThreshold(part->getThreshold());

                apart->setRadius(part->getRadius());

                mPartTable->insert(apart->objectName(), apart);
                mParticleNumber++;
            }
        }
    }
}

//__________________________________________________________________________
void TTMParticleSet::calculateThresholds()
{
    // Calculates thresholds based on channels in lists of particles. This
    // function iterates through the table calling
    // CalculateThreshold(TTMParticle *).
    //

    QHash<QString, TTMParticle*>::iterator part;
    for (part = mPartTable->begin(); part != mPartTable->end(); ++part)
        calculateThreshold((*part));
}

//__________________________________________________________________________
void TTMParticleSet::calculateThreshold(TTMParticle *part)
{
    // Private function that calculates the threshold of the particle pointed at
    //

    bool check;
    double thresh;

    do{

        check = false;
        thresh = 0.;

        QList<TTMDecayChannel*>::iterator ch;
        for (ch = part->getDecayChannels().begin(); ch != part->getDecayChannels().end(); ++ch) {
            QList<TTMIDObj*>::iterator id;
            double mch = 0.;
            for (id = (*ch)->getDaughterList()->begin(); id != (*ch)->getDaughterList()->end(); ++id) {
                TTMParticle* d = getParticle((*id)->getID());
                if(d->getWidth() == 0.){
                    mch += d->getMass();
                } else if(d->getThresholdFlag()){
                    mch += qMax(d->getMass() -2. * d->getWidth(), d->getThresholdCalc());
                } else {
                    check = true;
                    calculateThreshold(d);
                }
            }
            if(mch > thresh)
                thresh = mch;
        }
    } while(check);
    part->setThresholdCalc(thresh);
    part->setThresholdFlag(true);
}

//__________________________________________________________________________
void TTMParticleSet::generateBRatios()
{
    // Iterates over the particle table updating the summary decay lists based
    // on the channels, before calling the recursive GenerateBRatios(part)
    // function to determine the branching ratios to the stable particles in
    // the set. This function must be run after any changes have been made to
    // the decay channels in the set.
    //

    QHash<QString, TTMParticle*>::iterator part;
    for (part = mPartTable->begin(); part != mPartTable->end(); ++part) {
        (*part)->updateDecaySummary();
    }

    for (part = mPartTable->begin(); part != mPartTable->end(); ++part) {
        generateBRatios((*part));
    }
}

//__________________________________________________________________________
void TTMParticleSet::generateBRatios(TTMParticle *parent)
{
    // Private recursive function used by GenerateBRatios() to convert
    // decay summaries to lists containing just stable particles.
    // GenerateBRatios() updates the summaries first.
    //

    QList<TTMDecay*>& parent_decays = parent->getDecaySummary();

    if (!parent->getStable())    //Parent unstable
    {
        QList<TTMDecay*> temp_decays; // = new QList<TTMDecay*>;
        QList<TTMDecay*>::iterator p_decay;

        qint32 flag;

        do {
            flag = 0;
            qDeleteAll(temp_decays.begin(), temp_decays.end());
            for (p_decay = parent_decays.begin(); p_decay != parent_decays.end(); ++p_decay) {
                TTMParticle* daughter = getParticle((*p_decay)->getDaughterID());
                if(daughter){                 //if daughter is in the set
                    if (daughter->getStable())  //if daughter is stable
                    {
                        QList<TTMDecay*>::iterator t_decay;
                        bool found = false;
                        for (t_decay = temp_decays.begin(); t_decay != temp_decays.end(); ++t_decay) {
                            if (found)
                                break;
                            if ((*t_decay)->getDaughterID() == daughter->getID())
                                //daughter already in temp decay table of parent
                            {
                                (*t_decay)->setBRatio((*t_decay)->getBRatio()
                                                      + (*p_decay)->getBRatio());
                                found = true;
                            }
                        }
                        if (!found)
                            //daughter not yet in temp decay table of parent
                        {
                            temp_decays.append(new TTMDecay((*p_decay)->getParentID(),
                                                             (*p_decay)->getDaughterID(),
                                                             (*p_decay)->getBRatio()));
                        }
                    } else              // if daughter is unstable
                    {
                        QList<TTMDecay*>& daughter_decays = daughter->getDecaySummary();
                        QList<TTMDecay*>::iterator d_decay;
                        for (d_decay = daughter_decays.begin(); d_decay != daughter_decays.end(); ++d_decay) {
                            TTMParticle* grandaughter = getParticle((*d_decay)->getDaughterID());

                            if (grandaughter->getStable())
                                //if grandaughter is stable
                            {
                                QList<TTMDecay*>::iterator t_decay;
                                bool found = 0;
                                for (t_decay = temp_decays.begin(); t_decay != temp_decays.end(); ++t_decay) {
                                    if (found)
                                        break;
                                    if ((*t_decay)->getDaughterID() == grandaughter->getID())
                                        //grandaughter in temp decay table of parent
                                    {
                                        (*t_decay)->setBRatio((*t_decay)->getBRatio() +
                                                              (*p_decay)->getBRatio() *
                                                              (*d_decay)->getBRatio());
                                        found = true;
                                    }
                                }
                                if (!found)
                                    //grandaughter not yet in temp decay
                                    //table of parent
                                {
                                    TTMDecay* decay =
                                            new TTMDecay(parent->getID(),
                                                         grandaughter->getID(),
                                                         (*p_decay)->getBRatio() *
                                                         (*d_decay)->getBRatio());
                                    temp_decays.append(decay);
                                }
                            } else
                                //if grandaughter is unstable
                            {

                                generateBRatios(daughter);
                                flag = 1;  //once exited we try again
                            }
                            if (flag == 1) {
                                break;
                            }
                        }
                        if (flag == 1) {
                            break;
                        }
                    }
                } else {
                    qWarning() <<  Q_FUNC_INFO <<"WARNING: daughter "<< (*p_decay)->getDaughterID() <<" not in set (omitting this contribution)";
                }
                if (flag == 1) {
                    break;
                }
            }
        } while (flag == 1);
        parent->setDecaySummary(temp_decays);
    }
}

//__________________________________________________________________________
void TTMParticleSet::getParents(QList<TTMDecay *> *parents, qint32 id) const
{
    // On return parents points to a list containing the parents of particle id.
    //

    qDeleteAll(parents->begin(), parents->end());
    QHash<QString, TTMParticle*>::iterator p;
    for (p = mPartTable->begin(); p != mPartTable->end(); ++p) {
        QList<TTMDecay*>& parent_decays = (*p)->getDecaySummary();
        QList<TTMDecay*>::iterator d;
        for (d = parent_decays.begin(); d != parent_decays.end(); ++p) {
            if((*d)->getDaughterID()==id){parents->append((*d));}
        }
    }
}

//__________________________________________________________________________
TTMParticle *TTMParticleSet::getParticle(qint32 id) const
{
    // Returns a pointer to a particle object with the specified ID if
    // that particle is in the set. If not in the set, 0 is returned.
    // First id has to be converted into a TString in exactly the same
    // way as the particle fNames are set.
    //

    QString name = QString::number(id);

    if (TTMParticle* temp_part = mPartTable->value(name)) {
        return temp_part;
    } else {
        qWarning() << Q_FUNC_INFO << "WARNING: Particle " << id << " not in set";
        return 0;
    }
}

//__________________________________________________________________________
void TTMParticleSet::inputDecays(QString dir, bool scaleBRatios)
{
    // Iterates through the particle table and for each unstable PARTICLE it
    // attempts to read in the decay channels listed in the particle's decay file
    // in the specified directory ("dir/fPartName_decay.txt"). If
    // this file is not found, the particle is set to stable. If it is found
    // the channels are input using SetDecayChannels(char *) which generates
    // also a summary decay list. GenerateBRatios is then called to transform
    // this summary list into one containing just the stable daughters.
    // Once the PARTICLE has been dealt with, the decays of the corresponding
    // ANTI-PARTICLE (if it exists) are entered.
    //

    QHash<QString, TTMParticle*>::iterator part;
    for (part = mPartTable->begin(); part != mPartTable->end(); ++part) {
        if (!(*part)->getStable() && (*part)->getID() > 0)        //unstable PARTICLES!
        {
//            qDebug() << Q_FUNC_INFO << (*part)->getPartName() << file;

            QString temp = (*part)->getPartName();
            temp.append("_decay.txt").prepend(dir);
            (*part)->setDecayChannels(temp, scaleBRatios);
        }

//        if  (((*part)->getB() != 0 || (*part)->getS() != 0 || (*part)->getQ() != 0 || (*part)->getCharm() != 0 || (*part)->getBeauty() != 0 || (*part)->getTop() != 0) && getParticle(-(*part)->getID())) {
//            qDeleteAll(getParticle(-(*part)->getID())->getDecayChannels()->begin(),
//                       getParticle(-(*part)->getID())->getDecayChannels()->end());
//            QList<TTMDecayChannel*>* part_decays = (*part)->getDecayChannels();

//            QList<TTMDecayChannel*>::iterator decay;
//            for (decay = part_decays->begin(); decay != part_decays->end(); ++decay) {
//                double BR = (*decay)->getBRatio();
//                QList<TTMIDObj*>::iterator d_id;
//                QList<TTMIDObj*>* adlist = new QList<TTMIDObj*>;
//                for (d_id = (*decay)->getDaughterList()->begin(); d_id != (*decay)->getDaughterList()->end(); ++d_id) {
//                    qint32 id = (*d_id)->getID();
//                    TTMParticle* decay_part = getParticle(id);
//                    if (decay_part) {
//                        if (decay_part->getB() != 0 || decay_part->getS() != 0 ||
//                                decay_part->getQ() != 0 || decay_part->getCharm() != 0 ||
//                                decay_part->getBeauty() != 0 || decay_part->getTop() != 0) {
//                            TTMIDObj* aid = new TTMIDObj(-id);
//                            adlist->append(aid);
//                        } else {
//                            adlist->append(new TTMIDObj(id));
//                        }
//                    }
//                }
//                getParticle(-(*part)->getID())->getDecayChannels()->append(new TTMDecayChannel(BR, adlist));
//            }
//            getParticle(-(*part)->getID())->updateDecaySummary();
//        }

    }

//    generateBRatios();
}

//__________________________________________________________________________
//void TTMParticleSet::inputDecays(TDatabasePDG *pdg)
//{

//}

//__________________________________________________________________________
void TTMParticleSet::listParents(qint32 id) const
{
    // Lists parents of particle id.
    //

    QList<TTMDecay*>* list = new QList<TTMDecay*>;
    getParents(list, id);

    QList<TTMDecay*>::iterator next_decay;
    for (next_decay = list->begin(); next_decay != list->end(); ++next_decay) {
        qDebug() << Q_FUNC_INFO <<"INFO: " << getParticle((*next_decay)->getParentID())->getPartName() << " :"
                 << (*next_decay)->getBRatio();
    }

    delete list;
}

//__________________________________________________________________________
void TTMParticleSet::listParticle(qint32 id) const
{
    // List particle with specified ID if it is in set
    //

    if (TTMParticle* part = getParticle(id)) {
        part->list();
    }
}

//__________________________________________________________________________
void TTMParticleSet::listParticles(bool full) const
{
    // Lists all particles (and anti-particles) in the set,

    qInfo() << "************** ALL PARTICLES **************";

    for( TTMParticle* part : *mPartTable) {
        if (full)
            part->list();
        else
            qInfo() << QString("    %1").arg(part->getPartName());
    }
    qInfo() << "**********************************************";
}

//__________________________________________________________________________
void TTMParticleSet::listStableParticles()
{
    // Lists stable particles (and anti-particles) in the set.
    //

    qDebug() << Q_FUNC_INFO << "INFO: ************** STABLE PARTICLES **************";

    QHash<QString, TTMParticle*>::iterator part;
    for (part = mPartTable->begin(); part != mPartTable->end(); ++part) {
        if ((*part)->getStable()) {
            qDebug() << "\t\t" << (*part)->getPartName();
        }
    }
    qDebug() << "\t **********************************************";
}

//__________________________________________________________________________
void TTMParticleSet::massCut(double max)
{
    // Removes all particles with mass (in GeV) greater than max. Run this
    // function before InputDecays() to ensure that the feeding of those
    // massive unstable particles above the mass cut are excluded from
    // the lighter hadrons.
    //

    QHash<QString, TTMParticle*>::iterator part;
    for (part = mPartTable->begin(); part != mPartTable->end(); ++part) {
        if((*part)->getMass()>= max){
            mPartTable->remove((*part)->objectName());
            mParticleNumber--;
        }
    }
}

//__________________________________________________________________________
void TTMParticleSet::removeParticle(qint32 id)
{
    // Remove particle if specified particle is in the set.
    //

    if (TTMParticle* part = getParticle(id)) {
        mPartTable->remove(part->objectName());
        mParticleNumber--;
    }
}

//__________________________________________________________________________
void TTMParticleSet::setDecayEfficiency(qint32 p_id, qint32 d_id, double eff)
{
    // Set the decay efficiency of all decays leading from parent (p_id) to
    // daughter (d_id) to eff %. This is reflected only in the summary decay
    // list of the parent from which the decay contributions are calculated
    // (i.e. this change is not reflected in the decay channel list). Therefore,
    // running UpdateDecaySummary will erase the change brought about by calling
    // this method.
    //

    TTMParticle *parent = getParticle(p_id);
    TTMDecay *decay = parent->getDecay(d_id);

    if (decay)
        decay->setBRatio(decay->getBRatio() * eff / 100.);
}

//__________________________________________________________________________
void TTMParticleSet::setRadii(double radius)
{
    // BH 24/04/2014
    // Iterates over the particle table and updates all particles with radius.

    QHash<QString, TTMParticle*>::iterator part;
    qWarning() << Q_FUNC_INFO << "WARNING: updating all particle radii with radius = " << radius << " (to be used for excluded volume corrections)";
    for (part = mPartTable->begin(); part != mPartTable->end(); ++part)
        (*part)->setRadius(radius);
}

//__________________________________________________________________________
TTMParticleSet &TTMParticleSet::operator=(TTMParticleSet &obj)
{
    if (this == &obj)
        return *this;

    qDeleteAll(mPartTable->begin(), mPartTable->end());
    mParticleNumber = 0;

    QHash<QString, TTMParticle*>::iterator a;
    for (a = obj.getParticleTable()->begin(); a != obj.getParticleTable()->end(); ++a) {
        TTMParticle *part = new TTMParticle();

        part->setPartName((*a)->getPartName());
        part->setID((*a)->getID());
        if((*a)->getStable()){
            part->setStable();
        }else{
            part->setUnstable();
        }
        part->setDeg((*a)->getDeg());
        part->setStat((*a)->getStat());
        part->setB((*a)->getB());
        part->setS((*a)->getS());
        part->setQ((*a)->getQ());
        part->setCharm((*a)->getCharm());
        part->setBeauty((*a)->getBeauty());
        part->setTop((*a)->getTop());
        part->setMass((*a)->getMass());
        part->setWidth((*a)->getWidth());
        part->setThreshold((*a)->getThreshold());
        part->setSContent((*a)->getSContent());
        part->setCContent((*a)->getCContent());
        part->setBContent((*a)->getBContent());
        part->setTContent((*a)->getTContent());
        part->setRadius((*a)->getRadius());

        part->setThresholdCalc((*a)->getThresholdCalc());
        part->setThresholdFlag((*a)->getThresholdFlag());

        QList<TTMDecayChannel*>::iterator old;
        for (old = (*a)->getDecayChannels().begin(); old != (*a)->getDecayChannels().end(); ++old) {
            QList<TTMIDObj*>* ndlist = new QList<TTMIDObj*>;
            QList<TTMIDObj*>::iterator oldid;
            for (oldid = (*old)->getDaughterList()->begin(); oldid != (*old)->getDaughterList()->end(); ++oldid) {
                ndlist->append(new TTMIDObj((*oldid)->getID()));
            }
            part->getDecayChannels().append(new TTMDecayChannel((*old)->getBRatio(), ndlist));
        }
        part->updateDecaySummary();
        mPartTable->insert(part->objectName(), part);
        mParticleNumber++;
    }
    generateBRatios();
    mFilename = obj.getFilename();
    setParent(obj.parent());
    return *this;
}

