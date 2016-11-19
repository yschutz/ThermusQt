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

    QLabel *tfix       = new QLabel(tr("Fix"), this);

    QLabel *tempLabel = new QLabel(tr("T:"), this);
    mParameterValue[kTemp] = new QLineEdit(this);
    tempLabel->setBuddy(mParameterValue[kTemp]);
    QLabel *tempUnitLabel = new QLabel(tr("GeV"));
    mParameterFix[kTemp] = new QCheckBox("", this);
    mParameterFix[kTemp]->setChecked(true);
    mParameterValue[kTemp]->setPalette(mPalette);
    mParameterValue[kTemp]->setReadOnly(true);
    connect(mParameterFix[kTemp], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kTemp); });

    QGridLayout *tLayout = new QGridLayout(this);
    tLayout->addWidget(tfix, 0, 3, 1, 1);
    tLayout->addWidget(tempLabel, 1, 0, 1, 1);
    tLayout->addWidget(mParameterValue[kTemp], 1, 1, 1, 1);
    tLayout->addWidget(tempUnitLabel, 1, 2, 1, 1);
    tLayout->addWidget(mParameterFix[kTemp], 1, 3, 1, 1, Qt::AlignHCenter);
    tBox->setLayout(tLayout);

    mTFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *tmin = new QLabel(tr("min"), this);
    QLabel *tmax = new QLabel(tr("max"), this);
    QLabel *tste = new QLabel(tr("step"), this);
    mTFitBoxLayout = new QGridLayout(this);
    mTFitBoxLayout->addWidget(tmin, 0, 0, 1, 1, Qt::AlignTop);
    mTFitBoxLayout->addWidget(tmax, 0, 1, 1, 1, Qt::AlignTop);
    mTFitBoxLayout->addWidget(tste, 0, 2, 1, 1, Qt::AlignTop);
    mTFitBox->setLayout(mTFitBoxLayout);

    // the mu's

    QGroupBox *muBox = new QGroupBox(tr("Chemical potentials"), this);

    QLabel *mufix       = new QLabel(tr("Fix"), this);
    QLabel *muconstrain = new QLabel(tr("Constrain"), this);

    QLabel  *muQLabel = new QLabel(this);
    muQLabel->setTextFormat(Qt::RichText);
    muQLabel->setText("<html> &mu;<sub>Q</sub>:");
    mParameterValue[kMuQ] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kMuQ]);
    mParameterFix[kMuQ] = new QCheckBox("", this);
    mParameterFix[kMuQ]->setChecked(true);
    mParameterCon[kMuQ] = new QCheckBox("", this);
    mParameterCon[kMuQ]->setChecked(false);
    mParameterValue[kMuQ]->setPalette(mPalette);
    mParameterValue[kMuQ]->setReadOnly(true);
    connect(mParameterFix[kMuQ], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuQ); });
    connect(mParameterCon[kMuQ], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuQ); });
    QLabel *b2qLabel = new QLabel(this);
    b2qLabel->setText("<html> B/2Q");

    QLabel  *muBLabel = new QLabel(this);
    muBLabel->setTextFormat(Qt::RichText);
    muBLabel->setText("<html> &mu;<sub>B</sub>:");
    mParameterValue[kMuB] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kMuB]);
    mParameterFix[kMuB] = new QCheckBox("", this);
    mParameterFix[kMuB]->setChecked(true);
    mParameterCon[kMuB] = new QCheckBox("", this);
    mParameterValue[kMuB]->setPalette(mPalette);
    mParameterValue[kMuB]->setReadOnly(true);
    connect(mParameterFix[kMuB], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuB); });
    connect(mParameterCon[kMuB], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuB); });

    QLabel  *muSLabel = new QLabel(this);
    muSLabel->setTextFormat(Qt::RichText);
    muSLabel->setText("<html> &mu;<sub>S</sub>:");
    mParameterValue[kMuS] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kMuS]);
    mParameterFix[kMuS] = new QCheckBox("", this);
    mParameterFix[kMuS]->setChecked(true);
    mParameterCon[kMuS] = new QCheckBox("", this);
    mParameterCon[kMuS]->setChecked(false);
    mParameterValue[kMuS]->setPalette(mPalette);
    mParameterValue[kMuS]->setReadOnly(true);
    connect(mParameterFix[kMuS], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuS); });
    connect(mParameterCon[kMuS], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuS); });
    QLabel *sDensityLabel = new QLabel(this);
    sDensityLabel->setText("<html> S Density");


    QLabel  *muCLabel = new QLabel(this);
    muCLabel->setTextFormat(Qt::RichText);
    muCLabel->setText("<html> &mu;<sub>C</sub>:");
    mParameterValue[kMuC] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kMuC]);
    mParameterFix[kMuC] = new QCheckBox("", this);
    mParameterFix[kMuC]->setChecked(true);
    mParameterCon[kMuC] = new QCheckBox("", this);
    mParameterCon[kMuC]->setChecked(false);
    mParameterValue[kMuC]->setPalette(mPalette);
    mParameterValue[kMuC]->setReadOnly(true);
    connect(mParameterFix[kMuC], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuC); });
    connect(mParameterCon[kMuC], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuC); });
    QLabel *cDensityLabel = new QLabel(this);
    cDensityLabel->setText("<html> C Density");

    QLabel  *muBeautyLabel = new QLabel(this);
    muBeautyLabel->setTextFormat(Qt::RichText);
    muBeautyLabel->setText("<html> &mu;<sub>Beauty</sub>:");
    mParameterValue[kMuBeauty] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kMuBeauty]);
    mParameterFix[kMuBeauty] = new QCheckBox("", this);
    mParameterFix[kMuBeauty]->setChecked(true);
    mParameterCon[kMuBeauty] = new QCheckBox("", this);
    mParameterCon[kMuBeauty]->setChecked(false);
    mParameterValue[kMuBeauty]->setPalette(mPalette);
    mParameterValue[kMuBeauty]->setReadOnly(true);
    connect(mParameterFix[kMuBeauty], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kMuBeauty); });
    connect(mParameterCon[kMuBeauty], &QCheckBox::stateChanged, this, [this]{ conParanmeter(kMuBeauty); });
    QLabel *bDensityLabel = new QLabel(this);
    bDensityLabel->setText("<html> B Density");

    QGridLayout *muBoxLayout = new QGridLayout(this);
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

    muBox->setLayout(muBoxLayout);

    QLabel *blanck0 = new QLabel("", this);
    QLabel *blanck1 = new QLabel("", this);
    QLabel *blanck2 = new QLabel("", this);
    QLabel *blanck3 = new QLabel("", this);
    QLabel *blanck4 = new QLabel("", this);
    QLabel *blanck5 = new QLabel("", this);

    mMuFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *mumin = new QLabel(tr("min"), this);
    QLabel *mumax = new QLabel(tr("max"), this);
    QLabel *muste = new QLabel(tr("step"), this);
    mMuFitBoxLayout = new QGridLayout(this);
    mMuFitBoxLayout->addWidget(mumin, 0, 0, 1, 1, Qt::AlignTop);
    mMuFitBoxLayout->addWidget(mumax, 0, 1, 1, 1, Qt::AlignTop);
    mMuFitBoxLayout->addWidget(muste, 0, 2, 1, 1, Qt::AlignTop);
