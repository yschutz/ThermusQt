// Author: Yves Schutz november 2017
//
// A window to select interactively parametres needed by the macro

#include "macroparasel.h"

#include <QCheckBox>
#include <QDebug>
#include <QDir>
#include <QGroupBox>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QRadioButton>
#include <QStandardPaths>

//__________________________________________________________________________
MacroParaSel::MacroParaSel(QWidget* parent) :
    mExclVol(0.3), mModelBQ(false), mModelBSQ(true), mQstat(false), mWidth(false)
{
    // create an interactive window to set parameters for the macro
    setTitle("Macro parameters setting");

    // choice of model
    QGroupBox* modelBox  = new QGroupBox(tr("Model"));
    mRadBQ               = new QRadioButton(tr("Canonical (BQ)"));
    mRadBSQ              = new QRadioButton(tr("Grand Canonical (BSQ)"));
    connect(mRadBQ,  &QRadioButton::toggled, this, [this] { setModel(); });
    connect(mRadBSQ, &QRadioButton::toggled, this, [this] { setModel(); });

    QVBoxLayout* modelBoxLayout = new QVBoxLayout();
    modelBoxLayout->addWidget(mRadBQ);
    modelBoxLayout->addWidget(mRadBSQ);
    modelBox->setLayout(modelBoxLayout);

    // Q statistics, resonance width and exclusion volume
    QGroupBox* qstaBox     = new QGroupBox(tr("Quantum statistics, resonance width and exclusion volumetreatement"));
    mQstatBut              = new QCheckBox(tr("Quantum statistics on"));
    mWidthBut              = new QCheckBox(tr("Resonance width on"));
    QHBoxLayout* eVoLayout = new QHBoxLayout();
    mExclVolBut            = new QCheckBox(tr("Exclusion volume on"));
    mExclVolLE             = new QLineEdit(QString::number(mExclVol));
    eVoLayout->addWidget(mExclVolBut);
    eVoLayout->addWidget(mExclVolLE);
    connect(mQstatBut,    &QCheckBox::clicked, this, [this] { setQS(); });
    connect(mWidthBut,    &QCheckBox::clicked, this, [this] { setW(); });
    connect(mExclVolBut,  &QCheckBox::clicked, this, [this] { setEV(); });


    QVBoxLayout* qstaBoxLayout = new QVBoxLayout();
    qstaBoxLayout->addWidget(mQstatBut);
    qstaBoxLayout->addWidget(mWidthBut);
    qstaBoxLayout->addLayout(eVoLayout);
    qstaBox->setLayout(qstaBoxLayout);


    // choice of data
    QDir thermusDir(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "/ThermusQt");
    if (!thermusDir.exists()) {
        QMessageBox* msg = new QMessageBox(QMessageBox::Critical,"Wrong Installation", QString("ThermusQt installation is expected at %1").arg(thermusDir.path()));
        msg->exec();
        exit(1);
    }
    QDir datapath(thermusDir.path() + "/data");
    QStringList filterName("*.txt");
    QStringList files = datapath.entryList(filterName);
    QVBoxLayout* radFilesLayout = new QVBoxLayout();
    for (QString filename : files) {
        QRadioButton* radFile = new QRadioButton(filename);
        connect(radFile, &QRadioButton::toggled, this, [this] { setData(); });
        radFilesLayout->addWidget(radFile);
        mRadFiles.append(radFile);
    }
    QRadioButton* radFile = new QRadioButton(tr("other (enter full path)"));
    connect(radFile, &QRadioButton::toggled, this, [this] { setData(); });
    radFilesLayout->addWidget(radFile);
    mRadFiles.append(radFile);

    QGroupBox* dataBox   = new QGroupBox(tr("Select data file"));
    QHBoxLayout* dataBoxLayout = new QHBoxLayout();
    dataBoxLayout->addLayout(radFilesLayout);
    dataBox->setLayout(dataBoxLayout);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(modelBox);
    mainLayout->addWidget(qstaBox);
    mainLayout->addWidget(dataBox);

    setLayout(mainLayout);
}

//__________________________________________________________________________
void MacroParaSel::updateDisplay()
{
    // update the display after the default parameters have been set
    if (mModelBQ) {
        mRadBQ->setChecked(true);
        mRadBSQ->setChecked(false);
    }
    else {
        mRadBQ->setChecked(false);
        mRadBSQ->setChecked(true);
    }

    if (mQstat)
        mQstatBut->setChecked(true);
    if (mWidth)
        mWidthBut->setChecked(true);
    if (mExclVol)
        mExclVolBut->setChecked(true);
}

//__________________________________________________________________________
void MacroParaSel::setData()
{
    // picks up the data file name
    for (QRadioButton* rad : mRadFiles) {
        if (rad->isChecked()) {
            mDataFileName = rad->text();
        }
    }
    QRadioButton* rad = mRadFiles.last();
    if (rad->isChecked()) {
        mDataFileName = QInputDialog::getText(this, tr("Enter file name (full path"), tr("name?"), QLineEdit::Normal);
        if (!mDataFileName.isEmpty())
            rad->setText(mDataFileName);
    }
}

//__________________________________________________________________________
void MacroParaSel::setEV()
{
    if (mExclVolBut->isChecked())
       mExclVol = mExclVolLE->text().toDouble();
    else {
        mExclVol = 0.0;
        mExclVolLE->setText(QString::number(mExclVol));
    }
}

//__________________________________________________________________________
void MacroParaSel::setModel()
{
    // inspects the model radio button
    if (mRadBQ->isChecked()) {
        mModelBQ  = true;
        mModelBSQ = false;
    }

    if (mRadBSQ->isChecked()) {
        mModelBSQ  = true;
        mModelBQ   = false;
    }
}

//__________________________________________________________________________
void MacroParaSel::setQS()
{
    // sets Q stat
    mQstat =  mQstatBut->isChecked();
}

//__________________________________________________________________________
void MacroParaSel::setW()
{
    // sets Width
    mWidth = mWidthBut->isChecked();
}
