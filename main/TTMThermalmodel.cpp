// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: August 2017
//__________________________________________________________________________
// Base class for all thermal model classes.
//
#include <QMessageBox>
#include <QSqlQuery>
#include <QStandardItemModel>

#include "TTMDensobj.h"
#include "TTMThermalmodel.h"

#include "external/particlesdbmanager.h"

//__________________________________________________________________________
TTMThermalModel::TTMThermalModel(QObject *parent) : QObject(parent)
{
    // ctor
}

//__________________________________________________________________________
TTMThermalModel::~TTMThermalModel()
{
    //dtor
//    qDeleteAll(mDensTable.begin(), mDensTable.end());
//    mDensTable.clear();
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
        TTMDensObj* daughter_dens = getDensities(pdg);
        QHash<int, double> br;
        ParticlesDBManager::Instance().allDecays(pdg, br);
        double decay = 0.0;
        QHash<int, double>::iterator i;
        for (i = br.begin(); i != br.end(); ++i) {
            int parent = i.key();
            double br  = i.value();
            TTMDensObj *parent_dens = getDensities(parent);
            decay += br * parent_dens->getPrimaryDensity();
        }
        daughter_dens->setDecayDensity(decay);
    }
}

//__________________________________________________________________________
void TTMThermalModel::generateDecayPartDens(int pdg)
{
    // Calculates the decay contribution to particle with ID id
      // based on the primordial densities already in the hash table
      // provided the particle is stable!
      // (NB must first populate the hash table with primordial densities)

    if (!ParticlesDBManager::Instance().isStable(pdg))
        return;
    TTMDensObj* daughter_dens = getDensities(pdg);
    QHash<int, double> br;
    ParticlesDBManager::Instance().allDecays(pdg, br);
    double decay = 0.0;
    QHash<int, double>::iterator i;
    for (i = br.begin(); i != br.end(); ++i) {
        int parent = i.key();
        double br  = i.value();
        TTMDensObj *parent_dens = getDensities(parent);
        decay += br * parent_dens->getPrimaryDensity();
    }
    daughter_dens->setDecayDensity(decay);
}

//__________________________________________________________________________
TTMDensObj* TTMThermalModel::getDensities(int pdg) const
{

    // Returns a pointer to the density object of the particle with specified
    // ID

    QString name = QString::number(pdg);
    auto dens = std::find_if(mDensTable.begin(), mDensTable.end(), [name](TTMDensObj* d) {return d->objectName() == name;});
    if (dens != mDensTable.end()) {
        return *dens;
    } else {
        QMessageBox msg(QMessageBox::Critical, "getDensities", Q_FUNC_INFO);
        msg.setInformativeText(QString("Particle %1 not in table").arg(pdg));
        msg.exec();
        return nullptr;
    }
}

//__________________________________________________________________________
void TTMThermalModel::listDecayContributions(int daughter)
{
    // Lists the decay contributions (in percentage and absolute terms) to daughter
    // with ID d_id. The primordial and decay densities must already appear in the
    // density hash table (i.e. GenerateParticleDens() should be run first).

    QStandardItemModel* model = new QStandardItemModel;
    QStringList headers;
    headers << "parent pdg" << "parent name" << "daughter density (%)" << "partial parent density" << "total parent density";
    model->setHorizontalHeaderLabels(headers);

    QHash<int, double> parents;
    double n_decay = getDensities(daughter)->getDecayDensity();

    ParticlesDBManager::Instance().allDecays(daughter, parents);

    double total = 0.0;
    QHash<int, double>::iterator i;
    for (i = parents.begin(); i != parents.end(); ++i) {
        int parent = i.key();
        double br  = i.value();
        double contrib = etDensities(parent)->getPrimaryDensity() * br;
        total += contrib;
        QList<QStandardItem*> onerow;
        onerow << new QStandardItem(QString::number(parent)) <<
                  new QStandardItem(ParticlesDBManager::Instance().name(parent)) <<
                  new QStandardItem(QString::number(n_decay * 100)) <<
                  new QStandardItem(QString::number(contrib)) <<
                  new QStandardItem(QString::number(total));
        model->appendRow(onerow);
    }
}