//    mMuConBoxLayout->addWidget(blanck1, 1, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck2, 2, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck3, 3, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck4, 4, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck5, 5, 0, 1, 3, Qt::AlignHCenter);

    mMuFitBox->setLayout(mMuFitBoxLayout);

    mMuConBox = new QGroupBox(tr("Constrain"), this);
    mMuConBoxLayout =new QGridLayout(this);
    mMuConBoxLayout->addWidget(blanck0, 0, 0, 1, 2, Qt::AlignTop);
    mMuConBoxLayout->addWidget(blanck1, 1, 0, 1, 2, Qt::AlignHCenter);
    mMuConBoxLayout->addWidget(blanck2, 2, 0, 1, 2, Qt::AlignHCenter);
    mMuConBoxLayout->addWidget(blanck3, 3, 0, 1, 2, Qt::AlignHCenter);
    mMuConBoxLayout->addWidget(blanck4, 4, 0, 1, 2, Qt::AlignHCenter);
    mMuConBoxLayout->addWidget(blanck5, 5, 0, 1, 2, Qt::AlignHCenter);
    mMuConBox->setLayout(mMuConBoxLayout);

    // the gamma's
    QGroupBox *gammaBox = new QGroupBox(tr("Fugacities"), this);

    QLabel *gammafix       = new QLabel(tr("Fix"), this);

    QLabel  *gammaSLabel = new QLabel(this);
    gammaSLabel->setTextFormat(Qt::RichText);
    gammaSLabel->setText("<html> &gamma;<sub>S</sub>:");
    mParameterValue[kGammaS] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kGammaS]);
    mParameterFix[kGammaS] = new QCheckBox("", this);
    mParameterFix[kGammaS]->setChecked(true);
    mParameterValue[kGammaS]->setPalette(mPalette);
    mParameterValue[kGammaS]->setReadOnly(true);
    connect(mParameterFix[kGammaS], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kGammaS); });

    QLabel  *gammaCLabel = new QLabel(tr("gammaC:"), this);
    gammaCLabel->setTextFormat(Qt::RichText);
    gammaCLabel->setText("<html> &gamma;<sub>C</sub>:");
    mParameterValue[kGammaC] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kGammaC]);
    mParameterFix[kGammaC] = new QCheckBox("", this);
    mParameterFix[kGammaC]->setChecked(true);
    mParameterValue[kGammaC]->setPalette(mPalette);
    mParameterValue[kGammaC]->setReadOnly(true);
    connect(mParameterFix[kGammaC], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kGammaC); });

    QLabel  *gammaBeautyLabel = new QLabel(tr("gammaBeauty:"), this);
    gammaBeautyLabel->setTextFormat(Qt::RichText);
    gammaBeautyLabel->setText("<html> &gamma;<sub>Beauty</sub>:");
    mParameterValue[kGammaBeauty] = new QLineEdit(this);
    muQLabel->setBuddy(mParameterValue[kGammaBeauty]);
    mParameterFix[kGammaBeauty] = new QCheckBox("", this);
    mParameterFix[kGammaBeauty]->setChecked(true);
    mParameterValue[kGammaBeauty]->setPalette(mPalette);
    mParameterValue[kGammaBeauty]->setReadOnly(true);
    connect(mParameterFix[kGammaBeauty], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kGammaBeauty); });

    QGridLayout *gammaBoxLayout = new QGridLayout(this);
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
    QLabel *gammamin = new QLabel(tr("min"), this);
    QLabel *gammamax = new QLabel(tr("max"), this);
    QLabel *gammaste = new QLabel(tr("step"), this);
    mGammaFitBoxLayout = new QGridLayout(this);
    mGammaFitBoxLayout->addWidget(gammamin, 0, 0, 1, 1, Qt::AlignTop);
    mGammaFitBoxLayout->addWidget(gammamax, 0, 1, 1, 1, Qt::AlignTop);
    mGammaFitBoxLayout->addWidget(gammaste, 0, 2, 1, 1, Qt::AlignTop);
