// Author: Yves Schutz 23 novembre 2016
//
// Object to redefine logging. Singleton

#ifndef LOGGER_H
#define LOGGER_H

#include <QDir>
#include <QObject>
#include <QPlainTextEdit>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger& instance();

    bool    isVerbose() const                                      { return mVerbose; }
    QString logFileName() const                                    { return QDir::current().path() + "/" + mLogFile.fileName(); }
    void    setLogFileName(const QString& name);
    void    setTextEdit(QPlainTextEdit* val)                       { mTextEdit = val; }
    void    setVerbosity(bool val = true)                          { mVerbose = val; }
    void    writeMessage(const QString& message, bool verbose=true);

private:
    Logger();
    ~Logger() { mLogFile.close(); }
    Logger (const Logger&) {}

    static Logger      mInstance; // the unique instance
    QFile              mLogFile;  // the log file
    QPlainTextEdit*    mTextEdit; // the widget where to write logs
    bool               mVerbose;  // verbosity on/off
};

#endif // LOGGER_H
