#include <QTableView>
#include "particlesdbmanager.h"


ParticlesDBManager ParticlesDBManager::mPDBM = ParticlesDBManager();

//__________________________________________________________________________
ParticlesDBManager::ParticlesDBManager() : QObject(nullptr)
{
    //ctor
    mDB = QSqlDatabase::addDatabase("QSQLITE");
}

//__________________________________________________________________________
ParticlesDBManager::~ParticlesDBManager()
{
    mDB.close();
}

//__________________________________________________________________________
bool ParticlesDBManager::connect(const QString &path)
{
    mDB.setDatabaseName(path);
    if (!mDB.open()) {
        qWarning() << "Cannot open connection to " << path;
        return false;
    }
    else {
        qInfo() << "connected to DB " << path;
        return true;
    }
}

//__________________________________________________________________________
ParticlesDBManager &ParticlesDBManager::Instance()
{
    // return the unique instance

    return mPDBM;
}

//__________________________________________________________________________
void ParticlesDBManager::listParticles(const ParticlesDBManager::ListOption opt)
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
        qWarning() << "Query error";
}

//__________________________________________________________________________
QStringList ParticlesDBManager::listDecays(const QString &partPDG, qreal thr)
{
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
        QString soutput = QString("%1: ").arg(name);
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
                        soutput.append(QString("%1[%2] --> ").arg(br).arg(brn));
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
                            soutput.replace(soutput.lastIndexOf(","), 1, ";");
                        }
                    }
                }
            }
        } else {
            soutput.append("nodecay defined");
        }
        soutput.replace(soutput.lastIndexOf(";"), 1, "");
        return soutput.split(';');
    } else {
        qWarning() << "Query error";
        return QStringList();
    }
}
