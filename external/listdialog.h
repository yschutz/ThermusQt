#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include <QObject>

class QCheckBox;
class QPushButton;

class ListDialog : public QDialog
{   
  Q_OBJECT

public:
    ListDialog(const QStringList& decays, QWidget* parent = 0);

private slots:
    void add();
    void modify();
    void remove();

private:
    QList<QCheckBox*> mChecks; // checks decay to be modified;
    QPushButton* mDone;        // done button
    QList<int> mIndexList;     // list of indexes for the current decays
    QPushButton* mMinus;       // button to remove a selected entry
    QPushButton* mMod;         // button to modify a selected entry
    QPushButton* mPlus;        // button to add an entry
};

#endif // LISTDIALOG_H
