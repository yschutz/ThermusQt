// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Novembre 2016
//__________________________________________________________________________
// Base class for all thermal fit classes.
//

#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>

#include "logger.h"

#include "TTMDensObj.h"
#include "TTMParameterSet.h"
#include "TTMThermalFit.h"
#include "TTMThermalModel.h"

#include "particlesdbmanager.h"
#include "QMinuit.h"
//#include "mainwindow.h"

void fit_function(TTMThermalFit *fit, int flag = 0);

//__________________________________________________________________________
TTMThermalFit::TTMThermalFit(QObject *parent) : QObject(parent),
    mChiSquare(0.0), mQuadDev(0.0), mYieldView(nullptr)
{
    // ctor
}

//__________________________________________________________________________
TTMThermalFit::~TTMThermalFit()
{
    // dtor
    qDeleteAll(mYields.begin(), mYields.end());
    mYields.clear();
}

//__________________________________________________________________________
void TTMThermalFit::addYield(TTMYield *yield)
{
    // add a yield object to the yields list

    auto yy = std::find_if(mYields.begin(), mYields.end(), [yield](TTMYield* y) { return y->objectName() == yield->objectName(); });
    if (yy != mYields.end()) {
        QMessageBox msg(QMessageBox::Critical, "addYield", Q_FUNC_INFO);
        msg.setInformativeText(QString("Yield %1 already exists").arg(yield->objectName()));
        msg.exec();
    } else {
        mYields.append(yield);
    }
}

//__________________________________________________________________________
bool TTMThermalFit::compareYield(TTMYield *y1, TTMYield *y2)
{
    // returns true if mass for y1 is smaller than mass for yield2

    if (y1 && y2) {
        int pdg1 = ParticlesDBManager::instance().getPDG(y1->getTMName());
        int pdg2 = ParticlesDBManager::instance().getPDG(y2->getTMName());
        double mass1 = ParticlesDBManager::instance().getMass(pdg1);
        double mass2 = ParticlesDBManager::instance().getMass(pdg2);
        return mass1 < mass2;
    } else
        return false;
}

//__________________________________________________________________________
void TTMThermalFit::fitData(int flag)
{
    fit_function(this, flag);
}

