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
    QString                    currentPart() const { return mCurrentPart; }
    QString                    dbName() const { return mDBName; }
    int                        findPartID(QString name) const;
    double                     getBR(int decayindex) const;
    static ParticlesDBManager& Instance();
    void                       insertDecay(int mother, int dtype, double br, double brn, QStringList &daughters) const;
    void                       listParticles(const ListOption opt = kAll) const ;
    QStringList                listDecays(const QString& partPDG, qreal thr = 0.0) const;
    QStringList                listProperties(const QString& partPDG) const;
    void                       modifyBR(int decayid, double val) const;
    void                       setCurrentParticle(const QString& part) { mCurrentPart = part; }

private:
    ParticlesDBManager();
    ~ParticlesDBManager();

    ParticlesDBManager(const ParticlesDBManager&) {}
    ParticlesDBManager& operator = (const ParticlesDBManager&) {return *this;}

    void error(QString text, QString info) const;

private:
    QString                   mCurrentPart; // the current particle
    QSqlDatabase              mDB;          // the data base
    QString                   mDBName;      // DB name et path
    static ParticlesDBManager mPDBM;        // the unique instance

};

#endif // PARTICLESDBMANAGER_H
