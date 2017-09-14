//////////////////////////////////////////////////////////////////////////
//                                                                 	    //
// TTMParameterSetBQ    						                        //
//									                                    //
// The parameter set used when Baryon# and Charge are to be treated 	//
// grand-canonically, and strangeness canonically 			            //
//     									                                //
//     									                                //
//     	 mPar[0]:T 		    Temperature			                //
//     	 mPar[1]:muB 		    Baryon Chemical Potential	        //
//     	 mPar[2]:muQ 		    Charge Chemical Potential	        //
//     	 mPar[3]:gammas 		Strangeness Suppression Factor	    //
//	     mPar[4]:Can. radius	Correlation radius		            //
//     	 mPar[5]:radius		Fireball Radius			            //
//     	 								                                //
//   The ratio Baryon/(2*Charge) may be used to constrain muQ.		    //
//                                                                  	//
//////////////////////////////////////////////////////////////////////////
// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: September 2017


#ifndef TTMPARAMETERSETBQ_H
#define TTMPARAMETERSETBQ_H

#include "main/TTMParameterSet.h"

class TTMParameter;

class TTMParameterSetBQ : public TTMParameterSet
{
public:

    TTMParameterSetBQ();
    TTMParameterSetBQ(double temp, double mub,
                       double muq, double gs, double can_r,
                       double r = 0., double b2q = 0., double strangeness = 0.,
                       double temp_error = 0., double mub_error = 0.,
                       double muq_error = 0., double gs_error = 0.,
                       double can_r_error = 0., double r_error = 0.);

    void           conserveSGlobally();
    void           constrainMuQ(double b2q);
    void           fitCanRadius(double start, double min = 0.1, double max = 15., double step = 0.01)
                                            { mCorrRConstrain = false; mPar[kCRADIUS]->fit(start, min, max, step); }
    void           fitGammas(double start, double min = 0., double max = 1.5, double step = 0.001)
                                            { mPar[kGAMMAS]->fit(start, min, max, step); }
    void           fitMuB(double start, double min = 0.000, double max = 0.500, double step = 0.001)
                                            { mPar[kMUB]->fit(start, min, max, step); }
    void           fitMuQ(double start, double min = -0.200, double max = 0.200, double step = 0.001)
                                            { mMuQConstrain = false; mPar[kMUQ]->fit(start, min, max, step); }
    void           fitRadius(double start, double min = 0., double max = 15., double step = 0.01)
                                            { mPar[kRADIUS]->fit(start, min, max, step); }
    void           fitT(double start, double min = 0.050, double max = 0.180, double step = 0.001)
                                            { mPar[kT]->fit(start, min, max, step); }
    void           fixCanRadius(double value, double error = 0.)
                                            { mCorrRConstrain = false; mPar[kCRADIUS]->fix(value, error); }
    void           fixGammas(double value, double error = 0.)
                                            { mPar[kGAMMAS]->fix(value, error); }
    void           fixMuB(double value, double error = 0.)
                                            { mPar[kMUB]->fix(value, error); }
    void           fixMuQ(double value, double error = 0.)
                                            {  mMuQConstrain = false; mPar[kMUQ]->fix(value, error);}
    void           fixRadius(double value, double error = 0.)
                                            { mPar[kRADIUS]->fix(value, error); }
    void           fixT(double value, double error = 0.)
                                            { mPar[kT]->fix(value, error); }
    bool           getMuQConstrain() const  {return mMuQConstrain;}
    bool           getCorrRConstrain() const{return mCorrRConstrain;}
    double         getS() const             {return mS;}
    double         getB2Q() const           {return mB2Q;}
//    double         getT() const             {return mPar[kT]->getValue();}
//    TTMParameter*  getTPar()                {return mPar[kT];}
//    double         getMuB() const           {return mPar[kMUB]->getValue();}
//    TTMParameter*  getMuBPar()              {return mPar[kMUB];}
//    double         getMuQ() const           {return mPar[kMUQ]->getValue();}
//    TTMParameter*  getMuQPar()              {return mPar[kMUQ];}
//    double         getGammas() const        {return mPar[kGAMMAS]->getValue();}
//    TTMParameter*  getGammasPar()           {return mPar[kGAMMAS];}
//    double         getCanRadius() const     {return mPar[kCRADIUS]->getValue();}
//    TTMParameter*  getCanRadiusPar()        {return mPar[kCRADIUS];}
//    double         getRadius() const        {return mPar[kRADIUS]->getValue();}
//    TTMParameter*  getRadiusPar()           {return mPar[kRADIUS];}
//    void           setCanRadius(double x)   {mPar[kCRADIUS]->setValue(x);}

    TTMParameterSetBQ& operator=(const TTMParameterSetBQ& obj);


private:
    bool         mCorrRConstrain;	// true if the correlation and fireball
    double       mS;	            // the required strangeness inside the correlation volume
//    double       mB2Q;		        // the initial B/2Q ratio
    bool         mMuQConstrain;     // true if muQ must be constrained

};

#endif // TTMPARAMETERSETBQ_H
