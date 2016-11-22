#include <QDebug>
#include <QGroupBox>
#include <QLabel>
#include <QVariant>

#include "filedialog.h"
#include "summary.h"
#include "thermuswiz.h"

//__________________________________________________________________________
Summary::Summary(QWidget *parent) : QWizardPage(parent)
{
    // Create the summary of inputs selected to run a macro
    setTitle("Summary of settings");


//    // final layout
    mMainLayout = new QGridLayout(this);

    setLayout(mMainLayout);


}

//__________________________________________________________________________
void Summary::updateFileName(QString name)
{
    QGroupBox *gb = new QGroupBox("Particles", this);

    // the particles list file name
    QString tempo = QString("The selected particles list file is: %1").arg(name);
    QLabel *label = new QLabel(tempo, gb);

    QGridLayout *ly = new QGridLayout(gb);
    ly->addWidget(label, 0, 0, 1, 1);
    gb->setLayout(ly);

    // final layout
    mMainLayout->addWidget(gb);

    double t = field("temp").toDouble();

}

