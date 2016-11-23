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
    if (mFileGBlay->isEmpty())
        mFileGBlay->addWidget(new QLabel(tempo, mFileGB), 0, 0, 1, 1);
    else
        qobject_cast<QLabel*>(mFileGBlay->itemAtPosition(0, 0)->widget())->setText(tempo);

    mFileGB->setLayout(mFileGBlay);
}

//__________________________________________________________________________
void Summary::updateParameters()
{
    // displays the parameters

    qDebug() << Q_FUNC_INFO << field("TemperatureFix");
    for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
        QString name = ParaSel::getParaName((ParaSel::ParameterType)type);
        double value = field(name).toDouble();
        QString fix("is fixed");
        if (!mParaGBlay->itemAtPosition(type,0)) {
            mParaGBlay->addWidget(new QLabel(name, mParaGB), type, 0, 1, 1);
            mParaGBlay->addWidget(new QLabel(QString("%1").arg(value), mParaGB), type, 1, 1, 1);
            if (!field(QString("%1Fix").arg(name)).toBool())
                    fix = "is not fixed";
            mParaGBlay->addWidget(new QLabel(fix, mParaGB), type, 2, 1, 1);
        } else {
            qobject_cast<QLabel*>(mParaGBlay->itemAtPosition(type, 0)->widget())->setText(name);
            qobject_cast<QLabel*>(mParaGBlay->itemAtPosition(type, 1)->widget())->setText(QString("%1").arg(value));
            if (!field(QString("%1Fix").arg(name)).toBool())
                fix = "is not fixed";
            qobject_cast<QLabel*>(mParaGBlay->itemAtPosition(type, 2)->widget())->setText(fix);
        }
    }
    mParaGB->setLayout(mParaGBlay);
}
