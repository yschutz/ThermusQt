// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#ifndef THERMUSWIZ_H
#define THERMUSWIZ_H

#include <QEventLoop>
#include <QWizard>

#include "filedialog.h"
#include "parasel.h"
#include "summary.h"

class ThermusWiz : public QWizard
{
    Q_OBJECT

public:
    ThermusWiz(QString Title = " ", QWidget *parent = 0);

    qint32 getSummaryId() const { return mSummaryId; }

public slots:
    void accept();

private:
    FileDialog* mDialog;    // the file dialog window
    QEventLoop  mLoop;      // waiting for done button to be pushed
    ParaSel*    mParasel;   // the parameters settings window
    Summary*    mSummary;   // the summary of settings
    qint32      mSummaryId; // Id of the summary page
};

#endif // THERMUSWIZ_H
