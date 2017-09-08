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
    mCurrentPart("")
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
QString ParticlesDBManager::makeQuery(ENTRY en, const QString& para, QString &squery) const

{
    // prepares a query

    QMetaEnum metaEnum = QMetaEnum::fromType<ENTRY>();
    QString svalue = para;
    QString column = metaEnum.valueToKey(en);
    column = column.remove(0,1).toLower();
    QString bind(QString("(:%1), ").arg(column));
    column = QString("%1, ").arg(column);
    if (en == kLAST - 1) {
     column = column.remove(',').trimmed();
     bind = bind.remove(',').trimmed();
    }
    squery.insert(squery.indexOf(')'), column);
    squery.insert(squery.lastIndexOf(')'), bind);
    return bind.remove(')').remove('(').remove(',').trimmed();
}

//__________________________________________________________________________
bool ParticlesDBManager::connect(const QString &path)
{
    // connect to data base with given name

    mDB.setDatabaseName(path);
    if (!mDB.open()) {
        QMessageBox msg(QMessageBox::Critical, "DB connection",
                        QString("Could not connect to particles DB: %1").arg(path));
        msg.exec();
        return false;
    } else {
        return true;
    }
}

//__________________________________________________________________________
void ParticlesDBManager::deleteDecays(int motherid) const
{
    // delete all decays of given mother
    // get all the decays id
    QString squery("SELECT id FROM decay WHERE mother_id =(:mother)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":mother", motherid);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    while(query.next()) {
        int decayid = query.record().value(0).toInt();
        squery = "DELETE FROM daughter WHERE decay_id = (:decay)";
        query.prepare(squery);
        query.bindValue(":decay", decayid);
        if (!query.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return;
        }
        squery = "DELETE FROM decay WHERE id = (:decay)";
        query.prepare(squery);
        query.bindValue(":decay", decayid);
        if (!query.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return;
        }
    }
}

//__________________________________________________________________________
void ParticlesDBManager::deleteDecay(int id) const
{
    // delete a decay from the DB
    QString squery("SELECT mother_id FROM decay WHERE id = (:id)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":id", id);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    query.next();
    int mother_id = query.value(0).toInt();

    squery = "SELECT ndecay FROM particle WHERE id = (:motherid)";
    query.prepare(squery);
    query.bindValue(":motherid", mother_id);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    query.next();
    int ndecay = query.value(0).toInt();

    squery = "UPDATE particle SET ndecay = (:val1) WHERE id = (:motherid)";
    query.prepare(squery);
    query.bindValue(":val1", ndecay - 1);
    query.bindValue(":motherid", mother_id);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }

    squery = "DELETE FROM decay WHERE id = (:val)";
    query.prepare(squery);
    query.bindValue(":val", id);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    // now the daughters
    squery = "DELETE FROM daughter WHERE decay_id = (:val)";
    query.prepare(squery);
    query.bindValue(":val", id);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    normalizeBR();
}

//__________________________________________________________________________
QString ParticlesDBManager::getPartParameter(int pdg, ENTRY what, const QString &where) const
{
    // retrieve given parameter for given particle
    QMetaEnum metaEnum = QMetaEnum::fromType<ParticlesDBManager::ENTRY>();
    QString sEntry    = metaEnum.valueToKey(what);
    sEntry.remove(0,1).toLower();
    QString rv("");
    QString squery("SELECT * FROM particle WHERE pdg = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", pdg);
    QString save = ParticlesDBManager::Instance().dbName();
    if (where.contains(mkPDGName)) {
        QString newName(save);
        newName.replace(mkThermusName, mkPDGName);
        bool connected = ParticlesDBManager::Instance().connect(newName);
        if (!connected) {
            return rv;
        }
    }
    query.prepare(squery);
    query.bindValue(":val", pdg);
    if (query.exec()) {
        query.next();
        rv = query.record().value(sEntry).toString();
    }
    if (where.contains(mkPDGName))
        ParticlesDBManager::Instance().connect(save);
    return rv;
}

