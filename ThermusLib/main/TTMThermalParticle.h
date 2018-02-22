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
    explicit TTMThermalParticle(QObject *parent = nullptr);

    double                   densityBoltzmannNoWidth(double exclVolPressure = 0.0);
    double                   densityBoltzmannWidth(double exclVolPressure = 0.0);
    double                   energyBoltzmannNoWidth(double exclVolPressure = 0.0);
    double                   energyBoltzmannWidth(double exclVolPressure = 0.0);
    double                   getCorrFactor() const {return mCorrFactor;}
    int                      getParticle() const { return mParticle; }
    double                   pressureBoltzmannNoWidth(double exclVolPressure = 0.0);
    double                   pressureBoltzmannWidth(double exclVolPressure = 0.0);
    void                     setParticle(int pdg) {mParticle = pdg;}
    void                     setCorrFactor(double x) { mCorrFactor = x;}
    virtual void             updateMembers(double exclVolPressure = 0.0) = 0;


protected:
    double mB;          // copy of baryon number
    double mBeauty;     // copy of beauty
    double mC;          // copy of charm
    double mCorrFactor;	// w.r.t. grand-canonical results (in the Boltzmann approximation there is a simple
                        // multiplicative correction factor for e, n and P)
    double mDeg;		// copy of particle's degeneracy
    double mG;			// heavy flavour suppression
    double mM;			// copy of particle's mass
    double mMu;			// copy of particle's chemical potential
    double mMuB;	    // copy of baryon chemical potential
    double mMuBeauty;	// copy of beauty chemical potential
    double mMuC;	    // copy of charm chemical potential
    double mMuQ;	    // copy of charge chemical potential
    double mMuS;	    // copy of strangeness chemical potential
    int    mParticle;   // particle id
    double mQ;          // copy of charge
    double mS;          // copy of strangeness
    double mT;			// copy of temperature


};

#endif // TTMTHERMALPARTICLE_H
