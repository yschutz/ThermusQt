#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "newdecaydialog.h"
#include "particlesdbmanager.h"

//__________________________________________________________________________
NewDecayDialog::NewDecayDialog(QString &partName, QWidget *parent)
{
    // a dialog box to enter a new decay

    setAttribute(Qt::WA_DeleteOnClose);

    QLabel* brLabel = new QLabel("BR");
    QLabel* decayLabel = new QLabel("Decay(pdg1, pdg2, pdg3,...)");
    mbrEdit = new QLineEdit();
    mbrEdit->setReadOnly(false);
    mdecayEdit = new QLineEdit();
    mdecayEdit->setReadOnly(false);
    QHBoxLayout* editLayout = new QHBoxLayout;
    editLayout->addWidget(brLabel);
    editLayout->addWidget(mbrEdit);
    editLayout->addWidget(decayLabel);
    editLayout->addWidget(mdecayEdit);

    QPushButton* goButton = new QPushButton("GO");
    connect(goButton, &QPushButton::clicked, this, [partName, this]{ go(partName); });

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(editLayout);
    mainLayout->addWidget(goButton);

    setLayout(mainLayout);
    setWindowTitle(QString("New decay for partocle ID %1").arg(partName));
}

//__________________________________________________________________________
void NewDecayDialog::go(QString partName)
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
    for (int index = 0; index < pdgs.size(); index++) {
        int pdg = pdgs.at(index).toInt();
        if (pdg == 0) {
            QMessageBox msg(QMessageBox::Critical, "Wrong syntax", decay);
            msg.exec();
            return;
        }
    }
    int motherid = ParticlesDBManager::Instance().findPartID(partName);
    ParticlesDBManager::Instance().insertDecay(motherid, 0, br, 0.0, pdgs);
    close();
}
