#ifndef MACROPARASEL_H
#define MACROPARASEL_H

#include <QObject>
#include <QWizardPage>

class QRadioButton;
class MacroParaSel : public QWizardPage
{
public:
    MacroParaSel(QWidget* parent = nullptr);

    QString dataFileName() const { return mDataFileName; }
    bool    isModelBQ() const    { return mModelBQ; }
    bool    isModelBSQ() const   { return mModelBSQ; }
    bool    isQstat() const      { return mQstat; }
    void    setModelBQ()         { mModelBQ  = true; mModelBSQ = false; }
    void    setModelBSQ()        { mModelBSQ = true; mModelBQ  = false; }
    void    setQstat()           { mQstat    = true; }
    void    unsetModelBQ()       { mModelBQ  = false; mModelBSQ = true; }
    void    unsetModelBSQ()      { mModelBSQ = false; mModelBQ  = true; }
    void    unsetQstat()         { mQstat    = false; }
    void    updateDisplay();

public slots:
    void    setData();
    void    setModel();
    void    setQS();

private:
    QString              mDataFileName; // name of the data file
    bool                 mModelBQ;      // true if canonical model to apply
    bool                 mModelBSQ;     // true if grand canonical model to apply
    bool                 mQstat ;       // true if quantum stat and resonance width to be considered
    QRadioButton*        mRadBQ;        // radio button to select canonical model
    QRadioButton*        mRadBSQ;       // radio button to select grand canonical model
    QList<QRadioButton*> mRadFiles;      // radio button to switch on Q stat and resonance width
    QRadioButton*        mRadQsOn;      // radio button to switch on Q stat and resonance width
    QRadioButton*        mRadQsOff;     // radio button to switch off Q stat and resonance width
};

#endif // MACROPARASEL_H
