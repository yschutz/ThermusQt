#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QObject>

class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class ListDialog;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget* parent = 0);

private slots:
    int exec();

private:

    void          append(const QStringList& list);

    QLabel*       mFindLabel; // a label
    QLineEdit*    mFindText;  // the text to find a particle by name or pdg code
    ListDialog*   mLd;        // dialog box to list the decays
    QPushButton*  mDone;      // the Done button
    QPushButton*  mGo;        // the Go button
};

#endif // FINDDIALOG_H
