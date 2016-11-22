#ifndef SUMMARY_H
#define SUMMARY_H

#include <QGridLayout>
#include <QWizardPage>

class Summary : public QWizardPage
{
    Q_OBJECT

public:
    Summary(QWidget *parent = 0);

    void updateFileName(QString name);

private:
    QGridLayout*  mMainLayout;  // the main layout
};

#endif // SUMMARY_H
