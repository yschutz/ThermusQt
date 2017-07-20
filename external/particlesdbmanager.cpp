#include <QTableView>
#include "particlesdbmanager.h"

//__________________________________________________________________________
ParticlesDBManager::ParticlesDBManager(const QString& path) : QObject(nullptr)
{
    //ctor
    mDB = QSqlDatabase::addDatabase("QSQLITE");
    mDB.setDatabaseName(path);
    if (!mDB.open())
        qDebug() << "Cannot open connection to " << path;
    else
        qDebug() << "connected to DB " << path;
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
void ParticlesDBManager::listDecays(int partPDG, qreal thr)
{
    // list decays of particle part for br above thr

    //    QSqlQueryModel  *model = new QSqlQueryModel;
    //    QTableView *view = new QTableView;
    //    view->setModel(model);

    QSqlQuery query;
    QString squery = "SELECT * FROM particle WHERE pdg = (:val)";
    query.prepare(squery);
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
                    int did   = query.record().value("id").toInt();
                    double br = query.record().value("br").toDouble();
                    int ndau  = query.record().value("ndaughters").toInt();
                    if (br > thr) {
                        brt += br;
                        soutput.append(QString("%1 --> ").arg(br));
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
        qDebug() << soutput;
    } else
        qWarning() << "Query error";

    //    while (q.next()) {
    //        QString name = q.record().value("name").toString();

}
