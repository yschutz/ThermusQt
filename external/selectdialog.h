#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDebug>
#include <QDialog>
#include <QList>
#include <QObject>

class QPushButton;
class QRadioButton;

class SelectDialog : public QDialog
{
    Q_OBJECT

public:
    SelectDialog(const QString& dirname, QWidget* parent = 0);

    const QString fileName() { return mFileName; }

private slots:
    void select();

private:
    QString              mFileName; // name of the selected file
    QPushButton*         mSelect;   // the Select button
    QList<QRadioButton*> mFiles;    // list of files
};

#endif // SELECTDIALOG_H
