// Author: Yves Schutz octobre 2016
//
// The main window steering the show


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPlainTextEdit>

//static QPlainTextEdit* mLogConsol;         // Consol where to write log information

class FindDialog;
class MacroEditor;
class NewParticleDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum DBOPS {kConnectP, kCreateP, kUpdateP, kSearchP, kListP, kLastP,
                kConnectT, kCreateT, kUpdateT, kSearchT, kListT, kLastT,
                kInsert, kSelect};
    Q_ENUM(DBOPS)

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void        createConsol();
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    QString     getDBPath(const QString& opt);
    void        listParameters();
    void        particlesDBManagement(DBOPS option);
    void        setDebugMode(bool val);

public slots:
    void nsigmaContours();
    void openCreateMacro();
    void run(const QString& what);

protected:
    void paintEvent(QPaintEvent *event);

private:

    void createActions();
    void createMenus();

    QWidget*           mCentralwidget;         // The window so far inactive
    QAction*           mContourAction;         // calculates and display n-sigma contours following a fit
    QMenu*             mCreateMenu;            // to create the particles db from Root or Thermus Data
    QAction*           mCreatePDG;             // to create the particles db from Root Data
    QAction*           mCreateThermus;         // to create the particles db from Thermus Data
    static bool        mDebug;                 // True for running debug mode
    QMenu*             mParticlesMenu;         // Menu Tab to manage the particles database
    QMenu*             mDebugMenu;             // Menu Tab to set the debug on/off
    QAction*           mDebugOffAction;        // Action for debug mode off
    QAction*           mDebugOnAction;         // Action for debug mode on
    FindDialog*        mFd;                    // the dialog box for particle search
    QAction*           mFitAction;             // Action for the Fit macro
    QMenu*             mListMenu;              // to list all the particles db from Root or Thermus Data
    QAction*           mListPDG;               // to list the particles Root db
    QAction*           mListThermus;           // to list the particles Thermus db
    QAction*           mMacroAction;           // triggers opening or creation of a macro
    MacroEditor*       mMacroEditor;           // tool to edit, compile and load a macro on the flight
    bool               mNewDB;                 // to check if db has been changed
    QAction*           mNewParticleAction;     // insert a new particle in Thermus DB
    NewParticleDialog* mNPD;                   // dialog box to insert a new particle in Thermus DB
    QAction*           mParametersList;        // list all parameteres with properties
    QAction*           mParticlesListLong;     // list all particles properties and their decay (long output)
    QAction*           mParticlesListShort;    // list all particles properties and their decay (short output)
    QString            mPdgDBPath;             // the full path to PDG DB
    QAction*           mPdgSelectAction;       // select particles for the Thermus fit
    QAction*           mPdgUpdateMassAction;   // update particles mass and lifetime
    QAction*           mPredictionAction;      // Action for the Prediction macro
    QMenu*             mResultsMenu;           // Menu Tab to select functions displaying the results
    QMenu*             mRunMenu;               // Menu Tab to select the macro to run
    QMenu*             mSearchMenu;            // to serach and update one particle from Root or Thermus Data
    QAction*           mSearchPDG;             // to search for a particle Root db
    QAction*           mSearchThermus;         // to search for a particle Thermus db
    QDir               mThermusDir;            // directory of the Thermus installation ($HOME/ThermusQt)
    QString            mThermusDBPath;         // the full path to Thermus DB
    QMenu*             mUpdateMenu;            // to update the particles db from Root or Thermus Data
    QAction*           mUpdatePDG;             // to update the particles Root db
    QAction*           mUpdateThermus;         // to update the particles Thermus db    
    QString            mThermus;               // To be used for naming the temp directory
    QString            mWhat;                  // Prediction or Fit
};

#endif // MAINWINDOW_H
