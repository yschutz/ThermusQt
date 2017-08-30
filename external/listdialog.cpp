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
ListDialog::ListDialog(QWidget *parent) : QDialog(parent)
{
    // a dialog box, listing all decays of a given particle (called from FindDialog) and options for modifications

    setAttribute(Qt::WA_DeleteOnClose);

    // decode the string list
    QString partName = ParticlesDBManager::Instance().currentPart();

    QGridLayout* decayLayout = new QGridLayout;

    decayLayout->addWidget(new QLabel("BR"),    0, 0, Qt::AlignCenter);
    decayLayout->addWidget(new QLabel("BRN"),   0, 1, Qt::AlignCenter);
    decayLayout->addWidget(new QLabel("Decay"), 0, 2, Qt::AlignCenter
                           );
    QFontMetrics metrics(QApplication::font());

    QStringList decays = ParticlesDBManager::Instance().listDecays(partName, 0.);

    for (int index = 1; index < decays.size(); index++) {
        QString decay = decays.at(index);
        QString id = (decay.split(' ').first());
        QString br = decay.split('[').first().split(' ').last();
        QString brn = decay.split(']').first().split('[').last();
        QString sdecay = decay.split('>').last().trimmed();
        QLineEdit* brEdit = new QLineEdit(br);
        brEdit->setReadOnly(true);
        brEdit->setFixedSize(metrics.width("8888888888"), metrics.height() * 1.5);
        brEdit->setAlignment(Qt::AlignRight);
        brEdit->setStyleSheet("QLineEdit { background: rgb(255, 211, 231); selection-background-color: rgb(233, 99, 0); }");
        mbrs.append(brEdit);
        decayLayout->addWidget(brEdit, index, 0, Qt::AlignCenter);
        QLabel* brnLabel = new QLabel(brn);
        brnLabel->setAlignment(Qt::AlignRight);
        mbrns.append(brnLabel);
        decayLayout->addWidget(brnLabel, index, 1, Qt::AlignCenter);
        decayLayout->addWidget(new QLabel(sdecay), index, 2, Qt::AlignCenter);
        QCheckBox* check = new QCheckBox(id);
        check->setChecked(false);
        connect(check, &QCheckBox::stateChanged, this, [this]{ ckecked(); });
        mChecks.append(check);
        decayLayout->addWidget(check, index, 3, Qt::AlignLeft);
    }

    QPushButton* minus = new QPushButton("-");
    connect(minus, &QPushButton::clicked, this, [this]{ remove(); });
    QPushButton* plus = new QPushButton("+");
    connect(plus, &QPushButton::clicked, this, [this]{ add(); });
    QPushButton* mod = new QPushButton("->");
    connect(mod, &QPushButton::clicked, this, [this]{ modify(); });
    QPushButton* done   = new QPushButton("Done");
    connect(done, &QPushButton::clicked, this, [this]{ close(); });

    QGroupBox* gb = new QGroupBox("add/remove/modify decay");
    QHBoxLayout* plmiLayout = new QHBoxLayout;
    plmiLayout->addWidget(plus);
    plmiLayout->addWidget(minus);
    plmiLayout->addWidget(mod);
    gb->setLayout(plmiLayout);

    QHBoxLayout* doneLayout = new QHBoxLayout;
    doneLayout->addWidget(done);
    doneLayout->addWidget(gb);

    QWidget* viewport = new QWidget;
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scroll->setWidget(viewport);
    scroll->setWidgetResizable(true);

    viewport->setLayout(decayLayout);

    QVBoxLayout *dialog_layout = new QVBoxLayout(this);
    dialog_layout->addWidget(scroll); // add scroll to the QDialog's layout
    dialog_layout->addLayout(doneLayout);

    setLayout(dialog_layout);

    setWindowTitle(QString(tr("decays of %1")).arg(partName));
    resize(500, 100);
}

//__________________________________________________________________________
void ListDialog::add()
{
    // add a decay: firs check if  decays are found in PDGParticles.db

    QString save = ParticlesDBManager::Instance().dbName();
    QString newName(save);
    newName.replace(ParticlesDBManager::Instance().getThermusDBName(), ParticlesDBManager::Instance().getPDGDBName());
    if (ParticlesDBManager::Instance().connect(newName)) {
        QStringList list = ParticlesDBManager::Instance().listDecays(ParticlesDBManager::Instance().currentPart());
        ParticlesDBManager::Instance().connect(save);
        NewDecayDialog* nd = new NewDecayDialog(list, this);
        connect(nd, &QDialog::finished, this, [this]{ refresh(); });
        nd->move(pos().x() + 300, pos().y() + 300);
        nd->exec();
    }
}

//__________________________________________________________________________
void ListDialog::ckecked()
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
    for (QLineEdit* lied : mbrs)
        sum += lied->text().toDouble();
    for (int index = 0; index < mChecks.size(); index++) {
        double value = mbrs.at(index)->text().toDouble();
        mbrns.at(index)->setText(QString("%1").arg(value * 100 / sum));
        if (mChecks.at(index)->isChecked()) {
            decayIndex = mChecks.at(index)->text().toInt();
            value      = mbrs.at(index)->text().toDouble();
            ParticlesDBManager::Instance().modifyBR(decayIndex, value);
        }
    }
    refresh();
}

//__________________________________________________________________________
void ListDialog::refresh()
{
  // refresh the decay list after a new decay has been inserted

    close();
    emit QDialog::accepted();

}

//__________________________________________________________________________
void ListDialog::remove()
{
    // remove decays

    for (QCheckBox* check : mChecks) {
        if (check->isChecked()) {
            int id = check->text().toInt();
            ParticlesDBManager::Instance().deleteDecay(id);
        }
    }
    refresh();
}
