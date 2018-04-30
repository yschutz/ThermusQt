// Author: Yves Schutz novembre 2017
//
// The steering class to run a thermus fit (aka ROOT macro)

#include <QDateTime>
#include <QMessageBox>
#include <QTimer>

#include "fitmacro.h"
#include "fittingthread.h"
#include "macroparasel.h"
#include "QMinuit.h"
#include "TTMThermalFitBSQ.h"

#include "../PlotLib/plot.h"


FitMacro FitMacro::mInstance = FitMacro();

//__________________________________________________________________________
FitMacro &FitMacro::instance()
{
    // returns the unique instance

    return mInstance;
}

//__________________________________________________________________________
void FitMacro::setDefaultParameters()
{
    // ==== Starting parameters ======= (nucl-ex/0403014)

    double TchStart    =  0.150;   // GeV
    double muBStart    =  0.0001;  // GeV
    double muSStart    =  0.0;     // GeV
    double muQStart    =  0.0;     // GeV

    double gammaSStart =  1.0;   // 0.86

    double radiusStart  = 3.0;    // units = fm but no dependence for Grand Canonical.
    double cradiusStart = 0.0;

    double muCStart         = 0.0;
    double gammaCStart      = 1.0;
    double muBeautyStart    = 0.0;
    double gammaBeautyStart = 0.0;

    // ==== Start parameters =======

    mParaSel->setParaValue(TTMParameterSet::kT,           TchStart);
    mParaSel->setParaValue(TTMParameterSet::kMUQ,         muQStart);
    mParaSel->setParaValue(TTMParameterSet::kMUB,         muBStart);
    mParaSel->setParaValue(TTMParameterSet::kMUS,         muSStart);
    mParaSel->setParaValue(TTMParameterSet::kMUC,         muCStart);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAS,      gammaSStart);
    mParaSel->setParaValue(TTMParameterSet::kGAMMAC,      gammaCStart);
    mParaSel->setParaValue(TTMParameterSet::kRADIUS,      radiusStart);
    mParaSel->setParaValue(TTMParameterSet::kCRADIUS,     cradiusStart);
    mParaSel->setParaValue(TTMParameterSet::kMUBEAUTY,    muBeautyStart);
    mParaSel->setParaValue(TTMParameterSet::kGAMMABEAUTY, gammaBeautyStart);

    mParaSel->setFitValues(TTMParameterSet::kT, 0.130, 0.190, 0.0001);
    mParaSel->fixParameter(TTMParameterSet::kMUB);
    mParaSel->fixParameter(TTMParameterSet::kGAMMAS);
    mParaSel->fixParameter(TTMParameterSet::kGAMMABEAUTY);
    mParaSel->setFitValues(TTMParameterSet::kRADIUS, 1.0, 15.0, 0.01);
    mParaSel->fixParameter(TTMParameterSet::kMUQ);
    mParaSel->fixParameter(TTMParameterSet::kMUS);

    mParaSel->constrainParameter(TTMParameterSet::kMUQ);
    mParaSel->updateDisplay();
}

//__________________________________________________________________________
void FitMacro::setMacroDefaultParameters()
{
    // set additional parameters
    bool   bsq    = true;    // thermodynamical model bq (canonical) or bsq (grand canonical)
    bool   qstat  = true;    // apply quantum statistics
    bool   width  = true;    // apply resonance width
    double radius = 0.3;     // radius for the exclusion volume

    // set the default values for the macro parameters

    mMacroParaSel->setModelBSQ(bsq);
    mMacroParaSel->setQstat(qstat);
    mMacroParaSel->setWidth(width);
    mMacroParaSel->setExclVol(radius);
}

//__________________________________________________________________________
void FitMacro::run()
{
    QDateTime start = QDateTime::currentDateTime();
    QDate date = start.date();
    QTime time = start.time();
    QString info = QString(" *** Start at : %1 Time : %2").arg(date.toString("dd MMMM yyyy")).arg(time.toString());
    qInfo() << info;

    qInfo() << "******* Fitting:" << mMacroParaSel->getTitle() << "******* ";

    mFitInfo->generateYields();
//    mFitInfo->listYields();
    //    for (TTMYield* yield : mFitInfo->getYields())
    //        qInfo() << Q_FUNC_INFO << "; PREDICTION: ;" <<
    //                   yield->getID1() << ";" <<
    //                   yield->getID2() << ";" <<
    //                   yield->getTMName() << ";" <<
    //                   yield->getModelValue();
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

//__________________________________________________________________________
void FitMacro::wrapUp()
{
    // todo list when fitting process ended
    mBusy->setInformativeText(QString("Done in %1 s").arg(mBusytics * mTimer->interval() / 1000.));
    mBusy->update();
    const int knpoints = 50;
    const int kpa1     = 1, kpa2 = 0;
    QVector<double> xcoor(knpoints + 1);
    QVector<double> ycoor(knpoints + 1);

    Plot pl("n-σ contours", xcoor.size(), 2.5, 3.5, 0.14, 0.17);
    pl.setAxisTitle(QMinuit::instance().getParameterName(kpa1), "", QMinuit::instance().getParameterName(kpa2));

    QMinuit::instance().setErrorDef(9);
    QMinuit::instance().contour(xcoor, ycoor, knpoints, kpa1, kpa2);
    pl.addGraph("3-σ", xcoor, ycoor);

    QMinuit::instance().setErrorDef(4);
    QMinuit::instance().contour(xcoor, ycoor, knpoints, kpa1, kpa2);
    pl.addGraph("2-σ", xcoor, ycoor);

    QMinuit::instance().setErrorDef(1);
    QMinuit::instance().contour(xcoor, ycoor, knpoints, kpa1, kpa2);
    pl.addGraph("1-σ", xcoor, ycoor);

    pl.draw();

    mTimer->stop();
    delete mFT;
    mBusy->setInformativeText(QString("Done in %1 s").arg(mBusytics * mTimer->interval() / 1000.));
    mBusy->setStandardButtons(QMessageBox::Ok);
    mFitInfo->listYields();
    mFitInfo->getParameterSet()->list();
}

//__________________________________________________________________________
void FitMacro::timeout()
{
    // refresh mBusy
    mBusy->setInformativeText(mBusy->informativeText() + '.');
    mBusytics++;
}

//__________________________________________________________________________
FitMacro::FitMacro(QObject* parent) : Macro(parent), mFT(nullptr)
{
    // ctor
    setObjectName("Fit_Macro_");
}

//__________________________________________________________________________
FitMacro::~FitMacro()
{

}
