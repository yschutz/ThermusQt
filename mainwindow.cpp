// Author: Yves Schutz octobre 2016
//
// The main window steering the show

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractButton>
#include <QDateTime>
#include <QDebug>
#include <QMdiArea>
#include <QVBoxLayout>

#include "macros/runmacro.h"
#include "logger.h"
#include "thermuswiz.h"

bool MainWindow::mDebug = false;
//__________________________________________________________________________
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // ctor
    ui->setupUi(this);
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

    delete ui;
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

    // select the macro to run or quit
    mRunMenu = menuBar()->addMenu(tr("&Run"));
    mRunMenu->addAction(mPredictionAction);
    mRunMenu->addAction(mQuitAction);

}
