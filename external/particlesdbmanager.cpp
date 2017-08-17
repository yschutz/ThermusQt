#include <QMessageBox>
#include <QTableView>
#include "particlesdbmanager.h"

// THERMUS Particles DB structure
// Tables (3):
//  1. particle: id, name, pdg, spin, statistic, mass, S, Baryon, Q, C, B, T, SC, CC, BC, TC, width, lifetime, threshold, radius, ndecay
//  2. decay:    id, mother_id, dtype, br, brn, ndaughters
//  3. daugther: id, decay_id, pdg

// PDG Particle db Structure
// Tables (3):
// 1. particle:  id, name, pdg, matter, pcode, pclass, charge, mass, width, lifetime, isospin, iso3, strange, flavor, track, ndecay
//  2. decay:    id, mother_id, dtype, br, brn, ndaughters
//  3. daugther: id, decay_id, pdg


ParticlesDBManager ParticlesDBManager::mPDBM = ParticlesDBManager();

//__________________________________________________________________________
ParticlesDBManager::ParticlesDBManager() : QObject(nullptr),
    mCurrentPart(""), mDBName("")
{
    //ctor

    mDB = QSqlDatabase::addDatabase("QSQLITE");
}

//__________________________________________________________________________
ParticlesDBManager::~ParticlesDBManager()
{
    // close the active data base

    mDB.close();
}

//__________________________________________________________________________
void ParticlesDBManager::error(QString text, QString info) const
{
    // displays an error message
    QMessageBox msg(QMessageBox::Critical, "Error", text);
    msg.setInformativeText(info);
    msg.exec();
}

//__________________________________________________________________________
bool ParticlesDBManager::connect(const QString &path)
{
    // connect to data base with given name

    mDB.setDatabaseName(path);
    if (!mDB.open()) {
        error(Q_FUNC_INFO, QString("cannot connect to DB %1").arg(path));
        return false;
    }
    else {
        mDBName = path;
        return true;
    }
}

//__________________________________________________________________________
int ParticlesDBManager::findPartID(QString name) const
{
    // retrieve the particle id on the DB of the given name
    QString squery("SELECT * FROM particle WHERE name = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", name);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return -1;
    }
    query.next();
    int id = query.record().value("id").toInt();
    return id;
}

//__________________________________________________________________________
ParticlesDBManager &ParticlesDBManager::Instance()
{
    // return the unique instance

    return mPDBM;
}

//__________________________________________________________________________
void ParticlesDBManager::insertDecay(int mother, int dtype, double br, double brn, QStringList& daughters) const
{
    // insert a decay together with the daughters list
    QString squery("INSERT INTO decay (mother_id, dtype, br, brn, ndaughters) VALUES ((:v1), (:v2), (:v3), (:v4), (:v5))");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":v1", mother);
    query.bindValue(":v2", dtype);
    query.bindValue(":v3", br);
    query.bindValue(":v4", brn);
    query.bindValue(":v5", daughters.size());
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    qDebug() << Q_FUNC_INFO << query.lastInsertId();

// need to recalculate brn
}

//__________________________________________________________________________
void ParticlesDBManager::listParticles(const ParticlesDBManager::ListOption opt) const
{
    // list all particles of class opt (Quark, Meson, Lepton, ....)

    QSqlQueryModel  *model = new QSqlQueryModel;
    QTableView *view = new QTableView;
    view->setModel(model);

    QMetaEnum metaEnum = QMetaEnum::fromType<ListOption>();
    QString sopt = metaEnum.valueToKey(opt);
    sopt.remove(0,1);

    QSqlQuery query;
    QString squery = "SELECT * FROM particle WHERE pclass = (:val)";
    query.prepare(squery);
    query.bindValue(":val", sopt);
    if (query.exec()) {
        model->setQuery(query);
        view->show();
    } else
        error(Q_FUNC_INFO, query.lastError().text());
}

