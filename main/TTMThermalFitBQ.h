//////////////////////////////////////////////////////////////////////////
//                                                                   	//
// TTMThermalFitBQ						                            	//
//            	                                            		    //
// Fit class applying the canonical treatment of strangeness with 	    //
// B and Q treated grand canonically					                //
//									                                    //
// Author: Spencer Wheaton 14 July 2004                                 //
// Adapted for Qt by Yves Schutz: September 2017                        //
//////////////////////////////////////////////////////////////////////////

#ifndef TTMTHERMALFITBQ_H
#define TTMTHERMALFITBQ_H

#include "main/TTMParameterSetBQ.h"
#include "main/TTMThermalFit.h"

class TTMThermalFitBQ : public TTMThermalFit
{
public:
    TTMThermalFitBQ(QObject* parent = nullptr);


protected:
    bool               mNonStrangeQStats; // ?
    TTMParameterSetBQ* mParm;             // the parameters set
    bool               mWidth;            // ?


};

#endif // TTMTHERMALFITBQ_H
