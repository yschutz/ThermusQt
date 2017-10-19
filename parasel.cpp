// Author: Yves Schutz 25 octobre 2016
//
// A window to select interactively parametres


#include "parasel.h"

#include <QGridLayout>
#include <QDebug>
#include <QGroupBox>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>

//__________________________________________________________________________
ParaSel::ParaSel(QWidget *parent) : QWizardPage(parent)
{

    // create an interactive window to select parametres

    setTitle("Select parameters values and fit constrains");

    for (int index = 0; index < TTMParameterSet::kPARTYPES; index++) {
        mFitMax[index]       = nullptr;
        mFitMin[index]       = nullptr;
        mFitSte[index]       = nullptr;
    }

    QString *collSysNames = new QString[kCollTypes];
    collSysNames[kPbPb]  = "PbPb";
    collSysNames[kAuAu] = "AuAu";
    collSysNames[kpp]    = "pp";
    collSysNames[kpPb]   = "pPb";

    mPalette.setColor(QPalette::Base,Qt::white);
    mPalette.setColor(QPalette::Text,Qt::red);

    // the temperature
    QGroupBox *tBox = new QGroupBox(tr("Temperature"), this);

    QLabel *tfix       = new QLabel(tr("Fix"), tBox);
    QLabel *tempLabel  = new QLabel(tr("T:"), tBox);
    mParameterValue[TTMParameterSet::kT] = new QLineEdit(tBox);

    tempLabel->setBuddy(mParameterValue[TTMParameterSet::kT]);
    QLabel *tempUnitLabel = new QLabel(tr("GeV"));
    mParameterFix[TTMParameterSet::kT] = new QCheckBox(" ", tBox);

    registerField("temp", mParameterFix[TTMParameterSet::kT]);

    mParameterFix[TTMParameterSet::kT]->setChecked(true);
    mParameterValue[TTMParameterSet::kT]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kT]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kT], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kT); });

    QGridLayout *tLayout = new QGridLayout(tBox);
    tLayout->addWidget(tfix, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    tLayout->addWidget(tempLabel, 1, 0, 1, 1);
    tLayout->addWidget(mParameterValue[TTMParameterSet::kT], 1, 1, 1, 1);
    tLayout->addWidget(tempUnitLabel, 1, 2, 1, 1);
    tLayout->addWidget(mParameterFix[TTMParameterSet::kT], 1, 3, 1, 1, Qt::AlignHCenter);
    tBox->setLayout(tLayout);

    mTFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *tmin    = new QLabel(tr("min"), mTFitBox);
    QLabel *tmax    = new QLabel(tr("max"), mTFitBox);
    QLabel *tste    = new QLabel(tr("step"), mTFitBox);
    QLabel *blanckT = new QLabel(" ", mTFitBox);
    mTFitBoxLayout  = new QGridLayout(mTFitBox);
    mTFitBoxLayout->setColumnMinimumWidth(1, 50);
    mTFitBoxLayout->setColumnMinimumWidth(2, 50);
    mTFitBoxLayout->setColumnMinimumWidth(3, 50);
    mTFitBoxLayout->addWidget(tmin, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mTFitBoxLayout->addWidget(tmax, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mTFitBoxLayout->addWidget(tste, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mFitMin[TTMParameterSet::kT] = new QLineEdit(mTFitBox);
    mFitMax[TTMParameterSet::kT] = new QLineEdit(mTFitBox);
    mFitSte[TTMParameterSet::kT] = new QLineEdit(mTFitBox);
    mFitMin[TTMParameterSet::kT]->setText("Fixed");
    mFitMax[TTMParameterSet::kT]->setText("Fixed");
    mFitSte[TTMParameterSet::kT]->setText("Fixed");
    mFitMin[TTMParameterSet::kT]->setReadOnly(true);
    mFitMax[TTMParameterSet::kT]->setReadOnly(true);
    mFitSte[TTMParameterSet::kT]->setReadOnly(true);
    mFitMin[TTMParameterSet::kT]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kT]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kT]->setPalette(mPalette);
    mTFitBoxLayout->addWidget(blanckT, 1, 0, 1, 1);
    mTFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kT], 1, 1, 1, 1);
    mTFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kT], 1, 2, 1, 1);
    mTFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kT], 1, 3, 1, 1);
    mTFitBox->setLayout(mTFitBoxLayout);

    // the mu's

    QGroupBox *muBox = new QGroupBox(tr("Chemical potentials"), this);

    QLabel *mufix       = new QLabel(tr("Fix"), muBox);
    QLabel *muconstrain = new QLabel(tr("Constrain"), muBox);

    QLabel  *muQLabel = new QLabel(muBox);
    muQLabel->setTextFormat(Qt::RichText);
    muQLabel->setText("<html> &mu;<sub>Q</sub>:");
    mParameterValue[TTMParameterSet::kMUQ] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kMUQ]);
    mParameterFix[TTMParameterSet::kMUQ]   = new QCheckBox("", muBox);
    mParameterFix[TTMParameterSet::kMUQ]->setChecked(true);
    mParameterCon[TTMParameterSet::kMUQ]   = new QCheckBox("", muBox);
    mParameterCon[TTMParameterSet::kMUQ]->setChecked(false);
    mParameterValue[TTMParameterSet::kMUQ]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kMUQ]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kMUQ], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kMUQ); });
    connect(mParameterCon[TTMParameterSet::kMUQ], &QCheckBox::stateChanged, this, [this]{ conParameter(TTMParameterSet::kMUQ); });
    QLabel *b2qLabel = new QLabel("<html> B/2Q", muBox);

    QLabel  *muBLabel = new QLabel(muBox);
    muBLabel->setTextFormat(Qt::RichText);
    muBLabel->setText("<html> &mu;<sub>B</sub>:");
    mParameterValue[TTMParameterSet::kMUB] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kMUB]);
    mParameterFix[TTMParameterSet::kMUB]   = new QCheckBox("", muBox);
    mParameterFix[TTMParameterSet::kMUB]->setChecked(true);
    mParameterValue[TTMParameterSet::kMUB]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kMUB]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kMUB], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kMUB); });

    QLabel  *muSLabel = new QLabel(muBox);
    muSLabel->setTextFormat(Qt::RichText);
    muSLabel->setText("<html> &mu;<sub>S</sub>:");
    mParameterValue[TTMParameterSet::kMUS] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kMUS]);
    mParameterFix[TTMParameterSet::kMUS]   = new QCheckBox("", muBox);
    mParameterFix[TTMParameterSet::kMUS]->setChecked(true);
    mParameterCon[TTMParameterSet::kMUS]   = new QCheckBox("", muBox);
    mParameterCon[TTMParameterSet::kMUS]->setChecked(false);
    mParameterValue[TTMParameterSet::kMUS]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kMUS]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kMUS], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kMUS); });
    connect(mParameterCon[TTMParameterSet::kMUS], &QCheckBox::stateChanged, this, [this]{ conParameter(TTMParameterSet::kMUS); });
    QLabel *sDensityLabel = new QLabel("<html> S Density", muBox);


    QLabel  *muCLabel = new QLabel(muBox);
    muCLabel->setTextFormat(Qt::RichText);
    muCLabel->setText("<html> &mu;<sub>C</sub>:");
    mParameterValue[TTMParameterSet::kMUC] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kMUC]);
    mParameterFix[TTMParameterSet::kMUC]   = new QCheckBox("", muBox);
    mParameterFix[TTMParameterSet::kMUC]->setChecked(true);
    mParameterCon[TTMParameterSet::kMUC]   = new QCheckBox("", muBox);
    mParameterCon[TTMParameterSet::kMUC]->setChecked(false);
    mParameterValue[TTMParameterSet::kMUC]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kMUC]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kMUC], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kMUC); });
    connect(mParameterCon[TTMParameterSet::kMUC], &QCheckBox::stateChanged, this, [this]{ conParameter(TTMParameterSet::kMUC); });
    QLabel *cDensityLabel = new QLabel("<html> C Density", muBox);

    QLabel  *muBeautyLabel = new QLabel(muBox);
    muBeautyLabel->setTextFormat(Qt::RichText);
    muBeautyLabel->setText("<html> &mu;<sub>Beauty</sub>:");
    mParameterValue[TTMParameterSet::kMUBEAUTY] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kMUBEAUTY]);
    mParameterFix[TTMParameterSet::kMUBEAUTY]   = new QCheckBox("", muBox);
    mParameterFix[TTMParameterSet::kMUBEAUTY]->setChecked(true);
    mParameterCon[TTMParameterSet::kMUBEAUTY]   = new QCheckBox("", muBox);
    mParameterCon[TTMParameterSet::kMUBEAUTY]->setChecked(false);
    mParameterValue[TTMParameterSet::kMUBEAUTY]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kMUBEAUTY]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kMUBEAUTY], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kMUBEAUTY); });
    connect(mParameterCon[TTMParameterSet::kMUBEAUTY], &QCheckBox::stateChanged, this, [this]{ conParameter(TTMParameterSet::kMUBEAUTY); });
    QLabel *bDensityLabel = new QLabel("<html> B Density", muBox);

    QGridLayout *muBoxLayout = new QGridLayout(muBox);
    muBoxLayout->addWidget(mufix, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    muBoxLayout->addWidget(muconstrain, 0, 3, 1, 2, Qt::AlignTop | Qt::AlignHCenter);

    muBoxLayout->addWidget(muQLabel, 1, 0);
    muBoxLayout->addWidget(mParameterValue[TTMParameterSet::kMUQ], 1, 1, Qt::AlignLeft);
    muBoxLayout->addWidget(mParameterFix[TTMParameterSet::kMUQ], 1, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[TTMParameterSet::kMUQ], 1, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(b2qLabel, 1, 4, Qt::AlignLeft);

    muBoxLayout->addWidget(muBLabel, 2, 0);
    muBoxLayout->addWidget(mParameterValue[TTMParameterSet::kMUB], 2, 1, Qt::AlignLeft);
    muBoxLayout->addWidget(mParameterFix[TTMParameterSet::kMUB], 2, 2, Qt::AlignHCenter);

    muBoxLayout->addWidget(muSLabel, 3, 0);
    muBoxLayout->addWidget(mParameterValue[TTMParameterSet::kMUS], 3, 1, Qt::AlignLeft);
    muBoxLayout->addWidget(mParameterFix[TTMParameterSet::kMUS], 3, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[TTMParameterSet::kMUS], 3, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(sDensityLabel, 3, 4, Qt::AlignLeft);

    muBoxLayout->addWidget(muCLabel, 4, 0);
    muBoxLayout->addWidget(mParameterValue[TTMParameterSet::kMUC], 4, 1, Qt::AlignLeft);
    muBoxLayout->addWidget(mParameterFix[TTMParameterSet::kMUC], 4, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[TTMParameterSet::kMUC], 4, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(cDensityLabel, 4, 4, Qt::AlignLeft);

    muBoxLayout->addWidget(muBeautyLabel, 5, 0);
    muBoxLayout->addWidget(mParameterValue[TTMParameterSet::kMUBEAUTY], 5, 1, Qt::AlignLeft);
    muBoxLayout->addWidget(mParameterFix[TTMParameterSet::kMUBEAUTY], 5, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[TTMParameterSet::kMUBEAUTY], 5, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(bDensityLabel, 5, 4, Qt::AlignLeft);

    muBoxLayout->setColumnMinimumWidth(1, 50);
    muBox->setLayout(muBoxLayout);

    mMuFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *mumin    = new QLabel(tr("min"), mMuFitBox);
    QLabel *mumax    = new QLabel(tr("max"), mMuFitBox);
    QLabel *muste    = new QLabel(tr("step"), mMuFitBox);
    QLabel *blanckMu = new QLabel(" ", mMuFitBox);

    mMuFitBoxLayout = new QGridLayout(mMuFitBox);
    mMuFitBoxLayout->setColumnMinimumWidth(1, 50);
    mMuFitBoxLayout->setColumnMinimumWidth(2, 50);
    mMuFitBoxLayout->setColumnMinimumWidth(3, 50);
    mMuFitBoxLayout->addWidget(mumin, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mMuFitBoxLayout->addWidget(mumax, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mMuFitBoxLayout->addWidget(muste, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);

    mFitMin[TTMParameterSet::kMUQ] = new QLineEdit(mMuFitBox);
    mFitMax[TTMParameterSet::kMUQ] = new QLineEdit(mMuFitBox);
    mFitSte[TTMParameterSet::kMUQ] = new QLineEdit(mMuFitBox);
    mFitMin[TTMParameterSet::kMUQ]->setText("Fixed");
    mFitMax[TTMParameterSet::kMUQ]->setText("Fixed");
    mFitSte[TTMParameterSet::kMUQ]->setText("Fixed");
    mFitMin[TTMParameterSet::kMUQ]->setReadOnly(true);
    mFitMax[TTMParameterSet::kMUQ]->setReadOnly(true);
    mFitSte[TTMParameterSet::kMUQ]->setReadOnly(true);
    mFitMin[TTMParameterSet::kMUQ]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kMUQ]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kMUQ]->setPalette(mPalette);
    mMuFitBoxLayout->addWidget(blanckMu, 1, 0, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kMUQ], 1, 1, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kMUQ], 1, 2, 1, 1);
    mMuFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kMUQ], 1, 3, 1, 1);

    mFitMin[TTMParameterSet::kMUB] = new QLineEdit(mMuFitBox);
    mFitMax[TTMParameterSet::kMUB] = new QLineEdit(mMuFitBox);
    mFitSte[TTMParameterSet::kMUB] = new QLineEdit(mMuFitBox);
    mFitMin[TTMParameterSet::kMUB]->setText("Fixed");
    mFitMax[TTMParameterSet::kMUB]->setText("Fixed");
    mFitSte[TTMParameterSet::kMUB]->setText("Fixed");
    mFitMin[TTMParameterSet::kMUB]->setReadOnly(true);
    mFitMax[TTMParameterSet::kMUB]->setReadOnly(true);
    mFitSte[TTMParameterSet::kMUB]->setReadOnly(true);
    mFitMin[TTMParameterSet::kMUB]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kMUB]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kMUB]->setPalette(mPalette);
    mMuFitBoxLayout->addWidget(blanckMu, 2, 0, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kMUB], 2, 1, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kMUB], 2, 2, 1, 1);
    mMuFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kMUB], 2, 3, 1, 1);

    mFitMin[TTMParameterSet::kMUS] = new QLineEdit(mMuFitBox);
    mFitMax[TTMParameterSet::kMUS] = new QLineEdit(mMuFitBox);
    mFitSte[TTMParameterSet::kMUS] = new QLineEdit(mMuFitBox);
    mFitMin[TTMParameterSet::kMUS]->setText("Fixed");
    mFitMax[TTMParameterSet::kMUS]->setText("Fixed");
    mFitSte[TTMParameterSet::kMUS]->setText("Fixed");
    mFitMin[TTMParameterSet::kMUS]->setReadOnly(true);
    mFitMax[TTMParameterSet::kMUS]->setReadOnly(true);
    mFitSte[TTMParameterSet::kMUS]->setReadOnly(true);
    mFitMin[TTMParameterSet::kMUS]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kMUS]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kMUS]->setPalette(mPalette);
    mMuFitBoxLayout->addWidget(blanckMu, 3, 0, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kMUS], 3, 1, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kMUS], 3, 2, 1, 1);
    mMuFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kMUS], 3, 3, 1, 1);

    mFitMin[TTMParameterSet::kMUC] = new QLineEdit(mMuFitBox);
    mFitMax[TTMParameterSet::kMUC] = new QLineEdit(mMuFitBox);
    mFitSte[TTMParameterSet::kMUC] = new QLineEdit(mMuFitBox);
    mFitMin[TTMParameterSet::kMUC]->setText("Fixed");
    mFitMax[TTMParameterSet::kMUC]->setText("Fixed");
    mFitSte[TTMParameterSet::kMUC]->setText("Fixed");
    mFitMin[TTMParameterSet::kMUC]->setReadOnly(true);
    mFitMax[TTMParameterSet::kMUC]->setReadOnly(true);
    mFitSte[TTMParameterSet::kMUC]->setReadOnly(true);
    mFitMin[TTMParameterSet::kMUC]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kMUC]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kMUC]->setPalette(mPalette);
    mMuFitBoxLayout->addWidget(blanckMu, 4, 0, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kMUC], 4, 1, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kMUC], 4, 2, 1, 1);
    mMuFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kMUC], 4, 3, 1, 1);

    mFitMin[TTMParameterSet::kMUBEAUTY] = new QLineEdit(mMuFitBox);
    mFitMax[TTMParameterSet::kMUBEAUTY] = new QLineEdit(mMuFitBox);
    mFitSte[TTMParameterSet::kMUBEAUTY] = new QLineEdit(mMuFitBox);
    mFitMin[TTMParameterSet::kMUBEAUTY]->setText("Fixed");
    mFitMax[TTMParameterSet::kMUBEAUTY]->setText("Fixed");
    mFitSte[TTMParameterSet::kMUBEAUTY]->setText("Fixed");
    mFitMin[TTMParameterSet::kMUBEAUTY]->setReadOnly(true);
    mFitMax[TTMParameterSet::kMUBEAUTY]->setReadOnly(true);
    mFitSte[TTMParameterSet::kMUBEAUTY]->setReadOnly(true);
    mFitMin[TTMParameterSet::kMUBEAUTY]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kMUBEAUTY]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kMUBEAUTY]->setPalette(mPalette);
    mMuFitBoxLayout->addWidget(blanckMu, 5, 0, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kMUBEAUTY], 5, 1, 1, 1);
    mMuFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kMUBEAUTY], 5, 2, 1, 1);
    mMuFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kMUBEAUTY], 5, 3, 1, 1);

    mMuFitBox->setLayout(mMuFitBoxLayout);

    mMuConBox = new QGroupBox(tr("Constrain"), this);
    mMuConBoxLayout = new QGridLayout(mMuConBox);
    mMuConBoxLayout->setColumnMinimumWidth(2, 50);
    QLabel *blanck0 = new QLabel("", mMuConBox);
    mMuConBoxLayout->addWidget(blanck0, 0, 0, 1, 2, Qt::AlignTop);

    mDensity[TTMParameterSet::kMUQ] = new QLineEdit("S dens", mMuConBox);
    mDensity[TTMParameterSet::kMUQ]->setText("OFF");
    mDensity[TTMParameterSet::kMUQ]->setPalette(mPalette);
    mMuConBoxLayout->addWidget(mDensity[TTMParameterSet::kMUQ], 1, 2, 1, 1);

    mDensity[TTMParameterSet::kMUB] = new QLineEdit("S dens", mMuConBox);
    mDensity[TTMParameterSet::kMUB]->setText("OFF");
    mDensity[TTMParameterSet::kMUB]->setPalette(mPalette);
    mMuConBoxLayout->addWidget(mDensity[TTMParameterSet::kMUB], 2, 2, 1, 1);

    mDensity[TTMParameterSet::kMUS] = new QLineEdit("S dens", mMuConBox);
    mDensity[TTMParameterSet::kMUS]->setText("OFF");
    mDensity[TTMParameterSet::kMUS]->setPalette(mPalette);
    mMuConBoxLayout->addWidget(mDensity[TTMParameterSet::kMUS], 3, 2, 1, 1);

    mDensity[TTMParameterSet::kMUC] = new QLineEdit("S dens", mMuConBox);
    mDensity[TTMParameterSet::kMUC]->setText("OFF");
    mDensity[TTMParameterSet::kMUC]->setPalette(mPalette);
    mMuConBoxLayout->addWidget(mDensity[TTMParameterSet::kMUC], 4, 2, 1, 1);

    mDensity[TTMParameterSet::kMUBEAUTY] = new QLineEdit("S dens", mMuConBox);
    mDensity[TTMParameterSet::kMUBEAUTY]->setText("OFF");
    mDensity[TTMParameterSet::kMUBEAUTY]->setPalette(mPalette);
    mMuConBoxLayout->addWidget(mDensity[TTMParameterSet::kMUBEAUTY], 5, 2, 1, 1);

    mMuConBox->setLayout(mMuConBoxLayout);

    // the gamma's
    QGroupBox *gammaBox  = new QGroupBox(tr("Fugacities"), this);
    QLabel *gammafix     = new QLabel(tr("Fix"), gammaBox);
    QLabel  *gammaSLabel = new QLabel(gammaBox);
    gammaSLabel->setTextFormat(Qt::RichText);
    gammaSLabel->setText("<html> &gamma;<sub>S</sub>:");
    mParameterValue[TTMParameterSet::kGAMMAS] = new QLineEdit(gammaBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kGAMMAS]);
    mParameterFix[TTMParameterSet::kGAMMAS]   = new QCheckBox("", gammaBox);
    mParameterFix[TTMParameterSet::kGAMMAS]->setChecked(true);
    mParameterValue[TTMParameterSet::kGAMMAS]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kGAMMAS]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kGAMMAS], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kGAMMAS); });

    QLabel  *gammaCLabel = new QLabel(tr("gammaC:"), gammaBox);
    gammaCLabel->setTextFormat(Qt::RichText);
    gammaCLabel->setText("<html> &gamma;<sub>C</sub>:");
    mParameterValue[TTMParameterSet::kGAMMAC] = new QLineEdit(gammaBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kGAMMAC]);
    mParameterFix[TTMParameterSet::kGAMMAC]   = new QCheckBox("", gammaBox);
    mParameterFix[TTMParameterSet::kGAMMAC]->setChecked(true);
    mParameterValue[TTMParameterSet::kGAMMAC]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kGAMMAC]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kGAMMAC], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kGAMMAC); });

    QLabel  *gammaBeautyLabel = new QLabel(tr("gammaBeauty:"), gammaBox);
    gammaBeautyLabel->setTextFormat(Qt::RichText);
    gammaBeautyLabel->setText("<html> &gamma;<sub>Beauty</sub>:");
    mParameterValue[TTMParameterSet::kGAMMABEAUTY] = new QLineEdit(gammaBox);
    muQLabel->setBuddy(mParameterValue[TTMParameterSet::kGAMMABEAUTY]);
    mParameterFix[TTMParameterSet::kGAMMABEAUTY]   = new QCheckBox("", gammaBox);
    mParameterFix[TTMParameterSet::kGAMMABEAUTY]->setChecked(true);
    mParameterValue[TTMParameterSet::kGAMMABEAUTY]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kGAMMABEAUTY]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kGAMMABEAUTY], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kGAMMABEAUTY); });

    QGridLayout *gammaBoxLayout = new QGridLayout(gammaBox);
    gammaBoxLayout->addWidget(gammafix, 0, 2, 1, 1);

    gammaBoxLayout->addWidget(gammaSLabel, 1, 0);
    gammaBoxLayout->addWidget(mParameterValue[TTMParameterSet::kGAMMAS], 1, 1);
    gammaBoxLayout->addWidget(mParameterFix[TTMParameterSet::kGAMMAS], 1, 2, Qt::AlignHCenter);

    gammaBoxLayout->addWidget(gammaCLabel, 2, 0);
    gammaBoxLayout->addWidget(mParameterValue[TTMParameterSet::kGAMMAC], 2, 1);
    gammaBoxLayout->addWidget(mParameterFix[TTMParameterSet::kGAMMAC], 2, 2, Qt::AlignHCenter);

    gammaBoxLayout->addWidget(gammaBeautyLabel, 3, 0);
    gammaBoxLayout->addWidget(mParameterValue[TTMParameterSet::kGAMMABEAUTY], 3, 1);
    gammaBoxLayout->addWidget(mParameterFix[TTMParameterSet::kGAMMABEAUTY], 3, 2, Qt::AlignHCenter);

    gammaBox->setLayout(gammaBoxLayout);

    mGammaFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *gammamin = new QLabel(tr("min"), mGammaFitBox);
    QLabel *gammamax = new QLabel(tr("max"), mGammaFitBox);
    QLabel *gammaste = new QLabel(tr("step"), mGammaFitBox);
    QLabel *blanckGa = new QLabel(" ", mGammaFitBox);
    mGammaFitBoxLayout = new QGridLayout(mGammaFitBox);
    mGammaFitBoxLayout->setColumnMinimumWidth(2, 50);
    mGammaFitBoxLayout->addWidget(gammamin, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mGammaFitBoxLayout->addWidget(gammamax, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mGammaFitBoxLayout->addWidget(gammaste, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);

    mFitMin[TTMParameterSet::kGAMMAS] = new QLineEdit(mGammaFitBox);
    mFitMax[TTMParameterSet::kGAMMAS] = new QLineEdit(mGammaFitBox);
    mFitSte[TTMParameterSet::kGAMMAS] = new QLineEdit(mGammaFitBox);
    mFitMin[TTMParameterSet::kGAMMAS]->setText("Fixed");
    mFitMax[TTMParameterSet::kGAMMAS]->setText("Fixed");
    mFitSte[TTMParameterSet::kGAMMAS]->setText("Fixed");
    mFitMin[TTMParameterSet::kGAMMAS]->setReadOnly(true);
    mFitMax[TTMParameterSet::kGAMMAS]->setReadOnly(true);
    mFitSte[TTMParameterSet::kGAMMAS]->setReadOnly(true);
    mFitMin[TTMParameterSet::kGAMMAS]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kGAMMAS]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kGAMMAS]->setPalette(mPalette);
    mGammaFitBoxLayout->addWidget(blanckGa, 1, 0, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kGAMMAS], 1, 1, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kGAMMAS], 1, 2, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kGAMMAS], 1, 3, 1, 1);

    mFitMin[TTMParameterSet::kGAMMAC] = new QLineEdit(mGammaFitBox);
    mFitMax[TTMParameterSet::kGAMMAC] = new QLineEdit(mGammaFitBox);
    mFitSte[TTMParameterSet::kGAMMAC] = new QLineEdit(mGammaFitBox);
    mFitMin[TTMParameterSet::kGAMMAC]->setText("Fixed");
    mFitMax[TTMParameterSet::kGAMMAC]->setText("Fixed");
    mFitSte[TTMParameterSet::kGAMMAC]->setText("Fixed");
    mFitMin[TTMParameterSet::kGAMMAC]->setReadOnly(true);
    mFitMax[TTMParameterSet::kGAMMAC]->setReadOnly(true);
    mFitSte[TTMParameterSet::kGAMMAC]->setReadOnly(true);
    mFitMin[TTMParameterSet::kGAMMAC]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kGAMMAC]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kGAMMAC]->setPalette(mPalette);
    mGammaFitBoxLayout->addWidget(blanckGa, 2, 0, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kGAMMAC], 2, 1, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kGAMMAC], 2, 2, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kGAMMAC], 2, 3, 1, 1);

    mFitMin[TTMParameterSet::kGAMMABEAUTY] = new QLineEdit(mGammaFitBox);
    mFitMax[TTMParameterSet::kGAMMABEAUTY] = new QLineEdit(mGammaFitBox);
    mFitSte[TTMParameterSet::kGAMMABEAUTY] = new QLineEdit(mGammaFitBox);
    mFitMin[TTMParameterSet::kGAMMABEAUTY]->setText("Fixed");
    mFitMax[TTMParameterSet::kGAMMABEAUTY]->setText("Fixed");
    mFitSte[TTMParameterSet::kGAMMABEAUTY]->setText("Fixed");
    mFitMin[TTMParameterSet::kGAMMABEAUTY]->setReadOnly(true);
    mFitMax[TTMParameterSet::kGAMMABEAUTY]->setReadOnly(true);
    mFitSte[TTMParameterSet::kGAMMABEAUTY]->setReadOnly(true);
    mFitMin[TTMParameterSet::kGAMMABEAUTY]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kGAMMABEAUTY]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kGAMMABEAUTY]->setPalette(mPalette);
    mGammaFitBoxLayout->addWidget(blanckGa, 3, 0, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kGAMMABEAUTY], 3, 1, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kGAMMABEAUTY], 3, 2, 1, 1);
    mGammaFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kGAMMABEAUTY], 3, 3, 1, 1);
    mGammaFitBox->setLayout(mGammaFitBoxLayout);

    // the collision system
    QGroupBox *collisionsBox = new QGroupBox(tr("Collision system"));
    mCollisions[kPbPb] = new QRadioButton(collSysNames[kPbPb], collisionsBox);
    mCollisions[kAuAu] = new QRadioButton(collSysNames[kAuAu], collisionsBox);
    mCollisions[kpp]   = new QRadioButton(collSysNames[kpp], collisionsBox);
    mCollisions[kpPb]  = new QRadioButton(collSysNames[kpPb], collisionsBox);
    mCollisions[kPbPb]->setChecked(true);

    QVBoxLayout *collRadioBoxLayout = new QVBoxLayout(collisionsBox);
    for (int index = 0; index < kCollTypes; index ++)
        collRadioBoxLayout->addWidget(mCollisions[index]);

    collisionsBox->setLayout(collRadioBoxLayout);

    // the radius
    QGroupBox *rBox     = new QGroupBox(tr("Radius"), this);
    QLabel *rfix        = new QLabel(tr("Fix"), this);
    QLabel *radiusLabel = new QLabel(tr("r:"));
    mParameterValue[TTMParameterSet::kRADIUS] = new QLineEdit(rBox);
    radiusLabel->setBuddy(mParameterValue[TTMParameterSet::kRADIUS]);
    QLabel *radiusUnitLabel = new QLabel(tr("fm"));
    mParameterFix[TTMParameterSet::kRADIUS] = new QCheckBox("", rBox);
    mParameterFix[TTMParameterSet::kRADIUS]->setChecked(true);
    mParameterValue[TTMParameterSet::kRADIUS]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kRADIUS]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kRADIUS], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kRADIUS); });

    QLabel *cradiusLabel = new QLabel(tr("can r:"));
    mParameterValue[TTMParameterSet::kCRADIUS] = new QLineEdit(rBox);
    cradiusLabel->setBuddy(mParameterValue[TTMParameterSet::kCRADIUS]);
    QLabel *cradiusUnitLabel = new QLabel(tr("fm"));
    mParameterFix[TTMParameterSet::kCRADIUS] = new QCheckBox("", rBox);
    mParameterFix[TTMParameterSet::kCRADIUS]->setChecked(true);
    mParameterValue[TTMParameterSet::kCRADIUS]->setPalette(mPalette);
    mParameterValue[TTMParameterSet::kCRADIUS]->setReadOnly(true);
    connect(mParameterFix[TTMParameterSet::kCRADIUS], &QCheckBox::stateChanged, this, [this]{ fixParameter(TTMParameterSet::kCRADIUS); });

    QGridLayout *rLayout = new QGridLayout(rBox);
    rLayout->addWidget(rfix, 0, 3, 1, 1);
    rLayout->addWidget(radiusLabel, 1, 0, 1, 1);
    rLayout->addWidget(mParameterValue[TTMParameterSet::kRADIUS], 1, 1, 1, 1);
    rLayout->addWidget(radiusUnitLabel, 1, 2, 1, 1);
    rLayout->addWidget(mParameterFix[TTMParameterSet::kRADIUS], 1, 3, 1, 1, Qt::AlignHCenter);

    rLayout->addWidget(cradiusLabel, 2, 0, 1, 1);
    rLayout->addWidget(mParameterValue[TTMParameterSet::kCRADIUS], 2, 1, 1, 1);
    rLayout->addWidget(cradiusUnitLabel, 2, 2, 1, 1);
    rLayout->addWidget(mParameterFix[TTMParameterSet::kCRADIUS], 2, 3, 1, 1, Qt::AlignHCenter);

    rBox->setLayout(rLayout);

    mRFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *rmin = new QLabel(tr("min"), mRFitBox);
    QLabel *rmax = new QLabel(tr("max"), mRFitBox);
    QLabel *rste = new QLabel(tr("step"), mRFitBox);
    QLabel *blanckR = new QLabel(" ", mRFitBox);

    mRFitBoxLayout = new QGridLayout(this);
    mRFitBoxLayout->addWidget(rmin, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mRFitBoxLayout->addWidget(rmax, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mRFitBoxLayout->addWidget(rste, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);

    mFitMin[TTMParameterSet::kRADIUS] = new QLineEdit(mRFitBox);
    mFitMax[TTMParameterSet::kRADIUS] = new QLineEdit(mRFitBox);
    mFitSte[TTMParameterSet::kRADIUS] = new QLineEdit(mRFitBox);
    mFitMin[TTMParameterSet::kRADIUS]->setText("Fixed");
    mFitMax[TTMParameterSet::kRADIUS]->setText("Fixed");
    mFitSte[TTMParameterSet::kRADIUS]->setText("Fixed");
    mFitMin[TTMParameterSet::kRADIUS]->setReadOnly(true);
    mFitMax[TTMParameterSet::kRADIUS]->setReadOnly(true);
    mFitSte[TTMParameterSet::kRADIUS]->setReadOnly(true);
    mFitMin[TTMParameterSet::kRADIUS]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kRADIUS]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kRADIUS]->setPalette(mPalette);
    mRFitBoxLayout->addWidget(blanckR, 1, 0, 1, 1);
    mRFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kRADIUS], 1, 1, 1, 1);
    mRFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kRADIUS], 1, 2, 1, 1);
    mRFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kRADIUS], 1, 3, 1, 1);

    mFitMin[TTMParameterSet::kCRADIUS] = new QLineEdit(mRFitBox);
    mFitMax[TTMParameterSet::kCRADIUS] = new QLineEdit(mRFitBox);
    mFitSte[TTMParameterSet::kCRADIUS] = new QLineEdit(mRFitBox);
    mFitMin[TTMParameterSet::kCRADIUS]->setText("Fixed");
    mFitMax[TTMParameterSet::kCRADIUS]->setText("Fixed");
    mFitSte[TTMParameterSet::kCRADIUS]->setText("Fixed");
    mFitMin[TTMParameterSet::kCRADIUS]->setReadOnly(true);
    mFitMax[TTMParameterSet::kCRADIUS]->setReadOnly(true);
    mFitSte[TTMParameterSet::kCRADIUS]->setReadOnly(true);
    mFitMin[TTMParameterSet::kCRADIUS]->setPalette(mPalette);
    mFitMax[TTMParameterSet::kCRADIUS]->setPalette(mPalette);
    mFitSte[TTMParameterSet::kCRADIUS]->setPalette(mPalette);
    mRFitBoxLayout->addWidget(blanckR, 2, 0, 1, 1);
    mRFitBoxLayout->addWidget(mFitMin[TTMParameterSet::kCRADIUS], 2, 1, 1, 1);
    mRFitBoxLayout->addWidget(mFitMax[TTMParameterSet::kCRADIUS], 2, 2, 1, 1);
    mRFitBoxLayout->addWidget(mFitSte[TTMParameterSet::kCRADIUS], 2, 3, 1, 1);

    mRFitBox->setLayout(mRFitBoxLayout);

    // final layout
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(tBox,     0, 0, 1, 1, Qt::AlignTop);
    gridLayout->addWidget(mTFitBox, 0, 1, 1, 1, Qt::AlignTop);

    gridLayout->addWidget(muBox,     1, 0, 1, 1);
    gridLayout->addWidget(mMuFitBox, 1, 1, 1, 1, Qt::AlignTop);
    gridLayout->addWidget(mMuConBox, 1, 2, 1, 1,  Qt::AlignTop);

    gridLayout->addWidget(gammaBox,     2, 0, 1, 1, Qt::AlignTop);
    gridLayout->addWidget(mGammaFitBox, 2, 1, 1, 1, Qt::AlignTop);

    gridLayout->addWidget(rBox,     3, 0, 1, 1, Qt::AlignLeft);
    gridLayout->addWidget(mRFitBox, 3, 1, 1, 1, Qt::AlignLeft);

    gridLayout->addWidget(collisionsBox, 4, 0, 1, 1, Qt::AlignTop | Qt::AlignLeft);


    QLabel *infoLabel = new QLabel(tr("<i>Check box to fix/constrain the associated parameter</i>"), this);
    infoLabel->setStyleSheet("QLabel { background-color : blue; color : white; }");
    gridLayout->addWidget(infoLabel, 5, 0, 1, 3, Qt::AlignVCenter | Qt::AlignHCenter);

    setLayout(gridLayout);

    // register the fields
    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        QString name = TTMParameterSet::name((TTMParameterSet::ParameterType)type);
        QString text(name);
        registerField(text, mParameterValue[(TTMParameterSet::ParameterType)type]);
        text = name + "Fix";
        registerField(text, mParameterFix[type]);
        text = name + "Fixm";
        registerField(text, mFitMin[type]);
        text = name + "FixM";
        registerField(text, mFitMax[type]);
        text = name + "Fixs";
        registerField(text, mFitSte[type]);
    }
    registerField(TTMParameterSet::name(TTMParameterSet::kMUQ)+"C",       mParameterCon[TTMParameterSet::kMUQ]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUQ)+"CD",      mDensity[TTMParameterSet::kMUQ]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUS)+"C",       mParameterCon[TTMParameterSet::kMUS]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUS)+"CD",      mDensity[TTMParameterSet::kMUS]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUC)+"C",       mParameterCon[TTMParameterSet::kMUC]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUC)+"CD",      mDensity[TTMParameterSet::kMUC]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUBEAUTY)+"C",  mParameterCon[TTMParameterSet::kMUBEAUTY]);
    registerField(TTMParameterSet::name(TTMParameterSet::kMUBEAUTY)+"CD", mDensity[TTMParameterSet::kMUBEAUTY]);
}

