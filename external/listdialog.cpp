#include <QCheckBox>
#include <QDebug>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

#include"listdialog.h"

//__________________________________________________________________________
ListDialog::ListDialog(const QStringList &decays, QWidget *parent) : QDialog(parent)
{
    QString tempo = decays.at(0);
    QStringList ltempo = tempo.split(':');
    QString particleName = ltempo[0];
    QStringList particleDecays;
    particleDecays.append(ltempo[1]);
    for (int index = 1; index < decays.size(); index++)
        particleDecays.append(decays.at(index));

    QGridLayout* decayCheckLayout = new QGridLayout;
    for (int index = 0; index < particleDecays.size(); index++) {
        QLineEdit* line = new QLineEdit;
        line->setReadOnly(true);
        QString text = ((QString)particleDecays.at(index)).prepend(QString("%1: ").arg(index, 3));
        line->setText(text);
        decayCheckLayout->addWidget(line, index, 0);
        QCheckBox* check = new QCheckBox;
        check->setChecked(false);
        decayCheckLayout->addWidget(check, index, 1);
    }

    mModify = new QPushButton("Modify");
    connect(mModify, SIGNAL(clicked(bool)), this, SLOT(exec()));
    mDone   = new QPushButton("Done");
    connect(mDone, SIGNAL(clicked(bool)), this, SLOT(close()));

    QHBoxLayout* modDoneLayout = new QHBoxLayout;
    modDoneLayout->addWidget(mModify);
    modDoneLayout->addWidget(mDone);

    QWidget* viewport = new QWidget;
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    scroll->setWidget(viewport);
    scroll->setWidgetResizable(true);

    viewport->setLayout(decayCheckLayout);

    QVBoxLayout *dialog_layout = new QVBoxLayout(this);
    dialog_layout->addWidget(scroll); // add scroll to the QDialog's layout
    dialog_layout->addLayout(modDoneLayout);

    setLayout(dialog_layout);

    setWindowTitle(QString(tr("%1 decays")).arg(particleName));
    resize(500, 100);
}

//__________________________________________________________________________
int ListDialog::exec()
{
    QMessageBox* msg = new QMessageBox(QMessageBox::Information, "Modify decay", "not yet implemented");
    msg->show();
    return 1;
}
