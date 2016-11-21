#ifndef SUMMARY_H
#define SUMMARY_H

#include <QWizardPage>

class Summary : public QWizardPage
{
    Q_OBJECT

public:
    Summary(QWidget *parent = 0);

    void uppdate();
};

#endif // SUMMARY_H
