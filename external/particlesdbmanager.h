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

    bool                       connect(const QString& path);
    static ParticlesDBManager& Instance();
    void                       listParticles(const ListOption opt = kAll);
    QStringList                listDecays(const QString& partPDG, qreal thr = 0.0);

private:
    ParticlesDBManager();
    ~ParticlesDBManager();

    ParticlesDBManager(const ParticlesDBManager&) {}
    ParticlesDBManager& operator = (const ParticlesDBManager&) {return *this;}


private:
    static ParticlesDBManager mPDBM; // the unique instance
    QSqlDatabase              mDB;   // the data base
};

#endif // PARTICLESDBMANAGER_H
