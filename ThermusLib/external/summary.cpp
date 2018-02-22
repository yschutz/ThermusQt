#include <QDebug>
#include <QLabel>
#include <QVariant>

//#include "parasel.h"
#include "summary.h"

#include "TTMParameterSet.h"

//__________________________________________________________________________
Summary::Summary(QWidget *parent) : QWizardPage(parent)
{
    // Create the summary of inputs selected to run a macro
    setTitle("Summary of settings");

    mConsGB    = new QGroupBox("Constraints", this);
    mConsGBlay = new QGridLayout(mConsGB);

    mFileGB    = new QGroupBox("Particles", this);
    mFileGBlay = new QGridLayout(mFileGB);

    mParaGB    = new QGroupBox("Parameters", this);
    mParaGBlay = new QGridLayout(mParaGB);

   // final layout
    mMainLayout = new QGridLayout(this);
    mMainLayout->addWidget(mFileGB, 0, 0, 1, 1);
    mMainLayout->addWidget(mParaGB, 1, 0, 1, 1);
    mMainLayout->addWidget(mConsGB, 2, 0, 1, 1);
    setLayout(mMainLayout);

    resize(100, 100);
}

//__________________________________________________________________________
void Summary::updateFileName(const QString name)
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

    for (qint32 type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        QString name = TTMParameterSet::name((TTMParameterSet::ParameterType)type);
        double value = field(name).toDouble();
        double valm  = field(name + "Fixm").toDouble();
        double valM  = field(name + "FixM").toDouble();
        double vals  = field(name + "Fixs").toDouble();
        QString fix("is fixed");
        if (!mParaGBlay->itemAtPosition(type, 0)) {
            mParaGBlay->addWidget(new QLabel(name, mParaGB), type, 0, 1, 1);
            mParaGBlay->addWidget(new QLabel(QString("%1").arg(value), mParaGB), type, 1, 1, 1);
            if (!field(QString("%1Fix").arg(name)).toBool()) {
                fix = QString("is fitted between %1 and %2 in steps of %3").arg(valm).arg(valM).arg(vals);
            }
            mParaGBlay->addWidget(new QLabel(fix, mParaGB), type, 2, 1, 1);
        } else {
            qobject_cast<QLabel*>(mParaGBlay->itemAtPosition(type, 0)->widget())->setText(name);
            qobject_cast<QLabel*>(mParaGBlay->itemAtPosition(type, 1)->widget())->setText(QString("%1").arg(value));
            if (!field(QString("%1Fix").arg(name)).toBool()) {
                double valm = field(name + "Fixm").toDouble();
                double valM = field(name + "FixM").toDouble();
                double vals = field(name + "Fixs").toDouble();
                fix = QString("is fitted between %1 and %2 in steps of %3").arg(valm).arg(valM).arg(vals);
            }
            qobject_cast<QLabel*>(mParaGBlay->itemAtPosition(type, 2)->widget())->setText(fix);
        }
    }

    // and now the constrains
    QString text("Constrained");

    if (!field(TTMParameterSet::name(TTMParameterSet::kMUQ) + "C").toBool() &&
           !field(TTMParameterSet::name(TTMParameterSet::kMUS) + "C").toBool() &&
            !field(TTMParameterSet::name(TTMParameterSet::kMUC) + "C").toBool() &&
            !field(TTMParameterSet::name(TTMParameterSet::kMUQ) + "C").toBool()) {
        text = "Not Constrained";
        for (qint32 index = 1; index < mConsGBlay->count(); index++)
            if (mConsGBlay->itemAtPosition(index, 0))
                delete mConsGBlay->itemAtPosition(index, 0)->widget();
    }

    if (!mConsGBlay->itemAtPosition(0, 0))
        mConsGBlay->addWidget(new QLabel(text, mConsGB), 0, 0, 1, 1, Qt::AlignHCenter);
    else
        qobject_cast<QLabel*>(mConsGBlay->itemAtPosition(0, 0)->widget())->setText(text);

    if (field(TTMParameterSet::name(TTMParameterSet::kMUQ) + "C").toBool()) {
        text = QString("B/2Q constrained to %1").arg(field(TTMParameterSet::name(TTMParameterSet::kMUQ)+"CD").toDouble());
        if (!mConsGBlay->itemAtPosition(1, 0))
            mConsGBlay->addWidget(new QLabel(text, mConsGB), 1, 0, 1, 1);
        else
            qobject_cast<QLabel*>(mConsGBlay->itemAtPosition(1, 0)->widget())->setText(text);
    } else {
        if (mConsGBlay->itemAtPosition(1, 0))
            delete mConsGBlay->itemAtPosition(1, 0)->widget();
    }
    if (field(TTMParameterSet::name(TTMParameterSet::kMUS) + "C").toBool()) {
        text = QString("S density constrained to %1").arg(field(TTMParameterSet::name(TTMParameterSet::kMUS)+"CD").toDouble());
        if (!mConsGBlay->itemAtPosition(2, 0))
            mConsGBlay->addWidget(new QLabel(text, mConsGB), 2, 0, 1, 1);
        else
            qobject_cast<QLabel*>(mConsGBlay->itemAtPosition(2, 0)->widget())->setText(text);
    } else {
        if (mConsGBlay->itemAtPosition(2, 0))
            delete mConsGBlay->itemAtPosition(2, 0)->widget();
    }
    if (field(TTMParameterSet::name(TTMParameterSet::kMUC) + "C").toBool()) {
        text = QString("C density constrained to %1").arg(field(TTMParameterSet::name(TTMParameterSet::kMUC)+"CD").toDouble());
        if (!mConsGBlay->itemAtPosition(3, 0))
            mConsGBlay->addWidget(new QLabel(text, mConsGB), 3, 0, 1, 1);
        else
            qobject_cast<QLabel*>(mConsGBlay->itemAtPosition(3, 0)->widget())->setText(text);
    } else {
        if (mConsGBlay->itemAtPosition(3, 0))
            delete mConsGBlay->itemAtPosition(3, 0)->widget();
    }
    if (field(TTMParameterSet::name(TTMParameterSet::kMUBEAUTY) + "C").toBool()) {
        text = QString("Beauty density constrained to %1").arg(field(TTMParameterSet::name(TTMParameterSet::kMUBEAUTY)+"CD").toDouble());
        if (!mConsGBlay->itemAtPosition(4, 0))
            mConsGBlay->addWidget(new QLabel(text, mConsGB), 4, 0, 1, 1);
        else
            qobject_cast<QLabel*>(mConsGBlay->itemAtPosition(4, 0)->widget())->setText(text);
    } else {
        if (mConsGBlay->itemAtPosition(4, 0))
            delete mConsGBlay->itemAtPosition(4, 0)->widget();
    }

    mParaGB->setLayout(mParaGBlay);
}
