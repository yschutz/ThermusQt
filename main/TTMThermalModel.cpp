// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: August 2017
//__________________________________________________________________________
// Base class for all thermal model classes.
//
#include <QMessageBox>
#include <QTableView>
#include <QSqlQuery>
#include <QStandardItemModel>

#include "TTMDensObj.h"
#include "TTMThermalModel.h"

#include "external/particlesdbmanager.h"

//__________________________________________________________________________
TTMThermalModel::TTMThermalModel(QObject *parent) : QObject(parent),
    mBaryon(0.0), mBeauty(0.0), mBminus(0.0), mBplus(0.0), mbminus(0.0), mbplus(0.0),
    mCharge(0.0), mCharm(0.0), mCminus(0.0), mCplus(0.0), mDensity(0.0),
    mDescriptor(""), mEnergy(0.0), mEntropy(0.0), mPressure(0.0),
    mQminus(0.0), mQplus(0.0), mSminus(0.0), mSplus(0.0),
    mStrange(0.0), mWidth(true), mWroblewski(0.0)
{
    // ctor
    mPartPDGs.clear();
}

//__________________________________________________________________________
TTMThermalModel::TTMThermalModel(const TTMThermalModel &model)
{
    // coy ctor
    mBaryon     = model.mBaryon;
    mBeauty     = model.mBeauty;
    mBminus     = model.mBminus;
    mBplus      = model.mBplus;
    mbminus     = model.mbminus;
    mbplus      = model.mbplus;
    mCharge     = model.mCharge;
    mCharm      = model.mCharm;
    mCminus     = model.mCminus;
    mCplus      = model.mCplus;
    mDensity    = model.mDensity;

    QHashIterator<int, TTMDensObj*> i(model.mDensTable);
    while (i.hasNext())
        mDensTable[i.key()] = new TTMDensObj(i.value());

    mDescriptor = model.mDescriptor + " Copy";
    mEnergy     = model.mEnergy;
    mEntropy    = model.mEntropy;
    mPressure   = model.mPressure;
    mQminus     = model.mQminus;
    mQplus      = model.mQplus;
    mSminus     = model.mSminus;
    mSplus      = model.mSplus;
    mStrange    = model.mStrange;
    mWidth      = model.mWidth;
    mWroblewski = model.mWroblewski;

    setParent(model.parent());
}

//__________________________________________________________________________
TTMThermalModel::~TTMThermalModel()
{
    //dtor
    qDeleteAll(mDensTable.begin(), mDensTable.end());
    mDensTable.clear();
}

//__________________________________________________________________________
void TTMThermalModel::calcWroblewski()
{
    // Calculates the Wroblewski factor in the model from the primordial
    // particle densities in the hash table after it has been populated
    // by functions in the derived classes. Only correct if charm and
    // beauty are excluded.
    //

    double absstrange, str, nstrange, nstr;
    double absstrangemesons, absstrangebaryons, nstrangemesons,
            nstrangebaryons;
    absstrangemesons = absstrangebaryons = nstrangemesons
            = nstrangebaryons = 0.;

    QList<int> particles;
    if (!ParticlesDBManager::Instance().allParticles(particles))
        return;

    for (int pdg : particles) {
        TTMDensObj *dens = getDensities(pdg);
        if (!dens)
            return;
        double part_dens = dens->getPrimaryDensity();
        if (ParticlesDBManager::Instance().getBaryon(pdg)) { // mesons
            absstrangemesons += ParticlesDBManager::Instance().getSContent(pdg) * part_dens;
            nstrangemesons += (2. - ParticlesDBManager::Instance().getSContent(pdg))
                    * part_dens;

        } else {                                            //baryons
            absstrangebaryons += ParticlesDBManager::Instance().getSContent(pdg)
              * part_dens;
            nstrangebaryons += (3. - ParticlesDBManager::Instance().getSContent(pdg))
              * part_dens;

          }
    }

    absstrange = absstrangemesons + absstrangebaryons;
    //no. of s + sbar
    str = absstrange / 2.;
    //no. of s sbar (s = sbar since S=0)
    nstrange = nstrangemesons + nstrangebaryons - mBaryon * 3.;
    //no. of new ud
    nstr = nstrange / 2.;
    mWroblewski = 2. * str / nstr;
}

//__________________________________________________________________________
void TTMThermalModel::generateDecayPartDens()
{
    // Iterates through the hash table of TTMDensObj objects and calculates the
    // decay contributions to the densities of the particles considered
    // stable in the particle set.

    QList<int> stableParticles;
    ParticlesDBManager::Instance().allParticles(stableParticles, ParticlesDBManager::kSTABLE);
    for (int pdg : stableParticles) {
        QHash<int, double> br;
        ParticlesDBManager::Instance().allDecays(pdg, br, false);
        double decay = 0.0;
        QHash<int, double>::iterator i;
        for (i = br.begin(); i != br.end(); ++i) {
            int parent = i.key();
            double br  = i.value();
            decay += br * getDensities(parent)->getPrimaryDensity();
        }
        getDensities(pdg)->setDecayDensity(decay);
    }
}

