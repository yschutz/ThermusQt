#ifndef PARASEL_H
#define PARASEL_H

#include <QCheckBox>
#include <QDebug>
#include <QEventLoop>
#include <QGridLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QRadioButton>
#include <QWizardPage>

class ParaSel : public QWizardPage
{
    Q_OBJECT
    Q_ENUMS (CollSysType)
    Q_ENUMS (ParameterType)
    Q_ENUMS (ParameterName)
public:
    enum CollSysType {kPbPb, kAuAu, kpp, kpPb, kCollTypes };
    enum ParameterType {kGammaS, kGammaC, kGammaBeauty,
                        kMuQ, kMuB, kMuS, kMuC, kMuBeauty, kRadius, kTemp, kParTypes};

    explicit ParaSel(QWidget *parent = 0);

           double  getB2Q() const;
           double  getConstrainValue(ParameterType type) const  { return mDensity[type]->text().toDouble(); }
           qint32  getCollision() const;
           double  getFitMax(ParameterType type) const          { return mFitMax[type]->text().toDouble(); }
           double  getFitMin(ParameterType type) const          { return mFitMin[type]->text().toDouble(); }
           double  getFitSte(ParameterType type) const          { return mFitSte[type]->text().toDouble(); }
    static QString getParaName(ParameterType type);
           double  getParaValue(ParameterType type) const       { return mParameterValue[type]->text().toDouble(); }
           bool    isConstrained(ParameterType type) const;
           bool    isFixed(QCheckBox* box)                      { if (box->isChecked()) return true; else return false;}
           bool    isFixed(ParameterType type)                  { return isFixed(mParameterFix[type]); }
           void    setFitValues(ParameterType type, double min, double max, double step);
           void    setParaValue(ParameterType type, double val) { mParameterValue[type]->setText(QString("%1").arg(val)); }
           void    updateDisplay();

public slots:
    void fixParameter(ParameterType type);
    void conParameter(ParameterType type);

private:

    QRadioButton*  mCollisions[kCollTypes];      // the list of possible of collision systems
    bool           mConstrain[kParTypes];        // if true parameter is constrained
    QLineEdit*     mDensity[kParTypes];          // value of the density to constrain for BQ, S, C, and Beauty
    QLineEdit*     mFitMax[kParTypes];           // max value of the parameters for the fit
    double         mFitMaxD[kParTypes];          // default value;
    QLineEdit*     mFitMin[kParTypes];           // min value of the parameters for the fit
    double         mFitMinD[kParTypes];          // default value;
    QLineEdit*     mFitSte[kParTypes];           // step value of the parameters for the fit
    double         mFitSteD[kParTypes];          // default value;
    QGroupBox*     mGammaFitBox;                 // the fit box parameters for gammas
    QGridLayout*   mGammaFitBoxLayout;           // and its layout
    QGroupBox*     mMuConBox;                    // the constrain box parameters for the mus
    QGridLayout*   mMuConBoxLayout;              // and its layout
    QGroupBox*     mMuFitBox;                    // the fit box parameters for the mus
    QGridLayout*   mMuFitBoxLayout;              // and its layout
    QCheckBox*     mParameterCon[kParTypes];     // checkbox list to constrain or not the parameters
    QCheckBox*     mParameterFix[kParTypes];     // checkbox list to fix or not the parameters
    QLineEdit*     mParameterValue[kParTypes];   // checkbox list to fix or not the parameters
    QPalette       mPalette;                     // the palette for the QLineEdit objects
    QGroupBox*     mRFitBox;                     // the fit box parameters for R
    QGridLayout*   mRFitBoxLayout;               // and its layout
    QGroupBox*     mTFitBox;                     // the fit box parameters for T
    QGridLayout*   mTFitBoxLayout;               // and its layout
};

#endif // PARASEL_H
