// Author: Yves Schutz octobre 2016
//
// The wizard to setp up various things

#include "predictionmacro.h"
#include "fitmacro.h"

#include "macroparasel.h"
//#include "mainwindow.h"
#include "parasel.h"
#include "summary.h"
#include "thermuswiz.h"

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>

//__________________________________________________________________________
ThermusWiz::ThermusWiz(const QString &what, QWidget *parent) : QWizard(parent)
{
    // ctor
    QRect screenSize = QApplication::desktop()->screenGeometry();
    setMinimumSize(QSize(screenSize.width() / 2.0, screenSize.height() / 2.0));
    setMaximumSize(QSize(screenSize.width(), screenSize.height()));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
    setWindowTitle("THERMUS setting wizard");
    setWizardStyle(QWizard::ModernStyle);

    //    setPixmap(QWizard::WatermarkPixmap, QPixmap(":/Icons/Icons/Matrix2.jpg"));
    //    setPixmap(QWizard::LogoPixmap, QPixmap(":/Icons/Icons/RC30-icon-48x48.png"));
    //    //setPixmap(QWizard::BannerPixmap, QPixmap(":/Icons/Icons/NegativeSmile.png"));
    //    setPixmap(QWizard::BackgroundPixmap, QPixmap(":/Icons/Icons/RC30Controller.JPG"));


    // page for the parameters setting
    mParaSelId = addPage(new ParaSel(this));

    // page for the macro parameters setting
    mMacroParaSelId = addPage(new MacroParaSel(this));

    // page summarizing evrything

    //    mSummary->setSubTitle(summaryTitle);
    //    mSummaryId = addPage(new Summary(this));
    //    myMacro.setSummary((Summary*)page(mSummaryId));
    //    ((Summary*)page(mSummaryId))->setSubTitle(summaryTitle);

    Macro* myMacro = nullptr;
    if (what == "Prediction")
        myMacro = &PredictionMacro::instance();
    else if (what == "Fit")
        myMacro = &FitMacro::instance();

    if (myMacro) {
        myMacro->setParaSel((ParaSel*)page(mParaSelId));
        myMacro->setMacroParaSel((MacroParaSel*)page(mMacroParaSelId));
        show();
        mLoop.exec();
    }
}

//__________________________________________________________________________
void ThermusWiz::cleanupPage(int /*id*/)
{
    // called when back button is hit
    // implemented here to prevent clearing the fields of the page

}

//__________________________________________________________________________
void ThermusWiz::initializePage(int /*id*/)
{
    // called when the next button is hit

    //Summary * summary = (Summary*)page(mSummaryId);
    //MacroParaSel* macropara = (MacroParaSel*)page(mMacrooParaSelId);

    //    if (id - 1 == mDialogId) {
    //        FileSel * file = qobject_cast<FileSel*>(page(mDialogId));
    //        for (int index = 0; index < file->getRadioButtons().size(); index++) {
    //            QRadioButton * but = file->getRadioButtons().at(index);
    //            if (but->isChecked())
    //                file->setFileName(but->text());
    //        }
    //        summary->updateFileName(file->getFileName());
    //    } else if (id - 1 == mParaSelId) {
    //        summary->updateParameters();
    //    }
//    if (id - 1 == mParaSelId)
//        macropara->list();

}

//__________________________________________________________________________
void ThermusWiz::accept()
{
    // finish (done) button pressed

    mLoop.exit(0);
    close();
//    qobject_cast<MainWindow*>(parentWidget())->createConsol();
}
