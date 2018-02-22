// Enables to run fiiting procedure in a thread
// Author: Yves Schutz, November 2018

#ifndef FITTINGTHREAD_H
#define FITTINGTHREAD_H

#include <QObject>
#include <QThread>

class QTimer;
class TTMThermalFit;
class FittingThread : public QThread
{
    Q_OBJECT
public:    
    void run() override;
    FittingThread(TTMThermalFit* fit);

signals:
    void resultReady();

private:
    TTMThermalFit* mFit;   // the fit model
};

#endif // FITTINGTHREAD_H
