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
MacroParaSel::MacroParaSel(QWidget* parent) : QWizardPage(parent)
{
//     create an interactive window to set parameters for the macro
    setTitle("Macro parameters setting");

    QPalette palette;
    palette.setColor(QPalette::Base,Qt::white);
    palette.setColor(QPalette::Text,Qt::green);


//     choice of model
    QGroupBox* modelBox  = new QGroupBox("Model");
    mRadBQ               = new QRadioButton("Canonical (BQ)");
    mRadBSQ              = new QRadioButton("Grand Canonical (BSQ)");

    QVBoxLayout* modelBoxLayout = new QVBoxLayout();
    modelBoxLayout->addWidget(mRadBQ);
    modelBoxLayout->addWidget(mRadBSQ);
    modelBox->setLayout(modelBoxLayout);

    // Q statistics, resonance width and exclusion volume
    QGroupBox* qstaBox     = new QGroupBox("Quantum statistics, resonance width and exclusion volume treatement");
    mQstatBut              = new QCheckBox("Quantum statistics on");
    mWidthBut              = new QCheckBox("Resonance width on");
    QHBoxLayout* eVoLayout = new QHBoxLayout();
    mExclVolBut            = new QCheckBox("Exclusion volume on");
    mExclVolLE             = new QLineEdit(QString::number(0.0));
    mExclVolLE->setPalette(palette);
    eVoLayout->addWidget(mExclVolBut);
    eVoLayout->addWidget(mExclVolLE);

    // Exclude from the fit
    QGroupBox* fitBox    = new QGroupBox("To be excluded from the fit");
    mFitProton           = new QCheckBox("Proton");
    mFitHyperons         = new QCheckBox("Hyperons");
    mFitResonances       = new QCheckBox("Resonances");
    mFitNuclei           = new QCheckBox("Nuclei");
    mFitProton->setChecked(true);
    mFitHyperons->setChecked(true);
    mFitResonances->setChecked(false);
    mFitNuclei->setChecked(false);
    QVBoxLayout* fitBoxLayout = new QVBoxLayout();
    fitBoxLayout->addWidget(mFitProton);
    fitBoxLayout->addWidget(mFitHyperons);
    fitBoxLayout->addWidget(mFitResonances);
    fitBoxLayout->addWidget(mFitNuclei);
    fitBox->setLayout(fitBoxLayout);

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
    QRadioButton* radFile = new QRadioButton("other (enter full path)");
    connect(radFile, &QRadioButton::toggled, this, [this] { setData(); });
    radFilesLayout->addWidget(radFile);
    mRadFiles.append(radFile);

    QGroupBox* dataBox   = new QGroupBox("Select data file");
    mYieldRatio          = new QCheckBox("Yields (checked) or ratio");
    QHBoxLayout* dataBoxLayout = new QHBoxLayout();
    dataBoxLayout->addWidget(mYieldRatio);
    dataBoxLayout->addLayout(radFilesLayout);
    dataBox->setLayout(dataBoxLayout);

    // title
    QGroupBox* titleBox  = new QGroupBox("Enter an informative title");
    mTitle               = new QLineEdit("centrality class - collision system  - other");
    mTitle->setReadOnly(false);
    mTitle->setPalette(palette);
    QVBoxLayout* titleBoxLayout = new QVBoxLayout();
    titleBoxLayout->addWidget(mTitle);
    titleBox->setLayout(titleBoxLayout);

    // main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(modelBox);
    mainLayout->addWidget(qstaBox);
    mainLayout->addWidget(fitBox);
    mainLayout->addWidget(dataBox);
    mainLayout->addWidget(titleBox);

    setLayout(mainLayout);
}

//__________________________________________________________________________
QString MacroParaSel::dataFileName() const
{
    // return the data file name
    QString rv;
    for (QRadioButton* but : mRadFiles)
        if (but->isChecked()) {
            rv = but->text();
            break;
        }
    return rv;
}

//__________________________________________________________________________
void MacroParaSel::list()
{
    // update the display after the default parameters have been set
    if (isModelBQ())
        qInfo() << " Model:                  Canonical (BQ)";
    else if (isModelBSQ())
        qInfo() << "Model:                  Grand Canonical (BSQ)";
    else
        qInfo() << Q_FUNC_INFO << " something went rong in choosing the statistical model";
    QString qstat = "OFF", width = "OFF";

    if (isQstat())
        qstat = "ON";
    if (isWidth())
        width = "ON";
    qInfo() << QString("Quantum statistics: %1").arg(qstat);
    qInfo() << QString("Resonances width:   %1").arg(width);

    if (isExclVol())
        qInfo() << QString("Exclusion volume: %1").arg(getExclVol());

    QString hyper = "NO", proton = "NO", reso = "NO", nuclei = "NO";
    if (isHyperonsFitted())
        hyper = "YES";
    if (isProtonFitted())
        proton = "YES";
    if (isResonancesFitted())
        reso = "YES";
    if (isNucleiFitted())
        nuclei = "YES";
    qInfo() << QString("proton fitted:      %1").arg(proton);
    qInfo() << QString("hyperons fitted:    %1").arg(hyper);
    qInfo() << QString("resonances fitted:  %1").arg(reso);
    qInfo() << QString("nuclei fitted:      %1").arg(nuclei);

    QString yr = "ratios";
    if (isYields())
        yr = "yields";
    qInfo() << QString("Data is:            %1 from %2").arg(yr, dataFileName());
}

//__________________________________________________________________________
void MacroParaSel::setData()
{
//    // picks up the data file name
    QRadioButton* rad = mRadFiles.last();
    if (rad->isChecked()) {
        QString dataFileName = QInputDialog::getText(this, "Enter file name (full path)", "name?", QLineEdit::Normal);
        if (!dataFileName.isEmpty())
            rad->setText(dataFileName);
    }
}