//__________________________________________________________________________
int ParticlesDBManager::id(QString name) const
{
    // retrieve the particle id in the DB of the given name
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
QString ParticlesDBManager::name(int pdg, const QString& where) const
{
    // retrieve the particle name of the given pdg

    QString name("");
    QString squery("SELECT * FROM particle WHERE pdg = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", pdg);

    if (where.contains("all") || where.contains(mkThermusName)) {
        if (!query.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return name;
        }
        query.next();
        name = query.record().value("name").toString();
    }

    if (name.isEmpty() && (where.contains("all") || where.contains(mkPDGName))) { // try to find it in PDG DB
        QString save = ParticlesDBManager::Instance().dbName();
        QString newName(save);
        newName.replace(mkThermusName, mkPDGName);
        if (!ParticlesDBManager::Instance().connect(newName)) {
            return "";
        }  else {
            query.clear();
            query.prepare(squery);
            query.bindValue(":val", pdg);
            if (!query.exec()) {
                error(Q_FUNC_INFO, query.lastError().text());
                return "";
            }
            query.next();
            name = query.record().value("name").toString();
        }
        ParticlesDBManager::Instance().connect(save);
    }

    return name;
}

//__________________________________________________________________________
int ParticlesDBManager::getPDG(QString name) const
{
    // retrieve the particle pdg of the given name
    QString squery("SELECT pdg FROM particle WHERE name = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", name);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return -1;
    }
    query.next();
    int pdg = query.record().value(0).toInt();
    if (pdg == 0) { // try to find it in PDG DB
        QString save = ParticlesDBManager::Instance().dbName();
        QString newName(save);
        newName.replace(mkThermusName, mkPDGName);
//        bool connected = ParticlesDBManager::Instance().connect(newName);
        if (ParticlesDBManager::Instance().connect(newName)) {
            query.clear();
            query.prepare(squery);
            query.bindValue(":val", name);
            if (!query.exec()) {
                error(Q_FUNC_INFO, query.lastError().text());
                return -1;
            }
            query.next();
            pdg = query.record().value(0).toInt();
        }
        ParticlesDBManager::Instance().connect(save);
    }
    return pdg;
}

//__________________________________________________________________________
int ParticlesDBManager::getPDG(int id) const
{
    // retrieve the particle pdg of the given DB id
    QString squery("SELECT pdg FROM particle WHERE id = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", id);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return -1;
    }
    query.next();
    return query.record().value("pdg").toInt();
}

//__________________________________________________________________________
ParticlesDBManager &ParticlesDBManager::Instance()
{
    // return the unique instance

    return mPDBM;
}

//__________________________________________________________________________
void ParticlesDBManager::insertDecay(int mother, int dtype, double br, QStringList& daughters) const
{       
    // insert a decay together with the daughters list     and recalculate brn

    QString squery("SELECT ndecay FROM particle WHERE id = (:motherid)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":motherid", mother);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    query.next();
    int ndecay = query.value(0).toInt();

    squery = "UPDATE particle SET ndecay = (:val1) WHERE id = (:motherid)";
    query.prepare(squery);
    query.bindValue(":val1", ndecay + 1);
    query.bindValue(":motherid", mother);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    double sum = normalizeBR(br);
    squery = "INSERT INTO decay (mother_id, dtype, br, brn, ndaughters) VALUES ((:v1), (:v2), (:v3), (:v4), (:v5))";
    query.prepare(squery);
    query.bindValue(":v1", mother);
    query.bindValue(":v2", dtype);
    query.bindValue(":v3", br);
    query.bindValue(":v4", br * 100 / sum);
    query.bindValue(":v5", daughters.size());
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }

    // insert the decays

    int decayid = query.lastInsertId().toInt();
    squery = ("INSERT INTO daughter (decay_id, pdg) VALUES ((:v1), (:v2))");
    for (QString spdg : daughters) {
        query.clear();
        query.prepare(squery);
        query.bindValue(":v1", decayid);
        int pdg = spdg.toInt();
        query.bindValue(":v2", pdg);
        if (!query.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return;
        }
    }
}

