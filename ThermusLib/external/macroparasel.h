#ifndef MACROPARASEL_H
#define MACROPARASEL_H

#include <QCheckBox>
#include <QLineEdit>
#include <QObject>
#include <QRadioButton>
#include <QWizardPage>

class MacroParaSel : public QWizardPage
{
//    Q_OBJECT
public:
    explicit MacroParaSel(QWidget* parent = nullptr);

    QString dataFileName() const;
    double  getExclVol() const         { return mExclVolLE->text().toDouble(); }
    QString getTitle() const           { return mTitle->text(); }
    bool    isExclVol() const          { return mExclVolBut->isChecked(); }
    bool    isHyperonsFitted() const   { return mFitHyperons->isChecked(); }
    bool    isNucleiFitted() const     { return mFitNuclei->isChecked(); }
    bool    isModelBQ() const          { return mRadBQ->isChecked(); }
    bool    isModelBSQ() const         { return mRadBSQ->isChecked(); }
    bool    isProtonFitted() const     { return mFitProton->isChecked(); }
    bool    isQstat() const            { return mQstatBut->isChecked(); }
    bool    isResonancesFitted() const { return mFitResonances->isChecked(); }
    bool    isWidth() const            { return mWidthBut->isChecked(); }
    bool    isYields() const           { return mYieldRatio->isChecked(); }
    void    setData(const QString & d);
    void    setModelBQ(bool opt)       { mRadBQ->setChecked(opt); mRadBSQ->setChecked(!opt); }
    void    setModelBSQ(bool opt)      { mRadBSQ->setChecked(opt); mRadBQ->setChecked(!opt); }
    void    setExclVol(double v)       { mExclVolLE->setText(QString::number(v)); }
    void    setFitParticles(bool hyp, bool pro, bool res,bool nuc) const;
    void    setQstat(bool opt)         { mQstatBut->setChecked(opt); }
    void    setWidth(bool opt)         { mWidthBut->setChecked(opt); }
    void    list();

public slots:
    void    setData();

private:
    QString              mDataFileName; // name of the file containing data
    QCheckBox*           mExclVolBut;   // check button to switch on exclusion volume
    QLineEdit*           mExclVolLE;    // Line Editto enter exclusion volume value
    QCheckBox*           mFitHyperons;  // to exclude hyperons from the fit
    QCheckBox*           mFitNuclei;    // to exclude nuclei from the fit
    QCheckBox*           mFitProton;    // to exclude proton from the fit
    QCheckBox*           mFitResonances;// to exclude resonances from the fit
    QCheckBox*           mQstatBut;     // check button to switch on Q stat
    QRadioButton*        mRadBQ;        // radio button to select canonical model
    QRadioButton*        mRadBSQ;       // radio button to select grand canonical model
    QList<QRadioButton*> mRadFiles;     // radio button to switch on Q stat and resonance width
    QLineEdit*           mTitle;        // a title for the fit
    QCheckBox*           mWidthBut;     // check button to switch on resonance width
    QCheckBox*           mYieldRatio;   // data is yields or ratios
};

#endif // MACROPARASEL_H
