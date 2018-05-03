// Enables to run fitting procedure in a thread
// Author: Yves Schutz, November 2017

#ifndef FITTINGTHREAD_H
#define FITTINGTHREAD_H

#include <QObject>
#include <QThread>

class TTMThermalFit;
class FittingThread : public QThread
{
    Q_OBJECT
public:    
    FittingThread(TTMThermalFit* fit);
    void run() override;

signals:
    void resultReady();

private:
    TTMThermalFit* mFit;   // the fit model
};

#endif // FITTINGTHREAD_H
