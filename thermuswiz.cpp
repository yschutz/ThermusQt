// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#include "macros/runmacro.h"
#include "filedialog.h"
#include "parasel.h"
#include "summary.h"
#include "thermuswiz.h"

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

    show();

    QAbstractButton *donebutton = button(QWizard::FinishButton);
    mLoop.connect(donebutton, SIGNAL(clicked(bool)), this, SLOT(accept()));

    QAbstractButton *nextbutton = button(QWizard::NextButton);
    mLoop.connect(nextbutton, SIGNAL(clicked(bool)), this, SLOT(next()));


    mLoop.exec();
}

//__________________________________________________________________________
void ThermusWiz::accept()
{
    // finish (done) button pressed
    mLoop.exit(0);

}

//__________________________________________________________________________
void ThermusWiz::next()
{
    // fills the summary page each time a page is filled (next button pressed)

    if (currentId() -1 == mDialogId) {
        FileDialog * file = (FileDialog*)page(mDialogId);
        for (qint32 index = 0; index < file->getRadioButtons().size(); index++) {
            QRadioButton * but = file->getRadioButtons().at(index);
            if (but->isChecked())
              file->setFileName(but->text());
        }

        ((Summary*)page(mSummaryId))->updateFileName(file->getFileName());
    }
}
