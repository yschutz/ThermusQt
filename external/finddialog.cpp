#include <QDebug>
#include <QGridLayout>
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
    mFindText  = new QLineEdit(this);
    mFindLabel->setBuddy(mFindText);

    QHBoxLayout* searchLayout = new QHBoxLayout;
    searchLayout->addWidget(mFindLabel);
    searchLayout->addWidget(mFindText);

    mGo = new QPushButton("Find");
    connect(mGo, SIGNAL(pressed()), this, SLOT(go()));
    mDone = new QPushButton("Done");
    connect(mDone, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout* goDoneLayout = new QHBoxLayout;
    goDoneLayout->addWidget(mGo);
    goDoneLayout->addWidget(mDone);

    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    mainLayout->addLayout(searchLayout, 0, 0);
    mainLayout->addLayout(goDoneLayout, 1, 0, 1, 2, Qt::AlignHCenter);

    setLayout(mainLayout);

    setWindowTitle(QString(tr("Particle search (%1)")).arg(what));
}

//__________________________________________________________________________
void FindDialog::go()
{
    QStringList list;
//    if (mPDGRadio->isChecked()) {
//        int pdg = mFindText->text().toInt();
//        list = ParticlesDBManager::Instance().listDecays(pdg, 0.);
//    } else if (mNameRadio->isChecked()) {
//        QString name = mFindText->text();
//        list = ParticlesDBManager::Instance().listDecays(name, 0.);
//    }
    QString name = mFindText->text();
    list = ParticlesDBManager::Instance().listDecays(name, 0.);
    if (list.size() == 0) {
        qWarning() << Q_FUNC_INFO << QString("Something went wrong with the DB query or paricle %1 not found").arg(mFindText->text());
    } else {
        if (mLd)
            mLd->close();
        mLd = new ListDialog(list);
        mLd->exec();
    }
}

