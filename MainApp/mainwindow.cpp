// Author: Yves Schutz octobre 2016
//
// The main window steering the show

#include "mainwindow.h"

#include <QAbstractButton>
#include <QDateTime>
#include <QDebug>
#include <QDialogButtonBox>
#include <QDir>
#include <QFileInfo>
#include <QMdiArea>
#include <QMessageBox>
#include <QtWidgets>
#include <QVBoxLayout>

#include "qfonticon.h"

#include "fitmacro.h"
#include "predictionmacro.h"
#include "logger.h"
#include "thermuswiz.h"

#include "finddialog.h"
#include "macroeditor.h"
#include "newparticledialog.h"
#include "particlesdbmanager.h"
#include "plot.h"
#include "QMinuit.h"

#include "TTMThermalFitBSQ.h"

#include "plot.h"


bool MainWindow::mDebug = false;
//__________________________________________________________________________
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), mFd(nullptr), mMacroEditor(nullptr), mNewDB(false), mNPD(nullptr), mThermus("Thermus")
{



    setGeometry(0, 0, 50, 25);

    // ctor
    mThermusDir.setPath(qApp->applicationDirPath());
    mThermusDir.cdUp();

    const QString kPartDirOrg = mThermusDir.path() + "/Resources/particles/";
    QString partDir = QDir::tempPath() + "/" + mThermus + "/";

    // create a temp dir to store particles data files
    QDir::temp().mkdir(mThermus);
    QDir org(kPartDirOrg);
    for (QFileInfo f : org.entryInfoList())
        QFile::copy(kPartDirOrg + f.fileName(), partDir + f.fileName());
    QDir::setCurrent(partDir);

    QString thermusDBName(QString(ParticlesDBManager::instance().getThermusDBName()).append(".db"));
    QString pdgDBName(QString(ParticlesDBManager::instance().getPDGDBName()).append(".db"));

    mThermusDBPath = thermusDBName.prepend(partDir);
    mPdgDBPath     = pdgDBName.prepend(partDir);

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
    statusBar()->showMessage(tr("Welcome to Thermus"));

    // the toolbars
//    QToolBar *tb = new QToolBar(this);

//    QAction *pract = new QAction(QFontIcon::icon(0xf02f), "print");
//    pract->setWhatsThis("print current window");
//    connect(pract, SIGNAL(triggered(bool)), this, SLOT(printCurrentWindow()));
//    tb->addAction(pract);
//    addToolBar(tb);


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
    if (mNewDB) {
        QMessageBox msg(QMessageBox::Information, "DB modified", "Particles DB has been modified?");
        msg.setInformativeText("Do you want to save changes?");
        msg.setStandardButtons(QMessageBox::Yes);
        msg.addButton(QMessageBox::No);
        msg.setDefaultButton(QMessageBox::No);
        if(msg.exec() == QMessageBox::Yes) {
            QDir save(QDir::homePath());
            save.mkdir("ThermusSave");
            save.cd("ThermusSave");
            QDir::current().setNameFilters(QStringList() << "*.db");
            for (QFileInfo f : QDir::current().entryInfoList())
                QFile::copy(f.fileName(), save.absolutePath() + "/" + f.fileName());
            QMessageBox msgg(QMessageBox::Information, "", "Particles DB saved");
            msgg.setInformativeText(QString("in %1").arg(save.absolutePath()));
            msgg.exec();        }
    }
//    QDir::current().removeRecursively();
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

    // the console for log output from qDebug, qInfo, qWarning --> disabled, use log file instaed
//    mLogConsol = new QPlainTextEdit("log Consol", mdiArea);
//    mLogConsol->setReadOnly(true);
//    mdiArea->addSubWindow(mLogConsol);

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
void MainWindow::drawResults()
{
    // draw the results of the fit

    FitMacro::instance().makePlot();
}

//__________________________________________________________________________
void MainWindow::nsigmaContours()
{
    //    calculates and display n-sigma contours following a fit

    FitMacro::instance().makeContour();
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

    myMacro->start(mDebug);

    new ThermusWiz(what, this);

    // **************************************************
    // connect to the Thermus DB
    particlesDBManagement(kConnectT);

    message += ": macro " + myMacro->objectName();
    statusBar()->showMessage(message);

    // Choice of starting parameters, select parameters to fit or fix, add constraint
    myMacro->setParameters();

    // Create the fit model
    myMacro->setFit();

    // Run the stuff
    myMacro->run();

    if( what == "Fit") {
        mContourAction->setVisible(true);
        mPlotAction->setVisible(true);
    }
    mResultsMenu->menuAction()->setVisible(true);
}

//__________________________________________________________________________
void MainWindow::paintEvent(QPaintEvent *event)
{
    // decorate the central widget

    QPainter painter(this);

    // the background image

    QPixmap bkgnd(":/tumblr_static_9sry26wrvv484cg0wsw4o0gc0_640_v2.jpg");
    painter.drawPixmap(0,0,width(), height() ,bkgnd);

    // the Thermus banner, first the background
//    qint32 bheight = height()  * 0.3 ;
//    qint32 bwidth  = width() * 0.7 ;
//    qint32 bx = (width() - bwidth) / 2;
//    qint32 by = (height() - bheight) / 2;
//    QRect bg = QRect(bx, by, bwidth, bheight);

    QColor blue = Qt::blue;
    blue.setAlphaF(0.5);

    painter.setBrush(QBrush(blue));
//    painter.drawRect(bg);

    // the Thermus banner, then the text

    painter.setPen(QPen(blue));
    painter.setFont(QFont("Helvetica [Cronyx]", QFont::ExtraBold, 20, true));
    int y = rect().bottom() - height() * 0.3;
    int stepy = qrand() % height() * 0.3;
    int stepx = width() / 8;
    int x0 = width() / 2. - stepx / 2;
    int x = x0;
    painter.drawText(x, y-stepy,"r");
    stepy = qrand() % height()  * 0.3 ;
    x -= stepx;
    painter.drawText(x, y-stepy,"e");
    stepy = qrand() % height()  * 0.3 ;
    x -= stepx;
    painter.drawText(x, y-stepy,"h");
    stepy = qrand() % height()  * 0.3 ;
    x -= stepx;
    painter.drawText(x, y-stepy,"t");
    stepy = qrand() % height()  * 0.3 ;
    x = x0 + stepx;
    painter.drawText(x, y-stepy,"m");
    stepy = qrand() % height()  * 0.3 ;
    x += stepx;
    painter.drawText(x, y-stepy,"u");
    stepy = qrand() % height()  * 0.3 ;
    x += stepx;
    painter.drawText(x, y-stepy,"s");

    y = rect().bottom() - height() * 0.1;
    x = rect().right() - width() * 0.3;
    painter.setFont(QFont("Helvetica [Cronyx]", QFont::ExtraLight, 10, true));
    painter.setPen(QPen(Qt::black));
    painter.drawText(x, y, "arXiv:1108.4588 [hep-ph]");
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
    mPredictionAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_P));
    mPredictionAction->setStatusTip(tr("Makes a Thermus prediction"));
    connect(mPredictionAction, &QAction::triggered, this, [this] { run(mPredictionAction->text().remove(0,1)); });

    mFitAction = new QAction(QIcon(":/fiticon.png"), tr("&Fit"), this);
    mFitAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F));
    mFitAction->setStatusTip(tr("Makes a Thermus Fit"));
    connect(mFitAction, &QAction::triggered, this, [this] { run(mFitAction->text().remove(0,1)); });

    // edit macro
    mMacroAction = new QAction(QIcon(":/plugins-512.png"),tr("Macro"), this);
    mMacroAction->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_M));
    mMacroAction->setStatusTip(tr("Edit, compile, execute a macro"));
    connect(mMacroAction, &QAction::triggered, this, [this] { openCreateMacro(); });

    // Parameters list action

    mParametersList = new QAction(QIcon(":/listicon.png"), tr("Parameters list"), this);
    mParametersList->setStatusTip("Makes a list of parameters with properties");
    connect(mParametersList, &QAction::triggered, this, [this]{ listParameters(); });

    // makes n-sigma contour & dispay (following) a fit
    mContourAction = new QAction(QIcon(":/contouricon.png"), tr("n-sigma contours"), this);
    mContourAction->setStatusTip(tr("Makes a n-sigma contours plot following a fit"));
    connect(mContourAction, &QAction::triggered, this, [this]{ nsigmaContours(); });

    // makes plot & dispay results following a fit
    mPlotAction = new QAction(QIcon(":/ploticon.png"), tr("plot fit result"), this);
    mPlotAction->setStatusTip(tr("Makes a plot of the results of a fit"));
    connect(mPlotAction, &QAction::triggered, this, [this]{ drawResults(); });
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
    mRunMenu->addAction(mMacroAction);

    // provide functions to view the results
    mResultsMenu = menuBar()->addMenu(tr("Results"));
    mResultsMenu->addAction(mContourAction);
    mResultsMenu->addAction(mPlotAction);
    mResultsMenu->menuAction()->setVisible(false);
    mContourAction->setVisible(false);
    mPlotAction->setVisible(false);
}

