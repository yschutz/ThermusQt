#ifndef PARTICLESDBMANAGER_H
#define PARTICLESDBMANAGER_H

#include <QHash>
#include <QObject>
#include <QtSql>

class ParticlesDBManager : public QObject
{
    Q_OBJECT

public:
    enum ListOption {kALL, kSTABLE, kUNSTABLE};
    Q_ENUM (ListOption)
    enum ENTRY {kPDG, kNAME, kSPIN, kSTATISTIC, kMASS, kS, kBARYON, kCHARGE, kC, kB, kT,
                kSC, kCC, kBC, kTC, kWIDTH, kLIFETIME, kTHRESHOLD, kRADIUS, kNDECAY, kLAST};
    Q_ENUM(ENTRY)

    void                       allDecays(int pdg, QHash<int, double>& br) const ;
    void                       allParticles(QList<int>& list, ListOption opt = kALL) const;
    double                     br(int decayindex) const;
    double                     charge(int pdg, const QString& where = "Thermus");
    bool                       connect(const QString& path);
    QString                    currentPart() const { return mCurrentPart; }
    QString                    dbName() const { return mDB.databaseName(); } //mDBName; }
    void                       deleteDecays(int motherid) const;
    void                       deleteDecay(int id) const;
    QString                    getPartParameter(int pdg, ParticlesDBManager::ENTRY what, const QString& where = "Thermus") const;
    int                        getPDG(QString name) const;
    int                        getPDG(int id) const;
    QString                    getPDGDBName() const { return mkPDGName; }
    QString                    getThermusDBName() const { return mkThermusName; }
    int                        id(QString name) const;
    static ParticlesDBManager& Instance();
    void                       insertDecay(int mother, int dtype, double br, QStringList &daughters) const;
    void                       insertParticle(const QList<QString>& parameters);
    bool                       isStable(int pdg) const;
    double                     lifetime(int pdg, const QString& where = "Thermus");
    void                       listParticles(const ListOption opt = kALL) const ;
    QStringList                listDecays(const QString& partPDG, qreal thr = 0.0) const;
    QStringList                listProperties(const QString& partPDG) const;
    QString                    name(int pdg, const QString& where = "all") const;
    double                     mass(int pdg, const QString& where = "Thermus");
    void                       modifyBR(int decayid, double val) const;
    double                     normalizeBR(double sum = 0.0) const;
    void                       removeParticle(const QString &part) const;
    void                       setCurrentParticle(const QString& part);
    int                        size() const;
    double                     width(int pdg, const QString& where = "Thermus");

private:
    ParticlesDBManager();
    ~ParticlesDBManager();

    ParticlesDBManager(const ParticlesDBManager&) {}
    ParticlesDBManager& operator = (const ParticlesDBManager&) {return *this;}

    void     error(QString text, QString info) const;
    QString  makeQuery(ParticlesDBManager::ENTRY en, const QString &para, QString& squery) const;

private:
    QString                   mCurrentPart; // the current particle
    QSqlDatabase              mDB;          // the data base
    static ParticlesDBManager mPDBM;        // the unique instance
    const QString             mkThermusName = "ThermusParticles";
    const QString             mkPDGName     = "PDGParticles";

};

#endif // PARTICLESDBMANAGER_H