//__________________________________________________________________________
void ParticlesDBManager::insertParticle(const QList<QString> &parameters)
{
       // insert a particle and its decays in the Thermus DB

    QList<QString> binds;

    QString squery("INSERT INTO particle () VALUES ()");
    QString bind = makeQuery(kNAME, parameters.at(kNAME), squery);
    binds.insert(kNAME, bind);

    bind = makeQuery(kPDG, parameters.at(kPDG), squery);
    binds.insert(kPDG, bind);

    bind = makeQuery(kSTATISTIC, parameters.at(kSTATISTIC), squery);
    binds.insert(kSTATISTIC, bind);

    bind = makeQuery(kSPIN, parameters.at(kSPIN), squery);
    binds.insert(kSPIN, bind);

    bind = makeQuery(kMASS, parameters.at(kMASS), squery);
    binds.insert(kMASS, bind);

    bind = makeQuery(kS, parameters.at(kS), squery);
    binds.insert(kS, bind);

    bind = makeQuery(kBARYON, parameters.at(kBARYON), squery);
    binds.insert(kBARYON, bind);

    bind = makeQuery(kCHARGE, parameters.at(kCHARGE), squery);
    binds.insert(kCHARGE, bind);

    bind = makeQuery(kC, parameters.at(kC), squery);
    binds.insert(kC, bind);

    bind = makeQuery(kB, parameters.at(kB), squery);
    binds.insert(kB, bind);

    bind = makeQuery(kT, parameters.at(kT), squery);
    binds.insert(kT, bind);

    bind = makeQuery(kSC, parameters.at(kSC), squery);
    binds.insert(kSC, bind);

    bind = makeQuery(kCC, parameters.at(kCC), squery);
    binds.insert(kCC, bind);

    bind = makeQuery(kBC, parameters.at(kBC), squery);
    binds.insert(kBC, bind);

    bind = makeQuery(kTC, parameters.at(kTC), squery);
    binds.insert(kTC, bind);

    bind = makeQuery(kWIDTH, parameters.at(kWIDTH), squery);
    binds.insert(kWIDTH, bind);

    bind = makeQuery(kLIFETIME, parameters.at(kLIFETIME), squery);
    binds.insert(kLIFETIME, bind);

    bind = makeQuery(kTHRESHOLD, parameters.at(kTHRESHOLD), squery);
    binds.insert(kTHRESHOLD, bind);

    bind = makeQuery(kRADIUS, parameters.at(kRADIUS), squery);
    binds.insert(kRADIUS, bind);

    bind = makeQuery(kNDECAY, parameters.at(kNDECAY), squery);
    binds.insert(kNDECAY, bind);

    QSqlQuery query;
    query.prepare(squery);
    bool ok;
    for (int index = kPDG; index != kLAST; index++) {
        QString svalue = parameters.at(index);
        int ivalue = svalue.toInt(&ok);
        if (ok) {
            query.bindValue(binds.at(index), ivalue);
        }
        else {
            double dvalue= svalue.toDouble(&ok);
            if (ok) {
                query.bindValue(binds.at(index), dvalue);
            } else {
                query.bindValue(binds.at(index), svalue);
            }

        }
    }
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    QMessageBox msg(QMessageBox::Information, "Insert", QString("%1 added").arg(parameters.at(kNAME)));
    msg.exec();
    // and now the decays
//    int partid = query.lastInsertId().toInt();

    // do it by hand starting from finddialog

//    QMessageBox msg(QMessageBox::Information, "Insert", QString("%1 inserted with ID = %2").arg(parameters.at(kNAME)).arg(partid));
//    msg.exec();
//    QString save = dbName();
//    QString newName(save);
//    newName.replace(mkThermusName, mkPDGName);
//    QStringList tempo;
//    if (connect(newName)) {
//        squery = "SELECT id FROM particle WHERE name = (:name)";
//        query.prepare(squery);
//        query.bindValue(":name", parameters.at(kNAME));
//        if (!query.exec()) {
//            error(Q_FUNC_INFO, query.lastError().text());
//            return;
//        }
//        query.next();
//        int motherid = query.value(0).toInt();
//        squery = "SELECT * FROM decay WHERE mother_id = (:motherid)";
//        query.prepare(squery);
//        query.bindValue(":motherid", motherid);
//        if (!query.exec()) {
//            error(Q_FUNC_INFO, query.lastError().text());
//            return;
//        }
//        double brt = 0.0;
//        QStringList tempo;
//        while (query.next()) {
//            int did    = query.record().value("id").toInt();
//            double br  = query.record().value("br").toDouble();
//            double brn = query.record().value("brn").toDouble();
//            brt += br;
//            QString soutput(QString("%1 %2[%3] --> ").arg(did).arg(br).arg(brn));
//            QSqlQuery query2;
//            query2.clear();
//            squery = "SELECT * FROM daughter WHERE decay_id = (:val)";
//            query2.prepare(squery);
//            query2.bindValue(":val", did);
//            if (query2.exec()) {
//                while (query2.next()) {
//                    int dpdg = query2.record().value("pdg").toInt();
//                    soutput.append(QString("%1, ").arg(dpdg));
//                }
//                soutput.replace(soutput.lastIndexOf(", "), 2, "");
//                tempo.append(soutput);
//            }
//        }
//        QStringList rv;
//        for (QString decay : tempo) {
//            QString output = decay.split('>').first().append("> ");
//            QStringList pdgs = decay.split('>').last().split(',');
//            for (QString sdpdg : pdgs) {
//                int dpdg = sdpdg.trimmed().toInt();
//                QString name = ParticlesDBManager::Instance().name(dpdg);
//                output.append(QString("%1, ").arg(name));
//            }
//            output.remove(output.lastIndexOf(','), 2);
//            rv.append(output);
//        }
//        qDebug() << Q_FUNC_INFO << rv;
//    }
//    connect(save);

}