//    mMuConBoxLayout->addWidget(blanck1, 1, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck2, 2, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck3, 3, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck4, 4, 0, 1, 3, Qt::AlignHCenter);
//    mMuConBoxLayout->addWidget(blanck5, 5, 0, 1, 3, Qt::AlignHCenter);
    mGammaFitBox->setLayout(mGammaFitBoxLayout);

    // the collision system
    QGroupBox *collisionsBox = new QGroupBox(tr("Collision system"));
    mCollisions[kPbPb] = new QRadioButton(collSysNames[kPbPb], this);
    mCollisions[kAuAu] = new QRadioButton(collSysNames[kAuAu], this);
    mCollisions[kpp]   = new QRadioButton(collSysNames[kpp], this);
    mCollisions[kpPb]  = new QRadioButton(collSysNames[kpPb], this);
    mCollisions[kPbPb]->setChecked(true);

    QVBoxLayout *collRadioBoxLayout = new QVBoxLayout(this);
    for (qint32 index = 0; index < kCollTypes; index ++)
        collRadioBoxLayout->addWidget(mCollisions[index]);

    collisionsBox->setLayout(collRadioBoxLayout);

    // the radius
    QGroupBox *rBox = new QGroupBox(tr("Radius"), this);

    QLabel *rfix       = new QLabel(tr("Fix"), this);

    QLabel *radiusLabel = new QLabel(tr("r:"));
    mParameterValue[kRadius] = new QLineEdit(this);
    radiusLabel->setBuddy(mParameterValue[kRadius]);
    QLabel *radiusUnitLabel = new QLabel(tr("fm"));
    mParameterFix[kRadius] = new QCheckBox("", this);
    mParameterFix[kRadius]->setChecked(true);
    mParameterValue[kRadius]->setPalette(mPalette);
    mParameterValue[kRadius]->setReadOnly(true);
    connect(mParameterFix[kRadius], &QCheckBox::stateChanged, this, [this]{ fixParanmeter(kRadius); });

    QGridLayout *rLayout = new QGridLayout(this);
    rLayout->addWidget(rfix, 0, 3, 1, 1);
    rLayout->addWidget(radiusLabel, 1, 0, 1, 1);
    rLayout->addWidget(mParameterValue[kRadius], 1, 1, 1, 1);
    rLayout->addWidget(radiusUnitLabel, 1, 2, 1, 1);
    rLayout->addWidget(mParameterFix[kRadius], 1, 3, 1, 1, Qt::AlignHCenter);

    rBox->setLayout(rLayout);

    mRFitBox = new QGroupBox(tr("Fit"), this);
    QLabel *rmin = new QLabel(tr("min"), this);
    QLabel *rmax = new QLabel(tr("max"), this);
    QLabel *rste = new QLabel(tr("step"), this);
    mRFitBoxLayout = new QGridLayout(this);
    mRFitBoxLayout->addWidget(rmin, 0, 0, 1, 1, Qt::AlignTop);
    mRFitBoxLayout->addWidget(rmax, 0, 1, 1, 1, Qt::AlignTop);
    mRFitBoxLayout->addWidget(rste, 0, 2, 1, 1, Qt::AlignTop);
    mRFitBox->setLayout(mRFitBoxLayout);

