#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "newdecaydialog.h"
#include "particlesdbmanager.h"

//__________________________________________________________________________
NewDecayDialog::NewDecayDialog(QStringList &decays, QWidget*/*parent*/)
{
    // a dialog box to enter a new decay
    setAttribute(Qt::WA_DeleteOnClose);
    QString partName = ParticlesDBManager::Instance().currentPart();

    QVBoxLayout* decaysLayout = new QVBoxLayout;
    for (int index = 1; index < decays.size(); index++) {
        QRadioButton* dcheck = new QRadioButton(decays.at(index));
        dcheck->setChecked(false);
        QString decay = decays.at(index);
        connect(dcheck, &QRadioButton::clicked, this, [decay, this] { selectDecay(decay); });
        decaysLayout->addWidget(dcheck);
    }
    QRadioButton* other = new QRadioButton("Other");
    other->setChecked(false);
    connect(other, &QRadioButton::clicked, this, [this] { selectDecay("Other"); });
    decaysLayout->addWidget(other);

    QLabel* brLabel = new QLabel("BR");
    QLabel* decayLabel = new QLabel("Decay(pdg1, pdg2, pdg3,...)");
    mbrEdit = new QLineEdit();
    mbrEdit->setReadOnly(true);
    mbrEdit->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
    mdecayEdit = new QLineEdit();
    mdecayEdit->setReadOnly(true);
    mdecayEdit->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
    QHBoxLayout* editLayout = new QHBoxLayout;
    editLayout->addWidget(brLabel);
    editLayout->addWidget(mbrEdit);
    editLayout->addWidget(decayLabel);
    editLayout->addWidget(mdecayEdit);

    QPushButton* insertButton = new QPushButton("Insert");
    connect(insertButton, &QPushButton::clicked, this, [partName, this]{ insert(partName); });

    QPushButton* cancelButton = new QPushButton("Cancel");
    connect(cancelButton, &QPushButton::clicked, this, [this]{ close(); });

    QHBoxLayout* insertcancelLayout = new QHBoxLayout;
    insertcancelLayout->addWidget(insertButton);
    insertcancelLayout->addWidget(cancelButton);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(decaysLayout);
    mainLayout->addLayout(editLayout);
    mainLayout->addLayout(insertcancelLayout);

    setLayout(mainLayout);
    int pdg = ParticlesDBManager::Instance().getPDG(partName);
    setWindowTitle(QString("New decay for particle %1 (pdg = %2)").arg(partName).arg(pdg));
}

//__________________________________________________________________________
void NewDecayDialog::insert(QString partName)
{
    // action triggered by GO button
    double br = mbrEdit->text().toDouble();
    if (br == 0.0) {
        QMessageBox msg(QMessageBox::Critical, "Wrong syntax", QString("%1").arg(br));
        msg.exec();
        return;
    }
    QString decay = mdecayEdit->text();
    QStringList pdgs = decay.split(',');
    for (QString spdg : pdgs) {
        int pdg = spdg.toInt();
        if (pdg == 0) {
            QMessageBox msg(QMessageBox::Critical, "Wrong syntax", decay);
            msg.exec();
            return;
        }
    }
    int motherid = ParticlesDBManager::Instance().id(partName);
    ParticlesDBManager::Instance().insertDecay(motherid, 0, br, pdgs);
    close();
    emit QDialog::finished(0);
}

//__________________________________________________________________________
void NewDecayDialog::selectDecay(QString decay)
{
    // pick the selected decay
    if (decay != "Other") {
        QString id = (decay.split(' ').first());
        QString br = decay.split('[').first().split(' ').last();
        QString brn = decay.split(']').first().split('[').last();
        QString sdecay = decay.split('>').last().trimmed();
        QStringList sdecays = sdecay.split(",");
        sdecay.clear();
        for (QString dd : sdecays) {
            dd = dd.trimmed();
            int pdg = ParticlesDBManager::Instance().getPDG(dd);
            sdecay.append(QString("%1, ").arg(pdg));
        }
        sdecay.remove(sdecay.lastIndexOf(','), sdecay.size());
        mbrEdit->setText(br);
        mbrEdit->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
        mdecayEdit->setText(sdecay);
        mdecayEdit->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
    } else {
        mbrEdit->clear();
        mbrEdit->setReadOnly(false);
        mbrEdit->setStyleSheet("QLineEdit { background: rgb(212, 255, 255); selection-background-color: rgb(233, 99, 0); }");
        mdecayEdit->clear();
        mdecayEdit->setReadOnly(false);
        mdecayEdit->setStyleSheet("QLineEdit { background: rgb(212, 255, 255); selection-background-color: rgb(233, 99, 0); }");
    }
}
