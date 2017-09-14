// Author: Spencer Wheaton 14 July 2004
// Adapted for Qt by Yves Schutz: September 2017
//////////////////////////////////////////////////////////////////////////
//                                                                 	    //
// TTMThermalParticle                                          		    //
//                                                                      //
// Base class 	 				     	            	                //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALPARTICLE_H
#define TTMTHERMALPARTICLE_H

#include <QObject>

class TTMParameterSet;
class TTMThermalParticle : public QObject
{
    Q_OBJECT
public:
    explicit TTMThermalParticle(QObject *parent = 0);

    double                   densityBoltzmannNoWidth();
    double                   densityBoltzmannWidth();
    double                   energyBoltzmannNoWidth();
    double                   energyBoltzmannWidth();
    double                   getCorrFactor() const {return mCorrFactor;}
    int                      getParticle() const { return mParticle; }
    double                   pressureBoltzmannNoWidth();
    double                   pressureBoltzmannWidth();
    void                     setParticle(int pdg) {mParticle = pdg;}
    void                     setCorrFactor(double x) { mCorrFactor = x;}
    virtual void             updateMembers() = 0;


protected:
    double mCorrFactor;	// w.r.t. grand-canonical results (in the Boltzmann approximation there is a simple
                        // multiplicative correction factor for e, n and P)
    double mDeg;		// copy of particle's degeneracy
    double mG;			// heavy flavour suppression
    double mM;			// copy of particle's mass
    double mMu;			// copy of particle's chemical potential
    int    mParticle;   // particle id
    double mT;			// copy of temperature


};

#endif // TTMTHERMALPARTICLE_H