//__________________________________________________________________________
void TTMThermalModel::generateDecayPartDens(int pdg)
{
    // Calculates the decay contribution to particle with ID id
    // based on the primordial densities already in the hash table
    // provided the particle is stable!
    // (NB must first populate the hash table with primordial densities)

    if (!ParticlesDBManager::Instance().isStable(pdg)) {
        getDensities(pdg)->setDecayDensity(0.0);
        return;
    }
    QHash<int, double> br;
    ParticlesDBManager::Instance().allDecays(pdg, br, false);
    double decay = 0.0;
    QHash<int, double>::iterator i;
    for (i = br.begin(); i != br.end(); ++i) {
        int parent = i.key();
        double br  = i.value();
        decay += br * getDensities(parent)->getPrimaryDensity();
    }
    getDensities(pdg)->setDecayDensity(decay);
}

//__________________________________________________________________________
TTMDensObj* TTMThermalModel::getDensities(int pdg) const
{

    // Returns a pointer to the density object of the particle with specified
    // ID
    TTMDensObj* dens = mDensTable[pdg];
    if (dens)
        return dens;
    else {
        QMessageBox msg(QMessageBox::Critical, "getDensities", Q_FUNC_INFO);
        msg.setInformativeText(QString("Particle %1 not in density table").arg(pdg));
        msg.exec();
        return nullptr;
    }
}

//__________________________________________________________________________
void TTMThermalModel::listDecayContributions(int daughter) const
{
    // Lists the decay contributions (in percentage and absolute terms) to daughter
    // with ID d_id. The primordial and decay densities must already appear in the
    // density hash table (i.e. GenerateParticleDens() should be run first).

    QStandardItemModel* model = new QStandardItemModel;
    QStringList headers;
    headers << "parent pdg" << "parent name" << "daughter density (%)" << "partial parent density" << "total parent density";
    model->setHorizontalHeaderLabels(headers);

    QHash<int, double> parents;
    if (!ParticlesDBManager::Instance().allDecays(daughter, parents))
        return;

    TTMDensObj* dens = getDensities(daughter);
    if (!dens)
        return;

    double n_decay = dens->getDecayDensity();


    double total = 0.0;
    QHash<int, double>::iterator i;
    for (i = parents.begin(); i != parents.end(); ++i) {
        int parent = i.key();
        double br  = i.value();
        double contrib = getDensities(parent)->getPrimaryDensity() * br;
        total += contrib;
        QList<QStandardItem*> onerow;
        onerow << new QStandardItem(QString::number(parent)) <<
                  new QStandardItem(ParticlesDBManager::Instance().getName(parent)) <<
                  new QStandardItem(QString::number(n_decay * 100)) <<
                  new QStandardItem(QString::number(contrib)) <<
                  new QStandardItem(QString::number(total));
        model->appendRow(onerow);
    }
    QTableView* view = new QTableView;
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->setModel(model);
    view->show();
}

//__________________________________________________________________________
void TTMThermalModel::listDecayContributions(int parent, int daughter) const
{
    // Lists the contribution (in percentage and absolute terms) of the decay from
    // parent with ID p_id to daughter with ID d_id. The hash table must already
    // contain the primordial and decay densities (i.e. first run GenerateParticleDens())
    //

    QStandardItemModel* model = new QStandardItemModel;
    QStringList headers;
    headers << "parent pdg" << "parent name" << "daughter density (%)" << "partial parent density";
    model->setHorizontalHeaderLabels(headers);

    QHash<int, double> parents;
    if (!ParticlesDBManager::Instance().allDecays(daughter, parents))
        return;

    TTMDensObj* dens = getDensities(daughter);
    if (!dens)
        return;

    double n_decay = dens->getDecayDensity();

    double br = parents[parent];
    if (br == 0.) {
        QMessageBox msg(QMessageBox::Warning, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(QString("No such decay: parent %1, daughter %2").arg(parent, daughter));
        msg.exec();
    } else {
        double contrib = getDensities(parent)->getPrimaryDensity() * br;
        QList<QStandardItem*> onerow;
        onerow << new QStandardItem(QString::number(parent)) <<
                  new QStandardItem(ParticlesDBManager::Instance().getName(parent)) <<
                  new QStandardItem(QString::number(n_decay * 100)) <<
                  new QStandardItem(QString::number(contrib));
        model->appendRow(onerow);
        QTableView* view = new QTableView;
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setModel(model);
        view->show();
    }
}

//__________________________________________________________________________
void TTMThermalModel::listStableDensities() const
{
    // Lists the densities of the stable particles in the model

    QStandardItemModel* model = new QStandardItemModel;
    QStringList headers;
    headers << "parent pdg" << "particle name" << "primordial density" << "decay contribution";
    model->setHorizontalHeaderLabels(headers);

    QList<int> stableParticles;
    if(!ParticlesDBManager::Instance().allParticles(stableParticles, ParticlesDBManager::kSTABLE))
        return;
    for (int pdg : stableParticles) {
        TTMDensObj* dens = getDensities(pdg);
        if(!dens) {
            delete model;
            return;
        }
        QList<QStandardItem*> onerow;
        onerow << new QStandardItem(ParticlesDBManager::Instance().getName(pdg)) <<
                  new QStandardItem(QString::number(dens->getPrimaryDensity())) <<
                  new QStandardItem(QString::number(dens->getDecayDensity()));
        model->appendRow(onerow);
    }
    QTableView* view = new QTableView;
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->setModel(model);
    view->show();
}
