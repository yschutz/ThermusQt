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
    int exec();

private:
    QList<QCheckBox*> mChecks; // checks decay to be modified;
    QPushButton* mDone;        // done button
    QPushButton* mModify;      // modify button
};

#endif // LISTDIALOG_H
