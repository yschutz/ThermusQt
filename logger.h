// Author: Yves Schutz 23 novembre 2016
//
// Object to redefine logging. Singleton

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QPlainTextEdit>

class Logger : public QObject
{
    Q_OBJECT

public:
    static Logger& instance();

    void setTextEdit(QPlainTextEdit* val) {mTextEdit = val;}
    void writeMessage(const QString& message, bool debug=true);

private:
    Logger();
    ~Logger() {}
    Logger (const Logger&) {}

    static Logger      mInstance; // the unique instance
    QPlainTextEdit*    mTextEdit; // the widget where to write logs
};

#endif // LOGGER_H