//__________________________________________________________________________
void MainWindow::openCreateMacro()
{
    // edit, compile, load macros on the flight
    if (!mMacroEditor)
        mMacroEditor = new MacroEditor(this);
    mMacroEditor->start();
}

//__________________________________________________________________________
void MainWindow::particlesDBManagement(DBOPS option)
{
    // Manages the particles DB

    const QString kExtDir     = mThermusDir.path() + "/Resources/python/";

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

    QMetaEnum metaEnum = QMetaEnum::fromType<DBOPS>();
    QString soption    = metaEnum.valueToKey(option);

    statusBar()->showMessage(soption);

    soption.remove(0,1);
    soption.remove(soption.length() - 1 ,1);

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

        QString kPythonPath("/usr/local/bin/python3");
        QFileInfo check(kPythonPath);
        if (!check.exists()) {
            kPythonPath = "/usr/bin/python3";
            check.setFile(kPythonPath);
        }
        if (!check.exists() || !check.isExecutable()) {
            QMessageBox msg(QMessageBox::Critical, "", tr("missing python script"));
            msg.setInformativeText(QString(tr("%1: %2 is not found or not executable!")).arg(Q_FUNC_INFO, kPythonPath));
            msg.exec();
            return;
        }
        QProcess p;
        p.setProcessChannelMode(QProcess::SeparateChannels);
        QStringList params;
        QDir::temp().mkdir("Thermus");
        if (soption.contains("Create")) {
            // untar the particles data text files
            const QString kParticlesData("particles.tar.gz");
            check.setFile(QDir::currentPath() + "/" + kParticlesData);
            if (!check.exists()) {
                QMessageBox msg(QMessageBox::Critical, "", tr("Missing data"));
                msg.setInformativeText(QString(tr("%1 does not exist in %2")).arg(kParticlesData, QDir::currentPath()));
                msg.exec();
                return;
            }
            params << "-zxvf" << QDir::currentPath() + "/" + kParticlesData;
            p.start("tar", params);
            p.waitForFinished();
            p.close();
        }
        QString pythonScriptName;
        QString input("");
        if (pdg) {
            pythonScriptName = "PDGParticles.py";
        } else if (thermus) {
            pythonScriptName = "ThermusParticles.py";
            if (soption.contains("Create")) {
                // check if PDG particles db exists
//                QString pdgName(dbName);
//                pdgName.replace("Thermus", "PDG");
                //                pdgName.prepend(kPartDir);
                QFileInfo check(mPdgDBPath);
                if (! check.exists()) {
                    QMessageBox msg(QMessageBox::Critical, tr("Particles DB creation"), tr("You must first create the PDG particles list"));
                    msg.setInformativeText(QString(tr("DB %1 not found in %2")).arg(mPdgDBPath, QDir::currentPath()));
                    msg.exec();
                    return;
                }
                QString file = QFileDialog::getOpenFileName(this, tr("Open a particle list"), QDir::currentPath(),
                                                            tr("particles list (PartList_*)"));
                input = QFileInfo(file).fileName();
            }
        }
        QFile pythonScript(pythonScriptName);
        pythonScript.setPermissions(QFile::ExeGroup | QFile::ExeOther | QFile::ExeOther | QFile::ExeUser);
        if (soption.contains(("Update"))) {
            // check if particles db exists
            QFileInfo check(fullDBName);
            if (! check.exists()) {
                QMessageBox msg(QMessageBox::Critical, tr("Particles DB creation"), tr("You must first create the particles DB"));
                msg.setInformativeText(QString(tr("DB %1 not found in %2")).arg(fullDBName, QDir::currentPath()));
                msg.exec();
                return;
            }
        }
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        env.insert("EXTDIR", kExtDir); // Add an environment variable
        env.insert("PARTDIR", QDir::currentPath() + "/");
        QString dbName = fullDBName.right(fullDBName.length() - fullDBName.lastIndexOf("/") - 1);
        env.insert("DBNAME", dbName);
        p.setProcessEnvironment(env);

        params.clear();
        params << kExtDir + pythonScriptName << soption << input;
        QProgressDialog progress;
        p.start(kPythonPath, params);

        int iprogress = 0;
        while (p.state() == QProcess::Running || p.state() == QProcess::Starting) {
            if (iprogress == 0 && p.state() == QProcess::Running) {
                progress.setLabelText(QString("%1 particles DB (%2)").arg(soption, sourceName));
                progress.setCancelButtonText("Cancel");
                progress.setMinimum(0);
                progress.setMaximum(800);
                progress.setWindowModality(Qt::WindowModal);
                progress.resize(progress.width() * 1.5, progress.height()  * 0.3 );
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
            msg.setText(QString("%1 or %2 not found").arg(kPythonPath, pythonScriptName));
            msg.exec();
            return;
        }
        QString p_stdout = p.readAllStandardOutput();
        QStringList listout = p_stdout.split('\n');
        QString p_stderr = p.readAllStandardError();

        if (p.exitStatus() == QProcess::CrashExit) {
            QMessageBox msg(QMessageBox::Critical, "Python abort", "Python Abort");
            msg.setInformativeText("python process crashed");
            msg.exec();
        }
        if (p_stderr != "") {
            QMessageBox msg(QMessageBox::Critical, "Python abort", "Python Abort");
            msg.setInformativeText(msg.text() + " Error encountered in Python macro: " + p_stderr);
            msg.exec();
        }
        else {
            QMessageBox msg(QMessageBox::Information, "Python success", "Python success");
            msg.setInformativeText(QString("The particles DB has been %1d with %2 entries").arg(soption).arg(listout.size()));
            msg.exec();
            mNewDB = true;
        }
        p.close();
        return;
    }

    // ======================
    // search a particle with its decay channels
    if (soption.contains("Search")) {
        if (ParticlesDBManager::instance().connect(fullDBName)) {
            mFd = new FindDialog(sourceName, this);
            mFd->exec();
        }
        return;
    }

    // ======================
    // List all particles
    if (soption.contains("List")) {
        if (ParticlesDBManager::instance().connect(fullDBName)) {
            ParticlesDBManager::instance().listParticles(ParticlesDBManager::kALL);
        }
        return;
    }

    // ======================
    // Insert a new particle into Thermus DB
    if (option == kInsert) {
        bool connected = ParticlesDBManager::instance().connect(fullDBName);
        if (!connected) {
            QMessageBox msg(QMessageBox::Critical, "DB connection",
                            QString("Could not connect to particles DB: %1").arg(fullDBName));
            msg.exec();
        } else {
            mNPD = new NewParticleDialog(this);
            mNPD->exec();
            mNewDB = true;
        }
        return;
    }
}
