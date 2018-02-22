// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz:  Octobre 2016

//__________________________________________________________________________
// ID object for storage in Qt container.
//

#include "TTMIDObj.h"

//__________________________________________________________________________
TTMIDObj::TTMIDObj(QObject* parent) : QObject(parent),
   mID(0)
{
    // default ctor
}

//__________________________________________________________________________
TTMIDObj::TTMIDObj(int x, QObject* parent) : QObject(parent),
    mID(x)

{
    // ctor
}

//__________________________________________________________________________
TTMIDObj &TTMIDObj::operator=(const TTMIDObj &obj)
{
    // assignation operator

    if (this == &obj)
        return *this;

    setID(obj.getID());
    return *this;
}
