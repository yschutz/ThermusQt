// Enables to run fiiting procedure in a thread
// Author: Yves Schutz, November 2018

#include "fittingthread.h"
#include "TTMThermalFit.h"

#include <QDebug>
#include <QTimer>

//__________________________________________________________________________
FittingThread::FittingThread(TTMThermalFit *fit)
{
    mFit = fit;
}

//__________________________________________________________________________
void FittingThread::run()
{
    mFit->fitData();
    emit resultReady();
}
