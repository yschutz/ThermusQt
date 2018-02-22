#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QObject>

class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QRadioButton;
class ListDialog;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(const QString& what, QWidget* parent = 0);

private slots:
    void remove();
    void go();
    void showDecays();

private:

    void          append(const QStringList& list);

    QLabel*       mDecays;    // number of decay of the particle
    QLabel*       mFindLabel; // a label
    QLineEdit*    mFindText;  // the text to find a particle by name or pdg code
    ListDialog*   mLd;        // dialog box to list the decays
    QPushButton*  mDelete;    // the Delete button
    QPushButton*  mDone;      // the Done button
    QPushButton*  mGo;        // the Go button
    QLabel*       mLife;      // lifetime of the particle
    QLabel*       mMass;      // mass of the particle
    QLabel*       mPDG;       // PDG ID of the particle
    QGroupBox*    mProperties;// displays the particle's properties
};

#endif // FINDDIALOG_H
