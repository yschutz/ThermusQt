// Author: Yves Schutz February 2018
//
// Base class for the macros to run prediction or fit

#include <QFileDialog>

#include "logger.h"
#include "macrointerface.h"
#include "macroparasel.h"
#include "parasel.h"
#include "particlesdbmanager.h"
#include "thermuswiz.h"


//__________________________________________________________________________
bool MacroInterface::init(bool fit, double Tch, double muB, double muS, double muQ, double gammaS, double radius, double cradius, double muC, double gammaC, double muBeauty, double gammaBeauty,
                          bool bsq, bool qstat, bool width, bool exclvol,
                          bool fitHyp, bool fitPro, bool fitRes, bool fitNuc, const QString &data)
{
    // initializes things which should be transparent to the user

    mFit = fit;

    bool rv = true;
    ThermusWiz *wiz = new ThermusWiz("Custom");
    mMacroParaSel = (MacroParaSel*)wiz->page(wiz->macroparaselId());
    mParaSel      = qobject_cast<ParaSel*>(wiz->page(wiz->paraselId()));

    // ==== Set default parameters =======

    mParaSel->setParaValue(TTMParameterSet::kT,           Tch);
    mParaSel->setParaValue(TTMParameterSet::kMUQ,         muQ);
    mParaSel->setParaValue(TTMParameterSet::kMUB,         muB);
    mParaSel->setParaValue(TTMParameterSet::kMUS,         muS);
    mParaSel->setParaValue(TTMParameterSet::kMUC,         muC);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAS,      gammaS);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAC,      gammaC);
    mParaSel->setParaValue(TTMParameterSet::kRADIUS,      radius);
    mParaSel->setParaValue(TTMParameterSet::kCRADIUS,     cradius);
    mParaSel->setParaValue(TTMParameterSet::kMUBEAUTY,    muBeauty);
    mParaSel->setParaValue(TTMParameterSet::kGAMMABEAUTY, gammaBeauty);

    mParaSel->setFitValues(TTMParameterSet::kT, 0.130, 0.190, 0.0001);
    mParaSel->fixParameter(TTMParameterSet::kMUB);
    mParaSel->fixParameter(TTMParameterSet::kGAMMAS);
    mParaSel->fixParameter(TTMParameterSet::kGAMMABEAUTY);
    mParaSel->setFitValues(TTMParameterSet::kRADIUS, 1.0, 15.0, 0.01);
    mParaSel->fixParameter(TTMParameterSet::kMUQ);
    mParaSel->fixParameter(TTMParameterSet::kMUS);

    mParaSel->constrainParameter(TTMParameterSet::kMUQ);

    mMacroParaSel->setModelBSQ(bsq);
    mMacroParaSel->setQstat(qstat);
    mMacroParaSel->setWidth(width);
    mMacroParaSel->setExclVol(exclvol);

    mMacroParaSel->setFitParticles(fitHyp, fitPro, fitRes, fitNuc);

    mMacroParaSel->setData(data);

    wiz->show();
    wiz->startLoop();

    setParameters();

    // connect to the Thermus DB
    QString thermusDBName(QString(ParticlesDBManager::instance().getThermusDBName()).append(".db"));
    QString thermusDBPath(qApp->applicationDirPath() + "/../Resources/particles/");
    rv *= ParticlesDBManager::instance().connect(thermusDBPath + thermusDBName);

    setFit();

    QDateTime start = QDateTime::currentDateTime();
    QDate date = start.date();
    QTime time = start.time();
    QString info = QString(" *** Start at : %1 Time : %2").arg(date.toString("dd MMMM yyyy")).arg(time.toString());
    qInfo() << info;

    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    mBusytics = 0;

    return rv;
}

//__________________________________________________________________________
void MacroInterface::run()
{
    localInit(); // do not remove

    if (!isInitialized()) {
        QMessageBox::information(nullptr, Q_FUNC_INFO, "intitialization failed");
    }
    else {
        mFitInfo->generateYields();
        if (!mFit) {
            mFitInfo->listYields();
            qInfo() << "******* Prediction:" << mMacroParaSel->getTitle() << "******* ";
        } else {
            qInfo() << "******* Fitting:" << mMacroParaSel->getTitle() << "******* ";

            mFT = new FittingThread(mFitInfo);
            connect(mFT, &FittingThread::resultReady, this, [this]{wrapUp();});
            mBusy = new QMessageBox(QMessageBox::Information, Q_FUNC_INFO, "FIT");
            mBusy->setText(QString("%1: is fitting").arg(Q_FUNC_INFO));
            mBusy->setStandardButtons(QMessageBox::Abort);
            mBusy->setInformativeText("Busy");
            mTimer->start(2500);
            mFT->start();
            int ret = mBusy->exec();
            switch (ret) {
            case QMessageBox::Abort:
                mFT->blockSignals(true);
                mFT->terminate();
                mTimer->stop();
                break;
            case QMessageBox::Ok:
                mBusy->close();
                break;
            default:
                break;
            }
        }
    }
}

//__________________________________________________________________________
void MacroInterface::setConstrain()
{
    // option of adding constraints. b/2Q, S, C and B density

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        if (mParaSel->isConstrained((TTMParameterSet::ParameterType)type))
            mParaInfo->constrain((TTMParameterSet::ParameterType)type, mParaSel->getConstrainValue((TTMParameterSet::ParameterType)type));
    }
}

