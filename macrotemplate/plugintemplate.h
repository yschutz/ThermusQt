#ifndef XXXXXXXX_H
#define XXXXXXXX_H

#include <QtPlugin>

#include "plugin_global.h"
#include "macrointerface.h"

class PLUGINSHARED_EXPORT XxXxxxxx : public MacroInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "aaaa.bbbbb.MacroInterface" FILE "xxxxxxxx.json")
    Q_INTERFACES(MacroInterface)

public:
    void    localInit() override;
    void    localWrapup() override;
//    void    run() override;

public slots:
    void timeout();
};

#endif // XXXXXXXX_H