//__________________________________________________________________________
double ParaSel::getB2Q() const
{
    // calulates B / 2 * Q
    double rv = 0.0;

    int test = getCollision();
    switch (test) {
    case kPbPb:
        rv =  208. / (2. * 82.);
        break;
    case kAuAu:
        rv = 197. / (2. * 79.);
        break;
    case kpp:
        rv = 1.;
        break;
    case kpPb:
        rv =  208. / (2. * 82.);
        break;
    default:
        break;
    }
    return rv;
}

//__________________________________________________________________________
int ParaSel::getCollision() const
{
    // checks which collision system is selected

    int rv = 0;

    for (int index = 0; index <  kCollTypes; index++) {
        if (mCollisions[index]->isChecked()) {
            rv = index;
            break;
        }
    }
    return rv;
}

//__________________________________________________________________________
bool ParaSel::isConstrained(TTMParameterSet::ParameterType type) const
{
    bool rv = false;
    switch (type) {
    case TTMParameterSet::kMUQ:
        if (mParameterCon[type]->isChecked())
            rv = true;
        break;
    case TTMParameterSet::kMUS:
        if (mParameterCon[type]->isChecked())
            rv = true;
        break;
    case TTMParameterSet::kMUC:
        if (mParameterCon[type]->isChecked())
            rv = true;
        break;
    case TTMParameterSet::kMUBEAUTY:
        if (mParameterCon[type]->isChecked())
            rv = true;
        break;
    default:
        rv = false;
        break;
    }
    return rv;
}

