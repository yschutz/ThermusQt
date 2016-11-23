// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#include "macros/runmacro.h"
#include "filedialog.h"
#include "parasel.h"
#include "summary.h"
#include "thermuswiz.h"

#include <QDesktopServices>

//__________________________________________________________________________
ThermusWiz::ThermusWiz(QString summaryTitle, QWidget *parent) : QWizard(parent)
{
    // ctor

    setGeometry(200, 400, 1000, 100);
    setWindowTitle("THERMUS setting wizard");
    setWizardStyle(QWizard::ModernStyle);

//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/Icons/Icons/Matrix2.jpg"));
//    setPixmap(QWizard::LogoPixmap, QPixmap(":/Icons/Icons/RC30-icon-48x48.png"));
//    //setPixmap(QWizard::BannerPixmap, QPixmap(":/Icons/Icons/NegativeSmile.png"));
//    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/Icons/Icons/RC30Controller.JPG"));

    RunMacro& myMacro = RunMacro::instance();

    // page to select the particles list file

    mDialogId = addPage(new FileDialog(this));
    myMacro.setDialog((FileDialog*)page(mDialogId));

    // page for the parameters setting

    mParaSelId = addPage(new ParaSel(this));
    myMacro.setParaSel((ParaSel*)page(mParaSelId));

    // page summarizing evrything

//    mSummary->setSubTitle(summaryTitle);
    mSummaryId = addPage(new Summary(this));
    myMacro.setSummary((Summary*)page(mSummaryId));
    ((Summary*)page(mSummaryId))->setSubTitle(summaryTitle);

    show();

    mLoop.exec();
}

//__________________________________________________________________________
void ThermusWiz::cleanupPage(qint32 /*id*/)
{
    // called when back button is hit
    // implemented here to prevent clearing the fields of the page

}

//__________________________________________________________________________
void ThermusWiz::initializePage(qint32 id)
{
    Summary * summary = (Summary*)page(mSummaryId);

    if (id - 1 == mDialogId) {
        FileDialog * file = qobject_cast<FileDialog*>(page(mDialogId));
        for (qint32 index = 0; index < file->getRadioButtons().size(); index++) {
            QRadioButton * but = file->getRadioButtons().at(index);
            if (but->isChecked())
              file->setFileName(but->text());
        }

        summary->updateFileName(file->getFileName());
    } else if (id - 1 == mParaSelId) {
        summary->updateParameters();
    }
}

//__________________________________________________________________________
void ThermusWiz::accept()
{
    // finish (done) button pressed

    mLoop.exit(0);
    close();
}
