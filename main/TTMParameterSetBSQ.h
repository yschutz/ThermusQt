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
    Q_OBJECT

public:
    TTMParameterSetBSQ();
    TTMParameterSetBSQ(double temp, double mub,
                       double mus, double muq, double gs,
                       double r = 0., double muc = 0., double gc = 1., double mubeauty = 0., double gb = 1.,
                       double b2q = 0.,
                       double s = 0., double c = 0., double beauty = 0., double temp_error = 0.,
                       double mub_error = 0., double mus_error = 0.,
                       double muq_error = 0., double gs_error = 0.,
                       double r_error = 0., double muc_error = 0., double gc_error = 0.,
                       double mubeauty_error = 0., double gb_error = 0.);
    TTMParameterSetBSQ(const TTMParameterSetBSQ& set);

    void   constrain(ParameterType type, double x = 0.);
//    void   fit(ParameterType type, double start, double min = 0.050, double max = 0.180, double step = 0.001) { mPar[type]->fit(start, min, max, step); }
//    void   fix(ParameterType type, double value, double error = 0.) { mPar[type]->fix(value, error); }
    bool   getConstrain(ParameterType type) const;
    double getDens(ParameterType type) const;
    void   list() const;

    TTMParameterSetBSQ& operator=(const TTMParameterSetBSQ& obj);

private:
    double  mBeautyDens;           // the initial beauty density
    double  mCDens;                // the initial charm density
    double  mSDens;                // the initial strange density
    bool    mMuBeautyConstrain;    // true if muBeauty must be constrained
    bool    mMuCConstrain;         // true if muC must be constrained
    bool    mMuQConstrain;         // true if muQ must be constrained
    bool    mMuSConstrain;         // true if muS must be constrained
 };

#endif // TTMPARAMETERSETBSQ_H