//__________________________________________________________________________
void ParaSel::setFitValues(TTMParameterSet::ParameterType type, double min, double max, double step)
{
    // set the fitting constrain values
    mFitMin[type]->setText(QString("%1").arg(min));
    mFitMax[type]->setText(QString("%1").arg(max));
    mFitSte[type]->setText(QString("%1").arg(step));
}

//__________________________________________________________________________
void ParaSel::updateDisplay()
{
    // update the display after the default parameters have been set

    for (int type = 0; type < TTMParameterSet::kPARTYPES; type++) {
        if (mParameterFix[type]) {
            mFitMin[type]->setText("Fixed");
            mFitMax[type]->setText("Fixed");
            mFitSte[type]->setText("Fixed");
        }
    }
}

//__________________________________________________________________________
void ParaSel::conParameter(TTMParameterSet::ParameterType type)
{
    // displays the box to set constrain value
    bool read;
    QString text;
    if (mParameterCon[type]->isChecked()) {
        mPalette.setColor(QPalette::Text,Qt::green);
        read = false;
        text = "";
    } else {
        mPalette.setColor(QPalette::Text,Qt::red);
        read = true;
        text = "OFF";
    }
    mDensity[type]->setReadOnly(read);
    mDensity[type]->setPalette(mPalette);
    mDensity[type]->setText(text);
}

