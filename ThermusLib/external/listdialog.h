#ifndef LISTDIALOG_H
#define LISTDIALOG_H

#include <QDialog>
#include <QObject>

class QCheckBox;
class QGridLayout;
class QLabel;
class QLineEdit;
class QPushButton;
class QScrollArea;
class QWidget;

class ListDialog : public QDialog
{   
  Q_OBJECT

public:
    ListDialog(QWidget* parent = 0);

private slots:
    void add();
    void ckecked();
    void modify();
    void refresh();
    void remove();

private:    
    QList<QCheckBox*> mChecks;      // checks decay to be modified;
    QList<QLineEdit*> mbrs;         // Edit box to modify br
    QList<QLabel*>    mbrns;        // Labels box for brn
};

#endif // LISTDIALOG_H
