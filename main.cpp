// Author: Yves Schutz 24 octobre 2016
//
// The main program

#include <QHBoxLayout>

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QtWidgets>


//static QPlainTextEdit  * debug = nullptr;

//void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
//{
//    if (debug == nullptr){
//        debug = new QPlainTextEdit;
//        debug->show();
//    }

//    debug->appendPlainText(msg);
//}



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


//   qInstallMessageHandler(myMessageOutput);





    MainWindow w;
    w.show();
    return a.exec();
}
