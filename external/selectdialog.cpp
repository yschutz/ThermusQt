#include <QDir>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>
#include "selectdialog.h"

//__________________________________________________________________________
SelectDialog::SelectDialog(const QString &dirname, QWidget *parent) : QDialog(parent),
    mFileName("")
{
    QDir dir(dirname);
    dir.setFilter(QDir::Files);
    QStringList filterName("PartList_*");
    QStringList files = dir.entryList(filterName);

    QVBoxLayout* radioLayout = new QVBoxLayout;
    for (int index = 0; index < files.size(); index++) {
        QRadioButton* rad = new QRadioButton(files.at(index));
        if (index == 0)
            rad->setChecked(true);
        radioLayout->addWidget(rad);
        mFiles.append(rad);
    }
    mSelect = new QPushButton("Select");
    connect(mSelect, SIGNAL(clicked(bool)), this, SLOT(select()));

    QVBoxLayout* hlayout = new QVBoxLayout;
    hlayout->addLayout(radioLayout);
    hlayout->addWidget(mSelect);

    setLayout(hlayout);

    setWindowTitle(tr("Select a particle list"));
    show();
}

//__________________________________________________________________________
void SelectDialog::select()
{
    for(int index = 0; index < mFiles.size(); index++) {
        QRadioButton* rad = mFiles.at(index);
        if (rad->isChecked()) {
            mFileName = rad->text();
            break;
        }
    }
    accept();
}

