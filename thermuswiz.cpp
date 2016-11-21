// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#include "macros/prediction.h"
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

    Prediction& myMacro = Prediction::instance();

    // page to select the particles list file

    mDialog = new FileDialog(this);
    mDialogId = addPage(mDialog);
    myMacro.setDialog(mDialog);

    // page for the parameters setting

    mParasel = new ParaSel(this);
    addPage(mParasel);
    myMacro.setParaSel(mParasel);

    // page summarizing evrything

    mSummary = new Summary(this);
    mSummary->setSubTitle(summaryTitle);
    mSummaryId = addPage(mSummary);
    myMacro.setSummary(mSummary);

    show();

    QAbstractButton *donebutton = button(QWizard::FinishButton);
    mLoop.connect(donebutton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    mLoop.exec();
}

//__________________________________________________________________________
void ThermusWiz::accept()
{
    mLoop.exit(0);

    for (qint32 index = 0; index < mDialog->getRadioButtons().size(); index++) {
        if (mDialog->getRadioButtons().at(index)->isChecked()) {
            mDialog->setFileName(mDialog->getRadioButtons().at(index)->text());
            break;
        }
    }
    Prediction::instance().selectParticlesListFile();
}
