// Author: Yves Schutz 24 octobre 2016
//
// A window to select interactively a file among a list stored in partlist.qrc

#include <QBoxLayout>
#include <QDebug>
#include <QDir>
#include <QGroupBox>

#include "filedialog.h"
#include "macros/runmacro.h"

//__________________________________________________________________________
FileDialog::FileDialog(QWidget *parent) : QWizardPage(parent)
{
    // create an interactive widow to select a single file among a list

    setTitle("Select a particles list file");
    QGroupBox *groupBox = new QGroupBox(tr("Available files"));
    QDir dir(":/particles");
    QVBoxLayout *vbox = new QVBoxLayout;
    for (qint32 index = 0; index < dir.entryList().size(); index++) {
        if (dir.entryList().at(index).contains("PartList_")) {
            QRadioButton *button = new QRadioButton(dir.entryList().at(index), groupBox);
            mRadioButtons.append(button);
            vbox->addWidget(mRadioButtons.last());
        }
    }
    groupBox->setLayout(vbox);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(groupBox, Qt::AlignVCenter);

    setLayout(layout);
}