// final layout
    QGridLayout *gridLayout = new QGridLayout(this);

    gridLayout->addWidget(tBox,     0, 0, 1, 1);
    gridLayout->addWidget(mTFitBox, 0, 1, 1, 1);

    gridLayout->addWidget(muBox,     1, 0, 1, 1, Qt::AlignTop);
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

    resize( QSize(1000, 500));
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
void ParaSel::conParanmeter(ParameterType type)
{
    // displays the box to set constrain value
    switch (type) {
    case kMuQ:
    {
        if (mParameterCon[kMuQ]->isChecked()) {
            mDensity[kMuQ] = new QLineEdit("B2Q");
            mMuConBoxLayout->addWidget(mDensity[kMuQ], 1, 1, 1, 1);
        } else {
            delete mDensity[kMuQ];
        }
        break;
    }
    case kMuS:
    {
        if (mParameterCon[kMuS]->isChecked()) {
            mDensity[kMuS] = new QLineEdit("S dens");
            mMuConBoxLayout->addWidget(mDensity[kMuS], 3, 1, 1, 1);
        } else {
            delete mDensity[kMuS];
        }
        break;
    }
    case kMuC:
    {
        if (mParameterCon[kMuC]->isChecked()) {
            mDensity[kMuC] = new QLineEdit("C dens");
            mMuConBoxLayout->addWidget(mDensity[kMuC], 4, 1, 1, 1);
        } else {
            delete mDensity[kMuC];
        }
        break;
    }
    case kMuBeauty:
    {
        if (mParameterCon[kMuBeauty]->isChecked()) {
            mDensity[kMuBeauty] = new QLineEdit("B dens");
            mMuConBoxLayout->addWidget(mDensity[kMuBeauty], 5, 1, 1, 1);
        } else {
            delete mDensity[kMuBeauty];
        }
        break;
    }
    default:
        break;
    }
}

//__________________________________________________________________________
void ParaSel::fixParanmeter(ParaSel::ParameterType type)
{

    qint32 subRow  = 0.;
    QGroupBox *temp = NULL;
    QLabel *blanck = new QLabel(this);

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
        if (mFitMin[type] != NULL) {
            delete mFitMin[type];
            mFitMin[type] = NULL;
            delete mFitMax[type];
            mFitMax[type] = NULL;
            delete mFitSte[type];
            mFitSte[type] = NULL;
        }
    } else {
        mPalette.setColor(QPalette::Text,Qt::green);
        mParameterValue[type]->setReadOnly(false);
        if (mFitMin[type] == NULL) {
            mFitMin[type] = new QLineEdit(this);
            mFitMax[type] = new QLineEdit(this);
            mFitSte[type] = new QLineEdit(this);
            mFitMin[type]->setText(QString("Min"));
            mFitMax[type]->setText(QString("Max"));
            mFitSte[type]->setText(QString("Step"));
            QGridLayout * lay = (QGridLayout*)temp->layout();
            for (qint32 index = 1; index < subRow; index++) {
                lay->addWidget(blanck,index, 0, 1, 1);
            }
            lay->addWidget(mFitMin[type], subRow, 0, 1, 1);
            lay->addWidget(mFitMax[type], subRow, 1, 1, 1);
            lay->addWidget(mFitSte[type], subRow, 2, 1, 1);
            temp->setLayout(lay);
        }
    }
    mParameterValue[type]->setPalette(mPalette);
}