//__________________________________________________________________________
void TTMThermalFit::generateYields()
{
    // Calculates the primordial particle densities of all particles in the
    // set, then iterates through the list of yields, calculates
    // their decay contributions and inserts the new model
    // predictions into the TTMYield objects, and calculates chi-squared
    // and the quadratic deviation.
    //

    mChiSquare = mQuadDev = 0.0;

    TTMThermalModel* model = generateThermalModel();
    model->generateParticleDens();

    double volume = model->getParameterSet()->getVolume();
    for (TTMYield* yield : mYields) {
        int id1       = yield->getID1();
        int id2       = yield->getID2();
        if (id2 == 0) { // Yield
            if (id1 == 1) {
                yield->setModelValue(model->getBaryon() * volume);
            } else if (id1 == 2) {
                double hmin = 0.;
                model->generateDecayPartDens();
                for (TTMDensObj* dens : model->getDensityTable()) {
                    double charge = ParticlesDBManager::instance().getCharge(dens->getID());
                    if(ParticlesDBManager::instance().isStable(dens->getID()) && charge < 0)
                        hmin += dens->getFinalDensity();
                }
                yield->setModelValue(hmin * volume);
            } else if (id1 == 3) {
                double hplus = 0.;
                model->generateDecayPartDens();
                for (TTMDensObj* dens : model->getDensityTable()) {
                    double charge = ParticlesDBManager::instance().getCharge(dens->getID());
                    if(ParticlesDBManager::instance().isStable(dens->getID()) && charge > 0)
                        hplus += dens->getFinalDensity();
                }
                yield->setModelValue(hplus * volume);
            } else if (id1 == 33340) {
                model->generateDecayPartDens(3334);
                model->generateDecayPartDens(-3334);
                yield->setModelValue((model->getDensities(3334)->getFinalDensity() +
                                      model->getDensities(-3334)->getFinalDensity()) * volume);
            } else {
                model->generateDecayPartDens(id1);
                TTMDensObj *partDens = model->getDensities(id1);
               yield->setModelValue(partDens->getFinalDensity() * volume);
            }
        } else { // ratio
            if (id1 == 3130) {
                //                    model->SetParticleSet(next_yield->GetPartSet1());
                model->generateDecayPartDens(313);
                model->generateDecayPartDens(-313);
                double kstar  = model->getDensities(313)->getFinalDensity();
                double akstar = model->getDensities(-313)->getFinalDensity();
                double num    = (kstar + akstar) / 2.;
                //                    model->setParticleSet(next_yield->GetPartSet2());
                model->generateDecayPartDens(id2);
                double den = model->getDensities(id2)->getFinalDensity();
                yield->setModelValue(num / den);
            } else if (id1 == 33340) {
                //                    model->setParticleSet(next_yield->GetPartSet1());
                model->generateDecayPartDens(3334);
                model->generateDecayPartDens(-3334);
                double omega  = model->getDensities(3334)->getFinalDensity();
                double aomega = model->getDensities(-3334)->getFinalDensity();
                double num    = omega + aomega;
                //                    model->SetParticleSet(next_yield->GetPartSet2());
                model->generateDecayPartDens(id2);
                double den = model->getDensities(id2)->getFinalDensity();
                yield->setModelValue(num / den);
            } else if (id2 == 2) {
                //                    model->SetParticleSet(next_yield->GetPartSet1());
                model->generateDecayPartDens(id1);
                double num = model->getDensities(id1)->getFinalDensity();
                //                    model->SetParticleSet(next_yield->GetPartSet2());
                double hmin = 0.;
                model->generateDecayPartDens();
                for (TTMDensObj* dens : model->getDensityTable()) {
                    double charge = ParticlesDBManager::instance().getCharge(dens->getID());
                    if(ParticlesDBManager::instance().isStable(dens->getID()) && charge < 0)
                        hmin += dens->getFinalDensity();
                }
                double den = hmin;
                yield->setModelValue(num / den);
            } else if(id2 == 3){
                model->generateDecayPartDens(id1);
                double num = model->getDensities(id1)->getFinalDensity();
                double hplus = 0.;
                model->generateDecayPartDens();
                for (TTMDensObj* dens : model->getDensityTable()) {
                    double charge = ParticlesDBManager::instance().getCharge(dens->getID());
                    if(ParticlesDBManager::instance().isStable(dens->getID()) && charge > 0)
                        hplus += dens->getFinalDensity();
                }
                double den = hplus;
                yield->setModelValue(num / den);
            } else if(id2 == 33340) {
                model->generateDecayPartDens(id1);
                double num = model->getDensities(id1)->getFinalDensity();
                model->generateDecayPartDens(3334);
                model->generateDecayPartDens(-3334);
                double den = model->getDensities(3334)->getFinalDensity() + model->getDensities(-3334)->getFinalDensity();
                yield->setModelValue(num / den);
            } else {
                model->generateDecayPartDens(id1);
                double num = model->getDensities(id1)->getFinalDensity();
                model->generateDecayPartDens(id2);
                double den = model->getDensities(id2)->getFinalDensity();
                yield->setModelValue(num / den);
            }
        }
        if (yield->getFit()) {
            mChiSquare += qPow(yield->getStdDev(), 2);
            mQuadDev   += qPow(yield->getQuadDev(), 2);
        }
    }
}
//__________________________________________________________________________
QString TTMThermalFit::getTMName(int id1, int id2, const QString& /*descr*/) const
{
    // ??
    QString name;
    if(id2 == 0){
       if(id1 == 1){
         name = name.append("Npart");

       } else if(id1 == 2) {
         name = name.append("h-");
       } else if( id1 == 3) {
         name = name.append("h+");
       } else if (id1 == 33340) {
         name = name.append("Omega + anti-Omega");
       } else {
         name = ParticlesDBManager::instance().getName(id1, "ThermusParticles");
       }
     } else {
       if(id1 == 3130) {
         name = "<";
         name = name.append(ParticlesDBManager::instance().getName(313, "ThermusParticles"));
         name = name.append(">").append("/").append(ParticlesDBManager::instance().getName(id2, "ThermusParticles"));
       } else if(id1 == 33340){
         name = name.append("Omega + anti-Omega");
         name = name.append("/").append(ParticlesDBManager::instance().getName(id2, "ThermusParticles"));
       } else if(id2 == 2) {
         name = ParticlesDBManager::instance().getName(id1, "ThermusParticles");
         name = name.append("/").append("h-");
       } else if(id2 == 3){
         name = ParticlesDBManager::instance().getName(id1, "ThermusParticles");
         name = name.append("/").append("h+");
       } else if(id2 == 33340){
         name = ParticlesDBManager::instance().getName(id1, "ThermusParticles");
         name = name.append("/").append("Omega + anti-Omega");
       } else {
         name = ParticlesDBManager::instance().getName(id1, "ThermusParticles");
         name = name.append("/").append(ParticlesDBManager::instance().getName(id2, "ThermusParticles"));
       }
     }
//     if(!descr.isEmpty()) {
//       name.append(" ").append(descr);
//     }
     return name;
}

