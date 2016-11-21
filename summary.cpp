#include <QGroupBox>
#include <QLabel>
#include <QGridLayout>

#include "filedialog.h"
#include "summary.h"
#include "thermuswiz.h"

//__________________________________________________________________________
Summary::Summary(QWidget *parent) : QWizardPage(parent)
{
    // Create the summary of inputs selected to run a macro
    setTitle("Summary of settings");

//    QGroupBox *gb = new QGroupBox("Particles", this);

//    FileDialog *fd = (FileDialog*)(((ThermusWiz*)parent)->page(((ThermusWiz*)parent)->getDialogId()));

//    qDebug() << Q_FUNC_INFO  << fd->objectName() <<  fd->getFileName();
//    // the particles list file name
//    QString tempo = QString("The selected particles lis file is: %1").arg(fd->getFileName());
//    QLabel *label = new QLabel(tempo, gb);

//    QGridLayout *ly = new QGridLayout(gb);
//    ly->addWidget(label, 0, 0, 1, 1);
//    gb->setLayout(ly);

//    // final layout
//    QGridLayout *mainly = new QGridLayout(this);
//    mainly->addWidget(gb);

//    setLayout(mainly);

}

//__________________________________________________________________________
void Summary::uppdate()
{
    QGroupBox *gb = new QGroupBox("Particles", this);

    FileDialog *fd = (FileDialog*)(((ThermusWiz*)parentWidget())->page(((ThermusWiz*)parentWidget())->getDialogId()));

    qDebug() << Q_FUNC_INFO  << fd->objectName() <<  fd->getFileName();
    // the particles list file name
    QString tempo = QString("The selected particles lis file is: %1").arg(fd->getFileName());
    QLabel *label = new QLabel(tempo, gb);

    QGridLayout *ly = new QGridLayout(gb);
    ly->addWidget(label, 0, 0, 1, 1);
    gb->setLayout(ly);

    // final layout
    QGridLayout *mainly = new QGridLayout(this);
    mainly->addWidget(gb);

    setLayout(mainly);

}

