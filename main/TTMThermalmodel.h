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
    explicit TTMThermalModel(QObject *parent = 0);
    ~TTMThermalModel();

    void                     generateDecayPartDens();
    void                     generateDecayPartDens(int pdg);
    TTMDensObj*              getDensities(int pdg) const;
    virtual TTMParameterSet* getParameterSet() = 0;
    void                     listDecayContributions(int daughter);

protected:
    QHash<int, TTMDensObj*> mDensTable;    // pointer to density hash table
};

#endif // TTMTHERMALMODEL_H
