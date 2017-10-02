// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: September 2017
//////////////////////////////////////////////////////////////////////////
//                                                                   	//
// TTMThermalParticleBQ                                        		    //
//                                                                  	//
// A thermal particle in the formalism treating strangeness canonically	//
// and charge and baryon number grand-canonically. The Boltzmann 	    //
// approximation is used since in this case the correction to the 	    //
// grand-canonical results amounts to a simple multiplicative factor.	//
// The entropy cannot be split into the sum of particle entropies, so	//
// no entropy calculation is made here.				       	            //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALPARTICLEBQ_H
#define TTMTHERMALPARTICLEBQ_H

#include "TTMThermalParticle.h"

class TTMParameterSetBQ;

class TTMThermalParticleBQ : public TTMThermalParticle
{
public:
    TTMThermalParticleBQ(QObject* parent = nullptr);
    TTMThermalParticleBQ(int part, TTMParameterSetBQ *parm, double correction, QObject* parent = nullptr);
    TTMThermalParticleBQ(TTMThermalParticleBQ& obj);

    TTMParameterSetBQ* getParameters() const { return mParameters; }
    void               updateMembers();

    TTMThermalParticleBQ& operator=(TTMThermalParticleBQ& obj);


private:
    TTMParameterSetBQ* mParameters;	//pointer to thermal parameters

};

#endif // TTMTHERMALPARTICLEBQ_H