//__________________________________________________________________________
void MacroInterface::setFit()
{
    QString name = mMacroParaSel->dataFileName();
    QFile f(name);
    if (name.isEmpty() || ! f.exists()) {
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText(QString("data file not selected or %1 does not exist").arg(name));
        msg.exec();
        QFileDialog diag(nullptr, "Select a data file",  QDir::currentPath(), "Text Files (*.txt)");
        diag.setAcceptMode(QFileDialog::AcceptOpen);
        diag.setOption(QFileDialog::DontUseNativeDialog, true);
        if (diag.exec())
            name = diag.selectedFiles().at(0);
    }

    //    if (mMacroParaSel->isModelBSQ())
    mFitInfo = new TTMThermalFitBSQ(mParaInfo, name);
    //    else
    //    mFitInfo = new TTMThermalFitBQ(this);

    if (mMacroParaSel->isQstat())
        mFitInfo->setQStats(true);
    if (mMacroParaSel->isWidth())
        mFitInfo->setWidth(true);
    if (mMacroParaSel->isExclVol())
        mFitInfo->setExclVol(true);

    if (mMacroParaSel->isYields()) {
        if (!mMacroParaSel->isNucleiFitted()) {
            for (int id : mNuclei)
                mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
        if (!mMacroParaSel->isHyperonsFitted()) {
            for (int id : mHyperons)
                mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
        if (!mMacroParaSel->isProtonFitted()) {
            for (int id : mProton)
                mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
        if (!mMacroParaSel->isResonancesFitted()) {
            for (int id : mResonances)
                mFitInfo->getYield(id,  0, "ALICE")->predict();
        }
    }
}

//__________________________________________________________________________
void MacroInterface::setFitFix()
{
    // fix/free the parameters

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        if (mParaSel->isFixed((TTMParameterSet::ParameterType)type))
            mParaInfo->fix((TTMParameterSet::ParameterType)type, mParaSel->getParaValue((TTMParameterSet::ParameterType)type));
        else
            mParaInfo->fit((TTMParameterSet::ParameterType)type,
                           mParaSel->getParaValue((TTMParameterSet::ParameterType)type),
                           mParaSel->getFitMin((TTMParameterSet::ParameterType)type),
                           mParaSel->getFitMax((TTMParameterSet::ParameterType)type),
                           mParaSel->getFitSte((TTMParameterSet::ParameterType)type));
    }
}

//__________________________________________________________________________
void MacroInterface::setParameters()
{
    // set the parameters
    //    if (mMacroParaSel->isModelBSQ())
    mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(TTMParameterSet::kT),
                                       mParaSel->getParaValue(TTMParameterSet::kMUB),
                                       mParaSel->getParaValue(TTMParameterSet::kMUS),
                                       mParaSel->getParaValue(TTMParameterSet::kMUQ),
                                       mParaSel->getParaValue(TTMParameterSet::kGAMMAS),
                                       mParaSel->getParaValue(TTMParameterSet::kRADIUS),
                                       mParaSel->getParaValue(TTMParameterSet::kMUC),
                                       mParaSel->getParaValue(TTMParameterSet::kGAMMAC),
                                       mParaSel->getParaValue(TTMParameterSet::kMUBEAUTY),
                                       mParaSel->getParaValue(TTMParameterSet::kGAMMABEAUTY),
                                       mParaSel->getB2Q());
    //    else
    //        mParaInfo = new TTMParameterSetBSQ(mParaSel->getParaValue(TTMParameterSet::kT),
    //                                           mParaSel->getParaValue(TTMParameterSet::kMUB),
    //                                           mParaSel->getParaValue(TTMParameterSet::kMUQ),
    //                                           mParaSel->getParaValue(TTMParameterSet::kGAMMAS),
    //                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
    //                                           mParaSel->getParaValue(TTMParameterSet::kRADIUS),
    //                                           mParaSel->getB2Q());

    mParaInfo->setParent(nullptr);

    // choice of parameter to fit or to fix
    setFitFix();

    // option of adding some constraint
    setConstrain();

    // list all settings
    Logger::instance().setLogFileName("Thermus macro" + mMacroParaSel->getTitle());
    QMessageBox msg(QMessageBox::Information, Q_FUNC_INFO, Q_FUNC_INFO);
    msg.setInformativeText(QString("Log file is: %1").arg(Logger::instance().logFileName()));
    msg.exec();

    listParameters();
    mMacroParaSel->list();
}

//__________________________________________________________________________
void MacroInterface::timeout()
{
    // refresh mBusy
    mBusy->setInformativeText(mBusy->informativeText() + '.');
    mBusytics++;
}

//__________________________________________________________________________
void MacroInterface::wrapUp()
{
    // todo list when fitting process ended
    localWrapup();
    mTimer->stop();
    delete mFT;
    mBusy->setInformativeText(QString("Done in %1 s").arg(mBusytics * mTimer->interval() / 1000.));
    mBusy->setStandardButtons(QMessageBox::Ok);
    mFitInfo->listYields();
    mFitInfo->getParameterSet()->list();
    delete mTimer;
}
