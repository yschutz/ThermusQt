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
}

//__________________________________________________________________________
void Logger::writeMessage(const QString &message, bool verbose)
{
//    if (mTextEdit) {
//        if (!debug)
//            mTextEdit->appendPlainText("");
//        else
//            mTextEdit->appendPlainText(message);
//                    mTextEdit->appendHtml(message);
        if (verbose && mLogFile.isOpen()) {
            QTextStream out(&mLogFile);
            out << message << "\n";
        }
//    }
}

//__________________________________________________________________________
Logger &Logger::instance()
{
    // returns the unique instance
    return mInstance;
}

//__________________________________________________________________________
void Logger::setLogFileName(const QString &name)
{
    // set the Log file name
    //    QString logdir(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory) + "/ThermusQt/logs");
    QDir::setCurrent("/tmp");
    mLogFile.setFileName(name);
    mLogFile.open(QIODevice::WriteOnly | QIODevice::Text);
}
