// Author: Yves Schutz octobre 2016
//
// The main window steering the show


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QPainter>
#include <QPlainTextEdit>

static QPlainTextEdit* mLogConsol;         // Consol where to write log information

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void        createConsol();
    static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void        setDebugMode(bool val);

private slots:
    void listParameters();
    void listParticles(bool full);
    void runPrediction();
    void quit();

protected:
    void paintEvent(QPaintEvent *event);

private:

    void createActions();
    void createMenus();

    Ui::MainWindow *ui;

    QWidget*   mCentralwidget;     // The window so far inactive
    static bool       mDebug;             // True for running debug mode
    QMenu*     mDebugMenu;         // Menu Tab to set the debug on/off
    QAction*   mDebugOffAction;    // Action for debug mode off
    QAction*   mDebugOnAction;     // Action for debug mode on
    QAction*   mParametersList;    // list all parameteres with properties
    QAction*   mParticlesListLong; // list all particles properties and their decay (long output)
    QAction*   mParticlesListShort;// list all particles properties and their decay (short output)
    QAction*   mPredictionAction;  // Action for the Prediction macro
    QAction*   mQuitAction;        // Action for quitting the application
    QMenu*     mRunMenu;           // Menu Tab to select the macro to run

};

#endif // MAINWINDOW_H
