// Author: Yves Schutz octobre 2016
//
// The main window steering the show

#include "mainwindow.h"

#include <QAbstractButton>
#include <QDateTime>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFileInfo>
#include <QMdiArea>
#include <QMessageBox>
#include <QtWidgets>
#include <QVBoxLayout>

#include "macros/runmacro.h"
#include "logger.h"
#include "thermuswiz.h"

#include "external/finddialog.h"
#include "external/newparticledialog.h"
#include "external/particlesdbmanager.h"
#include "external/selectdialog.h"

bool MainWindow::mDebug = false;
//__________________________________________________________________________
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mFd(nullptr), mNPD(nullptr)
{
    // ctor

    mThermusDir.setPath(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "/ThermusQt");
    if (!mThermusDir.exists()) {
        QMessageBox* msg = new QMessageBox(QMessageBox::Critical,"Wrong Installation", QString("ThermusQt installation is expected at %1").arg(mThermusDir.path()));
        int ret = msg->exec();
        if(ret ==QMessageBox::Ok)
            exit(1);
    }

    setGeometry(0, 0, 50, 25);

    setWindowTitle("THERMUS");
    //    QPixmap bkgnd(":/2015-Jul-03-Fit_PbPb0010_Reference_final_SQM.png");
    //    bkgnd = bkgnd.scaled(QSize(550, 500));
    //    QPalette palette;
    //    palette.setBrush(QPalette::Background, bkgnd);
    //    this->setPalette(palette);

    // central widget

    mCentralwidget = new QWidget;

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QLabel *thermus  = new QLabel("THERMUS");
    thermus->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    thermus->setStyleSheet("QLabel { background-color : grey; color : darkblue; }");
    thermus->setFont(QFont("OldEnglish", 80, QFont::Bold));

    QLabel *infoLabel = new QLabel(tr("<i>Choose a menu option</i>"));
    infoLabel->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("QLabel { background-color : lightgrey; color : black; }");

    QWidget *intermediateFiller = new QWidget(this);
    intermediateFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(topFiller);
    layout->addWidget(thermus);
    layout->addWidget(intermediateFiller);
    layout->addWidget(infoLabel);
    layout->addWidget(bottomFiller);

    mCentralwidget->setLayout(layout);

    createActions();
    createMenus();
    QString message = tr("Welcome to Thermus");
    statusBar()->showMessage(message);

    setMinimumSize(160, 160);
    resize(480, 320);


}

//__________________________________________________________________________
MainWindow::~MainWindow()
{
    // dtor

}

//__________________________________________________________________________
void MainWindow::createConsol()
{
    // creates an area where to display subwindows for log infos, plots, etcc

    QMainWindow *outputAreaWindow = new QMainWindow(this);
    QMdiArea *mdiArea = new QMdiArea(outputAreaWindow);

    // the console for log output from qDebug, qInfo, qWarning
    mLogConsol = new QPlainTextEdit("log Consol", mdiArea);
    mLogConsol->setReadOnly(true);
    mdiArea->addSubWindow(mLogConsol);

    outputAreaWindow->setCentralWidget(mdiArea);

    outputAreaWindow->show();


    // direct the log info to the console
    qInstallMessageHandler(customMessageHandler);
}

//__________________________________________________________________________
void MainWindow::customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // redifinition on how to handle log messages

    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug<%1-%2>: %3").arg(context.function).arg(context.line).arg(msg);
        break;
    case QtInfoMsg:
        txt = QString("Info<%1>: %2").arg(context.function).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("Warning<%1-%2>: %3").arg(context.function).arg(context.line).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("Critical:<%1-%2>: %3").arg(context.function).arg(context.line).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("Fatal:<%1-%2>: %3").arg(context.function).arg(context.line).arg(msg);
        abort();
    }

    Logger& logger = Logger::instance();

    // Set my QTextEdit
    logger.setTextEdit(mLogConsol);

    // Write the message to QTextEdit
    if (mDebug)
        logger.writeMessage(txt, true);
    else
        logger.writeMessage(txt, false);
}

//__________________________________________________________________________
void MainWindow::setDebugMode(bool val)
{
    // set debug mode on/off
    mDebug = val;
    if (mDebug) {
        mDebugOnAction->setText("✓ On");
        mDebugOffAction->setText("Off");
    }
    else {
        mDebugOnAction->setText("On");
        mDebugOffAction->setText("✓ Off");
    }
}

//__________________________________________________________________________
void MainWindow::listParameters()
{
    RunMacro::instance().listParameters();
}

//__________________________________________________________________________
void MainWindow::listParticles(bool full)
{
    RunMacro::instance().listParticles(full);
}

