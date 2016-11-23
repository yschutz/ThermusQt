#ifndef SUMMARY_H
#define SUMMARY_H

#include <QGridLayout>
#include <QGroupBox>
#include <QWizardPage>

class Summary : public QWizardPage
{
    Q_OBJECT

public:
    Summary(QWidget *parent = 0);

    void updateFileName(QString name);
    void updateParameters();

private:
    QGroupBox*    mConsGB;      // the constraints Group Box
    QGridLayout*  mConsGBlay;   // the constraints Group Box layout
    QGroupBox*    mFileGB;      // the file Group Box
    QGridLayout*  mFileGBlay;   // the file Group Box layout
    QGridLayout*  mMainLayout;  // the main layout
    QGroupBox*    mParaGB;      // the parameters Group Box
    QGridLayout*  mParaGBlay;   // the parameters Group Box layout
};

#endif // SUMMARY_H
