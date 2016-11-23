// Author: Yves Schutz octobre 2016
//
// The main window steering the show

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAbstractButton>
#include <QDateTime>
#include <QDebug>
#include <QVBoxLayout>

#include "macros/runmacro.h"
#include "thermuswiz.h"

//__________________________________________________________________________
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow), mDebug(false)
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
//    setCentralWidget(mCentralwidget);

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

    // Choice of starting parameters

    myMacro.setParameters();

    // choice of parameter to fit or to fix
    // -> Default is fixed at zero or at unity so important to check !!

    myMacro.setFitFix();

    // option of adding some constraint

    myMacro.setConstrain();

    // list parameters settings
    myMacro.listParameters();
}

//__________________________________________________________________________
void MainWindow::quit()
{
    // end gracefully
    statusBar()->showMessage(mQuitAction->statusTip());
    close();
}
//__________________________________________________________________________
void MainWindow::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    QPixmap bkgnd(":/2015-Jul-03-Fit_PbPb0010_Reference_final_SQM.png");


    painter.drawPixmap(0,0,width(), height(),bkgnd);

    QFont font;
    font.setPixelSize(40);
    painter.setPen(QPen(Qt::red));
    painter.setFont(font);

    QRect bg = QRect(0,height()/2  , width(), height()/2 + 200);

    QColor blue = Qt::blue;
    blue.setAlphaF(0.5);

    painter.setBrush(QBrush(blue));
    painter.drawRect(bg);

    painter.drawText(rect(), Qt::AlignHCenter|Qt::AlignVCenter,"Thermus");

    QLabel *thermus  = new QLabel("THERMUS");





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
