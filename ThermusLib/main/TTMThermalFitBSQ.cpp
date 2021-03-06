// TTMThermalFitBSQ						                            	//
//            	                                            		    //
// Fit class applying the grand canonical formalism                     //
//									                                    //
// Author: Spencer Wheaton 14 July 2004                                 //
// Adapted for Qt by Yves Schutz: October 2017                          //
//////////////////////////////////////////////////////////////////////////

#include "TTMThermalFitBSQ.h"
#include "TTMThermalModelBSQ.h"

//__________________________________________________________________________
TTMThermalFitBSQ::TTMThermalFitBSQ(QObject* parent) : TTMThermalFit(parent),
    mExclVol(false), mModel(nullptr), mParm(nullptr), mQStats(true), mWidth(true)
{
    mDescriptor = "GCanonical";
}

//__________________________________________________________________________
TTMThermalFitBSQ::TTMThermalFitBSQ(TTMParameterSetBSQ *par, QString file, QObject *parent) : TTMThermalFit(parent),
    mExclVol(false), mModel(nullptr),  mParm(par), mQStats(true), mWidth(true)
{
    mDescriptor = "GCanonical";
    inputExpYields(file);

}

//__________________________________________________________________________
TTMThermalModel *TTMThermalFitBSQ::generateThermalModel()
{
    if (mModel) {
        mModel->reset();
        mModel->setParameters(mParm);
        mModel->setQStats(mQStats);
        mModel->setWidth(mWidth);
        mModel->setExcludedVolume(mExclVol);
    } else
        mModel = new TTMThermalModelBSQ(mParm, mQStats, mWidth);

    return mModel;
}

//__________________________________________________________________________
TTMThermalFitBSQ::~TTMThermalFitBSQ()
{
    if (mModel)
        delete mModel;
}

