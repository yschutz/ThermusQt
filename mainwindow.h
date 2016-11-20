// Author: Yves Schutz octobre 2016
//
// The main window steering the show


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setDebugMode(bool val);
    void runPrediction();
    void quit();

private:

    void createActions();
    void createMenus();

    Ui::MainWindow *ui;

    bool     mDebug;             // True for running debug mode
    QMenu*   mDebugMenu;         // Menu Tab to set the debug on/off
    QAction* mDebugOffAction;    // Action for debug mode off
    QAction* mDebugOnAction;     // Action for debug mode on
    QMenu*   mRunMenu;           // Menu Tab to select the macro to run
    QAction* mPredictionAction;  // Action for the Prediction macro
    QAction* mQuitAction;        // Action for quitting the application
    QWidget* mCentralwidget;     // The window so far inactive
};

#endif // MAINWINDOW_H
