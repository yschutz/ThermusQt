#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include "finddialog.h"
#include "listdialog.h"
#include "external/particlesdbmanager.h"

//__________________________________________________________________________
FindDialog::FindDialog(const QString& what, QWidget *parent) : QDialog(parent), mLd(nullptr)
{
    mFindLabel = new QLabel(tr("Particle to find:"), this);
    mFindText  = new QLineEdit("PDG or name");
    mFindText->setClearButtonEnabled(true);
    mFindLabel->setBuddy(mFindText);

    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(mFindLabel);
    searchLayout->addWidget(mFindText);

    mGo = new QPushButton("Find");
    connect(mGo, SIGNAL(pressed()), this, SLOT(go()));
    mDone = new QPushButton("Done");
    connect(mDone, SIGNAL(clicked(bool)), this, SLOT(close()));

    mProperties = new QGroupBox("Properties");
    mProperties->setVisible(false);
    mPDG    = new QLabel("empty");
    mMass   = new QLabel("empty");
    mLife   = new QLabel("empty");
    mDecays = new QLabel("empty");
    QHBoxLayout* propertiesLayout = new QHBoxLayout;
    propertiesLayout->addWidget(mPDG);
    propertiesLayout->addWidget(mMass);
    propertiesLayout->addWidget(mLife);
    propertiesLayout->addWidget(mDecays);
    mProperties->setLayout(propertiesLayout);

    QHBoxLayout* goDoneLayout = new QHBoxLayout;
    goDoneLayout->addWidget(mGo);
    goDoneLayout->addWidget(mDone);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(searchLayout, 0, 0);
    mainLayout->addWidget(mProperties,1, 0, 1, 2, Qt::AlignHCenter);
    mainLayout->addLayout(goDoneLayout, 2, 0, 1, 2, Qt::AlignHCenter);

    setLayout(mainLayout);

    setWindowTitle(QString(tr("Particle search (%1)")).arg(what));

}

//__________________________________________________________________________
void FindDialog::go()
{
    // display particle properties and decays as extracted from the db
    QString name = mFindText->text();

    QStringList properties = ParticlesDBManager::Instance().listProperties(name);
    mPDG->setText(properties.at(0));
    mMass->setText(properties.at(1));
    mLife->setText(properties.at(2));
    mDecays->setText(properties.at(3));
    mProperties->setVisible(true);

    QStringList list = ParticlesDBManager::Instance().listDecays(name, 0.);
    if (list.size() == 0) {
        qWarning() << Q_FUNC_INFO << QString("Something went wrong with the DB query or paricle %1 not found").arg(mFindText->text());
    } else {
        ParticlesDBManager::Instance().setCurrentParticle(name);
        if (mLd)
            mLd->close();
        mLd = new ListDialog(list);
        mLd->exec();
    }
}

