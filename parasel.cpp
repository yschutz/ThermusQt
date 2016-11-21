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

    // create an interactive widow to select parametres

    setTitle("Select parameters values and fit constrains");

    for (qint32 index = 0; index < kParTypes; index++) {
        mFitMax[index]       = NULL;
        mFitMin[index]       = NULL;
        mFitSte[index]       = NULL;
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
    QLabel *tempLabel = new QLabel(tr("T:"), tBox);
    mParameterValue[kTemp] = new QLineEdit(tBox);
    tempLabel->setBuddy(mParameterValue[kTemp]);
    QLabel *tempUnitLabel = new QLabel(tr("GeV"));
    mParameterFix[kTemp] = new QCheckBox(" ", tBox);
    mParameterFix[kTemp]->setChecked(true);
    mParameterValue[kTemp]->setPalette(mPalette);
    mParameterValue[kTemp]->setReadOnly(true);
    connect(mParameterFix[kTemp], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kTemp); });

    QGridLayout *tLayout = new QGridLayout(tBox);
    tLayout->addWidget(tfix, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    tLayout->addWidget(tempLabel, 1, 0, 1, 1);
    tLayout->addWidget(mParameterValue[kTemp], 1, 1, 1, 1);
    tLayout->addWidget(tempUnitLabel, 1, 2, 1, 1);
    tLayout->addWidget(mParameterFix[kTemp], 1, 3, 1, 1, Qt::AlignHCenter);
    tBox->setLayout(tLayout);

    mTFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *tmin    = new QLabel(tr("min"), mTFitBox);
    QLabel *tmax    = new QLabel(tr("max"), mTFitBox);
    QLabel *tste    = new QLabel(tr("step"), mTFitBox);
    QLabel *blanckT = new QLabel(" ", mTFitBox);
    mTFitBoxLayout = new QGridLayout(mTFitBox);
    mTFitBoxLayout->setColumnMinimumWidth(1, 50);
    mTFitBoxLayout->setColumnMinimumWidth(2, 50);
    mTFitBoxLayout->setColumnMinimumWidth(3, 50);
    mTFitBoxLayout->addWidget(tmin, 0, 1, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mTFitBoxLayout->addWidget(tmax, 0, 2, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mTFitBoxLayout->addWidget(tste, 0, 3, 1, 1, Qt::AlignTop | Qt::AlignHCenter);
    mFitMin[kTemp] = new QLineEdit(mTFitBox);
    mFitMax[kTemp] = new QLineEdit(mTFitBox);
    mFitSte[kTemp] = new QLineEdit(mTFitBox);
    mFitMin[kTemp]->setText("Fixed");
    mFitMax[kTemp]->setText("Fixed");
    mFitSte[kTemp]->setText("Fixed");
    mFitMin[kTemp]->setReadOnly(true);
    mFitMax[kTemp]->setReadOnly(true);
    mFitSte[kTemp]->setReadOnly(true);
    mFitMin[kTemp]->setPalette(mPalette);
    mFitMax[kTemp]->setPalette(mPalette);
    mFitSte[kTemp]->setPalette(mPalette);
    mTFitBoxLayout->addWidget(blanckT, 1, 0, 1, 1);
    mTFitBoxLayout->addWidget(mFitMin[kTemp], 1, 1, 1, 1);
    mTFitBoxLayout->addWidget(mFitMax[kTemp], 1, 2, 1, 1);
    mTFitBoxLayout->addWidget(mFitSte[kTemp], 1, 3, 1, 1);
    mTFitBox->setLayout(mTFitBoxLayout);

    // the mu's

    QGroupBox *muBox = new QGroupBox(tr("Chemical potentials"), this);

    QLabel *mufix       = new QLabel(tr("Fix"), muBox);
    QLabel *muconstrain = new QLabel(tr("Constrain"), muBox);

    QLabel  *muQLabel = new QLabel(muBox);
    muQLabel->setTextFormat(Qt::RichText);
    muQLabel->setText("<html> &mu;<sub>Q</sub>:");
    mParameterValue[kMuQ] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[kMuQ]);
    mParameterFix[kMuQ] = new QCheckBox("", muBox);
    mParameterFix[kMuQ]->setChecked(true);
    mParameterCon[kMuQ] = new QCheckBox("", muBox);
    mParameterCon[kMuQ]->setChecked(false);
    mParameterValue[kMuQ]->setPalette(mPalette);
    mParameterValue[kMuQ]->setReadOnly(true);
    connect(mParameterFix[kMuQ], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuQ); });
    connect(mParameterCon[kMuQ], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuQ); });
    QLabel *b2qLabel = new QLabel("<html> B/2Q", muBox);

    QLabel  *muBLabel = new QLabel(muBox);
    muBLabel->setTextFormat(Qt::RichText);
    muBLabel->setText("<html> &mu;<sub>B</sub>:");
    mParameterValue[kMuB] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[kMuB]);
    mParameterFix[kMuB] = new QCheckBox("", muBox);
    mParameterFix[kMuB]->setChecked(true);
    mParameterValue[kMuB]->setPalette(mPalette);
    mParameterValue[kMuB]->setReadOnly(true);
    connect(mParameterFix[kMuB], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuB); });

    QLabel  *muSLabel = new QLabel(muBox);
    muSLabel->setTextFormat(Qt::RichText);
    muSLabel->setText("<html> &mu;<sub>S</sub>:");
    mParameterValue[kMuS] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[kMuS]);
    mParameterFix[kMuS] = new QCheckBox("", muBox);
    mParameterFix[kMuS]->setChecked(true);
    mParameterCon[kMuS] = new QCheckBox("", muBox);
    mParameterCon[kMuS]->setChecked(false);
    mParameterValue[kMuS]->setPalette(mPalette);
    mParameterValue[kMuS]->setReadOnly(true);
    connect(mParameterFix[kMuS], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuS); });
    connect(mParameterCon[kMuS], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuS); });
    QLabel *sDensityLabel = new QLabel("<html> S Density", muBox);


    QLabel  *muCLabel = new QLabel(muBox);
    muCLabel->setTextFormat(Qt::RichText);
    muCLabel->setText("<html> &mu;<sub>C</sub>:");
    mParameterValue[kMuC] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[kMuC]);
    mParameterFix[kMuC] = new QCheckBox("", muBox);
    mParameterFix[kMuC]->setChecked(true);
    mParameterCon[kMuC] = new QCheckBox("", muBox);
    mParameterCon[kMuC]->setChecked(false);
    mParameterValue[kMuC]->setPalette(mPalette);
    mParameterValue[kMuC]->setReadOnly(true);
    connect(mParameterFix[kMuC], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuC); });
    connect(mParameterCon[kMuC], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuC); });
    QLabel *cDensityLabel = new QLabel("<html> C Density", muBox);

    QLabel  *muBeautyLabel = new QLabel(muBox);
    muBeautyLabel->setTextFormat(Qt::RichText);
    muBeautyLabel->setText("<html> &mu;<sub>Beauty</sub>:");
    mParameterValue[kMuBeauty] = new QLineEdit(muBox);
    muQLabel->setBuddy(mParameterValue[kMuBeauty]);
    mParameterFix[kMuBeauty] = new QCheckBox("", muBox);
    mParameterFix[kMuBeauty]->setChecked(true);
    mParameterCon[kMuBeauty] = new QCheckBox("", muBox);
    mParameterCon[kMuBeauty]->setChecked(false);
    mParameterValue[kMuBeauty]->setPalette(mPalette);
    mParameterValue[kMuBeauty]->setReadOnly(true);
    connect(mParameterFix[kMuBeauty], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuBeauty); });
    connect(mParameterCon[kMuBeauty], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuBeauty); });
    QLabel *bDensityLabel = new QLabel("<html> B Density", muBox);

    QGridLayout *muBoxLayout = new QGridLayout(muBox);
    muBoxLayout->addWidget(mufix, 0, 2, 1, 1, Qt::AlignTop);
    muBoxLayout->addWidget(muconstrain, 0, 3, 1, 2, Qt::AlignTop | Qt::AlignHCenter);

    muBoxLayout->addWidget(muQLabel, 1, 0);
    muBoxLayout->addWidget(mParameterValue[kMuQ], 1, 1);
    muBoxLayout->addWidget(mParameterFix[kMuQ], 1, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[kMuQ], 1, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(b2qLabel, 1, 4);

    muBoxLayout->addWidget(muBLabel, 2, 0);
    muBoxLayout->addWidget(mParameterValue[kMuB], 2, 1);
    muBoxLayout->addWidget(mParameterFix[kMuB], 2, 2, Qt::AlignHCenter);

    muBoxLayout->addWidget(muSLabel, 3, 0);
    muBoxLayout->addWidget(mParameterValue[kMuS], 3, 1);
    muBoxLayout->addWidget(mParameterFix[kMuS], 3, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[kMuS], 3, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(sDensityLabel, 3, 4);

    muBoxLayout->addWidget(muCLabel, 4, 0);
    muBoxLayout->addWidget(mParameterValue[kMuC], 4, 1);
    muBoxLayout->addWidget(mParameterFix[kMuC], 4, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[kMuC], 4, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(cDensityLabel, 4, 4, Qt::AlignHCenter);

    muBoxLayout->addWidget(muBeautyLabel, 5, 0);
    muBoxLayout->addWidget(mParameterValue[kMuBeauty], 5, 1, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterFix[kMuBeauty], 5, 2, Qt::AlignHCenter);
    muBoxLayout->addWidget(mParameterCon[kMuBeauty], 5, 3, Qt::AlignHCenter);
    muBoxLayout->addWidget(bDensityLabel, 5, 4);

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
    qint32 row = 1;
    for (qint32 type = kMuQ; type <= kMuBeauty; type++) {
        mFitMin[type] = new QLineEdit(mMuFitBox);
        mFitMax[type] = new QLineEdit(mMuFitBox);
        mFitSte[type] = new QLineEdit(mMuFitBox);
        mFitMin[type]->setText("Fixed");
        mFitMax[type]->setText("Fixed");
        mFitSte[type]->setText("Fixed");
        mFitMin[type]->setReadOnly(true);
        mFitMax[type]->setReadOnly(true);
        mFitSte[type]->setReadOnly(true);
        mFitMin[type]->setPalette(mPalette);
        mFitMax[type]->setPalette(mPalette);
        mFitSte[type]->setPalette(mPalette);
        mMuFitBoxLayout->addWidget(blanckMu, row, 0, 1, 1);
        mMuFitBoxLayout->addWidget(mFitMin[type], row, 1, 1, 1);
        mMuFitBoxLayout->addWidget(mFitMax[type], row, 2, 1, 1);
        mMuFitBoxLayout->addWidget(mFitSte[type], row, 3, 1, 1);
        row++;
    }

    mMuFitBox->setLayout(mMuFitBoxLayout);

    mMuConBox = new QGroupBox(tr("Constrain"), this);
    mMuConBoxLayout = new QGridLayout(mMuConBox);
    mMuConBoxLayout->setColumnMinimumWidth(2, 50);
    QLabel *blanck0 = new QLabel("", mMuConBox);
    mMuConBoxLayout->addWidget(blanck0, 0, 0, 1, 2, Qt::AlignTop);

    row = 1;
    for (qint32 type = kMuQ; type <= kMuBeauty; type++) {
        mDensity[type] = new QLineEdit("S dens", mMuConBox);
        mDensity[type]->setText("OFF");
        mDensity[type]->setPalette(mPalette);
        mMuConBoxLayout->addWidget(mDensity[type], row, 2, 1, 1);
        row++;
    }

    mMuConBox->setLayout(mMuConBoxLayout);

//    // the gamma's
    QGroupBox *gammaBox = new QGroupBox(tr("Fugacities"), this);

    QLabel *gammafix       = new QLabel(tr("Fix"), gammaBox);

    QLabel  *gammaSLabel = new QLabel(gammaBox);
    gammaSLabel->setTextFormat(Qt::RichText);
    gammaSLabel->setText("<html> &gamma;<sub>S</sub>:");
    mParameterValue[kGammaS] = new QLineEdit(gammaBox);
    muQLabel->setBuddy(mParameterValue[kGammaS]);
    mParameterFix[kGammaS] = new QCheckBox("", gammaBox);
    mParameterFix[kGammaS]->setChecked(true);
    mParameterValue[kGammaS]->setPalette(mPalette);
    mParameterValue[kGammaS]->setReadOnly(true);
    connect(mParameterFix[kGammaS], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kGammaS); });

    QLabel  *gammaCLabel = new QLabel(tr("gammaC:"), gammaBox);
    gammaCLabel->setTextFormat(Qt::RichText);
    gammaCLabel->setText("<html> &gamma;<sub>C</sub>:");
    mParameterValue[kGammaC] = new QLineEdit(gammaBox);
    muQLabel->setBuddy(mParameterValue[kGammaC]);
    mParameterFix[kGammaC] = new QCheckBox("", gammaBox);
    mParameterFix[kGammaC]->setChecked(true);
    mParameterValue[kGammaC]->setPalette(mPalette);
    mParameterValue[kGammaC]->setReadOnly(true);
    connect(mParameterFix[kGammaC], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kGammaC); });

    QLabel  *gammaBeautyLabel = new QLabel(tr("gammaBeauty:"), gammaBox);
    gammaBeautyLabel->setTextFormat(Qt::RichText);
    gammaBeautyLabel->setText("<html> &gamma;<sub>Beauty</sub>:");
    mParameterValue[kGammaBeauty] = new QLineEdit(gammaBox);
    muQLabel->setBuddy(mParameterValue[kGammaBeauty]);
    mParameterFix[kGammaBeauty] = new QCheckBox("", gammaBox);
    mParameterFix[kGammaBeauty]->setChecked(true);
    mParameterValue[kGammaBeauty]->setPalette(mPalette);
    mParameterValue[kGammaBeauty]->setReadOnly(true);
    connect(mParameterFix[kGammaBeauty], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kGammaBeauty); });

    QGridLayout *gammaBoxLayout = new QGridLayout(gammaBox);
    gammaBoxLayout->addWidget(gammafix, 0, 2, 1, 1);

    gammaBoxLayout->addWidget(gammaSLabel, 1, 0);
    gammaBoxLayout->addWidget(mParameterValue[kGammaS], 1, 1);
    gammaBoxLayout->addWidget(mParameterFix[kGammaS], 1, 2, Qt::AlignHCenter);

    gammaBoxLayout->addWidget(gammaCLabel, 2, 0);
    gammaBoxLayout->addWidget(mParameterValue[kGammaC], 2, 1);
    gammaBoxLayout->addWidget(mParameterFix[kGammaC], 2, 2, Qt::AlignHCenter);

    gammaBoxLayout->addWidget(gammaBeautyLabel, 3, 0);
    gammaBoxLayout->addWidget(mParameterValue[kGammaBeauty], 3, 1);
    gammaBoxLayout->addWidget(mParameterFix[kGammaBeauty], 3, 2, Qt::AlignHCenter);

    gammaBox->setLayout(gammaBoxLayout);

    mGammaFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *gammamin = new QLabel(tr("min"), mGammaFitBox);
    QLabel *gammamax = new QLabel(tr("max"), mGammaFitBox);
    QLabel *gammaste = new QLabel(tr("step"), mGammaFitBox);
    QLabel *blanckGa = new QLabel(" ", mGammaFitBox);
    mGammaFitBoxLayout = new QGridLayout(mGammaFitBox);
    mGammaFitBoxLayout->setColumnMinimumWidth(2, 50);
    mGammaFitBoxLayout->addWidget(gammamin, 0, 1, 1, 1, Qt::AlignTop);
    mGammaFitBoxLayout->addWidget(gammamax, 0, 2, 1, 1, Qt::AlignTop);
    mGammaFitBoxLayout->addWidget(gammaste, 0, 3, 1, 1, Qt::AlignTop);
    row = 1;
    for (qint32 type = kGammaS; type <= kGammaBeauty; type++) {
        mFitMin[type] = new QLineEdit(mGammaFitBox);
        mFitMax[type] = new QLineEdit(mGammaFitBox);
        mFitSte[type] = new QLineEdit(mGammaFitBox);
        mFitMin[type]->setText("Fixed");
        mFitMax[type]->setText("Fixed");
        mFitSte[type]->setText("Fixed");
        mFitMin[type]->setReadOnly(true);
        mFitMax[type]->setReadOnly(true);
        mFitSte[type]->setReadOnly(true);
        mFitMin[type]->setPalette(mPalette);
        mFitMax[type]->setPalette(mPalette);
        mFitSte[type]->setPalette(mPalette);
        mGammaFitBoxLayout->addWidget(blanckGa, row, 0, 1, 1);
        mGammaFitBoxLayout->addWidget(mFitMin[type], row, 1, 1, 1);
        mGammaFitBoxLayout->addWidget(mFitMax[type], row, 2, 1, 1);
        mGammaFitBoxLayout->addWidget(mFitSte[type], row, 3, 1, 1);
        row++;
    }

    mGammaFitBox->setLayout(mGammaFitBoxLayout);

    // the collision system
    QGroupBox *collisionsBox = new QGroupBox(tr("Collision system"));
    mCollisions[kPbPb] = new QRadioButton(collSysNames[kPbPb], collisionsBox);
    mCollisions[kAuAu] = new QRadioButton(collSysNames[kAuAu], collisionsBox);
    mCollisions[kpp]   = new QRadioButton(collSysNames[kpp], collisionsBox);
    mCollisions[kpPb]  = new QRadioButton(collSysNames[kpPb], collisionsBox);
    mCollisions[kPbPb]->setChecked(true);

    QVBoxLayout *collRadioBoxLayout = new QVBoxLayout(collisionsBox);
    for (qint32 index = 0; index < kCollTypes; index ++)
        collRadioBoxLayout->addWidget(mCollisions[index]);

    collisionsBox->setLayout(collRadioBoxLayout);

