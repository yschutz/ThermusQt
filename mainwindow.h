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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void appOutput(QString &msg) {mOutConsole->appendPlainText(msg);}
    void createConsol();
    void setDebugMode(bool val);

private slots:
    void runPrediction();
    void quit();

protected:
    void paintEvent(QPaintEvent *event);

private:

    void createActions();
    void createMenus();

    Ui::MainWindow *ui;

    QWidget* mCentralwidget;     // The window so far inactive
    bool     mDebug;             // True for running debug mode
    QMenu*   mDebugMenu;         // Menu Tab to set the debug on/off
    QAction* mDebugOffAction;    // Action for debug mode off
    QAction* mDebugOnAction;     // Action for debug mode on
    QAction* mPredictionAction;  // Action for the Prediction macro
    QAction* mQuitAction;        // Action for quitting the application
    QMenu*   mRunMenu;           // Menu Tab to select the macro to run

    QPlainTextEdit* mOutConsole; // Console for log qDebug/Info/Warning output
};

#endif // MAINWINDOW_H
