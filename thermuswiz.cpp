// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#include "macros/runmacro.h"
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

}
