// Author: Yves Schutz 24 octobre 2016
//
// The steering class to prediction thermus (aka ROOT macro)


#ifndef PREDICTION_H
#define PREDICTION_H

#include <QDebug>
#include <QObject>

#include "filedialog.h"
#include "parasel.h"
#include "summary.h"

#include "main/TTMParticleSet.h"
#include "main/TTMParameterSetBSQ.h"

class Prediction : public QObject
{
    Q_OBJECT

public:
    static      Prediction& instance();

    bool        isDebug() const { return mDebug; }
    void        listParameters() const { mParaInfo->list(); }
    void        selectParticlesListFile();
    void        setConstrain();
    void        setDebug(bool val) {mDebug = val; }
    void        setDefaultParameters();
    void        setDialog(FileDialog* val) {mFileDialog = val;}
    void        setFitFix() ;
    void        setParameters();
    void        setParaSel(ParaSel* val);
    void        setParticlesListFile();
    void        setSummary(Summary * val) {mSummary = val;}

private:
    Prediction();
    ~Prediction();
    Prediction (const Prediction&) { }
//    prediction& operator = (const prediction&) { }

    bool                mDebug;            // true if debug mode on
    static Prediction   mInstance;         // unique instance of the object
    QString             mParticlesList;    // txt file listing the particles
    FileDialog*         mFileDialog;       // dialog window to select a file
    TTMParameterSetBSQ* mParaInfo;         // holds the information on parameters
    ParaSel*            mParaSel;          // window for parameters selection
    TTMParticleSet*     mPartInfo;         // holds the information on particles
    Summary*            mSummary;          // summary wizard page
};

#endif // PREDICTION_H
