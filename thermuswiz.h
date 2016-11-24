// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#ifndef THERMUSWIZ_H
#define THERMUSWIZ_H

#include <QEventLoop>
#include <QWizard>


class ThermusWiz : public QWizard
{
    Q_OBJECT

public:
    ThermusWiz(QString Title = " ", QWidget *parent = 0);

    void    cleanupPage(qint32 id);
    qint32  getDialogId()  const { return mDialogId; }
    qint32  getSummaryId() const { return mSummaryId; }
    void    initializePage(qint32 id);

public slots:
    void accept();

private:
    qint32      mDialogId;  // Id of the Particles list file selection page
    QEventLoop  mLoop;      // waiting for done button to be pushed
    qint32      mParaSelId; // Id of the parameter selection page
    qint32      mSummaryId; // Id of the summary page
};

#endif // THERMUSWIZ_H
