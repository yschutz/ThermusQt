// Author: Yves Schutz 23 novembre 2016
//
// Object to redefine logging. Singleton

#include "logger.h"

#include <QDebug>
#include <QStandardPaths>
#include <QTextStream>

Logger Logger::mInstance = Logger();

//__________________________________________________________________________
Logger::Logger()
{
    // ctor (private)
    setObjectName("Logger");
    //    QString logdir(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "/ThermusQt/logs");
    QDir::setCurrent("/tmp");
    mLogFile.setFileName("ThermusLog.txt");
    mLogFile.open(QIODevice::WriteOnly | QIODevice::Text);
}

//__________________________________________________________________________
void Logger::writeMessage(const QString &message, bool debug)
{
    if (mTextEdit) {
        if (debug)
            mTextEdit->appendPlainText(message);
        else
            mTextEdit->appendPlainText(message);
        //            mTextEdit->appendHtml(message);
        QTextStream out(&mLogFile);
        out << message << "\n";
    }
}

//__________________________________________________________________________
Logger &Logger::instance()
{
    // returns the unique instance
    return mInstance;
}
