// Author: Yves Schutz octobre 2016
//
// The main window steering the show


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDir>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPlainTextEdit>

static QPlainTextEdit* mLogConsol;         // Consol where to write log information

class FindDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum DBOPS {kCreateP, kUpdateP, kListP, kLastP, kCreateT,  kUpdateT, kListT, kLastT, kInsert, kSelect};
    Q_ENUM(DBOPS)

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void        createConsol();
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void        setDebugMode(bool val);

private slots:
    void listParameters();
    void listParticles(bool full);
    void particlesDBManagement(DBOPS option);
    void runPrediction();
    void quit();

protected:
    void paintEvent(QPaintEvent *event);

private:

    void createActions();
    void createMenus();

    QWidget*     mCentralwidget;         // The window so far inactive
    QMenu*       mCreateMenu;            // to create the particles db from Root or Thermus Data
    QAction*     mCreatePDG;             // to create the particles db from Root Data
    QAction*     mCreateThermus;         // to create the particles db from Thermus Data
    static bool  mDebug;                 // True for running debug mode
    QMenu*       mPdgMenu;               // Menu Tab to manage the particles database
    QMenu*       mDebugMenu;             // Menu Tab to set the debug on/off
    QAction*     mDebugOffAction;        // Action for debug mode off
    QAction*     mDebugOnAction;         // Action for debug mode on
    FindDialog*  mFd;                    // the dialog box for particle search
    QMenu*       mListMenu;              // to list the particles db from Root or Thermus Data
    QAction*     mListPDG;               // to list the particles Root db
    QAction*     mListThermus;           // to list the particles Thermus db
    QAction*     mParametersList;        // list all parameteres with properties
    QAction*     mParticlesListLong;     // list all particles properties and their decay (long output)
    QAction*     mParticlesListShort;    // list all particles properties and their decay (short output)
    QAction*     mPdgUpdateMassAction;   // update particles mass and lifetime
    QAction*     mPdgSelectAction;       // select particles for the Thermus fit
    QAction*     mPredictionAction;      // Action for the Prediction macro
    QAction*     mQuitAction;            // Action for quitting the application
    QMenu*       mRunMenu;               // Menu Tab to select the macro to run
    QDir         mThermusDir;            // directory of the Thermus installation ($HOME/ThermusQt)
    QMenu*       mUpdateMenu;            // to update the particles db from Root or Thermus Data
    QAction*     mUpdatePDG;             // to update the particles Root db
    QAction*     mUpdateThermus;         // to update the particles Thermus db

};

#endif // MAINWINDOW_H
