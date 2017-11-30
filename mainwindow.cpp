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

#include "macros/fitmacro.h"
#include "macros/predictionmacro.h"
#include "logger.h"
#include "thermuswiz.h"

#include "external/finddialog.h"
#include "external/newparticledialog.h"
#include "external/particlesdbmanager.h"
#include "external/QMinuit.h"
#include "external/selectdialog.h"

#include "main/TTMThermalFitBSQ.h"

bool MainWindow::mDebug = false;
//__________________________________________________________________________
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mFd(nullptr), mNPD(nullptr)
{
    // ctor

    mThermusDir.setPath(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "/ThermusQt");
    if (!mThermusDir.exists()) {
        QMessageBox* msg = new QMessageBox(QMessageBox::Critical,"Wrong Installation", QString("ThermusQt installation is expected at %1").arg(mThermusDir.path()));
        if(msg->exec() == QMessageBox::Ok)
            exit(1);
    }

    QString partDir = mThermusDir.path() + "/particles/";
    QString thermusDBName(QString(ParticlesDBManager::instance().getThermusDBName()).append(".db"));
    QString pdgDBName(QString(ParticlesDBManager::instance().getPDGDBName()).append(".db"));

    mThermusDBPath = thermusDBName.prepend(partDir);
    mPdgDBPath     = pdgDBName.prepend(partDir);

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

//    createConsol();
    // direct the log info to the console
    qInstallMessageHandler(customMessageHandler);
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

    static bool firstCall = true;
    if (!firstCall)
        return;
    firstCall = false;
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
//    logger.setTextEdit(mLogConsol);

    // Write the message to log file
    logger.writeMessage(txt);
}

//__________________________________________________________________________
void MainWindow::setDebugMode(bool val)
{
    // set debug mode on/off
    mDebug = val;
    if (mDebug) {
        mDebugOnAction->setText("✓ On");
        mDebugOffAction->setText("Off");
        Logger::instance().setVerbosity(true);
    }
    else {
        mDebugOnAction->setText("On");
        mDebugOffAction->setText("✓ Off");
        Logger::instance().setVerbosity(false);
    }
}

//__________________________________________________________________________
QString MainWindow::getDBPath(const QString &opt)
{
    // return the full path to DB
    if (opt.contains("Thermus"))
        return mThermusDBPath;
    else if (opt.contains("PDG"))
        return mPdgDBPath;
    else
        return "";
}

//__________________________________________________________________________
void MainWindow::listParameters()
{
    if (mWhat == "Prediction")
        PredictionMacro::instance().listParameters();
    else if (mWhat == "Fit")
        FitMacro::instance().listParameters();
}

//__________________________________________________________________________
void MainWindow::run(const QString &what)
{
    Macro* myMacro = nullptr;
    QString message;
    mWhat = what;
    if (what == "Prediction") {
        myMacro = &PredictionMacro::instance();
        message = mPredictionAction->statusTip();
    }
    else if (what == "Fit") {
        myMacro = &FitMacro::instance();
        message = mFitAction->statusTip();
    }

    message += ": macro " + myMacro->objectName();
    statusBar()->showMessage(message);

    myMacro->start(mDebug);

    new ThermusWiz(what, this);

    // **************************************************
    // connect to the Thermus DB
    particlesDBManagement(kConnectT);

    // Choice of starting parameters, select parameters to fit or fix, add constraint
    myMacro->setParameters();

    mRunMenu->insertAction(mQuitAction, mParametersList);

    // Create the fit model
    myMacro->setFit();

    // Run the stuff
    myMacro->run();
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

    mDebugOnAction = new QAction(QIcon(":/verboseicone.png"), "", this);
    mDebugOffAction = new QAction(QIcon(":/noverboseicone.png"), "", this);

    if (mDebug) {
        mDebugOnAction->setText("✓ On");
        mDebugOffAction->setText("Off");
    }
    else {
        mDebugOnAction->setText("On");
        mDebugOffAction->setText("✓ Off");
    }
    mDebugOnAction->setStatusTip(tr("Set verbose mode on"));
    mDebugOffAction->setStatusTip(tr("Set verbose mode off"));
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

    mSearchPDG   = new QAction(QIcon(":/pdglogo.jpg"), tr("&PDG"), this);
    connect(mSearchPDG, &QAction::triggered, this, [this]{ particlesDBManagement(kSearchP); });
    mSearchThermus   = new QAction(QIcon(":/thermusicon.png"), tr("&Thermus"), this);
    connect(mSearchThermus, &QAction::triggered, this, [this]{ particlesDBManagement(kSearchT); });

    mListPDG   = new QAction(QIcon(":/pdglogo.jpg"), tr("&PDG"), this);
    connect(mListPDG, &QAction::triggered, this, [this]{ particlesDBManagement(kListP); });
    mListThermus   = new QAction(QIcon(":/thermusicon.png"), tr("&Thermus"), this);
    connect(mListThermus, &QAction::triggered, this, [this]{ particlesDBManagement(kListT); });

    mPdgSelectAction = new QAction(QIcon(":/selecticon.png"), tr("&Select"), this);
    connect(mPdgSelectAction, &QAction::triggered, this, [this]{ particlesDBManagement(kSelect); });

    // run  macro

    mPredictionAction = new QAction(QIcon(":/predictionicon.png"), tr("&Prediction"), this);
    mPredictionAction->setStatusTip(tr("Makes a Thermus prediction"));
    connect(mPredictionAction, &QAction::triggered, this, [this] { run(mPredictionAction->text().remove(0,1)); });

    mFitAction = new QAction(QIcon(":/fiticon.png"), tr("&Fit"), this);
    mFitAction->setStatusTip(tr("Makes a Thermus Fit"));
    connect(mFitAction, &QAction::triggered, this, [this] { run(mFitAction->text().remove(0,1)); });

    // Parameters list action

    mParametersList = new QAction(QIcon(":/listicon.png"), tr("Parameters list"), this);
    mParametersList->setStatusTip("Makes a list of parameters with properties");
    connect(mParametersList, &QAction::triggered, this, [this]{ listParameters(); });

    // quit

    mQuitAction = new QAction(QIcon(":/endicon.png"), tr("&End"), this);
    mQuitAction->setShortcuts(QKeySequence::Quit);
    mQuitAction->setStatusTip(tr("Makes a Thermus prediction"));
    connect(mQuitAction, &QAction::triggered, this, &MainWindow::quit);
}

