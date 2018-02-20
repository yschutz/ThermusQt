#include "xxxxxxxx.h"

//__________________________________________________________________________
QString XxXxxxxx::run(const QString &message)
{
    if (!isInitialized()) {
        QMessageBox::information(nullptr, Q_FUNC_INFO, "no intitialization done");
        return "no intialisation done";
    }
    else
        return QString("%1 : %2 --> %3").arg(objectName(), message, "Hello World");
}


//__________________________________________________________________________
bool XxXxxxxx::init()
{
    bool rv = false;

    mInitialized = rv;
    return rv;
}
