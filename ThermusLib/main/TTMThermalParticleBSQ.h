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
    TTMThermalParticleBSQ(QObject* parent = nullptr);
    TTMThermalParticleBSQ(int part, TTMParameterSetBSQ *parm, QObject* parent = nullptr);
    TTMThermalParticleBSQ(TTMThermalParticleBSQ& obj);

    double              densityQStatNoWidth(double exclVolPressure = 0.0);
    double              densityQStatWidth(double exclVolPressure = 0.0);
    double              energyQStatNoWidth(double exclVolPressure = 0.0);
    double              energyQStatWidth(double exclVolPressure = 0.0);
    double              entropyBoltzmannNoWidth(double exclVolPressure = 0.0);
    double              entropyBoltzmannWidth(double exclVolPressure = 0.0);
    double              entropyQStatNoWidth(double exclVolPressure = 0.0);
    double              entropyQStatWidth(double exclVolPressure = 0.0);
    TTMParameterSetBSQ* getParameters() { return mParameters; }
    bool                parametersAllowed();	      // checks for Bose-Einstein Condensation
    double              pressureQStatNoWidth(double exclVolPressure = 0.0);
    double              pressureQStatWidth(double exclVolPressure = 0.0);
    void                updateMembers(double exclVolPressure = 0.0);

private:
    TTMParameterSetBSQ *mParameters;    // pointer to thermal parameters
 };

#endif // TTMTHERMALPARTICLEBSQ_H