//__________________________________________________________________________
TTMYield *TTMThermalFit::getYield(int id1, int id2, const QString &descr) const
{
    // extract a given yield
    QString name = getTMName(id1, id2, descr);

    auto yield = std::find_if(mYields.begin(), mYields.end(), [name](TTMYield* y) {return y->objectName() == name;});
    if (yield != mYields.end()) {
        return *yield;
    } else {
        QMessageBox msg(QMessageBox::Critical, "getYield", Q_FUNC_INFO);
        msg.setInformativeText(QString("Yield %1 does not exist").arg(name));
        msg.exec();
        return nullptr;
    }
}

//__________________________________________________________________________
void TTMThermalFit::inputExpYields(QString& fileName)
{
    // Inserts the experimental yields listed in the specified file
    // in *mYields.
    //
    // The input file has the following format:
    //
    // ID \t Descriptor \t exp_yield \t exp_error\n
    // ID (of numerator) \t ID (of denominator) \t Descriptor \t exp_ratio \t exp_error\n
    // etc ....
    //
    // In addition to all of the particle id's in Thermus DB, the following are
    // also allowed:
    //
    // 		ID=1 	: Npart
    //		ID=2	: h-
    //		ID=3 	: h+
    //
    qDeleteAll(mYields.begin(), mYields.end());
    mYields.clear();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox msg(QMessageBox::Critical, "inputExpYields", Q_FUNC_INFO);
        msg.setInformativeText(QString("Cannot open %1").arg(fileName));
        msg.exec();
        return;
    }
    QTextStream data(&file);

    while (!data.atEnd()) {
        QString name(""), descr("");
        int id1(0), id2(0);
        double value, error;
        QString line = data.readLine();
        if (line.startsWith('#')) { // a comment
            descr = line.remove('#');
        } else {
            QStringList sline = line.split('\t');
            if (sline.size() == 5) {
                id1   = sline.at(0).toInt();
                id2   = sline.at(1).toInt();
                descr = sline.at(2);
                value = sline.at(3).toDouble();
                error = sline.at(4).toDouble();
            } else if (sline.size() == 4) { // yield
                id1   = sline.at(0).toInt();
                descr = sline.at(1);
                value = sline.at(2).toDouble();
                error = sline.at(3).toDouble();
            } else {
                QMessageBox msg(QMessageBox::Critical, "inputExpYields", Q_FUNC_INFO);
                msg.setInformativeText("Wrong file format");
                msg.exec();
                return;
            }
            name  = getTMName(id1, id2, descr);
            TTMYield * yield = new TTMYield(name, value, error, id1, id2);
            if (value < 1e-15)
                yield->predict();
            addYield(yield);
        }
    }
    file.close();
}

