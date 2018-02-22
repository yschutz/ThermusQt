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

#include "TTMParameterSet.h"

class ParaSel : public QWizardPage
{
    Q_OBJECT
    Q_ENUMS (CollSysType)
public:
    enum CollSysType {kPbPb, kAuAu, kpp, kpPb, kCollTypes };

    explicit ParaSel(QWidget* parent = nullptr);


    void    constrainParameter(TTMParameterSet::ParameterType type);
    double  getB2Q() const;
    double  getConstrainValue(TTMParameterSet::ParameterType type) const
            { if (type == TTMParameterSet::kMUQ) return getB2Q(); else return mDensity[type]->text().toDouble(); }
    qint32  getCollision() const;
    double  getFitMax(TTMParameterSet::ParameterType type) const
            { return mFitMax[type]->text().toDouble(); }
    double  getFitMin(TTMParameterSet::ParameterType type) const
            { return mFitMin[type]->text().toDouble(); }
    double  getFitSte(TTMParameterSet::ParameterType type) const
            { return mFitSte[type]->text().toDouble(); }
    double  getParaValue(TTMParameterSet::ParameterType type) const
            { return mParameterValue[type]->text().toDouble(); }
    bool    isConstrained(TTMParameterSet::ParameterType type) const;
    bool    isFixed(QCheckBox* box)
            { if (box->isChecked()) return true; else return false;}
    bool    isFixed(TTMParameterSet::ParameterType type)
            { return isFixed(mParameterFix[type]); }
    void    setFitValues(TTMParameterSet::ParameterType type, double min, double max, double step);
    void    setParaValue(TTMParameterSet::ParameterType type, double val)
            { mParameterValue[type]->setText(QString("%1").arg(val)); }
    void    updateDisplay();

public slots:
    void fixParameter(TTMParameterSet::ParameterType type);
    void conParameter(TTMParameterSet::ParameterType type);

private:
    void    fit(TTMParameterSet::ParameterType type, bool check)
            { mParameterFix[type]->setChecked(!check); fixParameter(type);}

    QRadioButton*  mCollisions[kCollTypes];                       // the list of possible of collision systems
    bool           mConstrain[TTMParameterSet::kPARTYPES];        // if true parameter is constrained
    QLineEdit*     mDensity[TTMParameterSet::kPARTYPES];          // value of the density to constrain for BQ, S, C, and Beauty
    QLineEdit*     mFitMax[TTMParameterSet::kPARTYPES];           // max value of the parameters for the fit
    QLineEdit*     mFitMin[TTMParameterSet::kPARTYPES];           // min value of the parameters for the fit
    QLineEdit*     mFitSte[TTMParameterSet::kPARTYPES];           // step value of the parameters for the fit
    QGroupBox*     mGammaFitBox;                                  // the fit box parameters for gammas
    QGridLayout*   mGammaFitBoxLayout;                            // and its layout
    QGroupBox*     mMuConBox;                                     // the constrain box parameters for the mus
    QGridLayout*   mMuConBoxLayout;                               // and its layout
    QGroupBox*     mMuFitBox;                                     // the fit box parameters for the mus
    QGridLayout*   mMuFitBoxLayout;                               // and its layout
    QCheckBox*     mParameterCon[TTMParameterSet::kPARTYPES];     // checkbox list to constrain or not the parameters
    QCheckBox*     mParameterFix[TTMParameterSet::kPARTYPES];     // checkbox list to fix or not the parameters
    QLineEdit*     mParameterValue[TTMParameterSet::kPARTYPES];   // checkbox list to fix or not the parameters
    QPalette       mPalette;                                      // the palette for the QLineEdit objects
    QGroupBox*     mRFitBox;                                      // the fit box parameters for R
    QGridLayout*   mRFitBoxLayout;                                // and its layout
    QGroupBox*     mTFitBox;                                      // the fit box parameters for T
    QGridLayout*   mTFitBoxLayout;                                // and its layout
};

#endif // PARASEL_H
