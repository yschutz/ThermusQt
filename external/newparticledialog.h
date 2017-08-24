#ifndef NEWPARTICLEDIALOG_H
#define NEWPARTICLEDIALOG_H

#include <QDialog>
#include <QObject>

#include "particlesdbmanager.h"

class QLineEdit;
class QVBoxLayout;

class NewParticleDialog : public QDialog
{
    Q_OBJECT
public:

    NewParticleDialog(QWidget * parent = 0);

public slots:
    void insert();
    void reset();
    void searchName();

private:
    void newEntry(ParticlesDBManager::ENTRY en, bool readonly = true);

    QList<QLineEdit*> mEntries;       // list of entries for one particle
    QVBoxLayout*      mEntriesLayout; // the lyout for the particles parameters
};

#endif // NEWPARTICLEDIALOG_H
