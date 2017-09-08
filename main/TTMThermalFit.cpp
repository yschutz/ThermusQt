// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Novembre 2016
//__________________________________________________________________________
// Base class for all thermal fit classes.
//

#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>

#include "TTMThermalFit.h"

#include "external/particlesdbmanager.h"

//__________________________________________________________________________
TTMThermalFit::TTMThermalFit(QObject *parent) : QObject(parent),
    mChiSquare(0.0), mMinuit(nullptr), mQuadDev(0.0)
{
    // ctor
}

//__________________________________________________________________________
TTMThermalFit::~TTMThermalFit()
{
    // dtor
    qDeleteAll(mYields.begin(), mYields.end());
    mYields.clear();
    delete mMinuit; //carefull in case QMinuit becomes a singleton
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
void TTMThermalFit::generateYields()
{
    // Calculates the primordial particle densities of all particles in the
     // set, then iterates through the list of yields, calculates
     // their decay contributions and inserts the new model
     // predictions into the TTMYield objects, and calculates chi-squared
     // and the quadratic deviation.
     //

    mChiSquare = mQuadDev = 0.0;
}

//__________________________________________________________________________
QString TTMThermalFit::getTMName(int id1, int id2, const QString& descr) const
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
         name = ParticlesDBManager::Instance().name(id1, "ThermusParticles");
       }
     } else {
       if(id1 == 3130) {
         name = "<";
         name = name.append(ParticlesDBManager::Instance().name(313, "ThermusParticles"));
         name = name.append(">").append("/").append(ParticlesDBManager::Instance().name(id2, "ThermusParticles"));
       } else if(id1 == 33340){
         name = name.append("Omega + anti-Omega");
         name = name.append("/").append(ParticlesDBManager::Instance().name(id2, "ThermusParticles"));
       } else if(id2 == 2) {
         name = ParticlesDBManager::Instance().name(id1, "ThermusParticles");
         name = name.append("/").append("h-");
       } else if(id2 == 3){
         name = ParticlesDBManager::Instance().name(id1, "ThermusParticles");
         name = name.append("/").append("h+");
       } else if(id2 == 33340){
         name = ParticlesDBManager::Instance().name(id1, "ThermusParticles");
         name = name.append("/").append("Omega + anti-Omega");
       } else {
         name = ParticlesDBManager::Instance().name(id1, "ThermusParticles");
         name = name.append("/").append(ParticlesDBManager::Instance().name(id2, "ThermusParticles"));
       }
     }
     if(!descr.isEmpty()) {
       name.append(" ").append(descr);
     }
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
void TTMThermalFit::inputExpYields(const QString &fileName)
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
//            yield->SetPartSet(fPartSet,fPartSet);
            addYield(yield);
        }
    }
    file.close();
}

//__________________________________________________________________________
void TTMThermalFit::listMinuitInfo() const
{
    if(mMinuit){

        double amin, edm, errdef;
        int nvpar, nparx, icstat;
        mMinuit->qmnstat(amin, edm, errdef, nvpar, nparx, icstat);

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
        mMinuit->qmnprin(2, amin);
        mMinuit->qmnmatu(1);
      } else {
        QMessageBox msg(QMessageBox::Warning, "ListMinuitInfo", Q_FUNC_INFO);
        msg.setInformativeText("Run FitData() to instantiate a QMinuit object");
        msg.exec();
      }
}

//__________________________________________________________________________
void TTMThermalFit::listYields() const
{
    // Lists the experimental values and model predictions
    QStandardItemModel* model = new QStandardItemModel();
    QStringList header;
    header.append("Name");
    header.append("Predicted Yield");
    header.append("Predicted Error");
    header.append("Fit Yield");
    header.append("Fit Error");
    header.append("Model Yield");
    header.append("Model Error");
    header.append("Model std dev");
    header.append("Model quad dev");
    model->setHorizontalHeaderLabels(header);
    for (TTMYield* yield : mYields) {
        QList<QStandardItem*> onerow;
        onerow << new QStandardItem(yield->objectName());
        double value = yield->getExpValue();
        double error = yield->getExpError();
        if (yield->isFitted()) {
            onerow << new QStandardItem("-") << new QStandardItem("-") <<
                      new QStandardItem(QString::number(value)) << new QStandardItem(QString::number(error)) <<
                      new QStandardItem("-") << new QStandardItem("-") << new QStandardItem("-") << new QStandardItem("-");
        } else {
            onerow << new QStandardItem(QString::number(value)) << new QStandardItem(QString::number(error)) <<
                      new QStandardItem("-") << new QStandardItem("-") <<
                      new QStandardItem("-") << new QStandardItem("-") << new QStandardItem("-") << new QStandardItem("-");
        }
        value = yield->getModelValue();
        if (value != 0.) {
            error       = yield->getModelError();
            double std  = yield->getStdDev();
            double quad = yield->getQuadDev();
            onerow << new QStandardItem("-") << new QStandardItem("-") << new QStandardItem("-") << new QStandardItem("-");
            onerow << new QStandardItem(QString::number(value)) << new QStandardItem(QString::number(error)) <<
                      new QStandardItem(QString::number(std)) << new QStandardItem(QString::number(quad));
        }
        model->appendRow(onerow);
    }
    QTableView* view = new QTableView;
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->setModel(model);
    view->resize(1000, 200);
    view->show();
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