//__________________________________________________________________________
void ParaSel::fixParameter(TTMParameterSet::ParameterType type)
{
    // fix/free parameters and request fitting constrains
    QGroupBox *temp = NULL;

    switch (type) {
    case TTMParameterSet::kT:
        temp = mTFitBox;
        break;
    case TTMParameterSet::kMUB:
        temp = mMuFitBox;
        break;
    case TTMParameterSet::kMUQ:
        temp = mMuFitBox;
        break;
    case TTMParameterSet::kGAMMAS:
        temp = mGammaFitBox;
        break;
    case TTMParameterSet::kCRADIUS:
        temp = mRFitBox;
        break;
    case TTMParameterSet::kRADIUS:
        temp = mRFitBox;
        break;
    case TTMParameterSet::kGAMMAC:
        temp = mGammaFitBox;
        break;
    case TTMParameterSet::kGAMMABEAUTY:
        temp = mGammaFitBox;
        break;
    case TTMParameterSet::kMUS:
        temp = mMuFitBox;
        break;
    case TTMParameterSet::kMUC:
        temp = mMuFitBox;
        break;
    case TTMParameterSet::kMUBEAUTY:
        temp = mMuFitBox;
        break;
    default:
        break;
    }
    if (mParameterFix[type]->isChecked()) {
        mPalette.setColor(QPalette::Text,Qt::red);
        mParameterValue[type]->setReadOnly(true);
        mFitMin[type]->setText("Fixed");
        mFitMax[type]->setText("Fixed");
        mFitSte[type]->setText("Fixed");
        mFitMin[type]->setReadOnly(true);
        mFitMax[type]->setReadOnly(true);
        mFitSte[type]->setReadOnly(true);

    } else {
        mParameterValue[type]->setReadOnly(true);
        mPalette.setColor(QPalette::Text,Qt::green);
        mParameterValue[type]->setReadOnly(false);
        mFitMin[type]->setText(QString("%1").arg(0.05));
        mFitMax[type]->setText(QString("%1").arg(0.180));
        mFitMin[type]->setReadOnly(false);
        mFitMax[type]->setReadOnly(false);
        mFitSte[type]->setReadOnly(false);
        mFitSte[type]->setText(QString("%1").arg(0.001));

    }
    mParameterValue[type]->setPalette(mPalette);
    mFitMin[type]->setPalette(mPalette);
    mFitMax[type]->setPalette(mPalette);
    mFitSte[type]->setPalette(mPalette);
}