//__________________________________________________________________________
bool ParticlesDBManager::isStable(int pdg) const
{
    // check if a particle is stable
    bool rv = false;
    QString squery("SELECT ndecay FROM particle WHERE pdg = (:val)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":val", pdg);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
    } else {
        query.next();
        if (query.record().value(0).toInt() == 0)
            rv = true;
    }
    return rv;
}

//__________________________________________________________________________
double ParticlesDBManager::lifetime(int pdg, const QString &where)
{
    // gets the lifetime of the given particle
    double rv = 0.0;
    rv = getPartParameter(pdg, kLIFETIME, where).toDouble();
    return rv;
}

//__________________________________________________________________________
void ParticlesDBManager::listParticles(const ParticlesDBManager::ListOption opt) const
{
    // list all particles of class opt (Quark, Meson, Lepton, ....)

    QSqlQueryModel  *model = new QSqlQueryModel;

//    QMetaEnum metaEnum = QMetaEnum::fromType<ListOption>();
//    QString sopt = metaEnum.valueToKey(opt);
//    sopt.remove(0,1);

    QSqlQuery query;
    QString squery("SELECT * FROM particle"); // WHERE pclass = (:val)";
    query.prepare(squery);
//    query.bindValue(":val", sopt);
    if (query.exec()) {
        model->setQuery(query);
    } else
        error(Q_FUNC_INFO, query.lastError().text());

    QSortFilterProxyModel* proxymodel=new QSortFilterProxyModel();
    proxymodel->setDynamicSortFilter(true);
    proxymodel->setSourceModel(model);

    QTableView *view = new QTableView;
    view->setModel(proxymodel);
    view->setSortingEnabled(true);

    view->resize(2100, 200);
    view->show();
}

//__________________________________________________________________________
QStringList ParticlesDBManager::listDecays(const QString &partPDG, qreal thr) const
{
    // lists all decays of given particle above given threshold

    QStringList rv;
    QStringList tempo;
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
                                soutput.append(QString("%1, ").arg(dpdg));
                            }
                            soutput.replace(soutput.lastIndexOf(", "), 2, "");
                            tempo.append(soutput);
                        }
                    }
                }
            }
            for (QString decay : tempo) {
                QString output = decay.split('>').first().append("> ");
                QStringList pdgs = decay.split('>').last().split(',');
                for (QString sdpdg : pdgs) {
                    int dpdg = sdpdg.trimmed().toInt();
                    QString name = ParticlesDBManager::Instance().name(dpdg);
                    output.append(QString("%1, ").arg(name));
                }
                output.remove(output.lastIndexOf(','), 2);
                rv.append(output);
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
    bool ok;
    int ipartPDG = partPDG.toInt(&ok);
    QString squery;
    if (ok)
        squery = "SELECT * FROM particle WHERE pdg = (:val)";
    else
        squery = "SELECT * FROM particle WHERE name = (:val)";
    QSqlQuery query;
    query.prepare(squery);
    if (ok)
        query.bindValue(":val", ipartPDG);
    else
        query.bindValue(":val", partPDG);
    if (query.exec()) {
        query.next(); // one unique answer
        int pdg       = query.record().value("pdg").toInt();
        QString name  = query.record().value("name").toString();
        if (pdg == 0 && name.isEmpty()) {
            QMessageBox msg(QMessageBox::Critical, "Find", QString("%1 not found").arg(partPDG));
            msg.exec();
            return rv;
        }

        if (!ok)
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
double ParticlesDBManager::mass(int pdg, const QString &where)
{
    // gets the mass of the given particle
    double rv = 0.0;
    rv = getPartParameter(pdg, kMASS, where).toDouble();
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
    normalizeBR();
}

//__________________________________________________________________________
double ParticlesDBManager::normalizeBR(double sum) const
{
    // normalize the BRs to 100
    // find all the decays of the mother particle
    QString name = currentPart();
    int mother = id(name);
    double rv = sum;
    QString squery(" SELECT * FROM decay WHERE mother_id = (:mother)");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":mother", mother);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return 0.0;
    }
    while (query.next())
        rv += query.record().value("br").toDouble();
    query.seek(-1);
    squery = "UPDATE decay SET brn = (:val1) WHERE id = (:val2)";
    QSqlQuery nquery;
    while (query.next()) {
        double nbr = query.record().value("br").toDouble();
        double nbrn = nbr * 100 / rv;
        int nid = query.record().value("id").toInt();
        nquery.clear();
        nquery.prepare(squery);
        nquery.bindValue(":val1", nbrn);
        nquery.bindValue(":val2", nid);
        if (!nquery.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return 0.0;
        }
    }
    return rv;
}