//__________________________________________________________________________
void MainWindow::runPrediction()
{
    // runs the prediction macro
    QDateTime start = QDateTime::currentDateTime();
    QDate date = start.date();
    QTime time = start.time();

    QString info = QString(" *** Start at Date : %1 Time : %2").arg(date.toString("dd MMMM yyyy")).arg(time.toString());
    if (mDebug)
        qDebug() << info;

    RunMacro& myMacro = RunMacro::instance();
    myMacro.setDebug(mDebug);
    myMacro.objectName() + ": test";

    QString message = mPredictionAction->statusTip() + ": macro " + myMacro.objectName();
    statusBar()->showMessage(message);

    new ThermusWiz(info, this);

    // **************************************************
    // Definition the particle list
    // and their properties (and decays):
    // open the particles list file

    myMacro.setParticlesListFile();

    // adds the Particles list to the menu

    mRunMenu->insertAction(mQuitAction, mParticlesListLong);
    mRunMenu->insertAction(mQuitAction, mParticlesListShort);

    // Choice of starting parameters

    myMacro.setParameters();

    // adds the Particles list to the menu

    // choice of parameter to fit or to fix
    // -> Default is fixed at zero or at unity so important to check !!

    myMacro.setFitFix();

    // option of adding some constraint

    myMacro.setConstrain();

    mRunMenu->insertAction(mQuitAction, mParametersList);

    // list parameters settings
    //    myMacro.listParameters();
}

//__________________________________________________________________________
void MainWindow::quit()
{
    // end gracefully
    statusBar()->showMessage(mQuitAction->statusTip());
    close();
    exit(0);
}
//__________________________________________________________________________
void MainWindow::paintEvent(QPaintEvent *event)
{
    // decorate the central widget

    QPainter painter(this);

    // the background image

    QPixmap bkgnd(":/2015-Jul-03-Fit_PbPb0010_Reference_final_SQM.png");
    painter.drawPixmap(0,0,width(), height(),bkgnd);

    // the Thermus banner, first the background
    qint32 bheight = height() / 3 ;
    qint32 bwidth  = width() * 0.7 ;
    qint32 bx = (width() - bwidth) / 2;
    qint32 by = (height() - bheight) / 2;
    QRect bg = QRect(bx, by, bwidth, bheight);

    QColor blue = Qt::blue;
    blue.setAlphaF(0.5);

    painter.setBrush(QBrush(blue));
    painter.drawRect(bg);

    // the Thermus banner, then the text

    painter.setPen(QPen(Qt::red));
    painter.setFont(QFont("Helvetica [Cronyx]", QFont::ExtraBold, 10, true));
    painter.drawText(rect(), Qt::AlignHCenter|Qt::AlignVCenter,"Thermus");

    QMainWindow::paintEvent(event);
}

