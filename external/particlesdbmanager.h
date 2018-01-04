#ifndef PARTICLESDBMANAGER_H
#define PARTICLESDBMANAGER_H

#include <QHash>
#include <QObject>
#include <QtSql>

#include "main/TTMParticle.h"

class ParticlesDBManager : public QObject
{
    Q_OBJECT

public:
    enum ListOption {kALL, kSTABLE, kUNSTABLE};
    Q_ENUM (ListOption)
    enum ENTRY {kPDG, kNAME, kSPIN, kSTATISTIC, kMASS, kS, kBARYON, kCHARGE, kC, kB, kT,
                kSC, kCC, kBC, kTC, kWIDTH, kLIFETIME, kTHRESHOLD, kRADIUS, kNDECAY, kLAST};
    Q_ENUM(ENTRY)

    bool                       allDecays(int pdg, QHash<int, double>& br, bool normalize = false) const ;
    QHash<int, TTMParticle*>   allParticles();
    double                     br(int decayindex) const;
    bool                       connect(const QString& path);
    QString                    currentPart() const { return mCurrentPart; }
    QString                    dbName() const { return mDB.databaseName(); } //mDBName; }
    void                       deleteDecays(int motherid) const;
    void                       deleteDecay(int id) const;
    double                     getBaryon(int pdg);
    double                     getBContent(int pdg);
    int                        getBeauty(int pdg);
    double                     getCharge(int pdg, const QString& where = "Thermus");
    double                     getCContent(int pdg);
    double                     getLifetime(int pdg, const QString& where = "Thermus") const;
    double                     getCharm(int pdg);
    double                     getMass(int pdg, const QString& where = "Thermus");
    QString                    getName(int pdg, const QString& where = "all") const;
    QString                    getPartParameter(int pdg, ParticlesDBManager::ENTRY what, const QString& where = "Thermus") const;
    int                        getPDG(QString name) const;
    int                        getPDG(int id) const;
    QString                    getPDGDBName() const { return mkPDGName; }
    double                     getRadius(int pdg);
    double                     getS(int pdg);
    double                     getSContent(int pdg);
    double                     getSpin(int pdg);
    int                        getStat(int pdg);
    QString                    getThermusDBName() const { return mkThermusName; }
    double                     getThreshold(int pdg, const QString& where = "Thermus");
    double                     getWidth(int pdg, const QString& where = "Thermus");
    int                        id(QString name) const;
    static ParticlesDBManager& instance();
    void                       insertDecay(int mother, int dtype, double br, QStringList &daughters) const;
    void                       insertParticle(const QList<QString>& parameters);
    bool                       isStable(int pdg) const;
    void                       listParticles(const ListOption opt = kALL) const ;
    QStringList                listDecays(const QString& partPDG, qreal thr = 0.0) const;
    QStringList                listProperties(const QString& partPDG) const;
    void                       modifyBR(int decayid, double val) const;
    double                     normalizeBR(double sum = 0.0) const;
    void                       removeParticle(const QString &part) const;
    void                       setCurrentParticle(const QString& part);
    int                        size() const;

private:
    ParticlesDBManager();
    ~ParticlesDBManager();

    ParticlesDBManager(const ParticlesDBManager& ma);
    ParticlesDBManager& operator = (const ParticlesDBManager&) {return *this;}

    void     error(QString text, QString info) const;
    QString  makeQuery(ParticlesDBManager::ENTRY en, const QString &para, QString& squery) const;

private:
    QString                   mCurrentPart;  // the current particle
    QSqlDatabase              mDB;           // the data base
    const QString             mkThermusName = "ThermusParticles";
    const QString             mkPDGName     = "PDGParticles";
    static ParticlesDBManager mPDBM;         // the unique instance
    QHash<int, TTMParticle*>  mParticles;    // particles list by pdgs
};

#endif // PARTICLESDBMANAGER_H
