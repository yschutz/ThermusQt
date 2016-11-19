// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#ifndef THERMUSWIZ_H
#define THERMUSWIZ_H

#include <QEventLoop>
#include <QWizard>

#include "filedialog.h"
#include "parasel.h"
class ThermusWiz : public QWizard
{
    Q_OBJECT

public:
    ThermusWiz(QWidget *parent = 0);

public slots:
    void accept();

private:
    FileDialog* mDialog;   // the file dialog window
    QEventLoop  mLoop;     // waiting for done button to be pushed
    ParaSel*    mParasel;  // the parameters selection window
};

#endif // THERMUSWIZ_H
