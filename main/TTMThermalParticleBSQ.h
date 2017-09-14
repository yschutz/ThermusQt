// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: September 2017
//////////////////////////////////////////////////////////////////////////
//                                                                 	    //
// TTMThermalParticleBSQ                                       		    //
//                                                                  	//
// A thermal particle in the complete grand-canonical formalism        	//
//                                                                  	//
//////////////////////////////////////////////////////////////////////////
#ifndef TTMTHERMALPARTICLEBSQ_H
#define TTMTHERMALPARTICLEBSQ_H

#include <QObject>

#include "TTMThermalParticle.h"

class TTMParameterSetBSQ;
class TTMThermalParticleBSQ : public TTMThermalParticle
{
public:
    TTMThermalParticleBSQ(QObject* parent = 0);

    double              densityQStatNoWidth();
    double              densityQStatWidth();
    TTMParameterSetBSQ* getParameters() { return mParameters; }
    bool                parametersAllowed();	      // checks for Bose-Einstein Condensation
    void                updateMembers();

private:
    TTMParameterSetBSQ *mParameters;    // pointer to thermal parameters
    TTMThermalParticleBSQ(int part, TTMParameterSetBSQ *parm, QObject* parent = 0);
    TTMThermalParticleBSQ(TTMThermalParticleBSQ& obj);
};

#endif // TTMTHERMALPARTICLEBSQ_H