//    // the radius
    QGroupBox *rBox = new QGroupBox(tr("Radius"), this);

    QLabel *rfix       = new QLabel(tr("Fix"), this);

    QLabel *radiusLabel = new QLabel(tr("r:"));
    mParameterValue[kRadius] = new QLineEdit(rBox);
    radiusLabel->setBuddy(mParameterValue[kRadius]);
    QLabel *radiusUnitLabel = new QLabel(tr("fm"));
    mParameterFix[kRadius] = new QCheckBox("", rBox);
    mParameterFix[kRadius]->setChecked(true);
    mParameterValue[kRadius]->setPalette(mPalette);
    mParameterValue[kRadius]->setReadOnly(true);
    connect(mParameterFix[kRadius], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kRadius); });

    QGridLayout *rLayout = new QGridLayout(rBox);
    rLayout->addWidget(rfix, 0, 3, 1, 1);
    rLayout->addWidget(radiusLabel, 1, 0, 1, 1);
    rLayout->addWidget(mParameterValue[kRadius], 1, 1, 1, 1);
    rLayout->addWidget(radiusUnitLabel, 1, 2, 1, 1);
    rLayout->addWidget(mParameterFix[kRadius], 1, 3, 1, 1, Qt::AlignHCenter);

    rBox->setLayout(rLayout);

    mRFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *rmin = new QLabel(tr("min"), mRFitBox);
    QLabel *rmax = new QLabel(tr("max"), mRFitBox);
    QLabel *rste = new QLabel(tr("step"), mRFitBox);
    QLabel *blanckR = new QLabel(" ", mRFitBox);

    mRFitBoxLayout = new QGridLayout(this);
    mRFitBoxLayout->addWidget(rmin, 0, 1, 1, 1, Qt::AlignTop);
    mRFitBoxLayout->addWidget(rmax, 0, 2, 1, 1, Qt::AlignTop);
    mRFitBoxLayout->addWidget(rste, 0, 3, 1, 1, Qt::AlignTop);

    mFitMin[kRadius] = new QLineEdit(mRFitBox);
    mFitMax[kRadius] = new QLineEdit(mRFitBox);
    mFitSte[kRadius] = new QLineEdit(mRFitBox);
    mFitMin[kRadius]->setText("Fixed");
    mFitMax[kRadius]->setText("Fixed");
    mFitSte[kRadius]->setText("Fixed");
    mFitMin[kRadius]->setReadOnly(true);
    mFitMax[kRadius]->setReadOnly(true);
    mFitSte[kRadius]->setReadOnly(true);
    mFitMin[kRadius]->setPalette(mPalette);
    mFitMax[kRadius]->setPalette(mPalette);
    mFitSte[kRadius]->setPalette(mPalette);
    mRFitBoxLayout->addWidget(blanckR, row, 0, 1, 1);
    mRFitBoxLayout->addWidget(mFitMin[kRadius], row, 1, 1, 1);
    mRFitBoxLayout->addWidget(mFitMax[kRadius], row, 2, 1, 1);
    mRFitBoxLayout->addWidget(mFitSte[kRadius], row, 3, 1, 1);

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

}

