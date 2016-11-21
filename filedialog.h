#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QEventLoop>
#include <QList>
#include <QObject>
#include <QPushButton>
#include <QRadioButton>
#include <QWizardPage>

class FileDialog : public QWizardPage
{
    Q_OBJECT
public:
    explicit FileDialog(QWidget *parent = 0);

    void                 setFileName(QString val) { mFileName = val;}
    QString              getFileName() const { return mFileName; }
    QList<QRadioButton*> getRadioButtons() const  { return mRadioButtons; }

public slots:
    void                 selectFile();


private:
    QEventLoop            mLoop;           // loo for waiting the validate button to be pressed
    QList<QRadioButton*>  mRadioButtons;   // one radio button per file choice
    QString               mFileName;       // the selected filename
};

#endif // FILEDIALOG_H
