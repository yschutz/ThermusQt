#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include <QObject>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class ListDialog : public QDialog
{   
  Q_OBJECT

public:
    ListDialog(const QStringList& decays, QWidget* parent = 0);

private slots:
    void add();
    void ckecked(int);
    void modify();
    void remove();

private:    
    QList<QCheckBox*> mChecks;   // checks decay to be modified;
    QPushButton*      mDone;     // done button
    QList<QLineEdit*> mbrs;      // Edit box to modify br
    QList<QLabel*>    mbrns;     // Labels box for brn
    QList<QLabel*>    mids;      // Labels box for DB ID
    QPushButton*      mMinus;    // button to remove a selected entry
    QPushButton*      mMod;      // button to modify a selected entry
    QString           mPartName; // name of the current particle
    QPushButton*      mPlus;     // button to add an entry
};

#endif // LISTDIALOG_H
