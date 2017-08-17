#include <QApplication>
#include <QCheckBox>
#include <QDebug>
#include <QFontMetrics>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "particlesdbmanager.h"
#include "listdialog.h"
#include "newdecaydialog.h"

//__________________________________________________________________________
ListDialog::ListDialog(const QStringList &decays, QWidget *parent) : QDialog(parent)
{
    // a dialog box, listing all decays of a given particle (called from FindDialog) and options for modifications

    // decode the string list
    mPartName = decays.at(0);

    QGridLayout* decayCheckLayout = new QGridLayout;
    decayCheckLayout->addWidget(new QLabel("DB ID"), 0, 0, Qt::AlignCenter);
    decayCheckLayout->addWidget(new QLabel("BR"),    0, 1, Qt::AlignCenter);
    decayCheckLayout->addWidget(new QLabel("BRN"),   0, 2, Qt::AlignCenter);
    decayCheckLayout->addWidget(new QLabel("Decay"), 0, 3, Qt::AlignCenter);
    QFontMetrics metrics(QApplication::font());
    for (int index = 1; index < decays.size(); index++) {
        QString decay = decays.at(index);
        QString id = (decay.split(' ').first());
        QString br = decay.split('[').first().split(' ').last();
        QString brn = decay.split(']').first().split('[').last();
        QString sdecay = decay.split('>').last().trimmed();
        QLabel* idLabel = new QLabel(id);
        mids.append(idLabel);
        decayCheckLayout->addWidget(idLabel, index, 0);
        QLineEdit* brEdit = new QLineEdit(br);
        brEdit->setReadOnly(true);
        brEdit->setFixedSize(metrics.width("8888888888"), metrics.height() * 1.5);
        brEdit->setAlignment(Qt::AlignRight);
        brEdit->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
        mbrs.append(brEdit);
        decayCheckLayout->addWidget(brEdit, index, 1, Qt::AlignCenter);
        QLabel* brnLabel = new QLabel(brn);
        brnLabel->setAlignment(Qt::AlignRight);
        mbrns.append(brnLabel);
        decayCheckLayout->addWidget(brnLabel, index, 2, Qt::AlignCenter);
        decayCheckLayout->addWidget(new QLabel(sdecay), index, 3, Qt::AlignCenter);
        QCheckBox* check = new QCheckBox;
        check->setChecked(false);
        connect(check, SIGNAL(stateChanged(int)), this, SLOT(ckecked(int)));
        mChecks.append(check);
        decayCheckLayout->addWidget(check, index, 4, Qt::AlignCenter);
    }

    mMinus = new QPushButton("-");
    connect(mMinus, SIGNAL(clicked(bool)), this, SLOT(remove()));
    mPlus = new QPushButton("+");
    connect(mPlus, SIGNAL(clicked(bool)), this, SLOT(add()));
    mMod = new QPushButton("->");
    connect(mMod, SIGNAL(clicked(bool)), this, SLOT(modify()));
    mDone   = new QPushButton("Done");
    connect(mDone, SIGNAL(clicked(bool)), this, SLOT(close()));

    QGroupBox* gb = new QGroupBox("add/remove/modify decay");
    QHBoxLayout* plmiLayout = new QHBoxLayout;
    plmiLayout->addWidget(mPlus);
    plmiLayout->addWidget(mMinus);
    plmiLayout->addWidget(mMod);
    gb->setLayout(plmiLayout);

    QHBoxLayout* doneLayout = new QHBoxLayout;
    doneLayout->addWidget(mDone);
    doneLayout->addWidget(gb);

    QWidget* viewport = new QWidget;
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scroll->setWidget(viewport);
    scroll->setWidgetResizable(true);

    viewport->setLayout(decayCheckLayout);

    QVBoxLayout *dialog_layout = new QVBoxLayout(this);
    dialog_layout->addWidget(scroll); // add scroll to the QDialog's layout
    dialog_layout->addLayout(doneLayout);

    setLayout(dialog_layout);

    setWindowTitle(QString(tr("%1 decays")).arg(mPartName));
    resize(500, 100);
}

//__________________________________________________________________________
void ListDialog::add()
{
    // add a decay: firs check if the decay is found in PDGParticles.db

//    QString save = ParticlesDBManager::Instance().dbName();
//    QString newName(save);
//    newName.replace("ThermusParticles", "PDGParticles");
//    bool connected = ParticlesDBManager::Instance().connect(newName);
//    if (!connected) {
//        QMessageBox msg(QMessageBox::Critical, "DB connection",
//                        QString("Could not connect to particles DB: %1").arg(newName));
//        msg.exec();
//    } else {
//    QStringList list = ParticlesDBManager::Instance().listDecays(ParticlesDBManager::Instance().currentPart());
//    qDebug() << list;
//    }
//    connected = ParticlesDBManager::Instance().connect(save);

      NewDecayDialog* nd = new NewDecayDialog(mPartName, this);
      nd->move(pos().x() + 300, pos().y() + 300);
      nd->exec();
}

//__________________________________________________________________________
void ListDialog::ckecked(int /*which*/)
{
    // called whenever the status of check button changed
    for (int index = 0; index < mChecks.size(); index++) {
        if (mChecks.at(index)->isChecked()) {
            mbrs.at(index)->setReadOnly(false);
            mbrs.at(index)->setStyleSheet("QLineEdit { background: rgb(212, 255, 255); selection-background-color: rgb(233, 99, 0); }");
        } else {
            mbrs.at(index)->setReadOnly(true);
            mbrs.at(index)->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");

        }
    }
}

//__________________________________________________________________________
void ListDialog::modify()
{
    // modify the branching ratio of a selected decay and recalculate the normalized branching ratio

    int decayIndex = 0;
    double sum = 0.0;
    for (int index = 0; index < mChecks.size(); index++)
        sum += mbrs.at(index)->text().toDouble();
    for (int index = 0; index < mChecks.size(); index++) {
        double value = mbrs.at(index)->text().toDouble();
        mbrns.at(index)->setText(QString("%1").arg(value * 100 / sum));
        if (mChecks.at(index)->isChecked()) {
            decayIndex = mids.at(index)->text().toInt();
            value      = mbrs.at(index)->text().toDouble();
            ParticlesDBManager::Instance().modifyBR(decayIndex, value);
        }
    }
}

//__________________________________________________________________________
void ListDialog::remove()
{

}
