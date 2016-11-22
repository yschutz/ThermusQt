#include <QDebug>
#include <QLabel>
#include <QVariant>

#include "parasel.h"
#include "summary.h"

//__________________________________________________________________________
Summary::Summary(QWidget *parent) : QWizardPage(parent)
{
    // Create the summary of inputs selected to run a macro
    setTitle("Summary of settings");

    mFileGB    = new QGroupBox("Particles", this);
    mFileGBlay = new QGridLayout(mFileGB);
    mParaGB    = new QGroupBox("Parameters", this);
    mParaGBlay = new QGridLayout(mParaGB);

   // final layout
    mMainLayout = new QGridLayout(this);
    mMainLayout->addWidget(mFileGB, 0, 0, 1, 1);
    mMainLayout->addWidget(mParaGB, 1, 0, 1, 1);
    setLayout(mMainLayout);

    resize(100, 100);
}

//__________________________________________________________________________
void Summary::updateFileName(QString name)
{
    // display particles file name

    QString tempo = QString("The selected particles list file is: %1").arg(name);

    static QLabel *label = new QLabel(tempo, mFileGB);

    if ( !mFileGBlay->itemAtPosition(0, 0)) {
        mFileGBlay->addWidget(label, 0, 0, 1, 1);
    }
    else
        label->setText(tempo);
    mFileGB->setLayout(mFileGBlay);

}

//__________________________________________________________________________
void Summary::updateParameters()
{
    // displays the parameters

    for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
        static QLabel *labelN = new QLabel(name, mParaGB);
        static QLabel *labelV = new QLabel(QString("%1").arg(value);
        QString name = ParaSel::getParaName((ParaSel::ParameterType)type);
        double value = field(name).toDouble();
        mParaGBlay->addWidget(labelN, type, 0, 1, 1);
        mParaGBlay->addWidget(labelV, mParaGB), type, 1);
    }
    mParaGB->setLayout(mParaGBlay);
}