//__________________________________________________________________________
void MainWindow::createActions()
{
    // created the actions associated to the menu items

    // Debug mode on/off

    mDebugOnAction = new QAction(this);
    mDebugOffAction = new QAction(this);

    if (mDebug) {
        mDebugOnAction->setText("✓ On");
        mDebugOffAction->setText("Off");
    }
    else {
        mDebugOnAction->setText("On");
        mDebugOffAction->setText("✓ Off");
    }
    mDebugOnAction->setStatusTip(tr("Set debug mode on"));
    mDebugOffAction->setStatusTip(tr("Set debug mode off"));
    connect(mDebugOnAction, &QAction::triggered, this, [this]{ setDebugMode(true); });
    connect(mDebugOffAction, &QAction::triggered, this, [this]{ setDebugMode(false); });

    // particles database related actions

    mCreatePDG        = new QAction(QIcon(":/pdglogo.jpg"), "&PDG", this);
    connect(mCreatePDG, &QAction::triggered, this, [this]{ particlesDBManagement(kCreateP); });
    mCreateThermus    = new QAction(QIcon(":/thermusicon.png"), "&Thermus", this);
    connect(mCreateThermus, &QAction::triggered, this, [this]{ particlesDBManagement(kCreateT); });

    mUpdatePDG   = new QAction(QIcon(":/pdglogo.jpg"), tr("&PDG"), this);
    connect(mUpdatePDG, &QAction::triggered, this, [this]{ particlesDBManagement(kUpdateP); });
    mUpdateThermus   = new QAction(QIcon(":/thermusicon.png"), tr("&Thermus"), this);
    connect(mUpdateThermus, &QAction::triggered, this, [this]{ particlesDBManagement(kUpdateT); });

    mNewParticleAction = new QAction(QIcon(":/inserticon.png"), tr("&New"), this);
    connect(mNewParticleAction, &QAction::triggered, this, [this]{ particlesDBManagement(kInsert); });

    mListPDG   = new QAction(QIcon(":/pdglogo.jpg"), tr("&PDG"), this);
    connect(mListPDG, &QAction::triggered, this, [this]{ particlesDBManagement(kListP); });
    mListThermus   = new QAction(QIcon(":/thermusicon.png"), tr("&Thermus"), this);
    connect(mListThermus, &QAction::triggered, this, [this]{ particlesDBManagement(kListT); });

    mPdgSelectAction = new QAction(QIcon(":/selecticon.png"), tr("&Select"), this);
    connect(mPdgSelectAction, &QAction::triggered, this, [this]{ particlesDBManagement(kSelect); });

    // run prediction macro

    mPredictionAction = new QAction(tr("&Prediction"), this);
    mPredictionAction->setShortcuts(QKeySequence::New);
    mPredictionAction->setStatusTip(tr("Makes a Thermus prediction"));
    connect(mPredictionAction, &QAction::triggered, this, &MainWindow::runPrediction);

    // Particles list action

    mParticlesListLong = new QAction(tr("Long particles list"), this);
    mParticlesListLong->setStatusTip("Makes a long list of all particles with full properties");
    connect(mParticlesListLong, &QAction::triggered, this, [this]{ listParticles(true); });

    mParticlesListShort = new QAction(tr("Short particles list"), this);
    mParticlesListShort->setStatusTip("Makes a short list of all particles with full properties");
    connect(mParticlesListShort, &QAction::triggered, this, [this]{ listParticles(false); });

    // Parameters list action

    mParametersList = new QAction(tr("Parameters list"), this);
    mParametersList->setStatusTip("Makes a list of parameters with properties");
    connect(mParametersList, &QAction::triggered, this, [this]{ listParameters(); });

    // quit

    mQuitAction = new QAction(tr("&End"), this);
    mQuitAction->setShortcuts(QKeySequence::Quit);
    mQuitAction->setStatusTip(tr("Makes a Thermus prediction"));
    connect(mQuitAction, &QAction::triggered, this, &MainWindow::quit);
}

//__________________________________________________________________________
void MainWindow::createMenus()
{
    // creates the menus

    // Debug
    mDebugMenu = menuBar()->addMenu(tr("&Debug"));
    mDebugMenu->addAction(mDebugOnAction);
    mDebugMenu->addAction(mDebugOffAction);

    // Menu related to the particles database
    mParticlesMenu = menuBar()->addMenu(tr("&Particles"));

    mCreateMenu = mParticlesMenu->addMenu(QIcon(":/createicon.png"), tr("&Create"));
    mCreateMenu->addAction(mCreatePDG);
    mCreateMenu->addAction(mCreateThermus);

    mUpdateMenu = mParticlesMenu->addMenu(QIcon(":/updateicon.png"), tr("&Update"));
    mUpdateMenu->addAction(mUpdatePDG);
    mUpdateMenu->addAction(mUpdateThermus);

    mListMenu = mParticlesMenu->addMenu(QIcon(":/listicon.png"), tr("&List"));
    mListMenu->addAction(mListPDG);
    mListMenu->addAction(mListThermus);

    mParticlesMenu->addAction(mNewParticleAction);

    // select the macro to run or quit
    mRunMenu = menuBar()->addMenu(tr("&Run"));
    mRunMenu->addAction(mPredictionAction);
    mRunMenu->addAction(mQuitAction);

}

