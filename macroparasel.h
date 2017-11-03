#ifndef MACROPARASEL_H
#define MACROPARASEL_H

#include <QObject>
#include <QWizardPage>

class QRadioButton;
class QCheckBox;
class QLineEdit;
class MacroParaSel : public QWizardPage
{
public:
    MacroParaSel(QWidget* parent = nullptr);

    QString dataFileName() const { return mDataFileName; }
    bool    isExclVol() const    { if (mExclVol == 0.) return false; else return true; }
    bool    isModelBQ() const    { return mModelBQ; }
    bool    isModelBSQ() const   { return mModelBSQ; }
    bool    isQstat() const      { return mQstat; }
    bool    isWidth() const      { return mWidth; }
    void    setModelBQ()         { mModelBQ  = true; mModelBSQ = false; }
    void    setModelBSQ()        { mModelBSQ = true; mModelBQ  = false; }
    void    setExclVol(double v) { mExclVol  = v; }
    void    setQstat()           { mQstat    = true; }
    void    setWidth()           { mWidth    = true; }
    void    unsetModelBQ()       { mModelBQ  = false; mModelBSQ = true; }
    void    unsetModelBSQ()      { mModelBSQ = false; mModelBQ  = true; }
    void    unsetExclVol()       { mExclVol  = 0.; }
    void    unsetQstat()         { mQstat    = false; }
    void    unsetWidth()         { mWidth    = false; }
    void    updateDisplay();

public slots:
    void    setData();
    void    setEV();
    void    setModel();
    void    setQS();
    void    setW();

private:
    QString              mDataFileName; // name of the data file
    double               mExclVol ;     // value exclusion volume to be considered
    QCheckBox*           mExclVolBut;   // check button to switch on exclusion volume
    QLineEdit*           mExclVolLE;    // Line Editto enter exclusion volume value
    bool                 mModelBQ;      // true if canonical model to apply
    bool                 mModelBSQ;     // true if grand canonical model to apply
    bool                 mQstat ;       // true if quantum stat
    QCheckBox*           mQstatBut;     // check button to switch on Q stat
    QRadioButton*        mRadBQ;        // radio button to select canonical model
    QRadioButton*        mRadBSQ;       // radio button to select grand canonical model
    QList<QRadioButton*> mRadFiles;     // radio button to switch on Q stat and resonance width
    bool                 mWidth ;       // true if resonance width to be considered
    QCheckBox*           mWidthBut;     // check button to switch on resonance width
};

#endif // MACROPARASEL_H
