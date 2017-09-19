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
    TTMThermalParticleBSQ(int part, TTMParameterSetBSQ *parm, QObject* parent = 0);
    TTMThermalParticleBSQ(TTMThermalParticleBSQ& obj);

    double              densityQStatNoWidth();
    double              densityQStatWidth();
    double              energyQStatNoWidth();
    double              energyQStatWidth();
    double              entropyBoltzmannNoWidth();
    double              entropyBoltzmannWidth();
    double              entropyQStatNoWidth();
    double              entropyQStatWidth();
    TTMParameterSetBSQ* getParameters() { return mParameters; }
    bool                parametersAllowed();	      // checks for Bose-Einstein Condensation
    double              pressureQStatNoWidth();
    double              pressureQStatWidth();
    void                updateMembers();

private:
    TTMParameterSetBSQ *mParameters;    // pointer to thermal parameters
 };

#endif // TTMTHERMALPARTICLEBSQ_H
