#ifndef MACROINTERFACE_H
#define MACROINTERFACE_H

#include <QString>
class MacroInterface
{
public:
    virtual QString run(const QString& message) = 0;
};

QT_BEGIN_NAMESPACE

#define MacroInterFace_iid "aaaa.bbbbb.MacroInterface"

Q_DECLARE_INTERFACE(MacroInterface, MacroInterFace_iid)

QT_END_NAMESPACE

#endif // MACROINTERFACE_H
