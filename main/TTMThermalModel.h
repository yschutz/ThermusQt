// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: August 2017
//////////////////////////////////////////////////////////////////////////
//                                                                   	//
// TTMThermalModel							                            //
//            	                                 		            	//
// Base class for thermal model objects. Thermal densities are stored	//
// as TTMDensObj objects in a hash table. All derived classes must 	    //
// define functions to calculate the primordial energy, entropy and  	//
// particle densities and pressure.					                    //
//				                                    				    //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALMODEL_H
#define TTMTHERMALMODEL_H

#include <QHash>
#include <QObject>

class TTMDensObj;
class TTMParameterSet;

class TTMThermalModel : public QObject
{
    Q_OBJECT
public:
    explicit TTMThermalModel(QObject *parent = nullptr);
    TTMThermalModel(const TTMThermalModel& model);

    ~TTMThermalModel();

    void                     generateDecayPartDens();
    void                     generateDecayPartDens(int pdg);
    virtual void             generateEnergyDens() = 0;
    virtual void             generateEntropyDens() = 0;
    virtual int              generateParticleDens() = 0;
    virtual void             generatePressure() = 0;
    double                   getBaryon() const       { return mBaryon; }
    double                   getCharge() const       { return mCharge; }
    double                   getDensity() const      { return mDensity; }
    TTMDensObj*              getDensities(int pdg) const;
    QHash<int, TTMDensObj*>  getDensityTable() const { return mDensTable; }
    double                   getEntropy() const      { return mEntropy; }
    double                   getEnergy() const       { return mEnergy; }
    virtual TTMParameterSet* getParameterSet() const = 0;
    void                     listDecayContributions(int daughter) const;
    void                     listDecayContributions(int parent, int daughter) const;
    virtual void             listInfo() const = 0;
    void                     listStableDensities() const;

protected:
    void                     calcWroblewski();
    virtual bool             primPartDens() = 0;

    double                  mBaryon;	   // total baryon density in model
    double                  mBeauty;	   // total beauty density in model
    double                  mBminus;
    double                  mBplus;
    double                  mbminus;
    double                  mbplus;
    double                  mCharge;	   // total charge density in model
    double                  mCharm;		   // total charm density in model
    double                  mCminus;
    double                  mCplus;
    double                  mDensity;	   // total particle density in model
    QHash<int, TTMDensObj*> mDensTable;    // pointer to density hash table
    QString                 mDescriptor;   // label for the model
    double                  mEnergy;	   // total energy density in model
    double                  mEntropy;	   // total entropy density in model
    double                  mPressure;	   // total pressure in model
    double                  mQminus;
    double                  mQplus;
    double                  mSminus;
    double                  mSplus;
    double                  mStrange;	   // total strangeness density in model
    bool                    mWidth;        // True if width is to be taken into account
    double                  mWroblewski;   // Wroblewski factor
};

#endif // TTMTHERMALMODEL_H