//__________________________________________________________________________
void TTMThermalFit::listMinuitInfo() const
{
    double amin, edm, errdef;
    int nvpar, nparx, icstat;
    QMinuit::instance().qmnstat(amin, edm, errdef, nvpar, nparx, icstat);

    QString comment;
    if(icstat == 0)
        comment = "Covariance matrix not calculated";
    else if(icstat == 1)
        comment ="Covariance matrix approximated only - not accurate";
    else if(icstat == 2)
        comment = "Full covariance matrix calculated but forced positive definite";
    else if(icstat == 3)
        comment = "Full accurate covariance matrix calculated";

    QStandardItemModel* model = new QStandardItemModel;
    QStringList headers;
    headers << "FCN" << "EDM" << "Errdef" << comment;
    model->setHorizontalHeaderLabels(headers);
    QList<QStandardItem*> onerow;
    onerow << new QStandardItem(QString::number(amin)) <<
              new QStandardItem(QString::number(edm))  <<
              new QStandardItem(QString::number(errdef));
    model->appendRow(onerow);
    QMinuit::instance().qmnprin(2, amin);
    QMinuit::instance().qmnmatu(1);
    QTableView* view = new QTableView;
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->setModel(model);
    view->show();
}

//__________________________________________________________________________
void TTMThermalFit::listYields(bool debug)
{
    // Lists the experimental values and model predictions
    QStandardItemModel* model = new QStandardItemModel();
    QStringList header;
    header.append("Name");
    header.append("Experimental Yield");
    header.append("Experimental Error");
    header.append("Fit Yield");
    header.append("Fit Error");
    header.append("Model Yield");
    header.append("Model Error");
    header.append("Model std dev");
    header.append("Model quad dev");
    model->setHorizontalHeaderLabels(header);
    for (TTMYield* yield : mYields) {
        QList<QStandardItem*> onerow;
        QString name  = yield->objectName();
        double  value = yield->getExpValue();
        double  error = yield->getExpError();
        if (debug) {
            qInfo() << QString("%1").arg(name, 20);
            qInfo() << QString("%1: %2 +/- %3").arg("experiment", 30).
                       arg(value, 12, 'e', 6).
                       arg(error, 12, 'e', 6);
        } else {
            onerow << new QStandardItem(yield->objectName());
            QStandardItem* valueItem = new QStandardItem();
            valueItem->setText(QString::number(value));
            valueItem->setTextAlignment(Qt::AlignRight);
            onerow << valueItem;

            QStandardItem* errorItem = new QStandardItem();
            errorItem->setText(QString("+/- %1").arg(error));
            errorItem->setTextAlignment(Qt::AlignRight);
            onerow << errorItem;

            QStandardItem* dash1Item = new QStandardItem();
            dash1Item->setText("-");
            dash1Item->setTextAlignment(Qt::AlignCenter);
            onerow << dash1Item;

            QStandardItem* dash2Item = new QStandardItem();
            dash2Item->setText("-");
            dash2Item->setTextAlignment(Qt::AlignCenter);
            onerow << dash2Item;
        }
        value = yield->getModelValue();
        if (value != 0.) {
            error       = yield->getModelError();
            double std  = yield->getStdDev();
            double quad = yield->getQuadDev();
            if (debug) {
                qInfo() << QString("%1: %2 +/- %3").arg("model", 30).
                           arg(value, 12, 'e', 6).
                           arg(error, 12, 'e', 6);
                qInfo() << QString("%1: %2  %3: %4").arg("Std.Dev.", 30).arg(std, 5).
                           arg("Quad.Dev", 30).arg(quad, 5);
                continue;
            } else {
                QStandardItem* valueItem = new QStandardItem();
                valueItem->setText(QString::number(value));
                valueItem->setTextAlignment(Qt::AlignRight);
                onerow << valueItem;

                QStandardItem* errorItem = new QStandardItem();
                errorItem->setText(QString("+/- %1").arg(error));
                errorItem->setTextAlignment(Qt::AlignRight);
                onerow << errorItem;

                QStandardItem* stdItem = new QStandardItem();
                stdItem->setText(QString::number(std));
                stdItem->setTextAlignment(Qt::AlignRight);
                onerow << stdItem;

                QStandardItem* quadItem = new QStandardItem();
                quadItem->setText(QString::number(quad));
                quadItem->setTextAlignment(Qt::AlignRight);
                onerow << quadItem;
            }
        } else {
            QStandardItem* dash1Item = new QStandardItem();
            dash1Item->setText("-");
            dash1Item->setTextAlignment(Qt::AlignCenter);
            onerow << dash1Item;

            QStandardItem* dash2Item = new QStandardItem();
            dash2Item->setText("-");
            dash2Item->setTextAlignment(Qt::AlignCenter);
            onerow << dash2Item;

            QStandardItem* dash3Item = new QStandardItem();
            dash3Item->setText("-");
            dash3Item->setTextAlignment(Qt::AlignCenter);
            onerow << dash3Item;

            QStandardItem* dash4Item = new QStandardItem();
            dash4Item->setText("-");
            dash4Item->setTextAlignment(Qt::AlignCenter);
            onerow << dash4Item;
        }
        model->appendRow(onerow);
    }
    if (!debug) {
        if (mYieldView == nullptr)
            mYieldView = new QTableView;
        mYieldView->setAttribute(Qt::WA_DeleteOnClose);
        mYieldView->setModel(model);
        mYieldView->resize(1000, 200);
        mYieldView->resizeColumnsToContents();
        mYieldView->setAlternatingRowColors(true);
        mYieldView->show();
    }
    QString textData;
    int rows = model->rowCount();
    int columns = model->columnCount();

    // save results in a csv file

    for (QString sHeader : header) {
        textData +=  sHeader;
        textData += ", ";
    }
    textData += "\n";

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {

                textData += model->data(model->index(i,j)).toString();
                textData += ", ";      // for .csv file format
        }
        textData += "\n";             // (optional: for new line segmentation)
    }

    // [Save to file] (header file <QFile> needed)
    // .csv
    ;
    QDir::setCurrent("/tmp");
    QFile csvFile(Logger::instance().logFileName() + ".csv");
    if (csvFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QTextStream out(&csvFile);
        out << textData;
        csvFile.close();
        QMessageBox msg(QMessageBox::Information, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(QString("Results saved as csv in %1").arg(csvFile.fileName()));
    }
}

//__________________________________________________________________________
void TTMThermalFit::removeYield(int id1, int id2, const QString &descr)
{
    // remove the given yield
    QString name = getTMName(id1, id2, descr);
    auto yield = std::find_if(mYields.begin(), mYields.end(), [name](TTMYield* y) {return y->objectName() == name;});
    if (yield != mYields.end()) {
        mYields.removeOne(*yield);
    } else {
        QMessageBox msg(QMessageBox::Critical, "removeYield", Q_FUNC_INFO);
        msg.setInformativeText(QString("Yield %1 does not exist").arg(name));
        msg.exec();
    }
}

//__________________________________________________________________________
void TTMThermalFit::sortYields()
{
    // sort yields according ascending mass
    std::sort(mYields.begin(), mYields.end(), [this] (TTMYield* y1, TTMYield* y2) { return compareYield(y1, y2);});
}
