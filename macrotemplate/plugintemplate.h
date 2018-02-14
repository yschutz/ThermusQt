#ifndef XXXXXXXX_H
#define XXXXXXXX_H

#include <QObject>
#include <QtPlugin>

#include "plugin_global.h"
#include "macrointerface.h"

class PLUGINSHARED_EXPORT XxXxxxxx : public QObject, MacroInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "aaaa.bbbbb.MacroInterface" FILE "xxxxxxxx.json")
    Q_INTERFACES(MacroInterface)

public:
    QString run(const QString& message);

};

#endif // XXXXXXXX_H
