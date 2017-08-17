#ifndef NEWDECAYDIALOG_H
#define NEWDECAYDIALOG_H

#include <QDialog>
#include <QObject>

class QLineEdit;

class NewDecayDialog : public QDialog
{
    Q_OBJECT

public:
    NewDecayDialog(QString& partName, QWidget* parent = 0);

private slots:
    void go(QString partName);

private:
    QLineEdit* mbrEdit;    // edit box for the branching ratio
    QLineEdit* mdecayEdit; // edit box for the decay
};

#endif // NEWDECAYDIALOG_H
