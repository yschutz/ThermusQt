// Author: Spencer Wheaton 26 April 2010 //
// Adapted for Qt by Yves Schutz: 27 Octobre 2016


#ifndef TTMPARAMETERsetBSQ_H
#define TTMPARAMETERsetBSQ_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMParametersetBSQ                                                   //
//                                                                      //
// The parameter set used when Baryon Content, Strangeness, Charge,     //
// Charm & Beauty are to be treated grand-canonically:                  //
//                                                                      //
//                                                       4               //
//                                                                      //
//   The initial strangeness density may be used to constrain muS.      //
//   The ratio Baryon/(2*Charge) may be used to constrain muQ.          //
//   The initial charm density may be used to constrain muC.            //
//   The initial beauty density may be used to constrain mub.           //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "parasel.h"
#include "TTMParameterSet.h"

class TTMParameterSetBSQ : public TTMParameterSet
{

public:


    TTMParameterSetBSQ(double temp, double mub,
                       double mus, double muq, double gs,
                       double r = 0., double muc = 0., double gc = 1., double mubeauty = 0., double gb = 1.,
                       double b2q = 0.,
                       double s = 0., double c = 0., double beauty = 0., double temp_error = 0.,
                       double mub_error = 0., double mus_error = 0.,
                       double muq_error = 0., double gs_error = 0.,
                       double r_error = 0., double muc_error = 0., double gc_error = 0.,
                       double mubeauty_error = 0., double gb_error = 0.);
    TTMParameterSetBSQ();
    ~TTMParameterSetBSQ() { }

    void   constrain(ParameterType type, double x = 0.);
    void   fit(ParameterType type, double start, double min = 0.050, double max = 0.180, double step = 0.001) { mPar[type]->fit(start, min, max, step); }
    void   fix(ParameterType type, double value, double error = 0.) { mPar[type]->fix(value, error); }
//    double get(ParameterType type) const                            { return mPar[type]->getValue(); }
//    double getB2Q() const                                           {return mB2Q;}
    bool   getConstrain(ParameterType type) const                   { return mConstrain[type]; }
    double getDens(ParameterType type) const                        { return mDens[type]; }
//    double getRadius() const                                        {return mPar[kRADIUS]->getValue();}
    void   list();
//    void   set(ParameterType type, double x)                        {mPar[type]->setValue(x);}
//    void   setB2Q(double x)                                         {mB2Q = x;}

    TTMParameterSetBSQ& operator=(const TTMParameterSetBSQ& obj);

private:
    double  mDens[kPARTYPES];      // the initial density for S, C and Beauty
    bool    mConstrain[kPARTYPES]; // true if parameter must be constrained
};

#endif // TTMPARAMETERsetBSQ_H