//__________________________________________________________________________
double ParaSel::getB2Q() const
{
    // calulates B / 2 * Q
    double rv = 0.0;

    qint32 test = getCollision();
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
qint32 ParaSel::getCollision() const
{
    // checks which collision system is selected

    qint32 rv = 0;

    for (qint32 index = 0; index <  kCollTypes; index++) {
        if (mCollisions[index]->isChecked()) {
            rv = index;
            break;
        }
    }
    return rv;
}

//__________________________________________________________________________
QString ParaSel::getParaName(ParaSel::ParameterType type)
{
 // returns the given name of the parameter

    QString rv = "";
    switch (type) {
    case kGammaS:
        rv= "gammaS";
        break;
    case kGammaC:
        rv = "gammaC";
        break;
    case kGammaBeauty:
        rv = "gammaBeauty";
        break;
    case kMuQ:
        rv = "muQ";
        break;
    case kMuB:
        rv = "muB";
        break;
    case kMuS:
        rv = "muS";
        break;
    case kMuC:
        rv = "muC";
        break;
    case kMuBeauty:
        rv = "muBeauty";
        break;
    case kRadius:
        rv = "radius";
        break;
    case kTemp:
        rv = "Temperature";
        break;
    default:
        break;
    }

    return rv;
}

//__________________________________________________________________________
bool ParaSel::isConstrained(ParaSel::ParameterType type) const
{
   bool rv = false;
   switch (type) {
   case kMuQ:
       if (mParameterCon[type]->isChecked())
           rv = true;
       break;
   case kMuS:
       if (mParameterCon[type]->isChecked())
           rv = true;
       break;
   case kMuC:
       if (mParameterCon[type]->isChecked())
           rv = true;
       break;
   case kMuBeauty:
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
void ParaSel::setFitValues(ParaSel::ParameterType type, double min, double max, double step)
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

    for (qint32 type = 0; type < ParaSel::kParTypes; type++) {
        if (mParameterFix[type]) {
            mFitMin[type]->setText("Fixed");
            mFitMax[type]->setText("Fixed");
            mFitSte[type]->setText("Fixed");
        }
    }
}

//__________________________________________________________________________
void ParaSel::conParanmeter(ParameterType type)
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
void ParaSel::fixParanmeter(ParaSel::ParameterType type)
{
   // fix/free parameters and request fitting constrains
    qint32 subRow  = 0.;
    QGroupBox *temp = NULL;

    switch (type) {
    case kTemp:
        subRow  = 1;
        temp = mTFitBox;
        break;
    case kMuQ:
        subRow  = 1;
        temp = mMuFitBox;
        break;
    case kMuB:
        subRow  = 2;
        temp = mMuFitBox;
        break;
    case kMuS:
        subRow  = 3;
        temp = mMuFitBox;
        break;
    case kMuC:
        subRow  = 4;
        temp = mMuFitBox;
        break;
    case kMuBeauty:
        subRow  = 5;
        temp = mMuFitBox;
        break;
    case kGammaS:
        subRow  = 1;
        temp = mGammaFitBox;
        break;
    case kGammaC:
        subRow  = 2;
        temp = mGammaFitBox;
        break;
    case kGammaBeauty:
        subRow  = 3;
        temp = mGammaFitBox;
        break;
    case kRadius:
        subRow  = 1;
        temp = mRFitBox;
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