//__________________________________________________________________________
void ParticlesDBManager::removeParticle(const QString& part) const
{
    // remove a particle and its decay dfrom the data base

    QString squery;
    QSqlQuery query;
    bool ok;
    int pdg = part.toInt(&ok);
    if (ok) {
        squery = "SELECT id, name FROM particle WHERE pdg = (:pdg)";
        query.prepare(squery);
        query.bindValue(":pdg", pdg);
     } else {
        squery = "SELECT id, name FROM particle WHERE name = (:name)";
        query.prepare(squery);
        query.bindValue(":name", part);
    }

    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    query.next();
    int motherid = query.record().value(0).toInt();
    QString name = query.record().value(1).toString();


    deleteDecays(motherid);

    squery = "DELETE FROM particle WHERE id = (:id)";
    query.prepare(squery);
    query.bindValue(":id", motherid);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }

    QMessageBox msg(QMessageBox::Information, "Remove", QString("%1 has been removed").arg(name));
    msg.exec();
}

//__________________________________________________________________________
void ParticlesDBManager::setCurrentParticle(const QString &part)
{
    // sets the current particle name

    bool ok;
    int pdg = part.toInt(&ok);
    if (ok) {
        QString squery("SELECT name FROM particle WHERE pdg = (:pdg)");
        QSqlQuery query;
        query.prepare(squery);
        query.bindValue(":pdg", pdg);
        if (!query.exec()) {
            error(Q_FUNC_INFO, query.lastError().text());
            return;
        }
        query.next();
        mCurrentPart = query.value(0).toString();
    } else
        mCurrentPart = part;
}

//__________________________________________________________________________
int ParticlesDBManager::size() const
{
    // returns the size of the Thermus DB (number of particles)

    QString squery("SELECT * FROM particle");
    QSqlQuery query;
    query.prepare(squery);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return 0;
    }
    query.last();
    return query.value(0).toInt();
}

//__________________________________________________________________________
double ParticlesDBManager::width(int pdg, const QString &where)
{
    // gets the width of the given particle
    double rv = 0.0;
    rv = getPartParameter(pdg, kWIDTH, where).toDouble();
    return rv;
}

//__________________________________________________________________________
void ParticlesDBManager::allDecays(int pdg, QHash<int, double> &br) const
{
    // retrieves all mother and br of daughter given by pdg
    QString squery("SELECT mother_id, brn FROM decay WHERE id IN (SELECT d.decay_id FROM daughter d where d.pdg = (:pdg))");
    QSqlQuery query;
    query.prepare(squery);
    query.bindValue(":pdg", pdg);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    while(query.next()) {
        int motherid = query.record().value(0).toInt();
        int mother = getPDG(motherid);
        double brn = query.record().value(1).toDouble();
        br[mother] =  brn + br[mother];
    }
}

//__________________________________________________________________________
void ParticlesDBManager::allParticles(QList<int>& list, ListOption opt) const
{
    // retrieves all particles in the Thermus DB
    QString squery;
    switch (opt) {
    case kALL:
        squery = "SELECT pdg FROM particle";
        break;
    case kSTABLE:
        squery = "SELECT pdg FROM particle WHERE ndecay = 0";
        break;
    case kUNSTABLE:
        squery = "SELECT pdg FROM particle WHERE ndecay != 0";
        break;
    default:
        break;
    }

    QSqlQuery query(squery);
    if (!query.exec()) {
        error(Q_FUNC_INFO, query.lastError().text());
        return;
    }
    while(query.next())

        list.append(query.record().value(0).toInt());
}

//__________________________________________________________________________
double ParticlesDBManager::br(int decayindex) const
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

//__________________________________________________________________________
double ParticlesDBManager::charge(int pdg, const QString &where)
{
    // gets the charge of the given particle
    double rv = 0.0;
    rv = getPartParameter(pdg, kCHARGE, where).toDouble();
    return rv;

}
