// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#include "macros/prediction.h"
#include "filedialog.h"
#include "parasel.h"
#include "thermuswiz.h"

//__________________________________________________________________________
ThermusWiz::ThermusWiz(QWidget *parent) : QWizard(parent)
{
    // ctor

    setGeometry(1000, 500, 1000, 100);
    setWindowTitle("THERMUS setting wizard");
    setWizardStyle(QWizard::ModernStyle);

//    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/Icons/Icons/Matrix2.jpg"));
//    setPixmap(QWizard::LogoPixmap, QPixmap(":/Icons/Icons/RC30-icon-48x48.png"));
//    //setPixmap(QWizard::BannerPixmap, QPixmap(":/Icons/Icons/NegativeSmile.png"));
//    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/Icons/Icons/RC30Controller.JPG"));

    Prediction& myMacro = Prediction::instance();

    QWizard *predictionWiz = new QWizard(this);
    mDialog = new FileDialog(this);
    predictionWiz->addPage(mDialog);
    myMacro.setDialog(mDialog);


    mParasel = new ParaSel(this);
    predictionWiz->addPage(mParasel);
    myMacro.setParaSel(mParasel);
    myMacro.selectDefaultParameters();

    predictionWiz->show();

    QAbstractButton *donebutton = predictionWiz->button(QWizard::FinishButton);
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