//__________________________________________________________________________
void MainWindow::particlesDBManagement(DBOPS option)
{
    // Manages the particles DB

    const QString kPartDir = mThermusDir.path() + "/particles/";
    const QString kThermusDBName("ThermusParticles.db");
    const QString kPDGDBName("PDGParticles.db");
    bool pdg     = false;
    bool thermus = false;
    QString dbName;
    QString sourceName("");
    if (option < kLastP) {
        pdg        = true;
        sourceName = "PDG";
        dbName     = kPDGDBName;
    }
    else {
        thermus    = true;
        sourceName = "Thermus";
        dbName     = kThermusDBName;
    }
    QString fullDBName(dbName);
    fullDBName.prepend(kPartDir);

    QMetaEnum metaEnum = QMetaEnum::fromType<DBOPS>();
    QString soption    = metaEnum.valueToKey(option);
    soption.remove(0,1);
    soption.remove(soption.length() - 1 ,1);

    QMessageBox msg;
    // ======================
    // create or update
    if (soption.contains("Create") || soption.contains("Update")) {

        QTimer t;
        QEventLoop loop;
        connect(&t, SIGNAL(timeout()), &loop, SLOT(quit()));

        const QString kPythonPath("/usr/local/bin/python3");
        QFileInfo check(kPythonPath);
        if (!check.exists() || !check.isExecutable()) {
            msg.setText(QString("%1: %2 is not found or not executable!").arg(Q_FUNC_INFO, kPythonPath));
            msg.exec();
            return;
        }
        const QString kExtDir = mThermusDir.path() + "/external/";
        QString pythonScript;
        QString input("");
        if (pdg) {
            pythonScript = "PDGParticles.py";
        } else if (thermus) {
            pythonScript = "ThermusParticles.py";
            if (soption.contains("Create")) {
                // check if PDG particles db exists
                QString pdgName(dbName);
                pdgName.replace("Thermus", "PDG");
                pdgName.prepend(kPartDir);
                QFileInfo check(pdgName);
                if (! check.exists()) {
                    QMessageBox msg(QMessageBox::Critical, "Particles DB creation", "You must first create the PDG particles list");
                    msg.exec();
                    return;
                }
                SelectDialog sdia(kPartDir, this);
                sdia.setModal(true);
                if (sdia.exec() == QDialog::Accepted)
                    input = sdia.fileName();
            }
        }
        if (soption.contains(("Update"))) {
            // check if PDG particles db exists
            QFileInfo check(fullDBName);
            if (! check.exists()) {
                QMessageBox msg(QMessageBox::Critical, "Particles DB creation", "You must first create the particles DB");
                msg.exec();
                return;
            }
        }
        QProcess p;
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("EXTDIR", kExtDir); // Add an environment variable
        env.insert("PARTDIR", kPartDir);
        env.insert("DBNAME", dbName);
        p.setProcessEnvironment(env);

        p.setProcessChannelMode(QProcess::SeparateChannels);
        QStringList params;
        params << kExtDir + pythonScript << soption << input;
        QProgressDialog progress;
        p.start(kPythonPath, params);
        int iprogress = 0;
        while (p.state() == QProcess::Running || p.state() == QProcess::Starting) {
            if (iprogress == 0 && p.state() == QProcess::Running) {
                progress.setLabelText(QString("%1 particles DB (%2)").arg(soption, sourceName));
                progress.setCancelButtonText("Cancel");
                progress.setMinimum(0);
                progress.setMaximum(200);
                progress.setWindowModality(Qt::WindowModal);
                progress.resize(progress.width() * 1.5, progress.height());
                progress.show();
            }
            if (p.state() == QProcess::Running) {
                progress.setValue(iprogress++);
                progress.show();
            }
            t.start(600);
            loop.exec();
            if (progress.wasCanceled()) {
                p.kill();
                QMessageBox msg;
                msg.setText(QString("The particles DB was not %1d").arg(soption));
                msg.exec();
                return;
            }
        }
        progress.close();
        if (p.error() == QProcess::FailedToStart) {
            QMessageBox msg;
            msg.setText(QString("%1 or %2 not found").arg(kPythonPath, pythonScript));
            msg.exec();
            return;
        }
        QString p_stdout = p.readAllStandardOutput();
        QStringList listout = p_stdout.split('\n');
        QString p_stderr = p.readAllStandardError();

        if (p.exitStatus() == QProcess::CrashExit)
            msg.setText("Python process ended abnormally");
        if (p_stderr != "")
            msg.setText(msg.text() + " Error encountered in Python macro: " + p_stderr);
        else
            msg.setText(QString("The particles DB has been %1d with %2 entries").arg(soption).arg(listout.size()));
        msg.exec();
        return;
    }

    // ======================
    // list a particle with its decay channels
    if (soption.contains("List")) {
        bool connected = ParticlesDBManager::Instance().connect(kPartDir + dbName);
        if (!connected) {
            QMessageBox msg(QMessageBox::Critical, "DB connection",
                            QString("Could not connect to particles DB: %1").arg(kPartDir + dbName));
            msg.exec();
        } else {
            if (mFd) {
                mFd->close();
            }
            mFd = new FindDialog(sourceName, this);
            mFd->exec();
        }
        return;
    }

    // ======================
    // Insert a new particle into Thermus DB
    if (option == kInsert) {
        bool connected = ParticlesDBManager::Instance().connect(kPartDir + dbName);
        if (!connected) {
            QMessageBox msg(QMessageBox::Critical, "DB connection",
                            QString("Could not connect to particles DB: %1").arg(kPartDir + dbName));
            msg.exec();
        } else {
            mNPD = new NewParticleDialog(this);
            mNPD->exec();
        }
        return;
    }
}
