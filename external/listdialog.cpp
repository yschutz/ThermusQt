#include <QCheckBox>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include "external/particlesdbmanager.h"
#include "listdialog.h"

//__________________________________________________________________________
ListDialog::ListDialog(const QStringList &decays, QWidget *parent) : QDialog(parent)
{
    QString tempo = decays.at(0);
    QStringList ltempo = tempo.split(':');
    QString particleName = ltempo[0];
    QString sdecays      = ltempo[1];
    sdecays = sdecays.trimmed();
    mIndexList.append((sdecays.left(sdecays.indexOf(' '))).toInt());

    QStringList particleDecays;
    particleDecays.append(ltempo[1]);
    for (int index = 1; index < decays.size(); index++) {
        sdecays = decays.at(index);
        sdecays = sdecays.trimmed();
        mIndexList.append((sdecays.left(sdecays.indexOf(' '))).toInt());
        particleDecays.append(sdecays);
    }

    QGridLayout* decayCheckLayout = new QGridLayout;
    for (int index = 0; index < particleDecays.size(); index++) {
        QLineEdit* line = new QLineEdit;
        line->setReadOnly(true);
        QString text = ((QString)particleDecays.at(index)).prepend(QString("%1: ").arg(index, 3));
        line->setText(text);
        decayCheckLayout->addWidget(line, index, 0);
        QCheckBox* check = new QCheckBox;
        check->setChecked(false);
        mChecks.append(check);
        decayCheckLayout->addWidget(check, index, 1);
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

    setWindowTitle(QString(tr("%1 decays")).arg(particleName));
    resize(500, 100);
}

//__________________________________________________________________________
void ListDialog::add()
{
    // add a decay: firs check if the decay is found in PDGParticles.db
    QString save = ParticlesDBManager::Instance().dbName();
    QString newName(save);
    newName.replace("ThermusParticles", "PDGParticles");
    bool connected = ParticlesDBManager::Instance().connect(newName);
    if (!connected) {
        QMessageBox msg(QMessageBox::Critical, "DB connection",
                        QString("Could not connect to particles DB: %1").arg(newName));
        msg.exec();
    } else {
    QStringList list = ParticlesDBManager::Instance().listDecays(ParticlesDBManager::Instance().currentPart());
    qDebug() << list;
    }
    connected = ParticlesDBManager::Instance().connect(save);
}

//__________________________________________________________________________
void ListDialog::modify()
{
    // modify the selected decay
    int decayIndex = 0;
    for (int index = 0; index < mChecks.size(); index++) {
        if (mChecks.at(index)->isChecked()) {
            decayIndex = mIndexList.at(index);
            double br = ParticlesDBManager::Instance().getBR(decayIndex);
            ParticlesDBManager::Instance().modifyBR(decayIndex, 0.0);
        }
    }
}

//__________________________________________________________________________
void ListDialog::remove()
{

}
