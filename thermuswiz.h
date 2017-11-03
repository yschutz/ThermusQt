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
    ThermusWiz(const QString& what, QWidget *parent = 0);

    void    cleanupPage(int id);
    int     getSummaryId() const { return mSummaryId; }
    void    initializePage(int id);

public slots:
    void accept();

private:
    QEventLoop  mLoop;            // waiting for done button to be pushed
    int      mParaSelId;       // Id of the parameter selection page
    int      mMacrooParaSelId; // Id of the parameter selection page
    int      mSummaryId;       // Id of the summary page
};

#endif // THERMUSWIZ_H
