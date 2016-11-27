// Author: Yves Schutz 23 novembre 2016
//
// Object to redefine logging. Singleton

#include "logger.h"

Logger Logger::mInstance = Logger();

//__________________________________________________________________________
Logger::Logger()
{
    // ctor (private)
    setObjectName("Logger");
}

//__________________________________________________________________________
void Logger::writeMessage(const QString &message, bool debug)
{
    if (mTextEdit) {
        if (debug)
            mTextEdit->appendPlainText(message);
        else
            mTextEdit->appendHtml(message);

    }
}

//__________________________________________________________________________
Logger &Logger::instance()
{
    // returns the unique instance
    return mInstance;
}