//__________________________________________________________________________
QStringList ParticlesDBManager::listDecays(const QString &partPDG, qreal thr) const
{
    // lists all decays of given particle above given threshold

    QStringList rv;
    QSqlQueryModel  *model = new QSqlQueryModel;
    QTableView *view = new QTableView;
    view->setModel(model);
    bool* ok = new bool;
    int ipartPDG = partPDG.toInt(ok);
    QString squery;
    if (*ok)
        squery = "SELECT * FROM particle WHERE pdg = (:val)";
    else
        squery = "SELECT * FROM particle WHERE name = (:val)";
    QSqlQuery query;
    query.prepare(squery);
    if (*ok)
        query.bindValue(":val", ipartPDG);
    else
        query.bindValue(":val", partPDG);
    if (query.exec()) {
        query.next(); // one unique answer
        int id       = query.record().value("id").toInt();
        QString name = query.record().value("name").toString();
        rv.append(name);
        int ndecay   = query.record().value("ndecay").toInt();
        if (ndecay > 0) {
            query.clear();
            squery = "SELECT * FROM decay WHERE mother_id = (:val)";
            query.prepare(squery);
            query.bindValue(":val", id);
            double brt = 0.0;
            if (query.exec()) {
                while (query.next()) {
                    int did    = query.record().value("id").toInt();
                    double br  = query.record().value("br").toDouble();
                    double brn = query.record().value("brn").toDouble();
                    //                    int ndau   = query.record().value("ndaughters").toInt();
                    if (br > thr) {
                        brt += br;
                        QString soutput(QString("%1 %2[%3] --> ").arg(did).arg(br).arg(brn));
                        QSqlQuery query2;
                        query2.clear();
                        squery = "SELECT * FROM daughter WHERE decay_id = (:val)";
                        query2.prepare(squery);
                        query2.bindValue(":val", did);
                        if (query2.exec()) {
                            while (query2.next()) {
                                int dpdg = query2.record().value("pdg").toInt();
                                QSqlQuery query3;
                                query3.clear();
                                squery = "SELECT * FROM particle WHERE pdg = (:val)";
                                query3.prepare(squery);
                                query3.bindValue(":val", dpdg);
                                if (query3.exec()) {
                                    query3.next(); // one unique answer
                                    QString name       = query3.record().value("name").toString();
                                    soutput.append(QString("%1, ").arg(name));
                                }
                            }
                            soutput.replace(soutput.lastIndexOf(", "), 2, "");
                            rv.append(soutput);
                        }
                    }
                }
            }
        } else {
            rv.append("nodecay defined");
        }
        return rv;
    } else {
        error(Q_FUNC_INFO, query.lastError().text());
        return QStringList();
    }
}

//__________________________________________________________________________
QStringList ParticlesDBManager::listProperties(const QString &partPDG) const
{
    // list properties of given particle

    QStringList rv;
    bool* ok = new bool;
    int ipartPDG = partPDG.toInt(ok);
    QString squery;
    if (*ok)
        squery = "SELECT * FROM particle WHERE pdg = (:val)";
    else
        squery = "SELECT * FROM particle WHERE name = (:val)";
    QSqlQuery query;
    query.prepare(squery);
    if (*ok)
        query.bindValue(":val", ipartPDG);
    else
        query.bindValue(":val", partPDG);
    if (query.exec()) {
        query.next(); // one unique answer
        int pdg       = query.record().value("pdg").toInt();
        QString name  = query.record().value("name").toString();
        if (!*ok)
            rv.append(QString("%1").arg(pdg));
        else
            rv.append(name);
        double mass = query.record().value("mass").toDouble();
        rv.append(QString("%1 GeV").arg(mass, 9, 'f', 7));
        double lifetime = query.record().value("lifetime").toDouble();
        rv.append(QString("%1 s").arg(lifetime, 9, 'g', 6));
        double ndecays = query.record().value("ndecay").toInt();
        rv.append(QString("%1 decays").arg(ndecays));
    }
    return rv;
}

//__________________________________________________________________________
void ParticlesDBManager::modifyBR(int decayid, double val) const
{
    // modify the branching ratio and renormalize

    QString squery("UPDATE decay SET br = (:val1) WHERE id = (:val2)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val1", val);
    query.bindValue(":val2", decayid);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }

    squery = "SELECT * FROM decay WHERE id = (:val)";
    query.prepare(squery);
    query.bindValue(":val", decayid);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    query.next();
    int mother = query.record().value("mother_id").toInt();
    squery = "SELECT * FROM decay where mother_id = (:val)";
    query.prepare(squery);
    query.bindValue(":val", mother);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    double sum  = 0.0;

    QList<int> ids;
    QList<double> brs;
    while (query.next()) {
        sum += query.record().value("br").toDouble();
        int id = query.record().value("id").toInt();
        ids.append(id);
        double br = query.record().value("br").toDouble();
        brs.append(br);
    }
    squery = "UPDATE decay SET brn = (:val1) WHERE id = (:val2)";
    query.prepare(squery);
    for (int index = 0; index < ids.size(); index++) {
        query.bindValue(":val2", ids.at(index));
        query.bindValue(":val1", brs.at(index) * 100.0 / sum);
        if (!query.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return;
        }
    }
}
//__________________________________________________________________________
double ParticlesDBManager::getBR(int decayindex) const
{
    // retrieves the branching ratio of the given decay

    double rv = 0.0;
    QString squery("SELECT * FROM decay WHERE id = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", decayindex);
    if (query.exec()) {
        query.next();
        rv = query.record().value("br").toDouble();
    }
    return rv;
}
