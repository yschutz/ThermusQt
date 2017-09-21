#include "newparticledialog.h"


#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <QVBoxLayout>

#include "particlesdbmanager.h"

//__________________________________________________________________________
NewParticleDialog::NewParticleDialog(QWidget *parent) : QDialog(parent)
{
    // ctor: a dialog box to include a new particle in Thermus DB
    setAttribute(Qt::WA_DeleteOnClose);

    mEntriesLayout = new QVBoxLayout;
    newEntry(ParticlesDBManager::kPDG, false);
    newEntry(ParticlesDBManager::kNAME);
    newEntry(ParticlesDBManager::kSPIN);
    newEntry(ParticlesDBManager::kSTATISTIC);
    newEntry(ParticlesDBManager::kMASS);
    newEntry(ParticlesDBManager::kS);
    newEntry(ParticlesDBManager::kBARYON);
    newEntry(ParticlesDBManager::kCHARGE);
    newEntry(ParticlesDBManager::kC);
    newEntry(ParticlesDBManager::kB);
    newEntry(ParticlesDBManager::kT);
    newEntry(ParticlesDBManager::kSC);
    newEntry(ParticlesDBManager::kCC);
    newEntry(ParticlesDBManager::kBC);
    newEntry(ParticlesDBManager::kTC);
    newEntry(ParticlesDBManager::kWIDTH);
    newEntry(ParticlesDBManager::kLIFETIME);
    newEntry(ParticlesDBManager::kTHRESHOLD);
    newEntry(ParticlesDBManager::kRADIUS);
    newEntry(ParticlesDBManager::kNDECAY);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(mEntriesLayout);

    QPushButton* dummyButton = new QPushButton("dummy");
    dummyButton->setVisible(false);

    QPushButton* insertButton = new QPushButton(QString(tr("&Insert")));
    connect(insertButton, &QPushButton::clicked, this, [this]{ insert(); });

    QPushButton *resetButton = new QPushButton(QString(tr("&Reset")));
    connect(resetButton, &QPushButton::clicked, this, [this]{ reset(); });

    QPushButton *doneButton = new QPushButton(QString(tr("&Done")));
    connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(dummyButton);
    buttonLayout->addWidget(insertButton);
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(doneButton);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
    setWindowTitle(QString(tr("New Particle")));

}

//__________________________________________________________________________
void NewParticleDialog::insert()
{
    // insert the particle into the Therms DB

    QList<QString> parameters;
    for (int index = ParticlesDBManager::kPDG; index != ParticlesDBManager::kLAST; index++)
        parameters.append(mEntries.at(index)->text());
    ParticlesDBManager::Instance().insertParticle(parameters);
}

//__________________________________________________________________________
void NewParticleDialog::reset()
{
    // resets all entries

    mEntries.at(ParticlesDBManager::kPDG)->setText("");
    for (int index = 1; index < mEntries.size(); index++) {
        mEntries.at(index)->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
        mEntries.at(index)->setReadOnly(true);
        mEntries.at(index)->setText("");

    }

}

//__________________________________________________________________________
void NewParticleDialog::searchName()
{
    // check if not exist in Thermus DB or PDG DB
    int pdg = mEntries.at(ParticlesDBManager::kPDG)->text().toInt();
    QString name = ParticlesDBManager::Instance().getName(pdg, ParticlesDBManager::Instance().getThermusDBName());
    if (!name.isEmpty()) {
        QMessageBox msg(QMessageBox::Critical, "Exists", QString("%1 already in Thermus DB").arg(name));
        msg.exec();
        reset();
        return;
    } else {
        name = ParticlesDBManager::Instance().getName(pdg, ParticlesDBManager::Instance().getPDGDBName());
    }
    if (name.isEmpty()) {
        for (int index = ParticlesDBManager::kNAME; index != ParticlesDBManager::kLAST; index++) {
            mEntries.at(index)->setReadOnly(false);
            mEntries.at(index)->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
        }
    } else {
        mEntries.at(ParticlesDBManager::kNAME)->setText(name);

        double mass = ParticlesDBManager::Instance().getMass(pdg, ParticlesDBManager::Instance().getPDGDBName());
        mEntries.at(ParticlesDBManager::kMASS)->setText(QString("%1").arg(mass, 13, 'g', 8));

        double charge = ParticlesDBManager::Instance().getCharge(pdg, ParticlesDBManager::Instance().getPDGDBName());
        mEntries.at(ParticlesDBManager::kCHARGE)->setText(QString("%1").arg(charge, 13, 'g', 8));

        double width = ParticlesDBManager::Instance().getWidth(pdg, ParticlesDBManager::Instance().getPDGDBName());
        mEntries.at(ParticlesDBManager::kWIDTH)->setText(QString("%1").arg(width, 13, 'g', 8));

        double lifetime = ParticlesDBManager::Instance().getLifetime(pdg, ParticlesDBManager::Instance().getPDGDBName());
        mEntries.at(ParticlesDBManager::kLIFETIME)->setText(QString("%1").arg(lifetime, 13, 'g', 8));

        for (int index = ParticlesDBManager::kNAME; index != ParticlesDBManager::kLAST; index++) {
            mEntries.at(index)->setReadOnly(false);
            mEntries.at(index)->setStyleSheet("QLineEdit { background: rgb(212, 255, 255); selection-background-color: rgb(233, 99, 0); }");
        }
    }
}

//__________________________________________________________________________
void NewParticleDialog::newEntry(ParticlesDBManager::ENTRY en, bool readonly)
{
   // add a new parameter
    QMetaEnum metaEnum = QMetaEnum::fromType<ParticlesDBManager::ENTRY>();
    QString labelText    = metaEnum.valueToKey(en);
    labelText = labelText.remove(0,1).toLower();

    QHBoxLayout* oneEntryLayout = new QHBoxLayout;
    oneEntryLayout->addWidget(new QLabel(labelText), Qt::AlignRight);
    QLineEdit* lined = new QLineEdit;
    lined->setReadOnly(readonly);
    if (readonly)
        lined->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
    else
        lined->setStyleSheet("QLineEdit { background: rgb(212, 255, 255); selection-background-color: rgb(233, 99, 0); }");
    mEntries.insert(en, lined);
    oneEntryLayout->addWidget(lined);
    mEntriesLayout->addLayout(oneEntryLayout);
    if (en == ParticlesDBManager::kPDG)
        connect(lined, &QLineEdit::returnPressed, this, [this]{ searchName(); });
}
