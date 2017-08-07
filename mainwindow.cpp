// Author: Yves Schutz octobre 2016
//
// The main window steering the show

#include "mainwindow.h"

#include <QAbstractButton>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QMdiArea>
#include <QMessageBox>
#include <QtWidgets>
#include <QVBoxLayout>

#include "macros/runmacro.h"
#include "logger.h"
#include "thermuswiz.h"

#include "external/finddialog.h"
#include "external/particlesdbmanager.h"

bool MainWindow::mDebug = false;
//__________________________________________________________________________
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mFd(nullptr)
{
    // ctor
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

    mPdgCreatePDG        = new QAction(QIcon(":/pdglogo.jpg"), "&PDG", this);
    connect(mPdgCreatePDG, &QAction::triggered, this, [this]{ particlesDBManagement(kCreateP); });
    mPdgCreateThermus    = new QAction(QIcon(":/thermusicon.png"), "&Thermus", this);
    connect(mPdgCreateThermus, &QAction::triggered, this, [this]{ particlesDBManagement(kCreateT); });

    mPdgUpdateMassAction   = new QAction(QIcon(":/updateicon.png"), tr("&Update masses"), this);
    connect(mPdgUpdateMassAction, &QAction::triggered, this, [this]{ particlesDBManagement(kUpdate); });

//    mPdgInsertAction = new QAction(QIcon(":/inserticon.png"), tr("&Insert"), this);
//    connect(mPdgInsertAction, &QAction::triggered, this, [this]{ particlesDBManagement(kInsert); });

    mPdgListAction   = new QAction(QIcon(":/listicon.png"), tr("&List"), this);
    connect(mPdgListAction, &QAction::triggered, this, [this]{ particlesDBManagement(kList); });

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
    mPdgMenu = menuBar()->addMenu(tr("&Particles"));

    mPdgCreateMenu = mPdgMenu->addMenu(QIcon(":/createicon.png"), tr("&Create"));
    mPdgCreateMenu->addAction(mPdgCreatePDG);
    mPdgCreateMenu->addAction(mPdgCreateThermus);

    mPdgMenu->addAction(mPdgUpdateMassAction);
    mPdgMenu->addAction(mPdgListAction);
    mPdgMenu->addAction(mPdgSelectAction);
//    mPdgMenu->addAction(mPdgInsertAction);

    // select the macro to run or quit
    mRunMenu = menuBar()->addMenu(tr("&Run"));
    mRunMenu->addAction(mPredictionAction);
    mRunMenu->addAction(mQuitAction);

}

//__________________________________________________________________________
void MainWindow::particlesDBManagement(DBOPS option)
{
    // Manages the particles DB

    // !!!!! FIXME: do not hard code the path

    const QString kPythonPath("/usr/local/bin/python3");
    QMessageBox msg;
    QFileInfo check(kPythonPath);
    if (!check.exists() || !check.isExecutable()) {
        msg.setText(QString("%1: %2 is not found or not executable!").arg(Q_FUNC_INFO, kPythonPath));
        msg.exec();
        return;
    }

    qDebug() << Q_FUNC_INFO << QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);

    QString pythonScript;
    QString dbPath;
    QString input("");
        if (option == kCreateP) {
        pythonScript = "/Users/schutz/work/ThermusQt/external/PDGParticles.py";
        dbPath       = "/Users/schutz/work/ThermusQt/external/PDGParticles.db";
    // !!!!! EMXIF
    } else if (option == kCreateT) {
        pythonScript = "/Users/schutz/work/ThermusQt/external/ThermusParticles.py";
        dbPath       = "/Users/schutz/work/ThermusQt/external/ThermusParticles.db";
        input        = "/Users/schutz/work/ThermusQt/particles/PartList_PPB2014_CBHN_fixed_saveQM14.txt";
    // !!!!! EMXIF
    }
    QMetaEnum metaEnum = QMetaEnum::fromType<DBOPS>();
    QString soption    = metaEnum.valueToKey(option);
    soption.remove(0,1);
    soption.remove(soption.length() - 1 ,1);

    QTimer t;
    QEventLoop loop;
    connect(&t, SIGNAL(timeout()), &loop, SLOT(quit()));

    // create or update
    QProcess p;
    p.setProcessChannelMode(QProcess::SeparateChannels);
    QStringList params;
    params << pythonScript << soption << input;
    QProgressDialog progress;
    if (option == kCreateP || option == kCreateT || option == kUpdate) {
        p.start(kPythonPath, params);
        int iprogress = 0;
        while (p.state() == QProcess::Running || p.state() == QProcess::Starting) {
            if (iprogress == 0 && p.state() == QProcess::Running) {
                progress.setLabelText(QString("%1 particles DB").arg(soption));
                progress.setCancelButtonText("Cancel");
                progress.setMinimum(0);
                progress.setMaximum(200);
                progress.setWindowModality(Qt::WindowModal);
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

    // list a particle with its decay channels
    if (option == kList) {
        bool connected = ParticlesDBManager::Instance().connect(dbPath);
        if (!connected) {
            qWarning() << QString("Could not connect to particles DB: %1").arg(dbPath);
        } else {
            if (mFd)
                mFd->close();
            mFd = new FindDialog(this);
            mFd->show();
        }
        return;
        }

    // select the particles to be taken into account by Thermus
    if (option == kSelect) {
        qDebug() << Q_FUNC_INFO;
        return;
    }
}