//__________________________________________________________________________
void MainWindow::createMenus()
{
    // creates the menus

    // Debug
    mDebugMenu = menuBar()->addMenu(QIcon(":/verboseicon.png"), tr("&Verbosity"));
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

    mSearchMenu = mParticlesMenu->addMenu(QIcon(":/searchicon.png"), tr("&Search/Update"));
    mSearchMenu->addAction(mSearchPDG);
    mSearchMenu->addAction(mSearchThermus);

    mListMenu = mParticlesMenu->addMenu(QIcon(":/listicon.png"), tr("&List"));
    mListMenu->addAction(mListPDG);
    mListMenu->addAction(mListThermus);

    mParticlesMenu->addAction(mNewParticleAction);

    // select the macro to run or quit
    mRunMenu = menuBar()->addMenu(tr("&Run"));
    mRunMenu->addAction(mPredictionAction);
    mRunMenu->addAction(mFitAction);
    mRunMenu->addAction(mQuitAction);

}

//__________________________________________________________________________
void MainWindow::particlesDBManagement(DBOPS option)
{
    // Manages the particles DB

    const QString kPartDir = mThermusDir.path() + "/particles/";

    bool pdg     = false;
    bool thermus = false;
    QString sourceName("");
    QString fullDBName;
    if (option < kLastP) {
        pdg        = true;
        sourceName = "PDG";
        fullDBName = mPdgDBPath;
    } else {
        thermus    = true;
        sourceName = "Thermus";
        fullDBName     = mThermusDBPath;
    }

    QString dbName = fullDBName.replace(kPartDir, "");
    QMetaEnum metaEnum = QMetaEnum::fromType<DBOPS>();
    QString soption    = metaEnum.valueToKey(option);
    soption.remove(0,1);
    soption.remove(soption.length() - 1 ,1);

    QMessageBox msg;

    // ======================
    // connect
    if (soption.contains("Connect")) {
        if (thermus)
            ParticlesDBManager::instance().connect(mThermusDBPath);
        else if (pdg)
            ParticlesDBManager::instance().connect(mPdgDBPath);
        return;
    }
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
//                QString pdgName(dbName);
//                pdgName.replace("Thermus", "PDG");
//                pdgName.prepend(kPartDir);
                QFileInfo check(mPdgDBPath);
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
                progress.setMaximum(400);
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
    // search a particle with its decay channels
    if (soption.contains("Search")) {
        if (ParticlesDBManager::instance().connect(kPartDir + dbName)) {
            mFd = new FindDialog(sourceName, this);
            mFd->exec();
        }
        return;
    }

    // ======================
    // List all particles
    if (soption.contains("List")) {
        if (ParticlesDBManager::instance().connect(kPartDir + dbName)) {
            ParticlesDBManager::instance().listParticles(ParticlesDBManager::kALL);
        }
        return;
    }

    // ======================
    // Insert a new particle into Thermus DB
    if (option == kInsert) {
        bool connected = ParticlesDBManager::instance().connect(kPartDir + dbName);
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
