#ifndef PARTICLESDBMANAGER_H
#define PARTICLESDBMANAGER_H

#include <QObject>
#include <QtSql>

class ParticlesDBManager : public QObject
{
    Q_OBJECT

public:
    enum ListOption {kAll, kQuark, kGaugeBoson, kLepton, kMeson, kBaryon};
    Q_ENUM (ListOption)

    explicit ParticlesDBManager(const QString& path);
    void listParticles(const ListOption opt = kAll);
    void listDecays(int partPDG, qreal thr = 0.0);


private:
    QSqlDatabase mDB; // the data base
};

#endif // PARTICLESDBMANAGER_H
