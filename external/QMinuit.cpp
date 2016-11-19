// derived from QMinuit by Yves Schutz November 2016

// @(#)root/minuit:$Id$
// Author: Rene Brun, Frederick James   12/08/95

/*************************************************************************
 * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/


#include <QDebug>
#include <QtMath>

#include "QMinuit.h"

const char charal[29] = " .ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//__________________________________________________________________________
QMinuit::QMinuit(const QMinuit &)
{
    // Private QMinuit copy ctor. QMinuit can not be copied

   qFatal("QMinuit can not copy construct QMinuit");
}

//__________________________________________________________________________
QMinuit::QMinuit(QObject *parent) : QObject(parent)
{
    setObjectName("MINUIT: The Minimization package");
    //preset all pointers to null
    mCpnam     = 0;
    mU         = 0;
    mAlim      = 0;
    mBlim      = 0;
    mPstar     = 0;
    mGin       = 0;
    mNvarl     = 0;
    mNiofex    = 0;

    mNexofi    = 0;
    mIpfix     = 0;
    mErp       = 0;
    mErn       = 0;
    mWerr      = 0;
    mGlobcc    = 0;
    mX         = 0;
    mXt        = 0;
    mDirin     = 0;
    mXs        = 0;
    mXts       = 0;
    mDirins    = 0;
    mGrd       = 0;
    mG2        = 0;
    mGstep     = 0;
    mDgrd      = 0;
    mGrds      = 0;
    mG2s       = 0;
    mGsteps    = 0;
    mPstst     = 0;
    mPbar      = 0;
    mPrho      = 0;
    mWord7     = 0;
    mVhmat     = 0;
    mVthmat    = 0;
    mP         = 0;
    mXpt       = 0;
    mYpt       = 0;
    mChpt      = 0;
    mCONTgcc   = 0;
    mCONTw     = 0;
    mFIXPyy    = 0;
    mGRADgf    = 0;
    mHESSyy    = 0;
    mIMPRdsav  = 0;
    mIMPRy     = 0;
    mMATUvline = 0;
    mMIGRflnu  = 0;
    mMIGRstep  = 0;
    mMIGRgs    = 0;
    mMIGRvg    = 0;
    mMIGRxxs   = 0;
    mMNOTxdev  = 0;
    mMNOTw     = 0;
    mMNOTgcc   = 0;
    mPSDFs     = 0;
    mSEEKxmid  = 0;
    mSEEKxbest = 0;
    mSIMPy     = 0;
    mVERTq     = 0;
    mVERTs     = 0;
    mVERTpp    = 0;
    mCOMDplist = 0;
    mPARSplist = 0;

    mUp        = 0;
    mEpsi      = 0;
    mApsi      = 0;
    mXmidcr    = 0;
    mYmidcr    = 0;
    mXdircr    = 0;
    mYdircr    = 0;

    mStatus       = 0;
    mEmpty        = 0;
    mObjectFit    = 0;
//    mMethodCall   = 0;
    mPlot         = 0;
    mGraphicsMode = true;

    BuildArrays(25);

    setMaxIterations();

    qmninit(5,6,7);

    fFCN = 0;

 //   gMinuit = this;
}

//__________________________________________________________________________
QMinuit::QMinuit(qint32 maxpar)
{
    // Minuit normal constructor
    // maxpar is the maximum number of parameters used with this QMinuit object

    fFCN = 0;

    BuildArrays(maxpar);

    mStatus       = 0;
    mEmpty        = 0;
    mObjectFit    = 0;
//    mMethodCall   = 0;
    mPlot         = 0;
    mGraphicsMode = true;

    setMaxIterations();

    qmninit(5,6,7);

//    gMinuit = this;
}

//__________________________________________________________________________
QMinuit::~QMinuit()
{
    // Minuit default destructor

    DeleteArrays();
    delete mPlot;
//    delete mMethodCall;
//    if (gMinuit == this)
//        gMinuit = nullptr;
}

//__________________________________________________________________________
void QMinuit::BuildArrays(qint32 maxpar)
{
    // Create internal Minuit arrays for the maxpar parameters*
    mMaxPar = 25;

    if (maxpar >= mMaxPar)
        mMaxPar = maxpar+1;

    mMaxPar1  = mMaxPar * ( mMaxPar + 1 );
    mMaxPar2  = 2 * mMaxPar;
    mMaxPar5  = mMaxPar1 /2 ;
    mMaxCptns = 101;

    mCpnam  = new QString[mMaxPar2];
    mU      = new double[mMaxPar2];
    mAlim   = new double[mMaxPar2];
    mBlim   = new double[mMaxPar2];
    mPstar  = new double[mMaxPar2];
    mGin    = new double[mMaxPar2];
    mNvarl  = new int[mMaxPar2];
    mNiofex = new int[mMaxPar2];

    mNexofi = new int[mMaxPar];
    mIpfix  = new int[mMaxPar];
    mErp    = new double[mMaxPar];
    mErn    = new double[mMaxPar];
    mWerr   = new double[mMaxPar];
    mGlobcc = new double[mMaxPar];
    mX      = new double[mMaxPar];
    mXt     = new double[mMaxPar];
    mDirin  = new double[mMaxPar];
    mXs     = new double[mMaxPar];
    mXts    = new double[mMaxPar];
    mDirins = new double[mMaxPar];
    mGrd    = new double[mMaxPar];
    mG2     = new double[mMaxPar];
    mGstep  = new double[mMaxPar];
    mDgrd   = new double[mMaxPar];
    mGrds   = new double[mMaxPar];
    mG2s    = new double[mMaxPar];
    mGsteps = new double[mMaxPar];
    mPstst  = new double[mMaxPar];
    mPbar   = new double[mMaxPar];
    mPrho   = new double[mMaxPar];
    mWord7  = new double[mMaxPar];
    mVhmat  = new double[mMaxPar5];
    mVthmat = new double[mMaxPar5];
    mP      = new double[mMaxPar1];
    mXpt    = new double[mMaxCptns];
    mYpt    = new double[mMaxCptns];
    mChpt   = new char[mMaxCptns+1];

    // initialisation of dynamic arrays used internally in some functions
    // these arrays had a fix dimension in Minuit

    mCONTgcc   = new double[mMaxPar];
    mCONTw     = new double[mMaxPar];
    mFIXPyy    = new double[mMaxPar];
    mGRADgf    = new double[mMaxPar];
    mHESSyy    = new double[mMaxPar];
    mIMPRdsav  = new double[mMaxPar];
    mIMPRy     = new double[mMaxPar];
    mMATUvline = new double[mMaxPar];
    mMIGRflnu  = new double[mMaxPar];
    mMIGRstep  = new double[mMaxPar];
    mMIGRgs    = new double[mMaxPar];
    mMIGRvg    = new double[mMaxPar];
    mMIGRxxs   = new double[mMaxPar];
    mMNOTxdev  = new double[mMaxPar];
    mMNOTw     = new double[mMaxPar];
    mMNOTgcc   = new double[mMaxPar];
    mPSDFs     = new double[mMaxPar];
    mSEEKxmid  = new double[mMaxPar];
    mSEEKxbest = new double[mMaxPar];
    mSIMPy     = new double[mMaxPar];
    mVERTq     = new double[mMaxPar];
    mVERTs     = new double[mMaxPar];
    mVERTpp    = new double[mMaxPar];
    mCOMDplist = new double[mMaxPar];
    mPARSplist = new double[mMaxPar];

    for (qint32 i = 0; i < mMaxPar; i++) {
       mErp[i] = 0;
       mErn[i] = 0;
    }
}

//__________________________________________________________________________
QObject *QMinuit::Clone(const char */*newname*/) const
{
    //  Make a clone of an object using the Streamer facility.
    // Function pointer is copied to Clone

    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED";
    qFatal("99");
//    QMinuit *named = (QMinuit*)TNamed::Clone(newname);
//     named->fFCN=fFCN;
//         return named;
}

//__________________________________________________________________________
qint32 QMinuit::Command(const char *command)
{
    //  execute a Minuit command
    //      Equivalent to MNEXCM except that the command is given as a
    //      character string.
    //  See QMinuit::mnhelp for the full list of available commands
    //  See also http://wwwasdoc.web.cern.ch/wwwasdoc/minuit/node18.html for
    //   a complete documentation of all the available commands
    //
    //  Returns the status of the execution:
    //    = 0: command executed normally
    //      1: command is blank, ignored
    //      2: command line unreadable, ignored
    //      3: unknown command, ignored
    //      4: abnormal termination (e.g., MIGRAD not converged)
    //      5: command is a request to read PARAMETER definitions
    //      6: 'SET INPUT' command
    //      7: 'SET TITLE' command
    //      8: 'SET COVAR' command
    //      9: reserved
    //     10: END command
    //     11: EXIT or STOP command
    //     12: RETURN command

    qint32 status = 0;
    qmncomd(command, status);
    return status;
}

//__________________________________________________________________________
QObject *QMinuit::Contour(qint32 npoints, qint32 pa1, qint32 pa2)
{
    //  Creates a TGraph object describing the n-sigma contour of a
    //  QMinuit fit. The contour of the parameters pa1 and pa2 is calculated
    //  unsing npoints (>=4) points. The QMinuit status will be
    //   0   on success and
    //  -1   if errors in the calling sequence (pa1, pa2 not variable)
    //   1   if less than four points can be found
    //   2   if npoints<4
    //   n>3 if only n points can be found (n < npoints)
    //  The status can be obtained via QMinuit::GetStatus().
    //
    //  To get the n-sigma contour the ERRDEF parameter in Minuit has to set
    //  to n^2. The fcn function has to be set before the routine is called.
    //
    //  The TGraph object is created via the interpreter. The user must cast it
    //  to a TGraph*. Note that the TGraph is created with npoints+1 in order to
    //  close the contour (setting last poqint32equal to first point).
    //
    //  You can find an example in $ROOTSYS/tutorials/fit/fitcont.C

    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED" << npoints << pa1 << pa2 ;
    qFatal("88");
//    if (npoints<4) {
//       // we need at least 4 points
//       mStatus= 2;
//       return (TObject *)0;
//    }
//    qint32   npfound;
//    double *xcoor = new double[npoints+1];
//    double *ycoor = new double[npoints+1];
//    qmncont(pa1,pa2,npoints,xcoor,ycoor,npfound);
//    if (npfound<4) {
//       // mncont did go wrong
//       Warning("Contour","Cannot find more than 4 points, no TGraph returned");
//       mStatus= (npfound==0 ? 1 : npfound);
//       delete [] xcoor;
//       delete [] ycoor;
//       return (TObject *)0;
//    }
//    if (npfound!=npoints) {
//       // mncont did go wrong
//       Warning("Contour","Returning a TGraph with %d points only",npfound);
//       npoints = npfound;
//    }
//    mStatus=0;
//    // create graph via the  PluginManager
//    xcoor[npoints] = xcoor[0];  // add first poqint32at end to get closed polyline
//    ycoor[npoints] = ycoor[0];
//    TObject *gr = 0;
//    TPluginHandler *h;
//    if ((h = gROOT->GetPluginManager()->FindHandler("QMinuitGraph"))) {
//       if (h->LoadPlugin() != -1)
//       gr = (TObject*)h->ExecPlugin(3,npoints+1,xcoor,ycoor);
//    }
//    delete [] xcoor;
//    delete [] ycoor;
//    return gr;
    qWarning() << Q_FUNC_INFO << "This is a root specific method";
    return NULL;
}

//__________________________________________________________________________
qint32 QMinuit::DefineParameter(qint32 parNo, const char *name, double initVal, double initErr, double lowerLimit, double upperLimit)
{
    // Define a parameter

    qint32 err;

    QString sname = name;
    qmNParm(parNo, sname, initVal, initErr, lowerLimit, upperLimit, err);

    return err;
}

//__________________________________________________________________________
void QMinuit::DeleteArrays()
{
    // Delete internal Minuit arrays*

    if (mEmpty)
        return;

    delete [] mCpnam;
    delete [] mU;
    delete [] mAlim;
    delete [] mBlim;
    delete [] mErp;
    delete [] mErn;
    delete [] mWerr;
    delete [] mGlobcc;
    delete [] mNvarl;
    delete [] mNiofex;
    delete [] mNexofi;
    delete [] mX;
    delete [] mXt;
    delete [] mDirin;
    delete [] mXs;
    delete [] mXts;
    delete [] mDirins;
    delete [] mGrd;
    delete [] mG2;
    delete [] mGstep;
    delete [] mGin;
    delete [] mDgrd;
    delete [] mGrds;
    delete [] mG2s;
    delete [] mGsteps;
    delete [] mIpfix;
    delete [] mVhmat;
    delete [] mVthmat;
    delete [] mP;
    delete [] mPstar;
    delete [] mPstst;
    delete [] mPbar;
    delete [] mPrho;
    delete [] mWord7;
    delete [] mXpt;
    delete [] mYpt;
    delete [] mChpt;
    delete [] mCONTgcc;
    delete [] mCONTw;
    delete [] mFIXPyy;
    delete [] mGRADgf;
    delete [] mHESSyy;
    delete [] mIMPRdsav;
    delete [] mIMPRy;
    delete [] mMATUvline;
    delete [] mMIGRflnu;
    delete [] mMIGRstep;
    delete [] mMIGRgs;
    delete [] mMIGRvg;
    delete [] mMIGRxxs;
    delete [] mMNOTxdev;
    delete [] mMNOTw;
    delete [] mMNOTgcc;
    delete [] mPSDFs;
    delete [] mSEEKxmid;
    delete [] mSEEKxbest;
    delete [] mSIMPy;
    delete [] mVERTq;
    delete [] mVERTs;
    delete [] mVERTpp;
    delete [] mCOMDplist;
    delete [] mPARSplist;

    mEmpty = 1;
}

//__________________________________________________________________________
qint32 QMinuit::Eval(qint32 npar, double *grad, double &fval, double *par, qint32 flag)
{
    // Evaluate the minimisation function
    //   Input parameters:
    //     npar:    number of currently variable parameters
    //     par:     array of (constant and variable) parameters
    //     flag:    Indicates what is to be calculated (see example below)
    //     grad:    array of gradients
    //   Output parameters:
    //     fval:    The calculated function value.
    //     grad:    The (optional) vector of first derivatives).
    //
    //  The meaning of the parameters par is of course defined by the user,
    //  who uses the values of those parameters to calculate their function value.
    //  The starting values must be specified by the user.
    //  Later values are determined by Minuit as it searches for the minimum
    //  or performs whatever analysis is requested by the user.
    //
    //  Note that this virtual function may be redefined in a class derived from QMinuit.
    //  The default function calls the function specified in SetFCN
    //
    //  Example of Minimisation function:

    /*
       if (flag == 1) {
          read input data,
          calculate any necessary constants, etc.
       }
       if (flag == 2) {
          calculate GRAD, the first derivatives of FVAL
         (this is optional)
       }
       Always calculate the value of the function, FVAL,
       which is usually a chisquare or log likelihood.
       if (iflag == 3) {
          will come here only after the fit is finished.
          Perform any final calculations, output fitted data, etc.
       }
    */
    //  See concrete examples in TH1::H1FitChisquare, H1FitLikelihood

    if (fFCN) (*fFCN)(npar,grad,fval,par,flag);
    return 0;
}

//__________________________________________________________________________
qint32 QMinuit::FixParameter( qint32 parNo)
{
    // fix a parameter
    qint32 err;
    double tmp[1];
    tmp[0] = parNo + 1; //set internal Minuit numbering

    qmnexcm( "FIX",tmp, 1, err );

    return err;
}

//__________________________________________________________________________
qint32 QMinuit::GetParameter( qint32 parNo, double &currentValue, double &currentError ) const
{
    // return parameter value and error
    qint32   err;
    QString  name; // ignored
    double bnd1, bnd2; // ignored

    qmnpout( parNo, name, currentValue, currentError, bnd1, bnd2, err );

    return err;
}

//__________________________________________________________________________
qint32 QMinuit::Migrad()
{
    // invokes the MIGRAD minimizer
    qint32 err;
    double tmp[1];
    tmp[0] = 0;

    qmnexcm( "MIGRAD", tmp, 0, err );

    return err;
}

//__________________________________________________________________________
void QMinuit::qmnamin()
{
    // *-*-*-*-*-*-*-*-*-*-*-*-*Initialize AMIN*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
    //        Called  from many places.  Initializes the value of AMIN by
    //       calling the user function. Prints out the function value and
    //      parameter values if Prqint32Flag value is high enough.

    /* Local variables */
    double fnew;

    if (mISW[4] >= 1) {
       qInfo() << Q_FUNC_INFO << "INFO: FIRST CALL TO USER FUNCTION AT NEW START POINT, WITH IFLAG=4.";
    }
    qmnexin(mX);
    Eval(mNPar, mGin, fnew, mU, 4);
    ++mNfcn;
    mAmin = fnew;
    mEDM  = mBigedm;
}

//__________________________________________________________________________
void QMinuit::qmnbins(double a1, double a2, qint32 naa, double &bl, double &bh, qint32 &nb, double &bwid)
{
    // *-*-*-*-*-*-*Compute reasonable histogram intervals*-*-*-*-*-*-*-*-*
    //              ======================================
    //        Function TO DETERMINE REASONABLE HISTOGRAM INTERVALS
    //        GIVEN ABSOLUTE UPPER AND LOWER BOUNDS  A1 AND A2
    //        AND DESIRED MAXIMUM NUMBER OF BINS NAA
    //        PROGRAM MAKES REASONABLE BINNING FROM BL TO BH OF WIDTH BWID
    //        F. JAMES,   AUGUST, 1974 , stolen for Minuit, 1988
    /* Local variables */
    double awid, ah, al, sigfig,  sigrnd, alb;
    qint32 kwid, lwid, na = 0, log_;

    al = qMin(a1, a2);
    ah = qMax(a1, a2);
    if (al == ah) ah = al + 1;

 //*-*-       IF NAA .EQ. -1 , PROGRAM USES BWID INPUT FROM CALLING ROUTINE
    if (naa == -1) goto L150;
 L10:
    na = naa - 1;
    if (na < 1) na = 1;

 //*-*-        GET NOMINAL BIN WIDTH IN EXPON FORM
 L20:
    awid = (ah - al) / double(na);
    log_ = int(qLn(awid) / qLn(10.)); //   TMath::Log10(awid));
    if (awid <= 1) --log_;
    sigfig = awid * qPow(10, -log_);
 //*-*-       ROUND MANTISSA UP TO 2, 2.5, 5, OR 10
    if (sigfig > 2) goto L40;
    sigrnd = 2;
    goto L100;
 L40:
    if (sigfig > 2.5) goto L50;
    sigrnd = 2.5;
    goto L100;
 L50:
    if (sigfig > 5) goto L60;
    sigrnd = 5;
    goto L100;
 L60:
    sigrnd = 1;
    ++log_;
 L100:
    bwid = sigrnd * qPow(10, log_);
    goto L200;
 //*-*-       GET NEW BOUNDS FROM NEW WIDTH BWID
 L150:
    if (bwid <= 0) goto L10;
 L200:
    alb  = al / bwid;
    lwid = int(alb);
    if (alb < 0) --lwid;
    bl   = bwid * double(lwid);
    alb  = ah / bwid + 1;
    kwid = int(alb);
    if (alb < 0) --kwid;
    bh = bwid * double(kwid);
    nb = kwid - lwid;
    if (naa > 5) goto L240;
    if (naa == -1) return;
 //*-*-        REQUEST FOR ONE BIN IS DIFFICULT CASE
    if (naa > 1 || nb == 1) return;
    bwid *= 2;
    nb = 1;
    return;
 L240:
    if (nb << 1 != naa) return;
    ++na;
    goto L20;
}

//__________________________________________________________________________
void QMinuit::qmncalf(double *pvec, double &ycalf)
{
    //*-*-*-*-*-*-*-*-*-*Transform FCN to find further minima*-*-*-*-*-*-*-*-*-*
    //*-*                ====================================
    //*-*        Called only from MNIMPR.  Transforms the function FCN
    //*-*        by dividing out the quadratic part in order to find further
    //*-*        minima.    Calculates  ycalf = (f-fmin)/(x-xmin)*v*(x-xmin)


    qmninex(&pvec[0]);

    double f;

    Eval(mNPar, mGin, f, mU, 4);    ++mNfcn;

    for (qint32 i = 1; i <= mNPar; ++i) {
       mGrd[i-1] = 0;
       for (qint32 j = 1; j <= mNPar; ++j) {
          qint32 m = qMax(i, j);
          qint32 n = qMin(i, j);
          qint32 ndex = m * (m - 1) / 2 + n;
          mGrd[i - 1] += mVthmat[ndex - 1] * (mXt[j - 1] - pvec[j - 1]);
       }
    }

    double denom = 0.0;
    for (qint32 i = 1; i <= mNPar; ++i)
        denom += mGrd[i - 1] * (mXt[i - 1] - pvec[i - 1]);
    if (denom <= 0) {
       mDcovar = 1;
       mISW[1] = 0;
       denom   = 1.;
    }
    ycalf = (f - mApsi) / denom;
}

//__________________________________________________________________________
void QMinuit::qmncler()
{
    //*-*-*-*-*-*-*-*-*-*-*Resets the parameter list to UNDEFINED*-*-*-*-*-*-*-*
    //*-*                  ======================================
    //*-*        Called from MINUIT and by option from MNEXCM

    mNPFix = 0;
    mNu    = 0;
    mNPar  = 0;
    mNfcn  = 0;

    mNwrmes[0] = 0;
    mNwrmes[1] = 0;

    for (qint32 i = 1; i <= mMaxExtr; ++i) {
       mU[i-1]      = 0;
       mCpnam[i-1]  = mCundef;
       mNvarl[i-1]  = -1;
       mNiofex[i-1] = 0;
    }
    qmnrset(1);
    mCfrom  = "CLEAR   ";
    mNfcnfr = mNfcn;
    mCstatu = "UNDEFINED ";
    mLnolim = true;
    mLphead = true;
}


//__________________________________________________________________________
void QMinuit::qmncntr(qint32 ike1, qint32 ike2, qint32 &ierrf)
{

    //*-*-*-*-*Printfunction contours in two variables, on line printer*-*-*-*-*
    //*-*      =========================================================
    //*-*
    //*-*                input arguments: parx, pary, devs, ngrid
    //*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

    static QString clabel = "0123456789ABCDEFGHIJ";

    /* Local variables */
    double d__1, d__2;
    double fcna[115], fcnb[115], contur[20];
    double  ylabel, fmn, fmx, xlo, ylo, xup, yup;
    double devs, xsav, ysav,  bwidx,  bwidy, unext, ff, xb4;
    qint32 ngrid, ixmid, nparx, nx, ny, ki1, ki2, ixzero, ics;
    QString chmid, chln, chzero;

    qint32 ke1 = ike1 + 1;
    qint32 ke2 = ike2 + 1;
    if (ke1 <= 0 || ke2 <= 0)   goto L1350;
    if (ke1 > mNu || ke2 > mNu) goto L1350;
    ki1 = mNiofex[ke1 - 1];
    ki2 = mNiofex[ke2 - 1];
    if (ki1 <= 0 || ki2 <= 0) goto L1350;
    if (ki1 == ki2)           goto L1350;

    if (mISW[1] < 1) {
       qmnhess();
       qmnwerr();
    }
    nparx = mNPar;
    xsav = mU[ke1 - 1];
    ysav = mU[ke2 - 1];
    devs = mWord7[2];
    if (devs <= 0) devs = 2;
    xlo = mU[ke1 - 1] - devs*mWerr[ki1 - 1];
    xup = mU[ke1 - 1] + devs*mWerr[ki1 - 1];
    ylo = mU[ke2 - 1] - devs*mWerr[ki2 - 1];
    yup = mU[ke2 - 1] + devs*mWerr[ki2 - 1];
    ngrid = int(mWord7[3]);
    if (ngrid <= 0) {
       ngrid = 25;
 //*-*  Computing MIN
       nx = qMin(mNpagwd - 15, ngrid);
 //*-*  Computing MIN
       ny = qMin(mNpagln - 7, ngrid);
    } else {
       nx = ngrid;
       ny = ngrid;
    }
    if (nx < 11)   nx = 11;
    if (ny < 11)   ny = 11;
    if (nx >= 115) nx = 114;

 //*-*-        ask if parameter outside limits
    if (mNvarl[ke1-1] > 1) {
       if (xlo < mAlim[ke1 - 1]) xlo = mAlim[ke1 - 1];
       if (xup > mBlim[ke1 - 1]) xup = mBlim[ke1 - 1];
    }
    if (mNvarl[ke2-1] > 1) {
       if (ylo < mAlim[ke2 - 1]) ylo = mAlim[ke2 - 1];
       if (yup > mBlim[ke2 - 1]) yup = mBlim[ke2 - 1];
    }
    bwidx = (xup - xlo) / double(nx);
    bwidy = (yup - ylo) / double(ny);
    ixmid = int(((xsav - xlo) * double(nx) / (xup - xlo)) + 1);
    if (ixmid < 1) ixmid = 1;
    if (mAmin == mUndefi) qmnamin();

    for (qint32 i = 1; i <= 20; ++i)
        contur[i - 1] = mAmin + mUp * (i - 1)*(i - 1);
    contur[0] += mUp * 0.01;
 //*-*-               fill FCNB to prepare first row, and find column zero/
    mU[ke2 - 1] = yup;
    ixzero = 0;
    xb4 = 1;
 //TH
    chmid.resize(nx + 1);
    chzero.resize(nx + 1);
    chln.resize(nx + 1);
    for (qint32 ix = 1; ix <= nx + 1; ++ix) {
       mU[ke1 - 1] = xlo + double(ix - 1) * bwidx;
       Eval(nparx, mGin, ff, mU, 4);
       fcnb[ix-1] = ff;
       if (xb4 < 0 && mU[ke1 - 1] > 0) ixzero = ix - 1;
       xb4            = mU[ke1-1];
       chmid[ix - 1]  = '*';
       chzero[ix - 1] = '-';
    }
    qInfo() << Q_FUNC_INFO << " Y-AXIS: PARAMETER:" << ke2 << mCpnam[ke2-1];
    if (ixzero > 0) {
       chzero[ixzero-1] = '+';
       chln = " ";
       qInfo() << Q_FUNC_INFO << "             X=0";
    }
 //*-*-                loop over rows
    for (qint32 iy = 1; iy <= ny; ++iy) {
       unext = mU[ke2-1] - bwidy;
 //*-*-                prepare this line background pattern for contour
       chln = " ";
 // TH
       chln.resize(nx + 1);
       chln[ixmid - 1] = '*';
       if (ixzero != 0) chln[ixzero - 1] = ':';
       if (mU[ke2 - 1] > ysav && unext < ysav) chln = chmid;
       if (mU[ke2 -1 ] > 0 && unext < 0)       chln = chzero;
       mU[ke2 - 1] = unext;
       ylabel = mU[ke2 - 1] + bwidy * .5;
 //*-*-                move FCNB to FCNA and fill FCNB with next row
       for (qint32 ix = 1; ix <= nx + 1; ++ix) {
          fcna[ix - 1] = fcnb[ix - 1];
          mU[ke1 - 1] = xlo + double(ix - 1) * bwidx;
          Eval(nparx, mGin, ff, mU, 4);
          fcnb[ix - 1] = ff;
       }
 //*-*-                look for contours crossing the FCNxy squares
       for (qint32 ix = 1; ix <= nx; ++ix) {
          d__1 = qMax(fcna[ix - 1], fcnb[ix - 1]),
          d__2 = qMax(fcna[ix], fcnb[ix]);
          fmx  = qMax(d__1, d__2);
          d__1 = qMin(fcna[ix - 1], fcnb[ix - 1]),
          d__2 = qMin(fcna[ix], fcnb[ix]);
          fmn  = qMin(d__1, d__2);
          for (ics = 1; ics <= 20; ++ics) {
             if (contur[ics - 1] > fmn)  goto L240;
          }
          continue;
 L240:
          if (contur[ics - 1] < fmx) chln[ix - 1] = clabel[ics - 1];
       }
 //*-*-                prqint32a row of the contour plot
       qInfo() << ylabel  << chln;
    }
 //*-*-                contours printed, label x-axis
    chln            = " ";
    chln.insert(1, 'I');
    chln.insert(ixmid - 1,'I');
    chln.insert(nx - 1,'I');
    qInfo() << QString("              %1").arg(chln);

 //*-*-               the hardest of all: print x-axis scale!
    chln =  " ";
    if (nx <= 26) {
       qInfo() << QString("        %1 %2 %3").arg(xlo).arg(chln).arg(xup);
       qInfo() << QString("              %1 %2").arg(chln).arg(xsav);
    } else {
       qInfo() << QString("        %1 %2 %2 %3 %4").arg(xlo).arg(chln).arg(xsav).arg(chln).arg(xup);
    }
    qInfo() << QString("       X-AXIS: PARAMETER %1 %2  ONE COLUMN=%3").arg(ke1).arg(mCpnam[ke1-1]).arg(bwidx);
    qInfo() << QString(" FUNCTION VALUES: F(I)=%1 + %2 *I**2").arg(mAmin).arg(mUp);
 //*-*-                finished.  reset input values
    mU[ke1-1] = xsav;
    mU[ke2-1] = ysav;
    ierrf     = 0;
    return;
 L1350:
    qInfo() << Q_FUNC_INFO << " INVALID PARAMETER NUMBER(S) REQUESTED.  IGNORED.";
    ierrf = 1;
}

//__________________________________________________________________________
void QMinuit::qmncomd(const char *crdbin, qint32&icondn)
{
    //-*-*-*-*-*-*-*-*-*-*Reads a command string and executes*-*-*-*-*-*-*-*-*-*
    // *-*                  ===================================
    // *-*        Called by user.  'Reads' a command string and executes.
    // *-*     Equivalent to MNEXCM except that the command is given as a
    // *-*          character string.
    // *-*
    // *-*     ICONDN = 0: command executed normally
    // *-*              1: command is blank, ignored
    // *-*              2: command line unreadable, ignored
    // *-*              3: unknown command, ignored
    // *-*              4: abnormal termination (e.g., MIGRAD not converged)
    // *-*              5: command is a request to read PARAMETER definitions
    // *-*              6: 'SET INPUT' command
    // *-*              7: 'SET TITLE' command
    // *-*              8: 'SET COVAR' command
    // *-*              9: reserved
    // *-*             10: END command
    // *-*             11: EXIT or STOP command
    // *-*             12: RETURN command

    qint32 ierr, ipos, llist, lenbuf, lnc;
    bool leader;
    QString comand, crdbuf, ctemp;

    crdbuf = crdbin;
    crdbuf.toUpper();
    lenbuf = crdbuf.size();
    icondn = 0;
    //*-*-    record not case-sensitive, get upper case, strip leading blanks
    leader = true;
    ipos = 1;
    for (qint32 i = 1; i <= qMin(20,lenbuf); ++i) {
        if (crdbuf[i - 1] == '\'') break;
        if (crdbuf[i - 1] == ' ') {
            if (leader) ++ipos;
            continue;
        }
        leader = false;
    }

    //*-*-                    blank or null command
    if (ipos > lenbuf) {
        qInfo() << Q_FUNC_INFO << " BLANK COMMAND IGNORED.";
        icondn = 1;
        return;
    }
    //*-*-                                          . .   preemptive commands
    //*-*-              if command is 'PARAMETER'
    //    if (crdbuf(ipos-1,3) == "PAR") {
    if (crdbuf.left(3) == "PAR") {
        icondn  = 5;
        mLphead = true;
        return;
    }
    //*-*-              if command is 'SET INPUT'
    //        if (crdbuf(ipos-1,3) == "SET INP") {
    if (crdbuf.left(3) == "SET INP") {
        icondn  = 6;
        mLphead = true;
        return;
    }
    //*-*-              if command is 'SET TITLE'
    //    if (crdbuf(ipos-1,7) == "SET TIT") {
    if (crdbuf.left(7) == "SET TIT") {
            icondn  = 7;
            mLphead = true;
            return;
        }
        //*-*-              if command is 'SET COVARIANCE'
        //    if (crdbuf(ipos-1,7) == "SET COV") {
        if (crdbuf.left(7) == "SET COV") {
            icondn  = 8;
            mLphead = true;
            return;
        }
        //*-*-              crack the command . . . . . . . . . . . . . . . .
        //        ctemp = crdbuf(ipos-1,lenbuf-ipos+1);
        ctemp = crdbuf.left(lenbuf-ipos+1);
        qmncrck(ctemp, 20, comand, lnc, mMaxPar, mCOMDplist, llist, ierr, mIsyswr);
        if (ierr > 0) {
            qInfo() << Q_FUNC_INFO << " COMMAND CANNOT BE INTERPRETED";
            icondn = 2;
            return;
        }

        qmnexcm(comand.toStdString().data(), mCOMDplist, llist, ierr);
        icondn = ierr;
}

//__________________________________________________________________________
void QMinuit::qmncont(qint32 ike1, qint32 ike2, qint32 nptu, double *xptu, double *yptu, qint32 &ierrf)
{
    // *-*-*-*-*-*-*Find points along a contour where FCN is minimum*-*-*-*-*-*-*
    // *-*          ================================================
    // *-*       Find NPTU points along a contour where the function
    // *-*             FMIN (X(KE1),X(KE2)) =  AMIN+UP
    // *-*       where FMIN is the minimum of FCN with respect to all
    // *-*       the other NPAR-2 variable parameters (if any).
    // *-*   IERRF on return will be equal to the number of points found:
    // *-*     NPTU if normal termination with NPTU points found
    // *-*     -1   if errors in the calling sequence (KE1, KE2 not variable)
    // *-*      0   if less than four points can be found (using MNMNOT)
    // *-*     n>3  if only n points can be found (n < NPTU)
    // *-*
    // *-*                 input arguments: parx, pary, devs, ngrid

    qint32 i__1;

     /* Local variables */
     double d__1, d__2;
     double dist, xdir, ydir, aopt,  u1min, u2min;
     double abest, scalx, scaly;
     double a1, a2, val2mi, val2pl, dc, sclfac, bigdis, sigsav;
     qint32 nall, iold, line, mpar, ierr, inew, move, next, i, j, nfcol, iercr;
     qint32 idist=0, npcol, kints, i2, i1, lr, nfcnco=0, ki1, ki2, ki3, ke3;
     qint32 nowpts, istrav, nfmxin, isw2, isw4;
     bool ldebug;

     /* Function Body */
     qint32 ke1 = ike1 + 1;
     qint32 ke2 = ike2 + 1;
     ldebug = mIdbg[6] >= 1;
     if (ke1 <= 0 || ke2 <= 0)   goto L1350;
     if (ke1 > mNu || ke2 > mNu) goto L1350;
     ki1 = mNiofex[ke1 - 1];
     ki2 = mNiofex[ke2 - 1];
     if (ki1 <= 0 || ki2 <= 0) goto L1350;
     if (ki1 == ki2)           goto L1350;
     if (nptu < 4)             goto L1400;

     nfcnco  = mNfcn;
     mNfcnmx = (nptu + 5) * 100 * (mNPar + 1);
  //*-*-          The minimum
     qmncuve();
     u1min  = mU[ke1 - 1];
     u2min  = mU[ke2 - 1];
     ierrf  = 0;
     mCfrom = "MNContour ";
     mNfcnfr = nfcnco;
     if (mISW[4] >= 0) {
        qInfo() << Q_FUNC_INFO << " START MNCONTOUR CALCULATION OF" << nptu << "POINTS ON CONTOUR.";
        if (mNPar > 2) {
           if (mNPar == 3) {
              ki3 = 6 - ki1 - ki2;
              ke3 = mNexofi[ki3 - 1];
              qInfo() << " EACH Point IS A MINIMUM WITH RESPECT TO PARAMETER" << ke3 << mCpnam[ke3-1];
           } else {
              qInfo() << " EACH Point IS A MINIMUM WITH RESPECT TO THE OTHER" << mNPar - 2 << "VARIABLE PARAMETERS.";
           }
        }
     }

  //*-*-          Find the first four points using MNMNOT
  //*-*-             ........................ first two points
     qmnmnot(ke1, ke2, val2pl, val2mi);
     if (mErn[ki1-1] == mUndefi) {
        xptu[0] = mAlim[ke1-1];
        qmnwarn("W", "MNContour ", "Contour squeezed by parameter limits.");
     } else {
        if (mErn[ki1-1] >= 0) goto L1500;
        xptu[0] = u1min + mErn[ki1-1];
     }
     yptu[0] = val2mi;

     if (mErp[ki1-1] == mUndefi) {
        xptu[2] = mBlim[ke1-1];
        qmnwarn("W", "MNContour ", "Contour squeezed by parameter limits.");
     } else {
        if (mErp[ki1-1] <= 0) goto L1500;
        xptu[2] = u1min + mErp[ki1-1];
     }
     yptu[2] = val2pl;
     scalx = 1 / (xptu[2] - xptu[0]);
  //*-*-             ........................... next two points
     qmnmnot(ke2, ke1, val2pl, val2mi);
     if (mErn[ki2-1] == mUndefi) {
        yptu[1] = mAlim[ke2-1];
        qmnwarn("W", "MNContour ", "Contour squeezed by parameter limits.");
     } else {
        if (mErn[ki2-1] >= 0) goto L1500;
        yptu[1] = u2min + mErn[ki2-1];
     }
     xptu[1] = val2mi;
     if (mErp[ki2-1] == mUndefi) {
        yptu[3] = mBlim[ke2-1];
        qmnwarn("W", "MNContour ", "Contour squeezed by parameter limits.");
     } else {
        if (mErp[ki2-1] <= 0) goto L1500;
        yptu[3] = u2min + mErp[ki2-1];
     }
     xptu[3] = val2pl;
     scaly   = 1 / (yptu[3] - yptu[1]);
     nowpts  = 4;
     next    = 5;
     if (ldebug) {
        qInfo() << " Plot of four points found by MINOS";
        mXpt[0]  = u1min;
        mYpt[0]  = u2min;
        mChpt[0] = ' ';
  //*-*  Computing MIN
        nall = qMin(nowpts + 1,101);
        for (i = 2; i <= nall; ++i) {
           mXpt[i-1] = xptu[i-2];
           mYpt[i-1] = yptu[i-2];
        }
        sprintf(mChpt,"%s"," ABCD");
        qmnplot(mXpt, mYpt, mChpt, nall, mNpagwd, mNpagln);
     }

  //*-*-              ..................... save some values before fixing
     isw2   = mISW[1];
     isw4   = mISW[3];
     sigsav = mEDM;
     istrav = mIstrat;
     dc     = mDcovar;
     mApsi  = mEpsi*.5;
     abest  = mAmin;
     mpar   = mNPar;
     nfmxin = mNfcnmx;
     for (i = 1; i <= mpar; ++i) { mXt[i-1] = mX[i-1]; }
     i__1 = mpar*(mpar + 1) / 2;
     for (j = 1; j <= i__1; ++j) { mVthmat[j-1] = mVhmat[j-1]; }
     for (i = 1; i <= mpar; ++i) {
        mCONTgcc[i-1] = mGlobcc[i-1];
        mCONTw[i-1]   = mWerr[i-1];
     }
  //*-*-                          fix the two parameters in question
     kints = mNiofex[ke1-1];
     qmnfixp(kints-1, ierr);
     kints = mNiofex[ke2-1];
     qmnfixp(kints-1, ierr);
  //*-*-              ......................Fill in the rest of the points
     for (inew = next; inew <= nptu; ++inew) {
  //*-*            find the two neighbouring points with largest separation
        bigdis = 0;
        for (iold = 1; iold <= inew - 1; ++iold) {
           i2 = iold + 1;
           if (i2 == inew) i2 = 1;
           d__1 = scalx*(xptu[iold-1] - xptu[i2-1]);
           d__2 = scaly*(yptu[iold-1] - yptu[i2-1]);
           dist = d__1*d__1 + d__2*d__2;
           if (dist > bigdis) {
              bigdis = dist;
              idist  = iold;
           }
        }
        i1 = idist;
        i2 = i1 + 1;
        if (i2 == inew) i2 = 1;
  //*-*-                  next point goes between I1 and I2
        a1 = .5;
        a2 = .5;
  L300:
        mXmidcr = a1*xptu[i1-1] + a2*xptu[i2-1];
        mYmidcr = a1*yptu[i1-1] + a2*yptu[i2-1];
        xdir    = yptu[i2-1] - yptu[i1-1];
        ydir    = xptu[i1-1] - xptu[i2-1];
        sclfac  = qMax(qAbs(xdir*scalx),qAbs(ydir*scaly));
        mXdircr = xdir / sclfac;
        mYdircr = ydir / sclfac;
        mKe1cr  = ke1;
        mKe2cr  = ke2;
  //*-*-               Find the contour crossing poqint32along DIR
        mAmin = abest;
        qmncros(aopt, iercr);
        if (iercr > 1) {
  //*-*-             If cannot find mid-point, try closer to poqint321
           if (a1 > .5) {
              if (mISW[4] >= 0) {
                 qInfo() << " MNCONT CANNOT FIND NEXT POqint32ON CONTOUR.  ONLY" << nowpts << "POINTS FOUND.";
              }
              goto L950;
           }
           qmnwarn("W", "MNContour ", "Cannot find midpoint, try closer.");
           a1 = .75;
           a2 = .25;
           goto L300;
        }
  //*-*-               Contour has been located, insert new poqint32in list
        for (move = nowpts; move >= i1 + 1; --move) {
           xptu[move] = xptu[move-1];
           yptu[move] = yptu[move-1];
        }
        ++nowpts;
        xptu[i1] = mXmidcr + mXdircr*aopt;
        yptu[i1] = mYmidcr + mYdircr*aopt;
     }
  L950:

     ierrf = nowpts;
     mCstatu = "SUCCESSFUL";
     if (nowpts < nptu)         mCstatu = "INCOMPLETE";

  //*-*-               make a lineprinter plot of the contour
     if (mISW[4] >= 0) {
        mXpt[0]  = u1min;
        mYpt[0]  = u2min;
        mChpt[0] = ' ';
        nall = qMin(nowpts + 1,101);
        for (i = 2; i <= nall; ++i) {
           mXpt[i-1]  = xptu[i-2];
           mYpt[i-1]  = yptu[i-2];
           mChpt[i-1] = 'X';
        }
        mChpt[nall] = 0;
        qInfo() << " Y-AXIS: PARAMETER" << ke2 << mCpnam[ke2-1];

        qmnplot(mXpt, mYpt, mChpt, nall, mNpagwd, mNpagln);

        qInfo() << "                         X-AXIS: PARAMETER" << ke1 << mCpnam[ke1-1];
     }
  //*-*-                prqint32out the coordinates around the contour
     if (mISW[4] >= 1) {
        npcol = (nowpts + 1) / 2;
        nfcol = nowpts / 2;
        qInfo() << nowpts << " POINTS ON CONTOUR.   FMIN=" << abest << "ERRDEF="<< mUp;
        qInfo() << "         " << mCpnam[ke1-1] << mCpnam[ke2-1] << mCpnam[ke1-1] << mCpnam[ke2-1];
        for (line = 1; line <= nfcol; ++line) {
           lr = line + npcol;
           qInfo() <<  line << xptu[line-1] << yptu[line-1] << "          "<< lr << xptu[lr-1] << yptu[lr-1];
        }
        if (nfcol < npcol) {
           qInfo() << npcol << xptu[npcol-1] << yptu[npcol-1];
        }
     }
  //*-*-                                   . . contour finished. reset v
     mItaur = 1;
     qmnfree(1);
     qmnfree(1);
     i__1 = mpar*(mpar + 1) / 2;
     for (j = 1; j <= i__1; ++j) { mVhmat[j-1] = mVthmat[j-1]; }
     for (i = 1; i <= mpar; ++i) {
        mGlobcc[i-1] = mCONTgcc[i-1];
        mWerr[i-1]   = mCONTw[i-1];
        mX[i-1]      = mXt[i-1];
     }
     qmninex(mX);
     mEDM    = sigsav;
     mAmin   = abest;
     mISW[1] = isw2;
     mISW[3] = isw4;
     mDcovar = dc;
     mItaur  = 0;
     mNfcnmx = nfmxin;
     mIstrat = istrav;
     mU[ke1-1] = u1min;
     mU[ke2-1] = u2min;
     goto L2000;
  //*-*-                                    Error returns
  L1350:
     qInfo() << " INVALID PARAMETER NUMBERS.";
     goto L1450;
  L1400:
     qInfo() << " LESS THAN FOUR POINTS REQUESTED.";
  L1450:
     ierrf   = -1;
     mCstatu = "USER ERROR";
     goto L2000;
  L1500:
     qInfo() << " MNCONT UNABLE TO FIND FOUR POINTS.";
     mU[ke1-1] = u1min;
     mU[ke2-1] = u2min;
     ierrf     = 0;
     mCstatu   = "FAILED";
  L2000:
     mCfrom  = "MNContour ";
     mNfcnfr = nfcnco;

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*-*-*Cracks the free-format input*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*                    ============================
///*-*       Cracks the free-format input, expecting zero or more
///*-*         alphanumeric fields (which it joins into COMAND(1:LNC))
///*-*         followed by one or more numeric fields separated by
///*-*         blanks and/or one comma.  The numeric fields are put into
///*-*         the LLIST (but at most MXP) elements of PLIST.
///*-*      IERR = 0 if no errors,
///*-*           = 1 if error(s).
///*-*
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmncrck(QString cardbuf, qint32 maxcwd, QString &comand, qint32 &lnc, qint32 mxp, double *plist, qint32 &llist, qint32 &ierr, qint32 /*isyswr*/)
{
    /* Initialized data */

    char *cnull  = 0;
    const char *cnumer = "123456789-.0+";

    /* Local variables */
    qint32 ifld, iend, lend, left, nreq, ipos, kcmnd, nextb, ic, ibegin, ltoadd;
    qint32 ielmnt, lelmnt[25], nelmnt;
//    QString ctemp;
    char *celmnt[25];
    char command[25];

    /* Function Body */
    char *crdbuf = (char*)cardbuf.toStdString().data();
    lend   = cardbuf.size();
    ielmnt = 0;
    nextb  = 1;
    ierr   = 0;
 //*-*-                                  . . . .  loop over words CELMNT
 L10:
    for (ipos = nextb; ipos <= lend; ++ipos) {
       ibegin = ipos;
       if (crdbuf[ipos-1] == ' ') continue;
       if (crdbuf[ipos-1] == ',') goto L250;
       goto L150;
    }
    goto L300;
 L150:
 //*-*-              found beginning of word, look for end
    for (ipos = ibegin + 1; ipos <= lend; ++ipos) {
       if (crdbuf[ipos-1] == ' ') goto L250;
       if (crdbuf[ipos-1] == ',') goto L250;
    }
    ipos = lend + 1;
 L250:
    iend = ipos - 1;
    ++ielmnt;
    if (iend >= ibegin) celmnt[ielmnt-1] = &crdbuf[ibegin-1];
    else                celmnt[ielmnt-1] = cnull;
    lelmnt[ielmnt-1] = iend - ibegin + 1;
    if (lelmnt[ielmnt-1] > 19) {
       qWarning() << " MINUIT WARNING: INPUT DATA WORD TOO LONG.";
       QString *str = new QString(cardbuf);
       QStringRef ctemp(str, ibegin-1,iend-ibegin+1);
       delete str;
       qWarning() << "     ORIGINAL:" << ctemp;
       qWarning() << " TRUNCATED TO" << celmnt[ielmnt-1];
       lelmnt[ielmnt-1] = 19;
    }
    if (ipos >= lend) goto L300;
    if (ielmnt >= 25) goto L300;
 //*-*-                    look for comma or beginning of next word
    for (ipos = iend + 1; ipos <= lend; ++ipos) {
       if (crdbuf[ipos-1] == ' ') continue;
       nextb = ipos;
       if (crdbuf[ipos-1] == ',') nextb = ipos + 1;
       goto L10;
    }
 //*-*-                All elements found, join the alphabetic ones to
 //*-*-                               form a command
 L300:
    nelmnt      = ielmnt;
    command[0]  = ' '; command[1] = 0;
    lnc         = 1;
    plist[0]    = 0;
    llist       = 0;
    if (ielmnt == 0) goto L900;
    kcmnd = 0;
    for (ielmnt = 1; ielmnt <= nelmnt; ++ielmnt) {
       if ( celmnt[ielmnt-1] == cnull) goto L450;
       for (ic = 1; ic <= 13; ++ic) {
          if (*celmnt[ielmnt-1] == cnumer[ic-1]) goto L450;
       }
       if (kcmnd >= maxcwd) continue;
       left = maxcwd - kcmnd;
       ltoadd = lelmnt[ielmnt-1];
       if (ltoadd > left) ltoadd = left;
       strncpy(&command[kcmnd],celmnt[ielmnt-1],ltoadd);
       kcmnd += ltoadd;
       if (kcmnd == maxcwd) continue;
       command[kcmnd] = ' ';
       ++kcmnd;
       command[kcmnd] = 0;
    }
    lnc = kcmnd;
    goto L900;
 L450:
    lnc = kcmnd;
 //*-*-                     . . . .  we have come to a numeric field
    llist = 0;
    for (ifld = ielmnt; ifld <= nelmnt; ++ifld) {
       ++llist;
       if (llist > mxp) {
          nreq = nelmnt - ielmnt + 1;
          qWarning() << " MINUIT WARNING IN MNCRCK: ";
          qWarning() << " COMMAND HAS INPUT %5d NUMERIC FIELDS, BUT MINUIT CAN ACCEPT ONLY" << nreq << mxp;
          goto L900;
       }
       if (celmnt[ifld-1] == cnull) plist[llist-1] = 0;
       else {
          sscanf(celmnt[ifld-1],"%lf",&plist[llist-1]);
       }
    }
 //*-*-                                 end loop over numeric fields
 L900:
    if (lnc <= 0) lnc = 1;
    comand = command;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*-*Find point where MNEVAL=AMIN+UP*-*-*-*-*-*-*-*-*-*-*-*
///*-*                  ===============================
///*-*       Find point where MNEVAL=AMIN+UP, along the line through
///*-*       XMIDCR,YMIDCR with direction XDIRCR,YDIRCR,   where X and Y
///*-*       are parameters KE1CR and KE2CR.  If KE2CR=0 (from MINOS),
///*-*       only KE1CR is varied.  From MNCONT, both are varied.
///*-*       Crossing point is at
///*-*        (U(KE1),U(KE2)) = (XMID,YMID) + AOPT*(XDIR,YDIR)
///*-*
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmncros(double &aopt, qint32 &iercr)
{
    /* Local variables */
      double alsb[3], flsb[3], bmin, bmax, zmid, sdev, zdir, zlim;
      double coeff[3], aleft, aulim, fdist, adist, aminsv;
      double anext, fnext, slope, s1, s2, x1, x2, ecarmn, ecarmx;
      double determ, rt, smalla, aright, aim, tla, tlf, dfda,ecart;
      qint32 iout=0, i, ileft, ierev, maxlk, ibest, ik, it;
      qint32 noless, iworst=0, iright, itoohi, kex, ipt;
      bool ldebug;
      const char *chsign;
      x2 = 0;

      ldebug = mIdbg[6] >= 1;
      aminsv = mAmin;
   //*-*-       convergence when F is within TLF of AIM and next prediction
   //*-*-       of AOPT is within TLA of previous value of AOPT
      aim      = mAmin + mUp;
      tlf      = mUp*.01;
      tla      = .01;
      mXpt[0]  = 0;
      mYpt[0]  = aim;
      mChpt[0] = ' ';
      ipt = 1;
      if (mKe2cr == 0) {
         mXpt[1]  = -1;
         mYpt[1]  = mAmin;
         mChpt[1] = '.';
         ipt      = 2;
      }
   //*-*-                   find the largest allowed A
      aulim = 100;
      for (ik = 1; ik <= 2; ++ik) {
         if (ik == 1) {
            kex  = mKe1cr;
            zmid = mXmidcr;
            zdir = mXdircr;
         } else {
            if (mKe2cr == 0) continue;
            kex  = mKe2cr;
            zmid = mYmidcr;
            zdir = mYdircr;
         }
         if (mNvarl[kex-1] <= 1) continue;
         if (zdir == 0) continue;
         zlim = mAlim[kex-1];
         if (zdir > 0) zlim = mBlim[kex-1];
         aulim = qMin(aulim,(zlim - zmid) / zdir);
      }
   //*-*-                 LSB = Line Search Buffer
   //*-*-         first point
      anext   = 0;
      aopt    = anext;
      mLimset = false;
      if (aulim < aopt + tla) mLimset = true;
      qmneval(anext, fnext, ierev);
   //*-* debug printout:
      if (ldebug) {
         qInfo() << " MNCROS: calls=" << mNfcn << "AIM=" << aim << "F,A=" << fnext << aopt;
      }
      if (ierev > 0) goto L900;
      if (mLimset && fnext <= aim) goto L930;
      ++ipt;
      mXpt[ipt-1]  = anext;
      mYpt[ipt-1]  = fnext;
      mChpt[ipt-1] = charal[ipt-1];
      alsb[0] = anext;
      flsb[0] = fnext;
      fnext   = qMax(fnext,aminsv + mUp*.1);
      aopt    = qSqrt(mUp / (fnext - aminsv)) - 1;
      if (qAbs(fnext - aim) < tlf) goto L800;

      if (aopt < -.5)aopt = -.5;
      if (aopt > 1)  aopt = 1;
      mLimset = false;
      if (aopt > aulim) {
         aopt    = aulim;
         mLimset = true;
      }
      qmneval(aopt, fnext, ierev);
   //*-* debug printout:
      if (ldebug) {
         qInfo() << " MNCROS: calls=" << mNfcn << "AIM=" << aim << "F,A=" << fnext << aopt;
      }
      if (ierev > 0) goto L900;
      if (mLimset && fnext <= aim) goto L930;
      alsb[1] = aopt;
      ++ipt;
      mXpt[ipt-1]  = alsb[1];
      mYpt[ipt-1]  = fnext;
      mChpt[ipt-1] = charal[ipt-1];
      flsb[1]      = fnext;
      dfda         = (flsb[1] - flsb[0]) / (alsb[1] - alsb[0]);
   //*-*-                  DFDA must be positive on the contour
      if (dfda > 0) goto L460;
   L300:
      qmnwarn("D", "MNCROS    ", "Looking for slope of the right sign");
      maxlk = 15 - ipt;
      for (it = 1; it <= maxlk; ++it) {
         alsb[0] = alsb[1];
         flsb[0] = flsb[1];
         aopt    = alsb[0] + double(it)*.2;
         mLimset = false;
         if (aopt > aulim) {
            aopt    = aulim;
            mLimset = true;
         }
         qmneval(aopt, fnext, ierev);
   //*-* debug printout:
         if (ldebug) {
            qInfo() << " MNCROS: calls=" << mNfcn << "AIM=" << aim << "F,A=" << fnext << aopt;
         }
         if (ierev > 0) goto L900;
         if (mLimset && fnext <= aim) goto L930;
         alsb[1] = aopt;
         ++ipt;
         mXpt[ipt-1]  = alsb[1];
         mYpt[ipt-1]  = fnext;
         mChpt[ipt-1] = charal[ipt-1];
         flsb[1]      = fnext;
         dfda         = (flsb[1] - flsb[0]) / (alsb[1] - alsb[0]);
         if (dfda > 0) goto L450;
      }
      qmnwarn("W", "MNCROS    ", "Cannot find slope of the right sign");
      goto L950;
   L450:
   //*-*-                   we have two points with the right slope
   L460:
      aopt  = alsb[1] + (aim - flsb[1]) / dfda;
      fdist = qMin(qAbs(aim - flsb[0]),qAbs(aim - flsb[1]));
      adist = qMin(qAbs(aopt - alsb[0]),qAbs(aopt - alsb[1]));
      tla = .01;
      if (qAbs(aopt) > 1) tla = qAbs(aopt)*.01;
      if (adist < tla && fdist < tlf) goto L800;
      if (ipt >= 15) goto L950;
      bmin = qMin(alsb[0],alsb[1]) - 1;
      if (aopt < bmin) aopt = bmin;
      bmax = qMax(alsb[0],alsb[1]) + 1;
      if (aopt > bmax) aopt = bmax;
   //*-*-                   Try a third point
      mLimset = false;
      if (aopt > aulim) {
         aopt    = aulim;
         mLimset = true;
      }
      qmneval(aopt, fnext, ierev);
   //*-* debug printout:
      if (ldebug) {
         qDebug() << " MNCROS: calls=" << mNfcn << "AIM=" << aim << "F,A=" << fnext << aopt;
      }
      if (ierev > 0) goto L900;
      if (mLimset && fnext <= aim) goto L930;
      alsb[2] = aopt;
      ++ipt;
      mXpt[ipt-1]  = alsb[2];
      mYpt[ipt-1]  = fnext;
      mChpt[ipt-1] = charal[ipt-1];
      flsb[2]      = fnext;
   //*-*-               now we have three points, ask how many <AIM
      ecarmn = qAbs(fnext-aim);
      ibest  = 3;
      ecarmx = 0;
      noless = 0;
      for (i = 1; i <= 3; ++i) {
         ecart = qAbs(flsb[i-1] - aim);
         if (ecart > ecarmx) { ecarmx = ecart; iworst = i; }
         if (ecart < ecarmn) { ecarmn = ecart; ibest = i; }
         if (flsb[i-1] < aim) ++noless;
      }
   //*-*-          if at least one on each side of AIM, fit a parabola
      if (noless == 1 || noless == 2) goto L500;
   //*-*-          if all three are above AIM, third must be closest to AIM
      if (noless == 0 && ibest != 3) goto L950;
   //*-*-          if all three below, and third is not best, then slope
   //*-*-            has again gone negative, look for positive slope.
      if (noless == 3 && ibest != 3) {
         alsb[1] = alsb[2];
         flsb[1] = flsb[2];
         goto L300;
      }
   //*-*-          in other cases, new straight line thru last two points
      alsb[iworst-1] = alsb[2];
      flsb[iworst-1] = flsb[2];
      dfda = (flsb[1] - flsb[0]) / (alsb[1] - alsb[0]);
      goto L460;
   //*-*-               parabola fit
   L500:
      qmnpfit(alsb, flsb, 3, coeff, sdev);
      if (coeff[2] <= 0) {
         qmnwarn("D", "MNCROS    ", "Curvature is negative near contour line.");
      }
      determ = coeff[1]*coeff[1] - coeff[2]*4*(coeff[0] - aim);
      if (determ <= 0) {
         qmnwarn("D", "MNCROS    ", "Problem 2, impossible determinant");
         goto L950;
      }
   //*-*-               Find which root is the right one
      rt = qSqrt(determ);
      x1 = (-coeff[1] + rt) / (coeff[2]*2);
      x2 = (-coeff[1] - rt) / (coeff[2]*2);
      s1 = coeff[1] + x1*2*coeff[2];
      s2 = coeff[1] + x2*2*coeff[2];
      if (s1*s2 > 0) {
         qInfo() << " MNCONTour problem 1";
      }
      aopt  = x1;
      slope = s1;
      if (s2 > 0) {
         aopt  = x2;
         slope = s2;
      }
   //*-*-        ask if converged
      tla = .01;
      if (qAbs(aopt) > 1) tla = qAbs(aopt)*.01;
      if (qAbs(aopt - alsb[ibest-1]) < tla && qAbs(flsb[ibest-1] - aim) < tlf) {
         goto L800;
      }
      if (ipt >= 15) goto L950;

   //*-*-        see if proposed point is in acceptable zone between L and R
   //*-*-        first find ILEFT, IRIGHT, IOUT and IBEST
      ileft  = 0;
      iright = 0;
      ibest  = 1;
      ecarmx = 0;
      ecarmn = qAbs(aim - flsb[0]);
      for (i = 1; i <= 3; ++i) {
         ecart = qAbs(flsb[i-1] - aim);
         if (ecart < ecarmn) { ecarmn = ecart; ibest = i; }
         if (ecart > ecarmx) { ecarmx = ecart; }
         if (flsb[i-1] > aim) {
            if (iright == 0) iright = i;
            else if (flsb[i-1] > flsb[iright-1]) iout = i;
            else { iout = iright; iright = i; }
         }
         else if (ileft == 0) ileft = i;
         else if (flsb[i-1] < flsb[ileft-1]) iout = i;
         else { iout = ileft; ileft = i;        }
      }
   //*-*-      avoid keeping a very bad point next time around
      if (ecarmx > qAbs(flsb[iout-1] - aim)*10) {
         aopt = aopt*.5 + (alsb[iright-1] + alsb[ileft-1])*.25;
      }
   //*-*-        knowing ILEFT and IRIGHT, get acceptable window
      smalla = tla*.1;
      if (slope*smalla > tlf) smalla = tlf / slope;
      aleft  = alsb[ileft-1] + smalla;
      aright = alsb[iright-1] - smalla;
   //*-*-        move proposed point AOPT into window if necessary
      if (aopt < aleft)   aopt = aleft;
      if (aopt > aright)  aopt = aright;
      if (aleft > aright) aopt = (aleft + aright)*.5;

   //*-*-        see if proposed point outside limits (should be impossible!)
      mLimset = false;
      if (aopt > aulim) {
         aopt    = aulim;
         mLimset = true;
      }
   //*-*-                 Evaluate function at new point AOPT
      qmneval(aopt, fnext, ierev);
   //*-* debug printout:
      if (ldebug) {
         qDebug() << " MNCROS: calls=" << mNfcn << "AIM=" << aim << "F,A=" << fnext << aopt;
      }
      if (ierev > 0) goto L900;
      if (mLimset && fnext <= aim) goto L930;
      ++ipt;
      mXpt[ipt-1]  = aopt;
      mYpt[ipt-1]  = fnext;
      mChpt[ipt-1] = charal[ipt-1];
   //*-*-               Replace odd point by new one
      alsb[iout-1] = aopt;
      flsb[iout-1] = fnext;
   //*-*-         the new point may not be the best, but it is the only one
   //*-*-         which could be good enough to pass convergence criteria
      ibest = iout;
      goto L500;

   //*-*-      Contour has been located, return point to MNCONT OR MINOS
   L800:
      iercr = 0;
      goto L1000;
   //*-*-               error in the minimization
   L900:
      if (ierev == 1) goto L940;
      goto L950;
   //*-*-               parameter up against limit
   L930:
      iercr = 1;
      goto L1000;
   //*-*-               too many calls to FCN
   L940:
      iercr = 2;
      goto L1000;
   //*-*-               cannot find next point
   L950:
      iercr = 3;
   //*-*-               in any case
   L1000:
      if (ldebug) {
         itoohi = 0;
         for (i = 1; i <= ipt; ++i) {
            if (mYpt[i-1] > aim + mUp) {
               mYpt[i-1]  = aim + mUp;
               mChpt[i-1] = '+';
               itoohi     = 1;
            }
         }
         mChpt[ipt] = 0;
         chsign = "POSI";
         if (mXdircr < 0) chsign = "NEGA";
         if (mKe2cr == 0) {
            qInfo() << chsign << "TIVE MINOS ERROR, PARAMETER" << mKe1cr;
         }
         if (itoohi == 1) {
            qInfo() << "POINTS LABELLED '+' WERE TOO HIGH TO PLOT.";
         }
         if (iercr == 1) {
            qInfo() << "RIGHTMOST POINT IS UP AGAINST LIMIT.";
         }
         qmnplot(mXpt, mYpt, mChpt, ipt, mNpagwd, mNpagln);
      }

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*Makes sure that the current point is a local minimum*-*-*-*-*
///*-*            ====================================================
///*-*        Makes sure that the current point is a local
///*-*        minimum and that the error matrix exists,
///*-*        or at least something good enough for MINOS and MNCONT
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmncuve()
{
    /* Local variables */
      double dxdi, wint;
      qint32 ndex, iext, i, j;

      if (mISW[3] < 1) {
         qInfo() << " FUNCTION MUST BE MINIMIZED BEFORE CALLING" << mCfrom;
         mApsi = mEpsi;
         qmnmigr();
      }
      if (mISW[1] < 3) {
         qmnhess();
         if (mISW[1] < 1) {
            qmnwarn("W", mCfrom.toStdString().data(), "NO ERROR MATRIX.  WILL IMPROVISE.");
            for (i = 1; i <= mNPar; ++i) {
               ndex = i*(i-1) / 2;
               for (j = 1; j <= i-1; ++j) {
                  ++ndex;
                  mVhmat[ndex-1] = 0;
               }
               ++ndex;
               if (mG2[i-1] <= 0) {
                  wint = mWerr[i-1];
                  iext = mNexofi[i-1];
                  if (mNvarl[iext-1] > 1) {
                     qmndxdi(mX[i-1], i-1, dxdi);
                     if (qAbs(dxdi) < .001) wint = .01;
                     else                   wint /= qAbs(dxdi);
                  }
                  mG2[i-1] = mUp / (wint*wint);
               }
               mVhmat[ndex-1] = 2 / mG2[i-1];
            }
            mISW[1] = 1;
            mDcovar = 1;
         } else  qmnwerr();
      }

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*Calculates the first derivatives of FCN (GRD)*-*-*-*-*-*-*-*
///*-*            =============================================
///*-*        Calculates the first derivatives of FCN (GRD),
///*-*        either by finite differences or by transforming the user-
///*-*        supplied derivatives to internal coordinates,
///*-*        according to whether mISW[2] is zero or one.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnderi()
{
    /* Local variables */
      double step, dfmin, stepb4, dd, df, fs1;
      double tlrstp, tlrgrd, epspri, optstp, stpmax, stpmin, fs2, grbfor=0, d1d2, xtf;
      qint32 icyc, ncyc, iint, iext, i, nparx;
      bool ldebug;

      nparx = mNPar;
      ldebug = mIdbg[2] >= 1;
      if (mAmin == mUndefi) qmnamin();
      if (mISW[2] == 1) goto L100;

      if (ldebug) {
   //*-*-                      make sure starting at the right place
         qmninex(mX);
         nparx = mNPar;
         Eval(nparx, mGin, fs1, mU, 4);        ++mNfcn;
         if (fs1 != mAmin) {
            df    = mAmin - fs1;
            QString tempo = QString("function value differs from AMIN by %1").arg(df);
            qmnwarn("D", "MNDERI", tempo.toStdString().data());
            mAmin = fs1;
         }
         qInfo() << "  FIRST DERIVATIVE DEBUG PRINTOUT.  MNDERI";
         qInfo() << " PAR    DERIV     STEP      MINSTEP   OPTSTEP  D1-D2    2ND DRV";
      }
      dfmin = mEpsma2*8*(qAbs(mAmin) + mUp);
      if (mIstrat <= 0) {
         ncyc   = 2;
         tlrstp = .5;
         tlrgrd = .1;
      } else if (mIstrat == 1) {
         ncyc   = 3;
         tlrstp = .3;
         tlrgrd = .05;
      } else {
         ncyc   = 5;
         tlrstp = .1;
         tlrgrd = .02;
      }
   //*-*-                               loop over variable parameters
      for (i = 1; i <= mNPar; ++i) {
         epspri = mEpsma2 + qAbs(mGrd[i-1]*mEpsma2);
   //*-*-        two-point derivatives always assumed necessary
   //*-*-        maximum number of cycles over step size depends on strategy
         xtf = mX[i-1];
         stepb4 = 0;
   //*-*-                              loop as little as possible here!/
         for (icyc = 1; icyc <= ncyc; ++icyc) {
   //*-*-                ........ theoretically best step
            optstp = qSqrt(dfmin / (qAbs(mG2[i-1]) + epspri));
   //*-*-                    step cannot decrease by more than a factor of ten
            step = qMax(optstp,qAbs(mGstep[i-1]*.1));
   //*-*-                but if parameter has limits, max step size = 0.5
            if (mGstep[i-1] < 0 && step > .5) step = .5;
   //*-*-                and not more than ten times the previous step
            stpmax = qAbs(mGstep[i-1])*10;
            if (step > stpmax) step = stpmax;
   //*-*-                minimum step size allowed by machine precision
            stpmin = qAbs(mEpsma2*mX[i-1])*8;
            if (step < stpmin) step = stpmin;
   //*-*-                end of iterations if step change less than factor 2
            if (qAbs((step - stepb4) / step) < tlrstp) goto L50;
   //*-*-        take step positive
            stepb4 = step;
            if (mGstep[i-1] > 0) mGstep[i-1] =  qAbs(step);
            else                 mGstep[i-1] = -qAbs(step);
            stepb4  = step;
            mX[i-1] = xtf + step;
            qmninex(mX);
            Eval(nparx, mGin, fs1, mU, 4);            ++mNfcn;
   //*-*-        take step negative
            mX[i-1] = xtf - step;
            qmninex(mX);
            Eval(nparx, mGin, fs2, mU, 4);            ++mNfcn;
            grbfor = mGrd[i-1];
            mGrd[i-1] = (fs1 - fs2) / (step*2);
            mG2[i-1]  = (fs1 + fs2 - mAmin*2) / (step*step);
            mX[i-1]   = xtf;
            if (ldebug) {
               d1d2 = (fs1 + fs2 - mAmin*2) / step;
               qInfo() << i << mGrd[i-1] << step << stpmin << optstp << d1d2 << mG2[i-1];
            }
   //*-*-        see if another iteration is necessary
            if (qAbs(grbfor - mGrd[i-1]) / (qAbs(mGrd[i-1]) + dfmin/step) < tlrgrd)
               goto L50;
         }
   //*-*-                          end of ICYC loop. too many iterations
         if (ncyc == 1) goto L50;
         {
             QString tempo = QString("First derivative not converged. %1%2").arg(mGrd[i-1]).arg(grbfor);
             qmnwarn("D", "MNDERI", tempo.toStdString().data());
         }
   L50:
         ;
      }
      qmninex(mX);
      return;
   //*-*-                                       .  derivatives calc by fcn
   L100:
      for (iint = 1; iint <= mNPar; ++iint) {
         iext = mNexofi[iint-1];
         if (mNvarl[iext-1] <= 1) {
            mGrd[iint-1] = mGin[iext-1];
         } else {
            dd = (mBlim[iext-1] - mAlim[iext-1])*.5*qCos(mX[iint-1]);
            mGrd[iint-1] = mGin[iext-1]*dd;
         }
      }

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Calculates the transformation factor between ext/internal values*-*
///*-*    =====================================================================
///*-*        calculates the transformation factor between external and
///*-*        internal parameter values.     this factor is one for
///*-*        parameters which are not limited.     called from MNEMAT.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmndxdi(double pint, qint32 ipar, double &dxdi)
{
    qint32 i = mNexofi[ipar];
    dxdi = 1;
    if (mNvarl[i-1] > 1) {
       dxdi = qAbs((mBlim[i-1] - mAlim[i-1])*qCos(pint))*.5;
    }

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*-*-*Compute matrix eigen values*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*                    ===========================

void QMinuit::qmneig(double *a, qint32 ndima, qint32 n, qint32 mits, double *work, double precis, qint32 &ifault)
{
    /* System generated locals */
    qint32 a_offset;
    double d__1;

    /* Local variables */
    double b, c, f, h, r, s, hh, gl, pr, pt;
    qint32 i, j, k, l, m=0, i0, i1, j1, m1, n1;

 //*-*-         PRECIS is the machine precision EPSMAC
    /* Parameter adjustments */
    a_offset = ndima + 1;
    a -= a_offset;
    --work;

    /* Function Body */
    ifault = 1;

    i = n;
    for (i1 = 2; i1 <= n; ++i1) {
       l  = i-2;
       f  = a[i + (i-1)*ndima];
       gl = 0;

       if (l < 1) goto L25;

       for (k = 1; k <= l; ++k) {
          d__1 = a[i + k*ndima];
          gl  += d__1*d__1;
       }
 L25:
       h = gl + f*f;

       if (gl > 1e-35) goto L30;

       work[i]     = 0;
       work[n + i] = f;
       goto L65;
 L30:
       ++l;
       gl = qSqrt(h);
       if (f >= 0) gl = -gl;
       work[n + i] = gl;
       h -= f*gl;
       a[i + (i-1)*ndima] = f - gl;
       f = 0;
       for (j = 1; j <= l; ++j) {
          a[j + i*ndima] = a[i + j*ndima] / h;
          gl = 0;
          for (k = 1; k <= j; ++k) { gl += a[j + k*ndima]*a[i + k*ndima]; }
          if (j >= l) goto L47;
          j1 = j + 1;
          for (k = j1; k <= l; ++k) {        gl += a[k + j*ndima]*a[i + k*ndima]; }
 L47:
          work[n + j] = gl / h;
          f += gl*a[j + i*ndima];
       }
       hh = f / (h + h);
       for (j = 1; j <= l; ++j) {
          f  = a[i + j*ndima];
          gl = work[n + j] - hh*f;
          work[n + j] = gl;
          for (k = 1; k <= j; ++k) {
             a[j + k*ndima] = a[j + k*ndima] - f*work[n + k] - gl*a[i + k*ndima];
          }
       }
       work[i] = h;
 L65:
       --i;
    }
    work[1] = 0;
    work[n + 1] = 0;
    for (i = 1; i <= n; ++i) {
       l = i-1;
       if (work[i] == 0 || l == 0) goto L100;

       for (j = 1; j <= l; ++j) {
          gl = 0;
          for (k = 1; k <= l; ++k) { gl += a[i + k*ndima]*a[k + j*ndima]; }
          for (k = 1; k <= l; ++k) { a[k + j*ndima] -= gl*a[k + i*ndima]; }
       }
 L100:
       work[i] = a[i + i*ndima];
       a[i + i*ndima] = 1;
       if (l == 0) continue;

       for (j = 1; j <= l; ++j) {
          a[i + j*ndima] = 0;
          a[j + i*ndima] = 0;
       }
    }

    n1 = n - 1;
    for (i = 2; i <= n; ++i) {
       i0 = n + i-1;
       work[i0] = work[i0 + 1];
    }
    work[n + n] = 0;
    b = 0;
    f = 0;
    for (l = 1; l <= n; ++l) {
       j = 0;
       h = precis*(qAbs(work[l]) + qAbs(work[n + l]));
       if (b < h) b = h;
       for (m1 = l; m1 <= n; ++m1) {
          m = m1;
          if (qAbs(work[n + m]) <= b)        goto L150;
       }

 L150:
       if (m == l) goto L205;

 L160:
       if (j == mits) return;
       ++j;
       pt = (work[l + 1] - work[l]) / (work[n + l]*2);
       r  = qSqrt(pt*pt + 1);
       pr = pt + r;
       if (pt < 0) pr = pt - r;

       h = work[l] - work[n + l] / pr;
       for (i = l; i <= n; ++i) { work[i] -= h; }
       f += h;
       pt = work[m];
       c  = 1;
       s  = 0;
       m1 = m - 1;
       i  = m;
       for (i1 = l; i1 <= m1; ++i1) {
          j = i;
          --i;
          gl = c*work[n + i];
          h  = c*pt;
          if (qAbs(pt) >= qAbs(work[n + i])) goto L180;

          c = pt / work[n + i];
          r = qSqrt(c*c + 1);
          work[n + j] = s*work[n + i]*r;
          s  = 1 / r;
          c /= r;
          goto L190;
 L180:
          c = work[n + i] / pt;
          r = qSqrt(c*c + 1);
          work[n + j] = s*pt*r;
          s = c / r;
          c = 1 / r;
 L190:
          pt = c*work[i] - s*gl;
          work[j] = h + s*(c*gl + s*work[i]);
          for (k = 1; k <= n; ++k) {
             h = a[k + j*ndima];
             a[k + j*ndima] = s*a[k + i*ndima] + c*h;
             a[k + i*ndima] = c*a[k + i*ndima] - s*h;
          }
       }
       work[n + l] = s*pt;
       work[l]     = c*pt;

       if (qAbs(work[n + l]) > b) goto L160;

 L205:
       work[l] += f;
    }
    for (i = 1; i <= n1; ++i) {
       k  = i;
       pt = work[i];
       i1 = i + 1;
       for (j = i1; j <= n; ++j) {
          if (work[j] >= pt) continue;
          k  = j;
          pt = work[j];
       }

       if (k == i) continue;

       work[k] = work[i];
       work[i] = pt;
       for (j = 1; j <= n; ++j) {
          pt = a[j + i*ndima];
          a[j + i*ndima] = a[j + k*ndima];
          a[j + k*ndima] = pt;
       }
    }
    ifault = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// Calculates the external error matrix from the internal matrix
///
/// Note that if the matrix is declared like double matrix[5][5]
/// in the calling program, one has to call mnemat with, eg
///     gMinuit->mnemat(&matrix[0][0],5);

void QMinuit::qmnemat(double *emat, qint32 ndim)
{
    /* System generated locals */
    qint32 emat_dim1, emat_offset;

    /* Local variables */
    double dxdi, dxdj;
    qint32 i, j, k, npard, k2, kk, iz, nperln, kga, kgb;
    QString ctemp;

    /* Parameter adjustments */
    emat_dim1 = ndim;
    emat_offset = emat_dim1 + 1;
    emat -= emat_offset;

    /* Function Body */
    if (mISW[1] < 1) return;
    if (mISW[4] >= 2) {
       qInfo() << " EXTERNAL ERROR MATRIX.    NDIM=" << ndim << "NPAR=" << mNPar <<  "ERR DEF=" << mUp;
    }
 //*-*-                   size of matrix to be printed
    npard = mNPar;
    if (ndim < mNPar) {
       npard = ndim;
       if (mISW[4] >= 0) {
          qInfo() << " USER-DIMENSIONED  ARRAY EMAT NOT BIG ENOUGH. REDUCED MATRIX CALCULATED.";
       }
    }
 //*-*-                NPERLN is the number of elements that fit on one line

    nperln = (mNpagwd - 5) / 10;
    nperln = qMin(nperln,13);
    if (mISW[4] >= 1 && npard > nperln) {
       qInfo() << " ELEMENTS ABOVE DIAGONAL ARE NOT PRINTED.";
    }
 //*-*-                I counts the rows of the matrix
    for (i = 1; i <= npard; ++i) {
       qmndxdi(mX[i-1], i-1, dxdi);
       kga = i*(i-1) / 2;
       for (j = 1; j <= i; ++j) {
          qmndxdi(mX[j-1], j-1, dxdj);
          kgb = kga + j;
          emat[i + j*emat_dim1] = dxdi*mVhmat[kgb-1]*dxdj*mUp;
          emat[j + i*emat_dim1] = emat[i + j*emat_dim1];
       }
    }
 //*-*-                   IZ is number of columns to be printed in row I
    if (mISW[4] >= 2) {
       for (i = 1; i <= npard; ++i) {
          iz = npard;
          if (npard >= nperln) iz = i;
          ctemp = " ";
          for (k = 1; nperln < 0 ? k >= iz : k <= iz; k += nperln) {
             k2 = k + nperln - 1;
             if (k2 > iz) k2 = iz;
             for (kk = k; kk <= k2; ++kk) {
                ctemp += QString("%1").arg(emat[i + kk*emat_dim1]);
             }
             qInfo() << ctemp;
          }
       }
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*Utility routine to get MINOS errors*-*-*-*-*-*-*-*-*-*-*
///*-*                ===================================
///*-*    Called by user.
///*-*    NUMBER is the parameter number
///*-*    values returned by MNERRS:
///*-*       EPLUS, EMINUS are MINOS errors of parameter NUMBER,
///*-*       EPARAB is 'parabolic' error (from error matrix).
///*-*                 (Errors not calculated are set = 0)
///*-*       GCC is global correlation coefficient from error matrix
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnerrs(qint32 number, double &eplus, double &eminus, double &eparab, double &gcc)
{
    double dxdi;
    qint32 ndiag, iin, iex;

    iex = number+1;

    if (iex > mNu || iex <= 0) goto L900;
    iin = mNiofex[iex-1];
    if (iin <= 0) goto L900;

 //*-*-            IEX is external number, IIN is internal number
    eplus  = mErp[iin-1];
    if (eplus == mUndefi)  eplus = 0;
    eminus = mErn[iin-1];
    if (eminus == mUndefi) eminus = 0;
    qmndxdi(mX[iin-1], iin-1, dxdi);
    ndiag  = iin*(iin + 1) / 2;
    eparab = qAbs(dxdi*qSqrt(qAbs(mUp*mVhmat[ndiag- 1])));
 //*-*-             global correlation coefficient
    gcc = 0;
    if (mISW[1] < 2) return;
    gcc = mGlobcc[iin-1];
    return;
 //*-*-                 ERROR.  parameter number not valid
 L900:
    eplus  = 0;
    eminus = 0;
    eparab = 0;
    gcc    = 0;

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*Evaluates the function being analyzed by MNCROS*-*-*-*-*-*-*-*
///*-*          ===============================================
///*-*      Evaluates the function being analyzed by MNCROS, which is
///*-*      generally the minimum of FCN with respect to all remaining
///*-*      variable parameters.  The class data members contains the
///*-*      data necessary to know the values of U(KE1CR) and U(KE2CR)
///*-*      to be used, namely     U(KE1CR) = XMIDCR + ANEXT*XDIRCR
///*-*      and (if KE2CR .NE. 0)  U(KE2CR) = YMIDCR + ANEXT*YDIRCR
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmneval(double anext, double &fnext, qint32 &ierev)
{
    qint32 nparx;

    mU[mKe1cr-1] = mXmidcr + anext*mXdircr;
    if (mKe2cr != 0) mU[mKe2cr-1] = mYmidcr + anext*mYdircr;
    qmninex(mX);
    nparx = mNPar;
    Eval(nparx, mGin, fnext, mU, 4);    ++mNfcn;
    ierev = 0;
    if (mNPar > 0) {
       mItaur = 1;
       mAmin = fnext;
       mISW[0] = 0;
       qmnmigr();
       mItaur = 0;
       fnext = mAmin;
       if (mISW[0] >= 1) ierev = 1;
       if (mISW[3] < 1)  ierev = 2;
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*Interprets a command and takes appropriate action*-*-*-*-*-*-*-*
///*-*        =================================================
///*-*        either directly by skipping to the corresponding code in
///*-*        MNEXCM, or by setting up a call to a function
///*-*
///*-*  recognized MINUIT commands:
///*-*  obsolete commands:
///*-*      IERFLG is now (94.5) defined the same as ICONDN in MNCOMD
///*-*            = 0: command executed normally
///*-*              1: command is blank, ignored
///*-*              2: command line unreadable, ignored
///*-*              3: unknown command, ignored
///*-*              4: abnormal termination (e.g., MIGRAD not converged)
///*-*              9: reserved
///*-*             10: END command
///*-*             11: EXIT or STOP command
///*-*             12: RETURN command
///*-*
///*-*     see also http://wwwasdoc.web.cern.ch/wwwasdoc/minuit/node18.html for the possible list
///*-*     of all Minuit commands
///*-*
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnexcm(const char *command, double *plist, qint32 llist, qint32 &ierflg)
{
    /* Initialized data */

      QString comand = command;
      static const char *cname[40] = {
         "MINImize  ",
         "SEEk      ",
         "SIMplex   ",
         "MIGrad    ",
         "MINOs     ",
         "SET xxx   ",
         "SHOw xxx  ",
         "TOP of pag",
         "FIX       ",
         "REStore   ",
         "RELease   ",
         "SCAn      ",
         "CONtour   ",
         "HESse     ",
         "SAVe      ",
         "IMProve   ",
         "CALl fcn  ",
         "STAndard  ",
         "END       ",
         "EXIt      ",
         "RETurn    ",
         "CLEar     ",
         "HELP      ",
         "MNContour ",
         "STOp      ",
         "JUMp      ",
         "          ",
         "          ",
         "          ",
         "          ",
         "          ",
         "          ",
         "          ",
         "COVARIANCE",
         "PRINTOUT  ",
         "GRADIENT  ",
         "MATOUT    ",
         "ERROR DEF ",
         "LIMITS    ",
         "PUNCH     "};

      qint32 nntot = 40;

      /* Local variables */
      double step, xptu[101], yptu[101], f, rno;
      qint32 icol, kcol, ierr, iint, iext, lnow, nptu, i, iflag, ierrf;
      qint32 ilist, nparx, izero, nf, lk, it, iw, inonde, nsuper;
      qint32 it2, ke1, ke2, nowprt, kll, krl;
      QString chwhy, c26, cvblnk, cneway, comd;
      QString ctemp;
      bool lfreed, ltofix, lfixed;

   //*-*  alphabetical order of command names!

      /* Function Body */

      lk = comand.size();
      if (lk > 20) lk = 20;
      mCword =  comand;
      mCword.toUpper();
   //*-*-          Copy the first MAXP arguments into WORD7, making
   //*-*-          sure that WORD7(1)=0 if LLIST=0
      for (iw = 1; iw <= mMaxPar; ++iw) {
         mWord7[iw-1] = 0;
         if (iw <= llist) mWord7[iw-1] = plist[iw-1];
      }
      ++mIcomnd;
      mNfcnlc = mNfcn;
//      if (mCword(0,7) != "SET PRI" || mWord7[0] >= 0) {
      if (mCword.left(7) != "SET PRI" || mWord7[0] >= 0) {
         if (mISW[4] >= 0) {
            lnow = llist;
            if (lnow > 4) lnow = 4;
            qInfo() << " **********";
            ctemp = QString(" **%1 **%2").arg(mIcomnd).arg(mCword);
//            ctemp.Form(" **%5d **%s",mIcomnd,(const char*)mCword);
            for (i = 1; i <= lnow; ++i) {
//                ctemp += QString::Format("%12.4g",plist[i-1]);
                ctemp += QString("%s").arg(plist[i-1]);
            }
            qInfo() << ctemp;
            inonde = 0;
            if (llist > lnow) {
               kll = llist;
               if (llist > mMaxPar) {
                  inonde = 1;
                  kll = mMaxPar;
               }
               qInfo() << " ***********";
               for (i = lnow + 1; i <= kll; ++i) {
                  qInfo() << plist[i-1];
               }
            }
            qInfo() << " **********";
            if (inonde > 0) {
               qWarning() << "  ERROR: ABOVE CALL TO MNEXCM TRIED TO PASS MORE THAN" << mMaxPar << "PARAMETERS.";
            }
         }
      }
      mNfcnmx = int(mWord7[0]);
      if (mNfcnmx <= 0) {
         mNfcnmx = mNPar*100 + 200 + mNPar*mNPar*5;
      }
      mEpsi = mWord7[1];
      if (mEpsi <= 0) {
         mEpsi = mUp*.1;
      }
      mLnewmn = false;
      mLphead = true;
      mISW[0] = 0;
      ierflg = 0;
   //*-*-               look for command in list CNAME . . . . . . . . . .
      ctemp = mCword.left(3);
      for (i = 1; i <= nntot; ++i) {
         if (strncmp(ctemp.toStdString().data(),cname[i-1],3) == 0) goto L90;
      }
      qWarning() << "UNKNOWN COMMAND IGNORED:" << comand;
      ierflg = 3;
      return;
   //*-*-               normal case: recognized MINUIT command . . . . . . .
   L90:
      if (mCword.left(4) == "MINO") i = 5;
      if (i != 6 && i != 7 && i != 8 && i != 23) {
         mCfrom  = cname[i-1];
         mNfcnfr = mNfcn;
      }
   //*-*-             1    2    3    4    5    6    7    8    9   10
      switch (i) {
         case 1:  goto L400;
         case 2:  goto L200;
         case 3:  goto L300;
         case 4:  goto L400;
         case 5:  goto L500;
         case 6:  goto L700;
         case 7:  goto L700;
         case 8:  goto L800;
         case 9:  goto L900;
         case 10:  goto L1000;
         case 11:  goto L1100;
         case 12:  goto L1200;
         case 13:  goto L1300;
         case 14:  goto L1400;
         case 15:  goto L1500;
         case 16:  goto L1600;
         case 17:  goto L1700;
         case 18:  goto L1800;
         case 19:  goto L1900;
         case 20:  goto L1900;
         case 21:  goto L1900;
         case 22:  goto L2200;
         case 23:  goto L2300;
         case 24:  goto L2400;
         case 25:  goto L1900;
         case 26:  goto L2600;
         case 27:  goto L3300;
         case 28:  goto L3300;
         case 29:  goto L3300;
         case 30:  goto L3300;
         case 31:  goto L3300;
         case 32:  goto L3300;
         case 33:  goto L3300;
         case 34:  goto L3400;
         case 35:  goto L3500;
         case 36:  goto L3600;
         case 37:  goto L3700;
         case 38:  goto L3800;
         case 39:  goto L3900;
         case 40:  goto L4000;
      }
   //*-*-                                       . . . . . . . . . . seek
   L200:
      qmnseek();
      return;
   //*-*-                                       . . . . . . . . . . simplex
   L300:
      qmnsimp();
      if (mISW[3] < 1) ierflg = 4;
      return;
   //*-*-                                       . . . . . . migrad, minimize
   L400:
      nf = mNfcn;
      mApsi = mEpsi;
      qmnmigr();
      qmnwerr();
      if (mISW[3] >= 1) return;
      ierflg = 4;
      if (mISW[0] == 1) return;
      if (mCword.left(3) == "MIG") return;

      mNfcnmx = mNfcnmx + nf - mNfcn;
      nf = mNfcn;
      qmnsimp();
      if (mISW[0] == 1) return;
      mNfcnmx = mNfcnmx + nf - mNfcn;
      qmnmigr();
      if (mISW[3] >= 1) ierflg = 0;
      qmnwerr();
      return;
   //*-*-                                       . . . . . . . . . . minos
   L500:
      nsuper = mNfcn + ((mNPar + 1) << 1)*mNfcnmx;
   //*-*-         possible loop over new minima
      mEpsi = mUp*.1;
   L510:
      mCfrom  = cname[i-1]; // ensure that mncuve complains about MINOS not MIGRAD
      qmncuve();
      qmnmnos();
      if (! mLnewmn) return;
      qmnrset(0);
      qmnmigr();
      qmnwerr();
      if (mNfcn < nsuper) goto L510;
      qInfo() << " TOO MANY FUNCTION CALLS. MINOS GIVES UP";
      ierflg = 4;
      return;
   //*-*-                                       . . . . . . . . . .set, show
   L700:
      qmnset();
      return;
   //*-*-                                       . . . . . . . . . . top of page

   L800:
      qInfo() << "1";
      return;
   //*-*-                                       . . . . . . . . . . fix
   L900:
      ltofix = true;
   //*-*-                                       . . (also release) ....
   L901:
      lfreed = false;
      lfixed = false;
      if (llist == 0) {
         qInfo() << mCword << ":  NO PARAMETERS REQUESTED ";
         return;
      }
      for (ilist = 1; ilist <= llist; ++ilist) {
         iext = int(plist[ilist-1]);
         chwhy = " IS UNDEFINED.";
         if (iext <= 0) goto L930;
         if (iext > mNu) goto L930;
         if (mNvarl[iext-1] < 0) goto L930;
         chwhy = " IS CONSTANT.  ";
         if (mNvarl[iext-1] == 0) goto L930;
         iint= mNiofex[iext-1];
         if (ltofix) {
            chwhy = " ALREADY FIXED.";
            if (iint== 0) goto L930;
            qmnfixp(iint-1, ierr);
            if (ierr == 0) lfixed = true;
            else           ierflg = 4;
         } else {
            chwhy = " ALREADY VARIABLE.";
            if (iint> 0) goto L930;
            krl = -abs(iext);
            qmnfree(krl);
            lfreed = true;
         }
         continue;
   L930:
         if (mISW[4] >= 0) qInfo() << " PARAMETER" << iext << chwhy << "IGNORED.";
      }
      if (lfreed || lfixed) qmnrset(0);
      if (lfreed) {
         mISW[1] = 0;
         mDcovar = 1;
         mEDM = mBigedm;
         mISW[3] = 0;
      }
      qmnwerr();
      if (mISW[4] > 1) qmnprin(5, mAmin);
      return;
   //*-*-                                       . . . . . . . . . . restore
   L1000:
      it = int(mWord7[0]);
      if (it > 1 || it < 0) goto L1005;
      lfreed = mNPFix > 0;
      qmnfree(it);
      if (lfreed) {
         qmnrset(0);
         mISW[1] = 0;
         mDcovar = 1;
         mEDM    = mBigedm;
      }
      return;
   L1005:
      qInfo() << " IGNORED.  UNKNOWN ARGUMENT:" << it;
      ierflg = 3;
      return;
   //*-*-                                       . . . . . . . . . . release
   L1100:
      ltofix = false;
      goto L901;
   //*-*-                                      . . . . . . . . . . scan . . .
   L1200:
      iext = int(mWord7[0]);
      if (iext <= 0) goto L1210;
      it2 = 0;
      if (iext <= mNu) it2 = mNiofex[iext-1];
      if (it2 <= 0) goto L1250;

   L1210:
      qmnscan();
      return;
   L1250:
      qInfo() << " PARAMETER" << iext << "NOT VARIABLE.";
      ierflg = 3;
      return;
   //*-*-                                       . . . . . . . . . . contour
   L1300:
      ke1 = int(mWord7[0]);
      ke2 = int(mWord7[1]);
      if (ke1 == 0) {
         if (mNPar == 2) {
            ke1 = mNexofi[0];
            ke2 = mNexofi[1];
         } else {
            qInfo() << mCword << "NO PARAMETERS REQUESTED ";
            ierflg = 3;
            return;
         }
      }
      mNfcnmx = 1000;
      qmncntr(ke1-1, ke2-1, ierrf);
      if (ierrf > 0) ierflg = 3;
      return;
   //*-*-                                       . . . . . . . . . . hesse
   L1400:
      qmnhess();
      qmnwerr();
      if (mISW[4] >= 0) qmnprin(2, mAmin);
      if (mISW[4] >= 1) qmnmatu(1);
      return;
   //*-*-                                       . . . . . . . . . . save
   L1500:
      qmnsave();
      return;
   //*-*-                                       . . . . . . . . . . improve
   L1600:
      qmncuve();
      qmnimpr();
      if (mLnewmn) goto L400;
      ierflg = 4;
      return;
   //*-*-                                       . . . . . . . . . . call fcn
   L1700:
      iflag = int(mWord7[0]);
      nparx = mNPar;
      f = mUndefi;
      Eval(nparx, mGin, f, mU, iflag);    ++mNfcn;
      nowprt = 0;
      if (f != mUndefi) {
         if (mAmin == mUndefi) {
            mAmin  = f;
            nowprt = 1;
         } else if (f < mAmin) {
            mAmin  = f;
            nowprt = 1;
         }
         if (mISW[4] >= 0 && iflag <= 5 && nowprt == 1) {
            qmnprin(5, mAmin);
         }
         if (iflag == 3)  mFval3 = f;
      }
      if (iflag > 5) qmnrset(1);
      return;
   //*-*-                                       . . . . . . . . . . standard
   L1800:
   //    stand();
      return;
   //*-*-                                      . . . return, stop, end, exit
   L1900:
      it = int(mWord7[0]);
      if (mFval3 != mAmin && it == 0) {
         iflag = 3;
         if (mISW[4] >= 0) qInfo() << " CALL TO USER FUNCTION WITH IFLAG = 3";
         nparx = mNPar;
         Eval(nparx, mGin, f, mU, iflag);        ++mNfcn;
      }
      ierflg = 11;
      if (mCword.left(3) == "END") ierflg = 10;
      if (mCword.left(3) == "RET") ierflg = 12;
      return;
   //*-*-                                       . . . . . . . . . . clear
   L2200:
      qmncler();
      if (mISW[4] >= 1) {
         qInfo() << " MINUIT MEMORY CLEARED. NO PARAMETERS NOW DEFINED.";
      }
      return;
   //*-*-                                       . . . . . . . . . . help
   L2300:
      kcol = 0;
      for (icol = 5; icol <= lk; ++icol) {
         if (mCword[icol-1] == ' ') continue;
         kcol = icol;
         goto L2320;
      }
   L2320:
      if (kcol == 0) comd = "*   ";
      else {
          QString *str = new QString(mCword);
          QStringRef ctemp(str, kcol-1,lk-kcol+1);
          comd = ctemp.toString().toStdString().data();
          delete str;
      }
      qmnhelp(comd);
      return;
   //*-*-                                      . . . . . . . . . . MNContour
   L2400:
      mEpsi = mUp*.05;
      ke1 = int(mWord7[0]);
      ke2 = int(mWord7[1]);
      if (ke1 == 0 && mNPar == 2) {
         ke1 = mNexofi[0];
         ke2 = mNexofi[1];
      }
      nptu = int(mWord7[2]);
      if (nptu <= 0)  nptu = 20;
      if (nptu > 101) nptu = 101;
      mNfcnmx = (nptu + 5)*100*(mNPar + 1);
      qmncont(ke1-1, ke2-1, nptu, xptu, yptu, ierrf);
      if (ierrf < nptu) ierflg = 4;
      if (ierrf == -1)  ierflg = 3;
      return;
   //*-*-                                     . . . . . . . . . . jump
   L2600:
      step = mWord7[0];
      if (step <= 0) step = 2;
      rno = 0;
      izero = 0;
      for (i = 1; i <= mNPar; ++i) {
         qmnrn15(rno, izero);
         rno      = rno*2 - 1;
         mX[i-1] += rno*step*mWerr[i-1];
      }
      qmninex(mX);
      qmnamin();
      qmnrset(0);
      return;
   //*-*-                                     . . . . . . . . . . blank line
   L3300:
      qInfo() << " BLANK COMMAND IGNORED.";
      ierflg = 1;
      return;
   //*-*  . . . . . . . . obsolete commands     . . . . . . . . . . . . . .
   //*-*-                                     . . . . . . . . . . covariance
   L3400:
      qInfo() << " THE *COVARIANCE* COMMAND IS OSBSOLETE. THE COVARIANCE MATRIX IS NOW SAVED IN A DIFFERENT FORMAT WITH THE *SAVE* COMMAND AND READ IN WITH:*SET COVARIANCE*";
      ierflg = 3;
      return;
   //*-*-                                       . . . . . . . . . . printout
   L3500:
      cneway = "SET PRINT";
      goto L3100;
   //*-*-                                       . . . . . . . . . . gradient
   L3600:
      cneway = "SET GRAd  ";
      goto L3100;
   //*-*-                                       . . . . . . . . . . matout
   L3700:
      cneway = "SHOW COVar";
      goto L3100;
   //*-*-                                       . . . . . . . . . error def
   L3800:
      cneway = "SET ERRdef";
      goto L3100;
   //*-*-                                       . . . . . . . . . . limits
   L3900:
      cneway = "SET LIMits";
      goto L3100;
   //*-*-                                       . . . . . . . . . . punch
   L4000:
      cneway = "SAVE      ";
   //*-*-                               ....... come from obsolete commands
   L3100:
      qInfo() << " OBSOLETE COMMAND:" << mCword << "PLEASE USE:"<< cneway;
      mCword = cneway;
      if (mCword == "SAVE      ") goto L1500;
      goto L700;
      //*-*
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Transforms the external parameter values U to internal values*-*-*
///*-*      =============================================================
///*-*        Transforms the external parameter values U to internal
///*-*        values in the dense array PINT.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnexin(double *pint)
{
    double pinti;
    qint32 iint, iext;

    mLimset = false;
    for (iint = 1; iint <= mNPar; ++iint) {
       iext = mNexofi[iint-1];
       qmnpint(mU[iext-1], iext-1, pinti);
       pint[iint-1] = pinti;
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*Removes parameter IINT from the internal parameter list*-*-*
///*-*          =======================================================
///*-*        and arranges the rest of the list to fill the hole.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnfixp(qint32 iint1, qint32 &ierr)
{
 // * Local variables *
    double yyover;
    qint32 kold, nold, ndex, knew, iext, i, j, m, n, lc, ik;

 // *-*-                          first see if it can be done
    ierr = 0;
    qint32 iint = iint1+1;
    if (iint > mNPar || iint <= 0) {
       ierr = 1;
       qInfo() << " MINUIT ERROR.  ARGUMENT TO MNFIXP=" << iint;
       return;
    }
    iext = mNexofi[iint-1];
    if (mNPFix >= mMaxPar) {
       ierr = 1;
       qInfo() << " MINUIT CANNOT FIX PARAMETER" << iext << "MAXIMUM NUMBER THAT CAN BE FIXED IS" << mMaxPar;
       return;
    }
 // *-*-                          reduce number of variable parameters by one

    mNiofex[iext-1] = 0;
    nold = mNPar;
    --mNPar;
 // *-*-                      save values in case parameter is later restored

    ++mNPFix;
    mIpfix[mNPFix-1]  = iext;
    lc                = iint;
    mXs[mNPFix-1]     = mX[lc-1];
    mXts[mNPFix-1]    = mXt[lc-1];
    mDirins[mNPFix-1] = mWerr[lc-1];
    mGrds[mNPFix-1]   = mGrd[lc-1];
    mG2s[mNPFix-1]    = mG2[lc-1];
    mGsteps[mNPFix-1] = mGstep[lc-1];
 // *-*-                       shift values for other parameters to fill hole
    for (ik = iext + 1; ik <= mNu; ++ik) {
       if (mNiofex[ik-1] > 0) {
          lc = mNiofex[ik-1] - 1;
          mNiofex[ik-1] = lc;
          mNexofi[lc-1] = ik;
          mX[lc-1]      = mX[lc];
          mXt[lc-1]     = mXt[lc];
          mDirin[lc-1]  = mDirin[lc];
          mWerr[lc-1]   = mWerr[lc];
          mGrd[lc-1]    = mGrd[lc];
          mG2[lc-1]     = mG2[lc];
          mGstep[lc-1]  = mGstep[lc];
       }
    }
    if (mISW[1] <= 0) return;
 // *-*-                   remove one row and one column from variance matrix
    if (mNPar <= 0)   return;
    for (i = 1; i <= nold; ++i) {
       m       = qMax(i,iint);
       n       = qMin(i,iint);
       ndex    = m*(m-1) / 2 + n;
       mFIXPyy[i-1] = mVhmat[ndex-1];
    }
    yyover = 1 / mFIXPyy[iint-1];
    knew   = 0;
    kold   = 0;
    for (i = 1; i <= nold; ++i) {
       for (j = 1; j <= i; ++j) {
          ++kold;
          if (j == iint || i == iint) continue;
          ++knew;
          mVhmat[knew-1] = mVhmat[kold-1] - mFIXPyy[j-1]*mFIXPyy[i-1]*yyover;
       }
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Restores one or more fixed parameter(s) to variable status*-*-*-*-*-*
///*-*    ==========================================================
///*-*        Restores one or more fixed parameter(s) to variable status
///*-*        by inserting it into the internal parameter list at the
///*-*        appropriate place.
///*-*
///*-*        K = 0 means restore all parameters
///*-*        K = 1 means restore the last parameter fixed
///*-*        K = -I means restore external parameter I (if possible)
///*-*        IQ = fix-location where internal parameters were stored
///*-*        IR = external number of parameter being restored
///*-*        IS = internal number of parameter being restored
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnfree(qint32 k)
{
    /* Local variables */
    double grdv, xv, dirinv, g2v, gstepv, xtv;
    qint32 i, ipsav, ka, lc, ik, iq, ir, is;

    if (k > 1) {
       qInfo() << " CALL TO MNFREE IGNORED.  ARGUMENT GREATER THAN ONE";
    }
    if (mNPFix < 1) {
       qInfo() << " CALL TO MNFREE IGNORED.  THERE ARE NO FIXED PARAMETERS";
    }
    if (k == 1 || k == 0) goto L40;

 //*-*-                  release parameter with specified external number
    ka = abs(k);
    if (mNiofex[ka-1] == 0) goto L15;
    qInfo() << " IGNORED.  PARAMETER SPECIFIED IS ALREADY VARIABLE.";
    return;
 L15:
    if (mNPFix < 1) goto L21;
    for (ik = 1; ik <= mNPFix; ++ik) { if (mIpfix[ik-1] == ka) goto L24; }
 L21:
    qInfo() << " PARAMETER" << ka << "NOT FIXED.  CANNOT BE RELEASED.";
    return;
 L24:
    if (ik == mNPFix) goto L40;

 //*-*-                  move specified parameter to end of list
    ipsav  = ka;
    xv     = mXs[ik-1];
    xtv    = mXts[ik-1];
    dirinv = mDirins[ik-1];
    grdv   = mGrds[ik-1];
    g2v    = mG2s[ik-1];
    gstepv = mGsteps[ik-1];
    for (i = ik + 1; i <= mNPFix; ++i) {
       mIpfix[i-2]  = mIpfix[i-1];
       mXs[i-2]     = mXs[i-1];
       mXts[i-2]    = mXts[i-1];
       mDirins[i-2] = mDirins[i-1];
       mGrds[i-2]   = mGrds[i-1];
       mG2s[i-2]    = mG2s[i-1];
       mGsteps[i-2] = mGsteps[i-1];
    }
    mIpfix[mNPFix-1]  = ipsav;
    mXs[mNPFix-1]     = xv;
    mXts[mNPFix-1]    = xtv;
    mDirins[mNPFix-1] = dirinv;
    mGrds[mNPFix-1]   = grdv;
    mG2s[mNPFix-1]    = g2v;
    mGsteps[mNPFix-1] = gstepv;
 //*-*-               restore last parameter in fixed list  -- IPFIX(NPFIX)
 L40:
    if (mNPFix < 1) goto L300;
    ir = mIpfix[mNPFix-1];
    is = 0;
    for (ik = mNu; ik >= ir; --ik) {
       if (mNiofex[ik-1] > 0) {
          lc = mNiofex[ik-1] + 1;
          is = lc - 1;
          mNiofex[ik-1] = lc;
          mNexofi[lc-1] = ik;
          mX[lc-1]      = mX[lc-2];
          mXt[lc-1]     = mXt[lc-2];
          mDirin[lc-1]  = mDirin[lc-2];
          mWerr[lc-1]   = mWerr[lc-2];
          mGrd[lc-1]    = mGrd[lc-2];
          mG2[lc-1]     = mG2[lc-2];
          mGstep[lc-1]  = mGstep[lc-2];
       }
    }
    ++mNPar;
    if (is == 0) is = mNPar;
    mNiofex[ir-1] = is;
    mNexofi[is-1] = ir;
    iq           = mNPFix;
    mX[is-1]     = mXs[iq-1];
    mXt[is-1]    = mXts[iq-1];
    mDirin[is-1] = mDirins[iq-1];
    mWerr[is-1]  = mDirins[iq-1];
    mGrd[is-1]   = mGrds[iq-1];
    mG2[is-1]    = mG2s[iq-1];
    mGstep[is-1] = mGsteps[iq-1];
    --mNPFix;
    mISW[1] = 0;
    mDcovar = 1;
    if (mISW[4] - mItaur >= 1) {
       qInfo() << "                   PARAMETER" << ir << mCpnam[ir-1] <<  "RESTORED TO VARIABLE.";
    }
    if (k == 0) goto L40;
 L300:
 //*-*-        if different from internal, external values are taken
    qmnexin(mX);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*Interprets the SET GRAD command*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*                ===============================
///*-*       Called from MNSET
///*-*       Interprets the SET GRAD command, which informs MINUIT whether
///*-*       the first derivatives of FCN will be calculated by the user
///*-*       inside FCN.  It can check the user derivative calculation
///*-*       by comparing it with a finite difference approximation.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmngrad()
{
    /* Local variables */
    double fzero, err;
    qint32 i, nparx, lc, istsav;
    bool lnone;
    static QString cwd = "    ";

    mISW[2] = 1;
    nparx   = mNPar;
    if (mWord7[0] > 0) goto L2000;

 //*-*-                 get user-calculated first derivatives from FCN
    for (i = 1; i <= mNu; ++i) { mGin[i-1] = mUndefi; }
    qmninex(mX);
    Eval(nparx, mGin, fzero, mU, 2);    ++mNfcn;
    qmnderi();
    for (i = 1; i <= mNPar; ++i) { mGRADgf[i-1] = mGrd[i-1]; }
 //*-*-                   get MINUIT-calculated first derivatives
    mISW[2] = 0;
    istsav  = mIstrat;
    mIstrat = 2;
    qmnhes1();
    mIstrat = istsav;
    qInfo() << " CHECK OF GRADIENT CALCULATION IN FCN";
    qInfo() << "            PARAMETER      G(IN FCN)   G(MINUIT)  DG(MINUIT)   AGREEMENT";
    mISW[2] = 1;
    lnone = false;
    for (lc = 1; lc <= mNPar; ++lc) {
       i   = mNexofi[lc-1];
       cwd = "GOOD";
       err = mDgrd[lc-1];
       if (qAbs(mGRADgf[lc-1] - mGrd[lc-1]) > err)  cwd = " BAD";
       if (mGin[i-1] == mUndefi) {
          cwd      = "NONE";
          lnone    = true;
          mGRADgf[lc-1] = 0;
       }
       if (cwd != "GOOD") mISW[2] = 0;
       qInfo() << i << mCpnam[i-1] << mGRADgf[lc-1] << mGrd[lc-1] << err << cwd;
    }
    if (lnone) {
       qInfo() << "  AGREEMENT=NONE  MEANS FCN DID NOT CALCULATE THE DERIVATIVE";
    }
    if (mISW[2] == 0) {
       qInfo() << " MINUIT DOES NOT ACCEPT DERIVATIVE CALCULATIONS BY FCN";
       qInfo() << " TO FORCE ACCEPTANCE, ENTER *SET GRAD    1*";
    }

 L2000:
    return;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*HELP routine for MINUIT interactive commands*-*-*-*-*-*-*-*-*
///*-*            ============================================
///*-*
///*-*      COMD ='*' or "" prints a global help for all commands
///*-*      COMD =Command_name: print detailed help for one command.
///*-*         Note that at least 3 characters must be given for the command
///*-*         name.
///*-*
///*-*     Author: Rene Brun
///*-*             comments extracted from the MINUIT documentation file.
///*-*
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnhelp(QString comd)
{
    //*-*.......................................................................
    //*-*
    //*-*  Global HELP: Summary of all commands
    //*-*  ====================================
    //*-*
    comd.toUpper();
    if( comd.length() == 0 || comd[0] == '*' || comd[0] == '?' || comd[0] == 0 || comd=="HELP" ) {
        qInfo() << "   ==>List of MINUIT Interactive commands:";
        qInfo() << " CLEar     Reset all parameter names and values undefined";
        qInfo() << " CONtour   Make contour map of the user function";
        qInfo() << " EXIT      Exit from Interactive Minuit";
        qInfo() << " FIX       Cause parameter(s) to remain constant";
        qInfo() << " HESse     Calculate the Hessian or error matrix.";
        qInfo() << " IMPROVE   Search for a new minimum around current minimum";
        qInfo() << " MIGrad    Minimize by the method of Migrad";
        qInfo() << " MINImize  MIGRAD + SIMPLEX method if Migrad fails";
        qInfo() << " MINOs     Exact (non-linear) parameter error analysis";
        qInfo() << " MNContour Calculate one MINOS function contour";
        qInfo() << " PARameter Define or redefine new parameters and values";
        qInfo() << " RELease   Make previously FIXed parameters variable again";
        qInfo() << " REStore   Release last parameter fixed";
        qInfo() << " SAVe      Save current parameter values on a file";
        qInfo() << " SCAn      Scan the user function by varying parameters";
        qInfo() << " SEEk      Minimize by the method of Monte Carlo";
        qInfo() << " SET       Set various MINUIT constants or conditions";
        qInfo() << " SHOw      Show values of current constants or conditions";
        qInfo() << " SIMplex   Minimize by the method of Simplex";
        goto L99;

    }

    //*-* __________________________________________________________________
    //*-*
    //*-* --  Command CLEAR
    //*-* --  =============
    //*-*
    if( !strncmp(comd.toStdString().data(),"CLE",3) ) {
        qInfo() << " ***>CLEAR";
        qInfo() << " Resets all parameter names and values to undefined.";
        qInfo() << " Must normally be followed by a PARameters command or ";
        qInfo() << " equivalent, in order to define parameter values.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command CONTOUR
    //*-* --  ===============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"CON",3) ) {
        qInfo() << " ***>CONTOUR <par1>  <par2>  [devs]  [ngrid]";
        qInfo() << " Instructs Minuit to trace contour lines of the user function";
        qInfo() << " with respect to the two parameters whose external numbers";
        qInfo() << " are <par1> and <par2>.";
        qInfo() << " Other variable parameters of the function, if any, will have";
        qInfo() << " their values fixed at the current values during the contour";
        qInfo() << " tracing. The optional parameter [devs] (default value 2.)";
        qInfo() << " gives the number of standard deviations in each parameter";
        qInfo() << " which should lie entirely within the plotting area.";
        qInfo() << " Optional parameter [ngrid] (default value 25 unless page";
        qInfo() << " size is too small) determines the resolution of the plot,";
        qInfo() << " i.e. the number of rows and columns of the grid at which the";
        qInfo() << " function will be evaluated. [See also MNContour.]";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command END
    //*-* --  ===========
    //*-* .
    if( !strncmp(comd.toStdString().data(),"END",3) ) {
        qInfo() << " ***>END" ;
        qInfo() << " Signals the end of a data block (i.e., the end of a fit)," ;
        qInfo() << " and implies that execution should continue, because another" ;
        qInfo() << " Data Block follows. A Data Block is a set of Minuit data" ;
        qInfo() << " consisting of";
        qInfo() << "     (1) A Title,";
        qInfo() << "     (2) One or more Parameter Definitions,";
        qInfo() << "     (3) A blank line, and";
        qInfo() << "     (4) A set of Minuit Commands.";
        qInfo() << " The END command is used when more than one Data Block is to";
        qInfo() << " be used with the same FCN function. It first causes Minuit";
        qInfo() << " to issue a CALL FCN with IFLAG=3, in order to allow FCN to";
        qInfo() << " perform any calculations associated with the final fitted";
        qInfo() << " parameter values, unless a CALL FCN 3 command has already";
        qInfo() << " been executed at the current FCN value.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* .
    //*-* --
    //*-* --  Command EXIT
    //*-* --  ============
    if( !strncmp(comd.toStdString().data(),"EXI",3) ) {
        qInfo() << " ***>EXIT";
        qInfo() << " Signals the end of execution.";
        qInfo() << " The EXIT command first causes Minuit to issue a CALL FCN";
        qInfo() << " with IFLAG=3, to allow FCN to perform any calculations";
        qInfo() << " associated with the final fitted parameter values, unless a";
        qInfo() << " CALL FCN 3 command has already been executed.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command FIX
    //*-* --  ===========
    //*-* .
    if( !strncmp(comd.toStdString().data(),"FIX",3) ) {
        qInfo() << " ***>FIX} <parno> [parno] ... [parno]";
        qInfo() << " Causes parameter(s) <parno> to be removed from the list of";
        qInfo() << " variable parameters, and their value(s) will remain constant";
        qInfo() << " during subsequent minimizations, etc., until another command";
        qInfo() << " changes their value(s) or status.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command HESSE
    //*-* --  =============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"HES",3) ) {
        qInfo() << " ***>HESse  [maxcalls]";
        qInfo() << " Calculate, by finite differences, the Hessian or error matrix.";
        qInfo() << "  That is, it calculates the full matrix of second derivatives";
        qInfo() << " of the function with respect to the currently variable";
        qInfo() << " parameters, and inverts it, printing out the resulting error";
        qInfo() << " matrix. The optional argument [maxcalls] specifies the";
        qInfo() << " (approximate) maximum number of function calls after which";
        qInfo() << " the calculation will be stopped.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command IMPROVE
    //*-* --  ===============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"IMP",3) ) {
        qInfo() << " ***>IMPROVE  [maxcalls]";
        qInfo() << " If a previous minimization has converged, and the current";
        qInfo() << " values of the parameters therefore correspond to a local";
        qInfo() << " minimum of the function, this command requests a search for";
        qInfo() << " additional distinct local minima.";
        qInfo() << " The optional argument [maxcalls] specifies the (approximate";
        qInfo() << " maximum number of function calls after which the calculation";
        qInfo() << " will be stopped.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command MIGRAD
    //*-* --  ==============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"MIG",3) ) {
        qInfo() << " ***>MIGrad  [maxcalls]  [tolerance]";
        qInfo() << " Causes minimization of the function by the method of Migrad,";
        qInfo() << " the most efficient and complete single method, recommended";
        qInfo() << " for general functions (see also MINImize).";
        qInfo() << " The minimization produces as a by-product the error matrix";
        qInfo() << " of the parameters, which is usually reliable unless warning";
        qInfo() << " messages are produced.";
        qInfo() << " The optional argument [maxcalls] specifies the (approximate)";
        qInfo() << " maximum number of function calls after which the calculation";
        qInfo() << " will be stopped even if it has not yet converged.";
        qInfo() << " The optional argument [tolerance] specifies required tolerance";
        qInfo() << " on the function value at the minimum.";
        qInfo() << " The default tolerance is 0.1, and the minimization will stop";
        qInfo() << " when the estimated vertical distance to the minimum (EDM) is";
        qInfo() << " less than 0.001*[tolerance]*UP (see [SET ERRordef]).";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command MINIMIZE
    //*-* --  ================
    //*-* .
    if( !strncmp(comd.toStdString().data(),"MINI",4) ) {
        qInfo() << " ***>MINImize  [maxcalls] [tolerance]";
        qInfo() << " Causes minimization of the function by the method of Migrad,";
        qInfo() << " as does the MIGrad command, but switches to the SIMplex method";
        qInfo() << " if Migrad fails to converge. Arguments are as for MIGrad.";
        qInfo() << " Note that command requires four characters to be unambiguous.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command MINOS
    //*-* --  =============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"MIN0",4) ) {
        qInfo() << " ***>MINOs  [maxcalls]  [parno] [parno] ...";
        qInfo() << " Causes a Minos error analysis to be performed on the parameters";
        qInfo() << " whose numbers [parno] are specified. If none are specified,";
        qInfo() << " Minos errors are calculated for all variable parameters.";
        qInfo() << " Minos errors may be expensive to calculate, but are very";
        qInfo() << " reliable since they take account of non-linearities in the";
        qInfo() << " problem as well as parameter correlations, and are in general";
        qInfo() << " asymmetric.";
        qInfo() << " The optional argument [maxcalls] specifies the (approximate)";
        qInfo() << " maximum number of function calls per parameter requested,";
        qInfo() << " after which the calculation will stop for that parameter.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command MNCONTOUR
    //*-* --  =================
    //*-* .
    if( !strncmp(comd.toStdString().data(),"MNC",3) ) {
        qInfo() << " ***>MNContour  <par1> <par2> [npts]";
        qInfo() << " Calculates one function contour of FCN with respect to";
        qInfo() << " parameters par1 and par2, with FCN minimized always with";
        qInfo() << " respect to all other NPAR-2 variable parameters (if any).";
        qInfo() << " Minuit will try to find npts points on the contour (default 20)";
        qInfo() << " If only two parameters are variable at the time, it is not";
        qInfo() << " necessary to specify their numbers. To calculate more than";
        qInfo() << " one contour, it is necessary to SET ERRordef to the appropriate";
        qInfo() << " value and issue the MNContour command for each contour.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command PARAMETER
    //*-* --  =================
    //*-* .
    if( !strncmp(comd.toStdString().data(),"PAR",3) ) {
        qInfo() << " ***>PARameters";
        qInfo() << " followed by one or more parameter definitions.";
        qInfo() << " Parameter definitions are of the form:";
        qInfo() << "   <number>  ''name''  <value>  <step>  [lolim] [uplim] ";
        qInfo() << " for example:";
        qInfo() << "  3  ''K width''  1.2   0.1";
        qInfo() << " the last definition is followed by a blank line or a zero.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command RELEASE
    //*-* --  ===============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"REL",3) ) {
        qInfo() << " ***>RELease  <parno> [parno] ... [parno]";
        qInfo() << " If <parno> is the number of a previously variable parameter";
        qInfo() << " which has been fixed by a command: FIX <parno>, then that";
        qInfo() << " parameter will return to variable status.  Otherwise a warning";
        qInfo() << " message is printed and the command is ignored.";
        qInfo() << " Note that this command operates only on parameters which were";
        qInfo() << " at one time variable and have been FIXed. It cannot make";
        qInfo() << " constant parameters variable; that must be done by redefining";
        qInfo() << " the parameter with a PARameters command.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command RESTORE
    //*-* --  ===============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"RES",3) ) {
        qInfo() << " ***>REStore  [code]";
        qInfo() << " If no [code] is specified, this command restores all previously";
        qInfo() << " FIXed parameters to variable status. If [code]=1, then only";
        qInfo() << " the last parameter FIXed is restored to variable status.";
        qInfo() << " If code is neither zero nor one, the command is ignored.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command RETURN
    //*-* --  ==============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"RET",3) ) {
        qInfo() << " ***>RETURN";
        qInfo() << " Signals the end of a data block, and instructs Minuit to return";
        qInfo() << " to the program which called it. The RETurn command first";
        qInfo() << " causes Minuit to CALL FCN with IFLAG=3, in order to allow FCN";
        qInfo() << " to perform any calculations associated with the final fitted";
        qInfo() << " parameter values, unless a CALL FCN 3 command has already been";
        qInfo() << " executed at the current FCN value.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command SAVE
    //*-* --  ============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"SAV",3) ) {
        qInfo() << " ***>SAVe";
        qInfo() << " Causes the current parameter values to be saved on a file in";
        qInfo() << " such a format that they can be read in again as Minuit";
        qInfo() << " parameter definitions. If the covariance matrix exists, it is";
        qInfo() << " also output in such a format. The unit number is by default 7,";
        qInfo() << " or that specified by the user in their call to MINTIO or";
        qInfo() << " MNINIT. The user is responsible for opening the file previous";
        qInfo() << " to issuing the [SAVe] command (except where this can be done";
        qInfo() << " interactively).";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command SCAN
    //*-* --  ============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"SCA",3) ) {
        qInfo() << " ***>SCAn  [parno]  [numpts] [from]  [to]";
        qInfo() << " Scans the value of the user function by varying parameter";
        qInfo() << " number [parno], leaving all other parameters fixed at the";
        qInfo() << " current value. If [parno] is not specified, all variable";
        qInfo() << " parameters are scanned in sequence.";
        qInfo() << " The number of points [numpts] in the scan is 40 by default,";
        qInfo() << " and cannot exceed 100. The range of the scan is by default";
        qInfo() << " 2 standard deviations on each side of the current best value,";
        qInfo() << " but can be specified as from [from] to [to].";
        qInfo() << " After each scan, if a new minimum is found, the best parameter";
        qInfo() << " values are retained as start values for future scans or";
        qInfo() << " minimizations. The curve resulting from each scan is plotted";
        qInfo() << " on the output unit in order to show the approximate behaviour";
        qInfo() << " of the function.";
        qInfo() << " This command is not intended for minimization, but is sometimes";
        qInfo() << " useful for debugging the user function or finding a";
        qInfo() << " reasonable starting point.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command SEEK
    //*-* --  ============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"SEE",3) ) {
        qInfo() << " ***>SEEk  [maxcalls]  [devs]";
        qInfo() << " Causes a Monte Carlo minimization of the function, by choosing";
        qInfo() << " random values of the variable parameters, chosen uniformly";
        qInfo() << " over a hypercube centered at the current best value.";
        qInfo() << " The region size is by default 3 standard deviations on each";
        qInfo() << " side, but can be changed by specifying the value of [devs].";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command SET
    //*-* --  ===========
    //*-* .
    if( !strncmp(comd.toStdString().data(),"SET",3) ) {
        qInfo() << " ***>SET <option_name>";
        qInfo() << "  SET BATch";
        qInfo() << "    Informs Minuit that it is running in batch mode.";

        qInfo() << " ";
        qInfo() << "  SET EPSmachine  <accuracy>";
        qInfo() << "    Informs Minuit that the relative floating point arithmetic";
        qInfo() << "    precision is <accuracy>. Minuit determines the nominal";
        qInfo() << "    precision itself, but the SET EPSmachine command can be";
        qInfo() << "    used to override Minuit own determination, when the user";
        qInfo() << "    knows that the FCN function value is not calculated to";
        qInfo() << "    the nominal machine accuracy. Typical values of <accuracy>";
        qInfo() << "    are between 10**-5 and 10**-14.";

        qInfo() << " ";
        qInfo() << "  SET ERRordef  <up>";
        qInfo() << "    Sets the value of UP (default value= 1.), defining parameter";
        qInfo() << "    errors. Minuit defines parameter errors as the change";
        qInfo() << "    in parameter value required to change the function value";
        qInfo() << "    by UP. Normally, for chisquared fits UP=1, and for negative";
        qInfo() << "    log likelihood, UP=0.5.";

        qInfo() << " ";
        qInfo() << "   SET GRAdient  [force]";
        qInfo() << "    Informs Minuit that the user function is prepared to";
        qInfo() << "    calculate its own first derivatives and return their values";
        qInfo() << "    in the array GRAD when IFLAG=2 (see specs of FCN).";
        qInfo() << "    If [force] is not specified, Minuit will calculate";
        qInfo() << "    the FCN derivatives by finite differences at the current";
        qInfo() << "    point and compare with the user calculation at that point,";
        qInfo() << "    accepting the user values only if they agree.";
        qInfo() << "    If [force]=1, Minuit does not do its own derivative";
        qInfo() << "    calculation, and uses the derivatives calculated in FCN.";

        qInfo() << " ";
        qInfo() << "   SET INPut  [unitno]  [filename]";
        qInfo() << "    Causes Minuit, in data-driven mode only, to read subsequent";
        qInfo() << "    commands (or parameter definitions) from a different input";
        qInfo() << "    file. If no [unitno] is specified, reading reverts to the";
        qInfo() << "    previous input file, assuming that there was one.";
        qInfo() << "    If [unitno] is specified, and that unit has not been opened,";
        qInfo() << "    then Minuit attempts to open the file [filename]} if a";
        qInfo() << "    name is specified. If running in interactive mode and";
        qInfo() << "    [filename] is not specified and [unitno] is not opened,";
        qInfo() << "    Minuit prompts the user to enter a file name.";
        qInfo() << "    If the word REWIND is added to the command (note:no blanks";
        qInfo() << "    between INPUT and REWIND), the file is rewound before";
        qInfo() << "    reading. Note that this command is implemented in standard";
        qInfo() << "    Fortran 77 and the results may depend on the  system;";
        qInfo() << "    for example, if a filename is given under VM/CMS, it must";
        qInfo() << "    be preceded by a slash.";

        qInfo() << " ";
        qInfo() << "   SET INTeractive";
        qInfo() << "    Informs Minuit that it is running interactively.";

        qInfo() << " ";
        qInfo() << "   SET LIMits  [parno]  [lolim]  [uplim]";
        qInfo() << "    Allows the user to change the limits on one or all";
        qInfo() << "    parameters. If no arguments are specified, all limits are";
        qInfo() << "    removed from all parameters. If [parno] alone is specified,";
        qInfo() << "    limits are removed from parameter [parno].";
        qInfo() << "    If all arguments are specified, then parameter [parno] will";
        qInfo() << "    be bounded between [lolim] and [uplim].";
        qInfo() << "    Limits can be specified in either order, Minuit will take";
        qInfo() << "    the smaller as [lolim] and the larger as [uplim].";
        qInfo() << "    However, if [lolim] is equal to [uplim], an error condition";
        qInfo() << "    results.";

        qInfo() << " ";
        qInfo() << "   SET LINesperpage";
        qInfo() << "     Sets the number of lines for one page of output.";
        qInfo() << "     Default value is 24 for interactive mode";

        qInfo() << " ";
        qInfo() << "   SET NOGradient";
        qInfo() << "    The inverse of SET GRAdient, instructs Minuit not to";
        qInfo() << "    use the first derivatives calculated by the user in FCN.";

        qInfo() << " ";
        qInfo() << "   SET NOWarnings";
        qInfo() << "    Supresses Minuit warning messages.";

        qInfo() << " ";
        qInfo() << "   SET OUTputfile  <unitno>";
        qInfo() << "    Instructs Minuit to write further output to unit <unitno>.";

        qInfo() << " ";
        qInfo() << "   SET PAGethrow  <integer>";
        qInfo() << "    Sets the carriage control character for ``new page'' to";
        qInfo() << "    <integer>. Thus the value 1 produces a new page, and 0";
        qInfo() << "    produces a blank line, on some devices (see TOPofpage)";


        qInfo() << " ";
        qInfo() << "   SET PARameter  <parno>  <value>";
        qInfo() << "    Sets the value of parameter <parno> to <value>.";
        qInfo() << "    The parameter in question may be variable, fixed, or";
        qInfo() << "    constant, but must be defined.";

        qInfo() << " ";
        qInfo() << "   SET PRIntout  <level>";
        qInfo() << "    Sets the print level, determining how much output will be";
        qInfo() << "    produced. Allowed values and their meanings are displayed";
        qInfo() << "    after a SHOw PRInt command, and are currently <level>=:";
        qInfo() << "      [-1]  no output except from SHOW commands";
        qInfo() << "       [0]  minimum output";
        qInfo() << "       [1]  default value, normal output";
        qInfo() << "       [2]  additional output giving intermediate results.";
        qInfo() << "       [3]  maximum output, showing progress of minimizations.";
        qInfo() << "    Note: See also the SET WARnings command.";

        qInfo() << " ";
        qInfo() << "   SET RANdomgenerator  <seed>";
        qInfo() << "    Sets the seed of the random number generator used in SEEk.";
        qInfo() << "    This can be any integer between 10000 and 900000000, for";
        qInfo() << "    example one which was output from a SHOw RANdom command of";
        qInfo() << "    a previous run.";

        qInfo() << " ";
        qInfo() << "   SET STRategy  <level>";
        qInfo() << "    Sets the strategy to be used in calculating first and second";
        qInfo() << "    derivatives and in certain minimization methods.";
        qInfo() << "    In general, low values of <level> mean fewer function calls";
        qInfo() << "    and high values mean more reliable minimization.";
        qInfo() << "    Currently allowed values are 0, 1 (default), and 2.";

        qInfo() << " ";
        qInfo() << "   SET TITle";
        qInfo() << "    Informs Minuit that the next input line is to be considered";
        qInfo() << "    the (new) title for this task or sub-task.  This is for";
        qInfo() << "    the convenience of the user in reading their output.";

        qInfo() << " ";
        qInfo() << "   SET WARnings";
        qInfo() << "    Instructs Minuit to output warning messages when suspicious";
        qInfo() << "    conditions arise which may indicate unreliable results.";
        qInfo() << "    This is the default.";

        qInfo() << " ";
        qInfo() << "    SET WIDthpage";
        qInfo() << "    Informs Minuit of the output page width.";
        qInfo() << "    Default values are 80 for interactive jobs";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command SHOW
    //*-* --  ============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"SHO",3) ) {
        qInfo() << " ***>SHOw  <option_name>";
        qInfo() << "  All SET XXXX commands have a corresponding SHOw XXXX command.";
        qInfo() << "  In addition, the SHOw commands listed starting here have no";
        qInfo() << "  corresponding SET command for obvious reasons.";

        qInfo() << " ";
        qInfo() << "   SHOw CORrelations";
        qInfo() << "    Calculates and prints the parameter correlations from the";
        qInfo() << "    error matrix.";

        qInfo() << " ";
        qInfo() << "   SHOw COVariance";
        qInfo() << "    Prints the (external) covariance (error) matrix.";

        qInfo() << " ";
        qInfo() << "   SHOw EIGenvalues";
        qInfo() << "    Calculates and prints the eigenvalues of the covariance";
        qInfo() << "    matrix.";

        qInfo() << " ";
        qInfo() << "   SHOw FCNvalue";
        qInfo() << "    Prints the current value of FCN.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command SIMPLEX
    //*-* --  ===============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"SIM",3) ) {
        qInfo() << " ***>SIMplex  [maxcalls]  [tolerance]";
        qInfo() << " Performs a function minimization using the simplex method of";
        qInfo() << " Nelder and Mead. Minimization terminates either when the";
        qInfo() << " function has been called (approximately) [maxcalls] times,";
        qInfo() << " or when the estimated vertical distance to minimum (EDM) is";
        qInfo() << " less than [tolerance].";
        qInfo() << " The default value of [tolerance] is 0.1*UP(see SET ERRordef).";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command STANDARD
    //*-* --  ================
    //*-* .
    if( !strncmp(comd.toStdString().data(),"STA",3) ) {
        qInfo() << " ***>STAndard";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command STOP
    //*-* --  ============
    //*-* .
    if( !strncmp(comd.toStdString().data(),"STO",3) ) {
        qInfo() << " ***>STOP";
        qInfo() << " Same as EXIT.";
        goto L99;
    }
    //*-* __________________________________________________________________
    //*-* --
    //*-* --  Command TOPOFPAGE
    //*-* --  =================
    //*-* .
    if( !strncmp(comd.toStdString().data(),"TOP",3) ) {
        qInfo() << " ***>TOPofpage";
        qInfo() << " Causes Minuit to write the character specified in a";
        qInfo() << " SET PAGethrow command (default = 1) to column 1 of the output";
        qInfo() << " file, which may or may not position your output medium to";
        qInfo() << " the top of a page depending on the device and system.";
        goto L99;
    }
    //*-* __________________________________________________________________
    qInfo() << " Unknown MINUIT command. Type HELP for list of commands.";

L99:
    return;
}

////////////////////////////////////////////////////////////////////////////////
///interface to Minuit help

void QMinuit::qmnhelp(const char *command)
{
    QString comd = command;
    qmnhelp(comd);

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*Calculates the full second-derivative matrix of FCN*-*-*-*-*-*-*-*
///*-*        ===================================================
///*-*        by taking finite differences. When calculating diagonal
///*-*        elements, it may iterate so that step size is nearly that
///*-*        which gives function change= UP/10. The first derivatives
///*-*        of course come as a free side effect, but with a smaller
///*-*        step size in order to obtain a known accuracy.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnhess()
{
    /* Local variables */
    double dmin_, dxdi, elem, wint, tlrg2, d, dlast, ztemp, g2bfor;
    double df, aimsag, fs1, tlrstp, fs2, stpinm, g2i, sag=0, xtf, xti, xtj;
    qint32 icyc, ncyc, ndex, idrv, iext, npar2, i, j, ifail, npard, nparx, id, multpy;
    bool ldebug;

    ldebug = mIdbg[3] >= 1;
    if (mAmin == mUndefi) {
       qmnamin();
    }
    if (mIstrat <= 0) {
       ncyc   = 3;
       tlrstp = .5;
       tlrg2  = .1;
    } else if (mIstrat == 1) {
       ncyc   = 5;
       tlrstp = .3;
       tlrg2  = .05;
    } else {
       ncyc   = 7;
       tlrstp = .1;
       tlrg2  = .02;
    }
    if (mISW[4] >= 2 || ldebug) {
       qInfo() << "   START COVARIANCE MATRIX CALCULATION.";
    }
    mCfrom  = "HESSE   ";
    mNfcnfr = mNfcn;
    mCstatu = "OK        ";
    npard   = mNPar;
 //*-*-                make sure starting at the right place
    qmninex(mX);
    nparx = mNPar;
    Eval(nparx, mGin, fs1, mU, 4);    ++mNfcn;
    if (fs1 != mAmin) {
       df    = mAmin - fs1;
       QString ctempo = QString("function value differs from AMIN %1").arg(df);
       qmnwarn("D", "MNHESS", ctempo.toStdString().data());
    }
    mAmin = fs1;
    if (ldebug) {
       qDebug() << " PAR D   GSTEP           D          G2         GRD         SAG    ";
    }
 //*-*-                                       . . . . . . diagonal elements .

 //*-*-        mISW[1] = 1 if approx, 2 if not posdef, 3 if ok
 //*-*-        AIMSAG is the sagitta we are aiming for in second deriv calc.

    aimsag = qSqrt(mEpsma2)*(qAbs(mAmin) + mUp);
 //*-*-        Zero the second derivative matrix
    npar2 = mNPar*(mNPar + 1) / 2;
    for (i = 1; i <= npar2; ++i) { mVhmat[i-1] = 0; }

 //*-*-        Loop over variable parameters for second derivatives
    idrv = 2;
    for (id = 1; id <= npard; ++id) {
       i = id + mNPar - npard;
       iext = mNexofi[i-1];
       if (mG2[i-1] == 0) {
          QString ctempo = QString("Second derivative enters zero, param %1").arg(iext);
          qmnwarn("W", "HESSE", ctempo.toStdString().data());
          wint = mWerr[i-1];
          if (mNvarl[iext-1] > 1) {
             qmndxdi(mX[i-1], i-1, dxdi);
             if (qAbs(dxdi) < .001) wint = .01;
             else                          wint /= qAbs(dxdi);
          }
          mG2[i-1] = mUp / (wint*wint);
       }
       xtf   = mX[i-1];
       dmin_ = mEpsma2*8*qAbs(xtf);

 //*-*-                              find step which gives sagitta = AIMSAG
       d = qAbs(mGstep[i-1]);
       int skip50 = 0;
       for (icyc = 1; icyc <= ncyc; ++icyc) {
 //*-*-                              loop here only if SAG=0
          for (multpy = 1; multpy <= 5; ++multpy) {
 //*-*-          take two steps
             mX[i-1] = xtf + d;
             qmninex(mX);
             nparx = mNPar;
             Eval(nparx, mGin, fs1, mU, 4);    ++mNfcn;
             mX[i-1] = xtf - d;
             qmninex(mX);
             Eval(nparx, mGin, fs2, mU, 4);    ++mNfcn;
             mX[i-1] = xtf;
             sag = (fs1 + fs2 - mAmin*2)*.5;
             if (sag != 0) goto L30;
             if (mGstep[i-1] < 0) {
                if (d >= .5) goto L26;
                d *= 10;
                if (d > .5)         d = .51;
                continue;
             }
             d *= 10;
          }
 L26:
          qmnwarn("W", "HESSE", QString("Second derivative zero for parameter%1").arg(iext).toStdString().data());
          goto L390;
 //*-*-                            SAG is not zero
 L30:
          g2bfor    = mG2[i-1];
          mG2[i-1]  = sag*2 / (d*d);
          mGrd[i-1] = (fs1 - fs2) / (d*2);
          if (ldebug) {
             qDebug() << i << idrv << mGstep[i-1] << d << mG2[i-1] << mGrd[i-1] << sag;
          }
          if (mGstep[i-1] > 0) mGstep[i-1] =  qAbs(d);
          else                 mGstep[i-1] = -qAbs(d);
          mDirin[i-1] = d;
          mHESSyy[i-1]= fs1;
          dlast       = d;
          d           = qSqrt(aimsag*2 / qAbs(mG2[i-1]));
 //*-*-        if parameter has limits, max int step size = 0.5
          stpinm = .5;
          if (mGstep[i-1] < 0) d = qMin(d,stpinm);
          if (d < dmin_) d = dmin_;
 //*-*-          see if converged
          if (qAbs((d - dlast) / d) < tlrstp ||
             qAbs((mG2[i-1] - g2bfor) / mG2[i-1]) < tlrg2) {
             skip50 = 1;
             break;
          }
          d = qMin(d,dlast*102);
          d = qMax(d,dlast*.1);
       }
 //*-*-                      end of step size loop
       if (!skip50) {
        QString  ctempo = QString("Second Deriv. SAG,AIM=%1%2%3").arg(iext).arg(sag).arg(aimsag);
          qmnwarn("D", "MNHESS", ctempo.toStdString().data());
        }
       ndex = i*(i + 1) / 2;
       mVhmat[ndex-1] = mG2[i-1];
    }
 //*-*-                             end of diagonal second derivative loop
    qmninex(mX);
 //*-*-                                    refine the first derivatives
    if (mIstrat > 0) qmnhes1();
    mISW[1] = 3;
    mDcovar = 0;
 //*-*-                                       . . . .  off-diagonal elements

    if (mNPar == 1) goto L214;
    for (i = 1; i <= mNPar; ++i) {
       for (j = 1; j <= i-1; ++j) {
          xti     = mX[i-1];
          xtj     = mX[j-1];
          mX[i-1] = xti + mDirin[i-1];
          mX[j-1] = xtj + mDirin[j-1];
          qmninex(mX);
          Eval(nparx, mGin, fs1, mU, 4);            ++mNfcn;
          mX[i-1] = xti;
          mX[j-1] = xtj;
          elem = (fs1 + mAmin - mHESSyy[i-1] - mHESSyy[j-1]) / (
                     mDirin[i-1]*mDirin[j-1]);
          ndex = i*(i-1) / 2 + j;
          mVhmat[ndex-1] = elem;
       }
    }
 L214:
    qmninex(mX);
 //*-*-                 verify matrix positive-definite
    qmnpsdf();
    for (i = 1; i <= mNPar; ++i) {
       for (j = 1; j <= i; ++j) {
          ndex = i*(i-1) / 2 + j;
          mP[i + j*mMaxPar - mMaxPar-1] = mVhmat[ndex-1];
          mP[j + i*mMaxPar - mMaxPar-1] = mP[i + j*mMaxPar - mMaxPar-1];
       }
    }
    qmnvert(mP, mMaxInt, mMaxInt, mNPar, ifail);
    if (ifail > 0) {
       qmnwarn("W", "HESSE", "Matrix inversion fails.");
       goto L390;
    }
 //*-*-                                       . . . . . . .  calculate  e d m
    mEDM = 0;

    for (i = 1; i <= mNPar; ++i) {
 //*-*-                             off-diagonal elements
       ndex = i*(i-1) / 2;
       for (j = 1; j <= i-1; ++j) {
          ++ndex;
          ztemp = mP[i + j*mMaxPar - mMaxPar-1]*2;
          mEDM += mGrd[i-1]*ztemp*mGrd[j-1];
          mVhmat[ndex-1] = ztemp;
       }
 //*-*-                             diagonal elements
       ++ndex;
       mVhmat[ndex-1] = mP[i + i*mMaxPar - mMaxPar-1]*2;
       mEDM += mP[i + i*mMaxPar - mMaxPar-1]*(mGrd[i-1]*mGrd[i-1]);
    }
    if (mISW[4] >= 1 && mISW[1] == 3 && mItaur == 0) {
       qInfo() << " COVARIANCE MATRIX CALCULATED SUCCESSFULLY";
    }
    goto L900;
 //*-*-                             failure to invert 2nd deriv matrix
 L390:
    mISW[1] = 1;
    mDcovar = 1;
    mCstatu = "FAILED    ";
    if (mISW[4] >= 0) {
       qInfo() << "  MNHESS FAILS AND WILL RETURN DIAGONAL MATRIX. ";
    }
    for (i = 1; i <= mNPar; ++i) {
       ndex = i*(i-1) / 2;
       for (j = 1; j <= i-1; ++j) {
          ++ndex;
          mVhmat[ndex-1] = 0;
       }
       ++ndex;
       g2i = mG2[i-1];
       if (g2i <= 0) g2i = 1;
       mVhmat[ndex-1] = 2 / g2i;
    }
 L900:
    return;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Calculate first derivatives (GRD) and uncertainties (DGRD)*-*-*-*-*-*
///*-*    ==========================================================
///*-*         and appropriate step sizes GSTEP
///*-*      Called from MNHESS and MNGRAD
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnhes1()
{
    /* Local variables */
    double dmin_, d, dfmin, dgmin=0, change, chgold, grdold=0, epspri;
    double fs1, optstp, fs2, grdnew=0, sag, xtf;
    qint32 icyc, ncyc=0, idrv, i, nparx;
    bool ldebug;

    ldebug = mIdbg[5] >= 1;
    if (mIstrat <= 0) ncyc = 1;
    if (mIstrat == 1) ncyc = 2;
    if (mIstrat > 1)  ncyc = 6;
    idrv = 1;
    nparx = mNPar;
    dfmin = mEpsma2*4*(qAbs(mAmin) + mUp);
 //*-*-                                    main loop over parameters
    for (i = 1; i <= mNPar; ++i) {
       xtf    = mX[i-1];
       dmin_  = mEpsma2*4*qAbs(xtf);
       epspri = mEpsma2 + qAbs(mGrd[i-1]*mEpsma2);
       optstp = qSqrt(dfmin / (qAbs(mG2[i-1]) + epspri));
       d = qAbs(mGstep[i-1])*.2;
       if (d > optstp) d = optstp;
       if (d < dmin_)  d = dmin_;
       chgold = 1e4;
 //*-*-                                      iterate reducing step size
       for (icyc = 1; icyc <= ncyc; ++icyc) {
          mX[i-1] = xtf + d;
          qmninex(mX);
          Eval(nparx, mGin, fs1, mU, 4);            ++mNfcn;
          mX[i-1] = xtf - d;
          qmninex(mX);
          Eval(nparx, mGin, fs2, mU, 4);            ++mNfcn;
          mX[i-1] = xtf;
 //*-*-                                      check if step sizes appropriate
          sag    = (fs1 + fs2 - mAmin*2)*.5;
          grdold = mGrd[i-1];
          grdnew = (fs1 - fs2) / (d*2);
          dgmin  = mEpsmac*(qAbs(fs1) + qAbs(fs2)) / d;
          if (ldebug) {
             qDebug() << i << idrv << mGstep[i-1] << d << mG2[i-1] << grdnew << sag;
          }
          if (grdnew == 0) goto L60;
          change = qAbs((grdold - grdnew) / grdnew);
          if (change > chgold && icyc > 1) goto L60;
          chgold    = change;
          mGrd[i-1] = grdnew;
          if (mGstep[i-1] > 0) mGstep[i-1] =  qAbs(d);
          else                 mGstep[i-1] = -qAbs(d);
 //*-*-                 decrease step until first derivative changes by <5%
          if (change < .05) goto L60;
          if (qAbs(grdold - grdnew) < dgmin) goto L60;
          if (d < dmin_) {
             qmnwarn("D", "MNHES1", "Step size too small for 1st drv.");
             goto L60;
          }
          d *= .2;
       }
 //*-*-                                      loop satisfied = too many iter
       qmnwarn("D", "MNHES1", QString("Too many iterations on D1.%1%1").arg(grdold).arg(grdnew).toStdString().data());
 L60:
       mDgrd[i-1] = qMax(dgmin,qAbs(grdold - grdnew));
    }
 //*-*-                                       end of first deriv. loop
    qmninex(mX);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*Attempts to improve on a good local minimum*-*-*-*-*-*-*-*-*-*
///*-*          ===========================================
///*-*        Attempts to improve on a good local minimum by finding a
///*-*        better one.   The quadratic part of FCN is removed by qmncalf
///*-*        and this transformed function is minimized using the simplex
///*-*        method from several random starting points.
///*-*        ref. -- Goldstein and Price, Math.Comp. 25, 569 (1971)
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnimpr()
{
    /* Initialized data */

     static double rnum = 0;

     /* Local variables */
     double amax, ycalf, ystar, ystst;
     double pb, ep, wg, xi, sigsav, reg, sig2;
     qint32 npfn, ndex, loop=0, i, j, ifail, iseed=0;
     qint32 jhold, nloop, nparx, nparp1, jh, jl, iswtr;

     if (mNPar <= 0) return;
     if (mAmin == mUndefi) qmnamin();
     mCstatu = "UNCHANGED ";
     mItaur  = 1;
     mEpsi   = mUp*.1;
     npfn    = mNfcn;
     nloop   = qint32(mWord7[1]);
     if (nloop <= 0) nloop = mNPar + 4;
     nparx  = mNPar;
     nparp1 = mNPar + 1;
     wg = 1 / double(mNPar);
     sigsav = mEDM;
     mApsi  = mAmin;
     iswtr   = mISW[4] - 2*mItaur;
     for (i = 1; i <= mNPar; ++i) {
        mXt[i-1]       = mX[i-1];
        mIMPRdsav[i-1] = mWerr[i-1];
        for (j = 1; j <= i; ++j) {
           ndex = i*(i-1) / 2 + j;
           mP[i + j*mMaxPar - mMaxPar-1] = mVhmat[ndex-1];
           mP[j + i*mMaxPar - mMaxPar-1] = mP[i + j*mMaxPar - mMaxPar-1];
        }
     }
     qmnvert(mP, mMaxInt, mMaxInt, mNPar, ifail);
     if (ifail >= 1) goto L280;
  //*-*-              Save inverted matrix in VT
     for (i = 1; i <= mNPar; ++i) {
        ndex = i*(i-1) / 2;
        for (j = 1; j <= i; ++j) {
           ++ndex;
           mVthmat[ndex-1] = mP[i + j*mMaxPar - mMaxPar-1];
        }
     }
     loop = 0;

  L20:
     for (i = 1; i <= mNPar; ++i) {
        mDirin[i-1] = mIMPRdsav[i-1]*2;
        qmnrn15(rnum, iseed);
        mX[i-1] = mXt[i-1] + mDirin[i-1]*2*(rnum - .5);
     }
     ++loop;
     reg = 2;
     if (mISW[4] >= 0) {
        qInfo() << "START ATTEMPT NO." << loop << "TO FIND NEW MINIMUM";
     }
  L30:
     qmncalf(mX, ycalf);
     mAmin = ycalf;
  //*-*-                                       . . . . set up  random simplex
     jl = nparp1;
     jh = nparp1;
     mIMPRy[nparp1-1] = mAmin;
     amax = mAmin;
     for (i = 1; i <= mNPar; ++i) {
        xi = mX[i-1];
        qmnrn15(rnum, iseed);
        mX[i-1] = xi - mDirin[i-1]*(rnum - .5);
        qmncalf(mX, ycalf);
        mIMPRy[i-1] = ycalf;
        if (mIMPRy[i-1] < mAmin) {
           mAmin = mIMPRy[i-1];
           jl    = i;
        } else if (mIMPRy[i-1] > amax) {
           amax = mIMPRy[i-1];
           jh   = i;
        }
        for (j = 1; j <= mNPar; ++j) { mP[j + i*mMaxPar - mMaxPar-1] = mX[j-1]; }
        mP[i + nparp1*mMaxPar - mMaxPar-1] = xi;
        mX[i-1] = xi;
     }

     mEDM = mAmin;
     sig2 = mEDM;
  //*-*-                                       . . . . . . .  start main loop
  L50:
     if (mAmin < 0)   goto L95;
     if (mISW[1] <= 2) goto L280;
     ep = mAmin*.1;
     if (sig2 < ep && mEDM < ep) goto L100;
     sig2 = mEDM;
     if (mNfcn - npfn > mNfcnmx) goto L300;
  //*-*-        calculate new point * by reflection
     for (i = 1; i <= mNPar; ++i) {
        pb = 0;
        for (j = 1; j <= nparp1; ++j) { pb += wg*mP[i + j*mMaxPar - mMaxPar-1]; }
        mPbar[i-1]  = pb - wg*mP[i + jh*mMaxPar - mMaxPar-1];
        mPstar[i-1] = mPbar[i-1]*2 - mP[i + jh*mMaxPar - mMaxPar-1]*1;
     }
     qmncalf(mPstar, ycalf);
     ystar = ycalf;
     if (ystar >= mAmin) goto L70;
  //*-*-        point * better than jl, calculate new point **
     for (i = 1; i <= mNPar; ++i) {
        mPstst[i-1] = mPstar[i-1]*2 + mPbar[i- 1]*-1;
     }
     qmncalf(mPstst, ycalf);
     ystst = ycalf;
     if (ystst < mIMPRy[jl-1]) goto L67;
     qmnrazz(ystar, mPstar, mIMPRy, jh, jl);
     goto L50;
  L67:
     qmnrazz(ystst, mPstst, mIMPRy, jh, jl);
     goto L50;
  //*-*-        point * is not as good as jl
  L70:
     if (ystar >= mIMPRy[jh-1]) goto L73;
     jhold = jh;
     qmnrazz(ystar, mPstar, mIMPRy, jh, jl);
     if (jhold != jh) goto L50;
  //*-*-        calculate new point **
  L73:
     for (i = 1; i <= mNPar; ++i) {
        mPstst[i-1] = mP[i + jh*mMaxPar - mMaxPar-1]*.5 + mPbar[i-1]*.5;
     }
     qmncalf(mPstst, ycalf);
     ystst = ycalf;
     if (ystst > mIMPRy[jh-1]) goto L30;
  //*-*-    point ** is better than jh
     if (ystst < mAmin) goto L67;
     qmnrazz(ystst, mPstst, mIMPRy, jh, jl);
     goto L50;
  //*-*-                                       . . . . . .  end main loop
  L95:
     if (mISW[4] >= 0) {
        qInfo() << " AN IMPROVEMENT ON THE PREVIOUS MINIMUM HAS BEEN FOUND";
     }
     reg = .1;
  //*-*-                                       . . . . . ask if point is new
  L100:
     qmninex(mX);
     Eval(nparx, mGin, mAmin, mU, 4);    ++mNfcn;
     for (i = 1; i <= mNPar; ++i) {
        mDirin[i-1] = reg*mIMPRdsav[i-1];
        if (qAbs(mX[i-1] - mXt[i-1]) > mDirin[i-1])     goto L150;
     }
     goto L230;
  L150:
     mNfcnmx = mNfcnmx + npfn - mNfcn;
     npfn    = mNfcn;
     qmnsimp();
     if (mAmin >= mApsi) goto L325;
     for (i = 1; i <= mNPar; ++i) {
        mDirin[i-1] = mIMPRdsav[i-1]*.1;
        if (qAbs(mX[i-1] - mXt[i-1]) > mDirin[i-1])     goto L250;
     }
  L230:
     if (mAmin < mApsi)         goto L350;
     goto L325;
  /*                                        . . . . . . truly new minimum */
  L250:
     mLnewmn = true;
     if (mISW[1] >= 1) {
        mISW[1] = 1;
        mDcovar = qMax(mDcovar,.5);
     } else mDcovar = 1;
     mItaur  = 0;
     mNfcnmx = mNfcnmx + npfn - mNfcn;
     mCstatu = "NEW MINIMU";
     if (mISW[4] >= 0) {
        qInfo() << " IMPROVE HAS FOUND A TRULY NEW MINIMUM";
        qInfo() << " *************************************";
     }
     return;
  //*-*-                                       . . . return to previous region
  L280:
     if (mISW[4] > 0) {
        qInfo() << " COVARIANCE MATRIX WAS NOT POSITIVE-DEFINITE";
     }
     goto L325;
  L300:
     mISW[0] = 1;
  L325:
     for (i = 1; i <= mNPar; ++i) {
        mDirin[i-1] = mIMPRdsav[i-1]*.01;
        mX[i-1]     = mXt[i-1];
     }
     mAmin = mApsi;
     mEDM  = sigsav;
  L350:
     qmninex(mX);
     if (mISW[4] > 0) {
        qInfo() << " IMPROVE HAS RETURNED TO REGION OF ORIGINAL MINIMUM";
     }
     mCstatu = "UNCHANGED ";
     qmnrset(0);
     if (mISW[1] < 2) goto L380;
     if (loop < nloop && mISW[0] < 1) goto L20;
  L380:
     if (iswtr >= 0) qmnprin(5, mAmin);
     mItaur = 0;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Transforms from internal coordinates (PINT) to external (U)*-*-*-*
///*-*      ===========================================================
///*-*        The minimizing routines which work in
///*-*        internal coordinates call this routine before calling FCN.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmninex(double *pint)
{
    qint32 i, j;

    for (j = 0; j < mNPar; ++j) {
       i = mNexofi[j]-1;
       if (mNvarl[i] == 1) {
          mU[i] = pint[j];
       } else {
          mU[i] = mAlim[i] + (qSin(pint[j]) + 1)*.5*(mBlim[i] - mAlim[i]);
       }
    }

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*Main initialization member function for MINUIT*-*-*-*-*-*-*-*-*
///*-*        ==============================================
///*-*     It initializes some constants
///*-*                (including the logical I/O unit nos.),
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmninit(qint32 i1, qint32 i2, qint32 i3)
{
    /* Local variables */
    volatile double epsp1;
    double piby2, epstry, epsbak, distnn;
    qint32 i, idb;

 //*-*-           I/O unit numbers
    mIsysrd = i1;
    mIsyswr = i2;
    mIstkwr[0] = mIsyswr;
    mNstkwr = 1;
    mIsyssa = i3;
    mNstkrd = 0;
 //*-*-              version identifier
    mCvrsn  = "95.03++ ";
 //*-*-              some CONSTANT
    mMaxInt= mMaxPar;
    mMaxExtr = 2*mMaxPar;
    mUndefi = -54321;
    mBigedm = 123456;
    mCundef = ")UNDEFINED";
    mCovmes[0] = "NO ERROR MATRIX       ";
    mCovmes[1] = "ERR MATRIX APPROXIMATE";
    mCovmes[2] = "ERR MATRIX NOT POS-DEF";
    mCovmes[3] = "ERROR MATRIX ACCURATE ";
 //*-*-               some starting values
    mNblock = 0;
    mIcomnd = 0;
    mCtitl  = mCundef;
    mCfrom  = "INPUT   ";
    mNfcn   = 0;
    mNfcnfr = mNfcn;
    mCstatu = "INITIALIZE";
    mISW[2] = 0;
    mISW[3] = 0;
    mISW[4] = 1;
 //*-*-        mISW[5]=0 for batch jobs,  =1 for interactive jobs
 //*-*-                     =-1 for originally interactive temporarily batch

    mISW[5] = 0;
 //    if (intrac(&dummy)) mISW[5] = 1;
 //*-*-       DEBUG options set to default values
    for (idb = 0; idb <= 10; ++idb) { mIdbg[idb] = 0; }
    mLrepor = false;
    mLwarn  = true;
    mLimset = false;
    mLnewmn = false;
    mIstrat = 1;
    mItaur  = 0;
 //*-*-       default page dimensions and 'new page' carriage control integer
    mNpagwd = 120;
    mNpagln = 56;
    mNewpag = 1;
    if (mISW[5] > 0) {
       mNpagwd = 80;
       mNpagln = 30;
       mNewpag = 0;
    }
    mUp = 1;
    mUpdflt = mUp;
 //*-*-                  determine machine accuracy epsmac
    epstry = .5;
    for (i = 1; i <= 100; ++i) {
       epstry *= .5;
       epsp1 = epstry + 1;
       qmntiny(epsp1, epsbak);
       if (epsbak < epstry) goto L35;
    }
    epstry = 1e-7;
    mEpsmac = epstry*4;
    qInfo() << Q_FUNC_INFO << " MNINIT UNABLE TO DETERMINE ARITHMETIC PRECISION. WILL ASSUME:" << mEpsmac;
 L35:
    mEpsmac = epstry*8;
    mEpsma2 = qSqrt(mEpsmac)*2;
 //*-*-                the vlims are a non-negligible distance from pi/2
 //*-*-        used by MNPqint32to set variables "near" the physical limits
    piby2   = qAtan(1.)*2;
    distnn  = qSqrt(mEpsma2)*8;
    mVlimhi =  piby2 - distnn;
    mVlimlo = -piby2 + distnn;
    qmncler();
    //    qInfo() << "  MINUIT RELEASE %s INITIALIZED.   DIMENSIONS 100/50  EPSMAC=%g",(const char*)mCvrsn,mEpsmac);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Interprets the SET LIM command, to reset the parameter limits*-*-*-*
///*-*    =============================================================
///*-*       Called from MNSET
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnlims()
{
    /* Local variables */
    double dxdi, snew;
    qint32 kint, i2, newcod, ifx=0, inu;

    mCfrom  = "SET LIM ";
    mNfcnfr = mNfcn;
    mCstatu = "NO CHANGE ";
    i2 = qint32(mWord7[0]);
    if (i2 > mMaxExtr || i2 < 0) goto L900;
    if (i2 > 0) goto L30;
 //*-*-                                    set limits on all parameters
    newcod = 4;
    if (mWord7[1] == mWord7[2]) newcod = 1;
    for (inu = 1; inu <= mNu; ++inu) {
       if (mNvarl[inu-1] <= 0) continue;
       if (mNvarl[inu-1] == 1 && newcod == 1) continue;
       kint = mNiofex[inu-1];
 //*-*-            see if parameter has been fixed
       if (kint <= 0) {
          if (mISW[4] >= 0) {
             qInfo() << "           LIMITS NOT CHANGED FOR FIXED PARAMETER:" << inu;
          }
          continue;
       }
       if (newcod == 1) {
 //*-*-           remove limits from parameter
          if (mISW[4] > 0) {
             qInfo() << " LIMITS REMOVED FROM PARAMETER  :" << inu;
          }
          mCstatu = "NEW LIMITS";
          qmndxdi(mX[kint-1], kint-1, dxdi);
          snew           = mGstep[kint-1]*dxdi;
          mGstep[kint-1] = qAbs(snew);
          mNvarl[inu-1]  = 1;
       } else {
 //*-*-            put limits on parameter
          mAlim[inu-1] = qMin(mWord7[1],mWord7[2]);
          mBlim[inu-1] = qMax(mWord7[1],mWord7[2]);
          if (mISW[4] > 0) {
             qInfo() << " PARAMETER" << inu << "LIMITS SET TO " << mAlim[inu-1] << mBlim[inu-1];
          }
          mNvarl[inu-1]  = 4;
          mCstatu        = "NEW LIMITS";
          mGstep[kint-1] = -.1;
       }
    }
    goto L900;
 //*-*-                                      set limits on one parameter
 L30:
    if (mNvarl[i2-1] <= 0) {
       qInfo() << " PARAMETER " << i2 << " IS NOT VARIABLE.";
       goto L900;
    }
    kint = mNiofex[i2-1];
 //*-*-                                      see if parameter was fixed
    if (kint == 0) {
       qInfo() << " REQUEST TO CHANGE LIMITS ON FIXED PARAMETER:" << i2;
       for (ifx = 1; ifx <= mNPFix; ++ifx) {
          if (i2 == mIpfix[ifx-1]) goto L92;
       }
       qInfo() << " MINUIT BUG IN MNLIMS. SEE F. JAMES";
 L92:
       ;
    }
    if (mWord7[1] != mWord7[2]) goto L235;
 //*-*-                                      remove limits
    if (mNvarl[i2-1] != 1) {
       if (mISW[4] > 0) {
          qInfo() << " LIMITS REMOVED FROM PARAMETER " << i2;
       }
       mCstatu = "NEW LIMITS";
       if (kint <= 0) {
          mGsteps[ifx-1] = qAbs(mGsteps[ifx-1]);
       } else {
          qmndxdi(mX[kint-1], kint-1, dxdi);
          if (qAbs(dxdi) < .01) dxdi = .01;
          mGstep[kint-1] = qAbs(mGstep[kint-1]*dxdi);
          mGrd[kint-1]  *= dxdi;
       }
       mNvarl[i2-1] = 1;
    } else {
       qInfo() << " NO LIMITS SPECIFIED.  PARAMETER " << i2 << " IS ALREADY UNLIMITED.  NO CHANGE.";
    }
    goto L900;
 //*-*-                                       put on limits
 L235:
    mAlim[i2-1]  = qMin(mWord7[1],mWord7[2]);
    mBlim[i2-1]  = qMax(mWord7[1],mWord7[2]);
    mNvarl[i2-1] = 4;
    if (mISW[4] > 0) {
       qInfo() << " PARAMETER " << i2 << " LIMITS SET TO " << mAlim[i2-1] << mBlim[i2-1];
    }
    mCstatu = "NEW LIMITS";
    if (kint <= 0) mGsteps[ifx-1] = -.1;
    else           mGstep[kint-1] = -.1;

 L900:
    if (mCstatu != "NO CHANGE ") {
       qmnexin(mX);
       qmnrset(1);
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*Perform a line search from position START*-*-*-*-*-*-*-*
///*-*                =========================================
///*-*        along direction STEP, where the length of vector STEP
///*-*                   gives the expected position of minimum.
///*-*        FSTART is value of function at START
///*-*        SLOPE (if non-zero) is df/dx along STEP at START
///*-*        TOLER is initial tolerance of minimum in direction STEP
///*-*
///*-* SLAMBG and ALPHA control the maximum individual steps allowed.
///*-* The first step is always =1. The max length of second step is SLAMBG.
///*-* The max size of subsequent steps is the maximum previous successful
///*-*   step multiplied by ALPHA + the size of most recent successful step,
///*-*   but cannot be smaller than SLAMBG.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnline(double *start, double fstart, double *step, double slope, double toler)
{
    /* Local variables */
    double xpq[12], ypq[12], slam, sdev, coeff[3], denom, flast;
    double fvals[3], xvals[3], f1, fvmin, xvmin, ratio, f2, f3 = 0., fvmax;
    double toler8, toler9, overal, undral, slamin, slamax, slopem;
    qint32 i, nparx=0, nvmax=0, nxypt, kk, ipt;
    bool ldebug;
    QString cmess;
    char chpq[13];
    int     l65, l70, l80;

    /* Function Body */
    l65 = 0; l70 = 0; l80 = 0;
    ldebug = mIdbg[1] >= 1;
 //*-*-                 starting values for overall limits on total step SLAM
    overal = 1e3;
    undral = -100;
 //*-*-                             debug check if start is ok
    if (ldebug) {
       qmninex(&start[0]);
       Eval(nparx, mGin, f1, mU, 4);        ++mNfcn;
       if (f1 != fstart) {
          qInfo() << " MNLINE start point not consistent, F values, parameters=";
          for (kk = 1; kk <= mNPar; ++kk) {
             qInfo() << mX[kk-1];
          }
       }
    }
 //*-*-                                     . set up linear search along STEP
    fvmin   = fstart;
    xvmin   = 0;
    nxypt   = 1;
    chpq[0] = charal[0];
    xpq[0]  = 0;
    ypq[0]  = fstart;
 //*-*-              SLAMIN = smallest possible value of ABS(SLAM)
    slamin = 0;
    for (i = 1; i <= mNPar; ++i) {
       if (step[i-1] != 0) {
          ratio = qAbs(start[i-1] / step[i-1]);
          if (slamin == 0)    slamin = ratio;
          if (ratio < slamin) slamin = ratio;
       }
       mX[i-1] = start[i-1] + step[i-1];
    }
    if (slamin == 0) slamin = mEpsmac;
    slamin *= mEpsma2;
    nparx = mNPar;

    qmninex(mX);
    Eval(nparx, mGin, f1, mU, 4);    ++mNfcn;
    ++nxypt;
    chpq[nxypt-1] = charal[nxypt-1];
    xpq[nxypt-1] = 1;
    ypq[nxypt-1] = f1;
    if (f1 < fstart) {
       fvmin = f1;
       xvmin = 1;
    }
 //*-*-                        . quadr interp using slope GDEL and two points
    slam   = 1;
    toler8 = toler;
    slamax = 5;
    flast  = f1;
 //*-*-                        can iterate on two-points (cut) if no imprvmnt

    do {
       denom = (flast - fstart - slope*slam)*2 / (slam*slam);
       slam  = 1;
       if (denom != 0)    slam = -slope / denom;
       if (slam < 0)      slam = slamax;
       if (slam > slamax) slam = slamax;
       if (slam < toler8) slam = toler8;
       if (slam < slamin) {
          l80 = 1;
          break;
       }
       if (qAbs(slam - 1) < toler8 && f1 < fstart) {
          l70 = 1;
          break;
       }
       if (qAbs(slam - 1) < toler8) slam = toler8 + 1;
       if (nxypt >= 12) {
          l65 = 1;
          break;
       }
       for (i = 1; i <= mNPar; ++i) { mX[i-1] = start[i-1] + slam*step[i-1]; }
       qmninex(mX);
       nparx = mNPar;
       Eval(nparx, mGin, f2, mU, 4);    ++mNfcn;
       ++nxypt;
       chpq[nxypt-1] = charal[nxypt-1];
       xpq[nxypt-1]  = slam;
       ypq[nxypt-1]  = f2;
       if (f2 < fvmin) {
          fvmin = f2;
          xvmin = slam;
       }
       if (fstart == fvmin) {
          flast  = f2;
          toler8 = toler*slam;
          overal = slam - toler8;
          slamax = overal;
       }
    } while (fstart == fvmin);

    if (!l65 && !l70 && !l80) {
 //*-*-                                       . quadr interp using 3 points
       xvals[0] = xpq[0];
       fvals[0] = ypq[0];
       xvals[1] = xpq[nxypt-2];
       fvals[1] = ypq[nxypt-2];
       xvals[2] = xpq[nxypt-1];
       fvals[2] = ypq[nxypt-1];
 //*-*-                            begin iteration, calculate desired step
       do {
          slamax = qMax(slamax,qAbs(xvmin)*2);
          qmnpfit(xvals, fvals, 3, coeff, sdev);
          if (coeff[2] <= 0) {
             slopem = coeff[2]*2*xvmin + coeff[1];
             if (slopem <= 0)  slam = xvmin + slamax;
             else              slam = xvmin - slamax;
          } else {
             slam = -coeff[1] / (coeff[2]*2);
             if (slam > xvmin + slamax) slam = xvmin + slamax;
             if (slam < xvmin - slamax) slam = xvmin - slamax;
          }
          if (slam > 0) {
             if (slam > overal)
                slam = overal;
             else if (slam < undral)
                slam = undral;
          }

 //*-*-              come here if step was cut below
          do {
             toler9 = qMax(toler8,qAbs(toler8*slam));
             for (ipt = 1; ipt <= 3; ++ipt) {
                if (qAbs(slam - xvals[ipt-1]) < toler9) {
                   l70 = 1;
                   break;
                }
             }
             if (l70) break;
 //*-*-               take the step
             if (nxypt >= 12) {
                l65 = 1;
                break;
             }
             for (i = 1; i <= mNPar; ++i) { mX[i-1] = start[i-1] + slam*step[i-1]; }
             qmninex(mX);
             Eval(nparx, mGin, f3, mU, 4);    ++mNfcn;
             ++nxypt;
             chpq[nxypt-1] = charal[nxypt-1];
             xpq[nxypt-1]  = slam;
             ypq[nxypt-1]  = f3;
 //*-*-            find worst previous point out of three
             fvmax = fvals[0];
             nvmax = 1;
             if (fvals[1] > fvmax) {
                fvmax = fvals[1];
                nvmax = 2;
             }
             if (fvals[2] > fvmax) {
                fvmax = fvals[2];
                nvmax = 3;
             }
 //*-*-             if latest point worse than all three previous, cut step
             if (f3 >= fvmax) {
                if (nxypt >= 12) {
                   l65 = 1;
                   break;
                }
                if (slam > xvmin) overal = qMin(overal,slam - toler8);
                if (slam < xvmin) undral = qMax(undral,slam + toler8);
                slam = (slam + xvmin)*.5;
             }
          } while (f3 >= fvmax);

 //*-*-             prepare another iteration, replace worst previous point
          if (l65 || l70) break;

          xvals[nvmax-1] = slam;
          fvals[nvmax-1] = f3;
          if (f3 < fvmin) {
             fvmin = f3;
             xvmin = slam;
          } else {
             if (slam > xvmin) overal = qMin(overal,slam - toler8);
             if (slam < xvmin) undral = qMax(undral,slam + toler8);
          }
       } while (nxypt < 12);
    }

 //*-*-                                           . . end of iteration . . .
 //*-*-           stop because too many iterations
    if (!l70 && !l80) {
       cmess = " LINE SEARCH HAS EXHAUSTED THE LIMIT OF FUNCTION CALLS ";
       if (ldebug) {
          qInfo() << " MNLINE DEBUG: steps=";
          for (kk = 1; kk <= mNPar; ++kk) {
             qInfo() << step[kk-1];
          }
       }
    }
 //*-*-           stop because within tolerance
    if (l70) cmess = " LINE SEARCH HAS ATTAINED TOLERANCE ";
    if (l80) cmess = " STEP SIZE AT ARITHMETICALLY ALLOWED MINIMUM";

    mAmin = fvmin;
    for (i = 1; i <= mNPar; ++i) {
       mDirin[i-1] = step[i-1]*xvmin;
       mX[i-1]     = start[i-1] + mDirin[i-1];
    }
    qmninex(mX);
    if (xvmin < 0) {
       qmnwarn("D", "MNLINE", " LINE MINIMUM IN BACKWARDS DIRECTION");
    }
    if (fvmin == fstart) {
       qmnwarn("D", "MNLINE", " LINE SEARCH FINDS NO IMPROVEMENT ");
    }
    if (ldebug) {
       qDebug() << " AFTER " << nxypt << " POINTS, "<< cmess;
       qmnplot(xpq, ypq, chpq, nxypt, mNpagwd, mNpagln);
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*Prints the covariance matrix v when KODE=1*-*-*-*-*-*-*-*-*
///*-*            ==========================================
///*-*        always prints the global correlations, and
///*-*        calculates and prints the individual correlation coefficients
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnmatu(qint32 kode)
{
    /* Local variables */
    qint32 ndex, i, j, m, n, ncoef, nparm, id, it, ix;
    qint32 nsofar, ndi, ndj, iso, isw2, isw5;
    QString ctemp;

    isw2 = mISW[1];
    if (isw2 < 1) {
       qInfo() << mCovmes[isw2];
       return;
    }
    if (mNPar == 0) {
       qInfo() << " MNMATU: NPAR=0";
       return;
    }
 //*-*-                                      . . . . .external error matrix
    if (kode == 1) {
       isw5    = mISW[4];
       mISW[4] = 2;
       qmnemat(mP, mMaxInt);
       if (isw2 < 3) {
          qInfo() << mCovmes[isw2];
       }
       mISW[4] = isw5;
    }
 //*-*-                                      . . . . . correlation coeffs. .
    if (mNPar <= 1) return;
    qmnwerr();
 //*-*-    NCOEF is number of coeff. that fit on one line, not to exceed 20
    ncoef = (mNpagwd - 19) / 6;
    ncoef = qMin(ncoef,20);
    nparm = qMin(mNPar,ncoef);
    qInfo() << " PARAMETER  CORRELATION COEFFICIENTS  ";
    ctemp = "       NO.  GLOBAL";
    for (id = 1; id <= nparm; ++id) {
       ctemp += QString(" %1").arg(mNexofi[id-1]);
    }
    qInfo() << ctemp;
    for (i = 1; i <= mNPar; ++i) {
       ix  = mNexofi[i-1];
       ndi = i*(i + 1) / 2;
       for (j = 1; j <= mNPar; ++j) {
          m    = qMax(i,j);
          n    = qMin(i,j);
          ndex = m*(m-1) / 2 + n;
          ndj  = j*(j + 1) / 2;
          mMATUvline[j-1] = mVhmat[ndex-1] / qSqrt(qAbs(mVhmat[ndi-1]*mVhmat[ndj-1]));
       }
       nparm = qMin(mNPar,ncoef);
       ctemp = QString("      %1  %2 ").arg(ix).arg(mGlobcc[i-1]);
       for (it = 1; it <= nparm; ++it) {
          ctemp += QString(" %1").arg(mMATUvline[it-1]);
       }
       qInfo() << ctemp;
       if (i <= nparm) continue;
       ctemp = "                   ";
       for (iso = 1; iso <= 10; ++iso) {
          nsofar = nparm;
          nparm  = qMin(mNPar,nsofar + ncoef);
          for (it = nsofar + 1; it <= nparm; ++it) {
             ctemp = ctemp + QString(" %1").arg(mMATUvline[it-1]);
          }
          qInfo() << ctemp;
          if (i <= nparm) break;
       }
    }
    if (isw2 < 3) {
       qInfo() << mCovmes[isw2];
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*Performs a local function minimization*-*-*-*-*-*-*-*-*-*
///*-*              ======================================
///*-*        Performs a local function minimization using basically the
///*-*        method of Davidon-Fletcher-Powell as modified by Fletcher
///*-*        ref. -- Fletcher, Comp.J. 13,317 (1970)   "switching method"
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnmigr()
{
    /* Local variables */
    double gdel, gami, vlen, dsum, gssq, vsum, d;
    double fzero, fs, ri, delgam, rhotol;
    double gdgssq, gvg, vgi;
    qint32 npfn, ndex, iext, i, j, m, n, npsdf, nparx;
    qint32 iswtr, lined2, kk, nfcnmg, nrstrt,iter;
    bool ldebug;
    double toler = 0.05;

    if (mNPar <= 0) return;
    if (mAmin == mUndefi) qmnamin();
    ldebug  = false; if ( mIdbg[4] >= 1) ldebug = true;
    mCfrom  = "MIGRAD  ";
    mNfcnfr = mNfcn;
    nfcnmg  = mNfcn;
    mCstatu = "INITIATE  ";
    iswtr   = mISW[4] - 2*mItaur;
    npfn    = mNfcn;
    nparx   = mNPar;
    vlen    = (double) (mNPar*(mNPar + 1) / 2);
    nrstrt  = 0;
    npsdf   = 0;
    lined2  = 0;
    mISW[3] = -1;
    rhotol  = mApsi*.001;
    if (iswtr >= 1) {
       qInfo() << " START MIGRAD MINIMIZATION.  STRATEGY " << mIstrat << " CONVERGENCE WHEN EDM .LT." << rhotol;
    }
 //*-*-                                          initialization strategy
    if (mIstrat < 2 || mISW[1] >= 3) goto L2;
 //*-*-                               come (back) here to restart completely
 L1:
    if (nrstrt > mIstrat) {
       mCstatu = "FAILED    ";
       mISW[3] = -1;
       goto L230;
    }
 //*-*-                                     . get full covariance and gradient
    qmnhess();
    qmnwerr();
    npsdf = 0;
    if (mISW[1] >= 1) goto L10;
 //*-*-                                       . get gradient at start point
 L2:
    qmninex(mX);
    if (mISW[2] == 1) {
       Eval(nparx, mGin, fzero, mU, 2);        ++mNfcn;
    }
    qmnderi();
    if (mISW[1] >= 1) goto L10;
 //*-*-                                  sometimes start with diagonal matrix
    for (i = 1; i <= mNPar; ++i) {
       mMIGRxxs[i-1]  = mX[i-1];
       mMIGRstep[i-1] = 0;
    }
 //*-*-                          do line search if second derivative negative
    ++lined2;
    if (lined2 < (mIstrat + 1)*mNPar) {
       for (i = 1; i <= mNPar; ++i) {
          if (mG2[i-1] > 0) continue;
          if (mGrd[i-1] > 0) mMIGRstep[i-1] = -qAbs(mGstep[i-1]);
          else               mMIGRstep[i-1] =  qAbs(mGstep[i-1]);
          gdel = mMIGRstep[i-1]*mGrd[i-1];
          fs   = mAmin;
          qmnline(mMIGRxxs, fs, mMIGRstep, gdel, toler);
          qmnwarn("D", "MNMIGR", "Negative G2 line search");
          iext = mNexofi[i-1];
          if (ldebug) {
             qDebug() << " Negative G2 line search, param "<< iext << fs << mAmin;
          }
          goto L2;
       }
    }
 //*-*-                          make diagonal error matrix
    for (i = 1; i <= mNPar; ++i) {
       ndex = i*(i-1) / 2;
       for (j = 1; j <= i-1; ++j) {
          ++ndex;
          mVhmat[ndex-1] = 0;
       }
       ++ndex;
       if (mG2[i-1] <= 0) mG2[i-1] = 1;
       mVhmat[ndex-1] = 2 / mG2[i-1];
    }
    mDcovar = 1;
    if (ldebug) {
       qDebug() << " DEBUG MNMIGR, STARTING MATRIX DIAGONAL,  VHMAT=";
       for (kk = 1; kk <= qint32(vlen); ++kk) {
          qDebug() << mVhmat[kk-1];
       }
    }
 //*-*-                                        ready to start first iteration
 L10:
    ++nrstrt;
    if (nrstrt > mIstrat + 1) {
       mCstatu = "FAILED    ";
       goto L230;
    }
    fs = mAmin;
 //*-*-                                       . . . get EDM and set up loop
    mEDM = 0;
    for (i = 1; i <= mNPar; ++i) {
       mMIGRgs[i-1]  = mGrd[i-1];
       mMIGRxxs[i-1] = mX[i-1];
       ndex     = i*(i-1) / 2;
       for (j = 1; j <= i-1; ++j) {
          ++ndex;
          mEDM += mMIGRgs[i-1]*mVhmat[ndex-1]*mMIGRgs[j-1];
       }
       ++ndex;
       mEDM += mMIGRgs[i-1]*mMIGRgs[i-1]*.5*mVhmat[ndex-1];
    }
    mEDM = mEDM*.5*(mDcovar*3 + 1);
    if (mEDM < 0) {
       qmnwarn("W", "MIGRAD", "STARTING MATRIX NOT POS-DEFINITE.");
       mISW[1] = 0;
       mDcovar = 1;
       goto L2;
    }
    if (mISW[1] == 0) mEDM = mBigedm;
    iter = 0;
    qmninex(mX);
    qmnwerr();
    if (iswtr >= 1) qmnprin(3, mAmin);
    if (iswtr >= 2) qmnmatu(0);
 //*-*-                                       . . . . .  start main loop
 L24:
    if (mNfcn - npfn >= mNfcnmx) goto L190;
    gdel = 0;
    gssq = 0;
    for (i = 1; i <= mNPar; ++i) {
       ri = 0;
       gssq += mMIGRgs[i-1]*mMIGRgs[i-1];
       for (j = 1; j <= mNPar; ++j) {
          m    = qMax(i,j);
          n    = qMin(i,j);
          ndex = m*(m-1) / 2 + n;
          ri  += mVhmat[ndex-1]*mMIGRgs[j-1];
       }
       mMIGRstep[i-1] = ri*-.5;
       gdel += mMIGRstep[i-1]*mMIGRgs[i-1];
    }
    if (gssq == 0) {
       qmnwarn("D", "MIGRAD", " FIRST DERIVATIVES OF FCN ARE ALL ZERO");
       goto L300;
    }
 //*-*-                if gdel positive, V not posdef
    if (gdel >= 0) {
       qmnwarn("D", "MIGRAD", " NEWTON STEP NOT DESCENT.");
       if (npsdf == 1) goto L1;
       qmnpsdf();
       npsdf = 1;
       goto L24;
    }
 //*-*-                                       . . . . do line search
    qmnline(mMIGRxxs, fs, mMIGRstep, gdel, toler);
    if (mAmin == fs) goto L200;
    mCfrom  = "MIGRAD  ";
    mNfcnfr = nfcnmg;
    mCstatu = "PROGRESS  ";
 //*-*-                                       . get gradient at new point
    qmninex(mX);
    if (mISW[2] == 1) {
       Eval(nparx, mGin, fzero, mU, 2);        ++mNfcn;
    }
    qmnderi();
 //*-*-                                        . calculate new EDM
    npsdf = 0;
 L81:
    mEDM = 0;
    gvg = 0;
    delgam = 0;
    gdgssq = 0;
    for (i = 1; i <= mNPar; ++i) {
       ri  = 0;
       vgi = 0;
       for (j = 1; j <= mNPar; ++j) {
          m    = qMax(i,j);
          n    = qMin(i,j);
          ndex = m*(m-1) / 2 + n;
          vgi += mVhmat[ndex-1]*(mGrd[j-1] - mMIGRgs[j-1]);
          ri  += mVhmat[ndex-1]*mGrd[j-1];
       }
       mMIGRvg[i-1] = vgi*.5;
       gami    = mGrd[i-1] - mMIGRgs[i-1];
       gdgssq += gami*gami;
       gvg    += gami*mMIGRvg[i-1];
       delgam += mDirin[i-1]*gami;
       mEDM   += mGrd[i-1]*ri*.5;
    }
    mEDM = mEDM*.5*(mDcovar*3 + 1);
 //*-*-                         . if EDM negative,  not positive-definite
    if (mEDM < 0 || gvg <= 0) {
       qmnwarn("D", "MIGRAD", "NOT POS-DEF. EDM OR GVG NEGATIVE.");
       mCstatu = "NOT POSDEF";
       if (npsdf == 1) goto L230;
       qmnpsdf();
       npsdf = 1;
       goto L81;
    }
 //*-*-                           print information about this iteration
    ++iter;
    if (iswtr >= 3 || (iswtr == 2 && iter % 10 == 1)) {
       qmnwerr();
       qmnprin(3, mAmin);
    }
    if (gdgssq == 0) {
       qmnwarn("D", "MIGRAD", "NO CHANGE IN FIRST DERIVATIVES OVER LAST STEP");
    }
    if (delgam < 0) {
       qmnwarn("D", "MIGRAD", "FIRST DERIVATIVES INCREASING ALONG SEARCH LINE");
    }
 //*-*-                                       .  update covariance matrix
    mCstatu = "IMPROVEMNT";
    if (ldebug) {
       qDebug() << " VHMAT 1 =";
       for (kk = 1; kk <= 10; ++kk) {
          qDebug() << mVhmat[kk-1];
       }
    }
    dsum = 0;
    vsum = 0;
    for (i = 1; i <= mNPar; ++i) {
       for (j = 1; j <= i; ++j) {
          if(delgam == 0 || gvg == 0) d = 0;
          else d = mDirin[i-1]*mDirin[j-1] / delgam - mMIGRvg[i-1]*mMIGRvg[j-1] / gvg;
          dsum += qAbs(d);
          ndex  = i*(i-1) / 2 + j;
          mVhmat[ndex-1] += d*2;
          vsum += qAbs(mVhmat[ndex-1]);
       }
    }
 //*-*-               smooth local fluctuations by averaging DCOVAR
    mDcovar = (mDcovar + dsum / vsum)*.5;
    if (iswtr >= 3 || ldebug) {
       qDebug() << " RELATIVE CHANGE IN COV. MATRIX=" << mDcovar*100 << "per cent";
    }
    if (ldebug) {
       qDebug() << " VHMAT 2 =";
       for (kk = 1; kk <= 10; ++kk) {
          qDebug() << mVhmat[kk-1];
       }
    }
    if (delgam <= gvg) goto L135;
    for (i = 1; i <= mNPar; ++i) {
       mMIGRflnu[i-1] = mDirin[i-1] / delgam - mMIGRvg[i-1] / gvg;
    }
    for (i = 1; i <= mNPar; ++i) {
       for (j = 1; j <= i; ++j) {
          ndex = i*(i-1) / 2 + j;
          mVhmat[ndex-1] += gvg*2*mMIGRflnu[i-1]*mMIGRflnu[j-1];
       }
    }
 L135:
 //*-*-                                             and see if converged
    if (mEDM < rhotol*.1) goto L300;
 //*-*-                                   if not, prepare next iteration
    for (i = 1; i <= mNPar; ++i) {
       mMIGRxxs[i-1] = mX[i-1];
       mMIGRgs[i-1]  = mGrd[i-1];
    }
    fs = mAmin;
    if (mISW[1] == 0 && mDcovar < .5)  mISW[1] = 1;
    if (mISW[1] == 3 && mDcovar > .1)  mISW[1] = 1;
    if (mISW[1] == 1 && mDcovar < .05) mISW[1] = 3;
    goto L24;
 //*-*-                                       . . . . .  end main loop
 //*-*-                                        . . call limit in MNMIGR
 L190:
    mISW[0] = 1;
    if (mISW[4] >= 0) {
       qInfo() << " CALL LIMIT EXCEEDED IN MIGRAD.";
    }
    mCstatu = "CALL LIMIT";
    goto L230;
 //*-*-                                        . . fails to improve . .
 L200:
    if (iswtr >= 1) {
       qInfo() << " MIGRAD FAILS TO FIND IMPROVEMENT";
    }
    for (i = 1; i <= mNPar; ++i) { mX[i-1] = mMIGRxxs[i-1]; }
    if (mEDM < rhotol) goto L300;
    if (mEDM < qAbs(mEpsma2*mAmin)) {
       if (iswtr >= 0) {
          qDebug() << " MACHINE ACCURACY LIMITS FURTHER IMPROVEMENT.";
       }
       goto L300;
    }
    if (mIstrat < 1) {
       if (mISW[4] >= 0) {
          qDebug() << " MIGRAD FAILS WITH STRATEGY=0.   WILL TRY WITH STRATEGY=1.";
       }
       mIstrat = 1;
    }
    goto L1;
 //*-*-                                        . . fails to converge
 L230:
    if (iswtr >= 0) {
       qDebug() << " MIGRAD TERMINATED WITHOUT CONVERGENCE.";
    }
    if (mISW[1] == 3) mISW[1] = 1;
    mISW[3] = -1;
    goto L400;
 //*-*-                                        . . apparent convergence
 L300:
    if (iswtr >= 0) {
       qDebug() << " MIGRAD MINIMIZATION HAS CONVERGED.";
    }
    if (mItaur == 0) {
       if (mIstrat >= 2 || (mIstrat == 1 && mISW[1] < 3)) {
          if (mISW[4] >= 0) {
             qDebug() << " MIGRAD WILL VERIFY CONVERGENCE AND ERROR MATRIX.";
          }
          qmnhess();
          qmnwerr();
          npsdf = 0;
          if (mEDM > rhotol) goto L10;
       }
    }
    mCstatu = "CONVERGED ";
    mISW[3] = 1;
 //*-*-                                          come here in any case
 L400:
    mCfrom  = "MIGRAD  ";
    mNfcnfr = nfcnmg;
    qmninex(mX);
    qmnwerr();
    if (iswtr >= 0) qmnprin(3, mAmin);
    if (iswtr >= 1) qmnmatu(1);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*-*Performs a MINOS error analysis*-*-*-*-*-*-*-*-*-*-*-*
///*-*                  ===============================
///*-*        Performs a MINOS error analysis on those parameters for
///*-*        which it is requested on the MINOS command by calling
///*-*        MNMNOT for each parameter requested.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnmnos()
{
    /* Local variables */
    double val2mi, val2pl;
    qint32 nbad, ilax, ilax2, ngood, nfcnmi, iin, knt;

    if (mNPar <= 0) goto L700;
    ngood = 0;
    nbad = 0;
    nfcnmi = mNfcn;
 //*-*-                                     . loop over parameters requested
    for (knt = 1; knt <= mNPar; ++knt) {
       if (qint32(mWord7[1]) == 0) {
          ilax = mNexofi[knt-1];
       } else {
          if (knt >= 7) break;
          ilax = qint32(mWord7[knt]);
          if (ilax == 0) break;
          if (ilax > 0 && ilax <= mNu) {
             if (mNiofex[ilax-1] > 0) goto L565;
          }
          qInfo() << " PARAMETER NUMBER " << ilax << " NOT A VARIABLE. IGNORED.";
          continue;
       }
 L565:
 //*-*-                                        calculate one pair of M E s
       ilax2 = 0;
       qmnmnot(ilax, ilax2, val2pl, val2mi);
       if (mLnewmn) goto L650;
 //*-*-                                         update NGOOD and NBAD
       iin = mNiofex[ilax-1];
       if (mErp[iin-1] > 0) ++ngood;
       else                   ++nbad;
       if (mErn[iin-1] < 0) ++ngood;
       else                   ++nbad;
    }
 //*-*-                                          end of loop . . . . . . .
 //*-*-                                       . . . . printout final values .
    mCfrom  = "MINOS   ";
    mNfcnfr = nfcnmi;
    mCstatu = "UNCHANGED ";
    if (ngood == 0 && nbad == 0) goto L700;
    if (ngood > 0 && nbad == 0)  mCstatu = "SUCCESSFUL";
    if (ngood == 0 && nbad > 0)  mCstatu = "FAILURE   ";
    if (ngood > 0 && nbad > 0)   mCstatu = "PROBLEMS  ";
    if (mISW[4] >= 0)    qmnprin(4, mAmin);
    if (mISW[4] >= 2)    qmnmatu(0);
    return;
 //*-*-                                       . . . new minimum found . . . .
 L650:
    mCfrom  = "MINOS   ";
    mNfcnfr = nfcnmi;
    mCstatu = "NEW MINIMU";
    if (mISW[4] >= 0) qmnprin(4, mAmin);
    qInfo() <<  "NEW MINIMUM FOUND.  GO BACK TO MINIMIZATION STEP.";
    qInfo() <<  "=================================================";
    qInfo() <<  "                                                 V";
    qInfo() <<  "                                                 V";
    qInfo() <<  "                                                 V";
    qInfo() <<  "                                              VVVVVVV";
    qInfo() <<  "                                               VVVVV";
    qInfo() <<  "                                                VVV";
    qInfo() <<  "                                                 V\n";
    return;
 L700:
    qInfo() <<  "THERE ARE NO MINOS ERRORS TO CALCULATE.";

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*Performs a MINOS error analysis on one parameter*-*-*-*-*-*-*-*-*
///*-*        ================================================
///*-*        The parameter ILAX is varied, and the minimum of the
///*-*        function with respect to the other parameters is followed
///*-*        until it crosses the value FMIN+UP.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnmnot(qint32 ilax, qint32 ilax2, double &val2pl, double &val2mi)
{
    /* System generated locals */
    qint32 i__1;

    /* Local variables */
    double delu, aopt, eros;
    double abest, xunit, dc, ut, sigsav, du1;
    double fac, sig, sav;
    qint32 marc, isig, mpar, ndex, imax, indx, ierr, i, j;
    qint32 iercr, it, istrav, nfmxin, nlimit, isw2, isw4;
    QString csig;

 //*-*-                                       . . save and prepare start vals
    isw2    = mISW[1];
    isw4    = mISW[3];
    sigsav  = mEDM;
    istrav  = mIstrat;
    dc      = mDcovar;
    mLnewmn = false;
    mApsi   = mEpsi*.5;
    abest   = mAmin;
    mpar    = mNPar;
    nfmxin  = mNfcnmx;
    for (i = 1; i <= mpar; ++i) { mXt[i-1] = mX[i-1]; }
    i__1 = mpar*(mpar + 1) / 2;
    for (j = 1; j <= i__1; ++j) { mVthmat[j-1] = mVhmat[j-1]; }
    for (i = 1; i <= mpar; ++i) {
       mMNOTgcc[i-1] = mGlobcc[i-1];
       mMNOTw[i-1]   = mWerr[i-1];
    }
    it = mNiofex[ilax-1];
    mErp[it-1] = 0;
    mErn[it-1] = 0;
    qmninex(mXt);
    ut = mU[ilax-1];
    if (mNvarl[ilax-1] == 1) {
       mAlim[ilax-1] = ut - mMNOTw[it-1]*100;
       mBlim[ilax-1] = ut + mMNOTw[it-1]*100;
    }
    ndex  = it*(it + 1) / 2;
    xunit = qSqrt(mUp / mVthmat[ndex-1]);
    marc  = 0;
    for (i = 1; i <= mpar; ++i) {
       if (i == it) continue;
       ++marc;
       imax = qMax(it,i);
       indx = imax*(imax-1) / 2 + qMin(it,i);
       mMNOTxdev[marc-1] = xunit*mVthmat[indx-1];
    }
 //*-*-                          fix the parameter in question
    qmnfixp(it-1, ierr);
    if (ierr > 0) {
       qInfo() << " MINUIT ERROR. CANNOT FIX PARAMETER " << ilax << "   INTERNAL" << it;
       goto L700;
    }
 //*-*-                      . . . . . Nota Bene: from here on, NPAR=MPAR-1
 //*-*-     Remember: MNFIXP squeezes IT out of X, XT, WERR, and VHMAT,
 //*-*-                                                   not W, VTHMAT
    for (isig = 1; isig <= 2; ++isig) {
       if (isig == 1) {
          sig  = 1;
          csig = "POSI";
       } else {
          sig  = -1;
          csig = "NEGA";
       }
 //*-*-                                       . sig=sign of error being calcd
       if (mISW[4] > 1) {
          qInfo() << " DETERMINATION OF " << csig <<  "TIVE MINOS ERROR FOR PARAMETER " << ilax << mCpnam[ilax-1];
       }
       if (mISW[1] <= 0) {
             qmnwarn("D", "MINOS", "NO COVARIANCE MATRIX.");
       }
       nlimit     = mNfcn + nfmxin;
       mIstrat    = qMax(istrav-1,0);
       du1        = mMNOTw[it-1];
       mU[ilax-1] = ut + sig*du1;
       mU[ilax-1] = qMin(mU[ilax-1],mBlim[ilax-1]);
       mU[ilax-1] = qMax(mU[ilax-1],mAlim[ilax-1]);
       delu = mU[ilax-1] - ut;
 //*-*-        stop if already at limit with negligible step size
       if (qAbs(delu) / (qAbs(ut) + qAbs(mU[ilax-1])) < mEpsmac) goto L440;
       fac = delu / mMNOTw[it-1];
       for (i = 1; i <= mNPar; ++i) {
          mX[i-1] = mXt[i-1] + fac*mMNOTxdev[i-1];
       }
       if (mISW[4] > 1) {
          qInfo() << " PARAMETER " << ilax << " SET TO" << ut << "+" << delu << " = " << mU[ilax-1];
       }
 //*-*-                                       loop to hit AMIN+UP
       mKe1cr  = ilax;
       mKe2cr  = 0;
       mXmidcr = mU[ilax-1];
       mXdircr = delu;

       mAmin = abest;
       mNfcnmx = nlimit - mNfcn;
       qmncros(aopt, iercr);
       if (abest - mAmin > mUp*.01) goto L650;
       if (iercr == 1) goto L440;
       if (iercr == 2) goto L450;
       if (iercr == 3) goto L460;
 //*-*-                                       . error successfully calculated
       eros = mXmidcr - ut + aopt*mXdircr;
       if (mISW[4] > 1) {
          qInfo() << "        THE " << csig << "TIVE MINOS ERROR OF PARAMETER" << ilax << mCpnam[ilax-1] << "IS" << eros;
       }
       goto L480;
 //*-*-                                       . . . . . . . . failure returns
 L440:
       if (mISW[4] >= 1) {
          qInfo() << "    THE " << csig << "TIVE MINOS ERROR OF PARAMETER" << ilax << mCpnam[ilax-1] << "EXCEEDS ITS LIMIT.";
       }
       eros = mUndefi;
       goto L480;
 L450:
       if (mISW[4] >= 1) {
          qInfo() << "       THE " << csig << "TIVE MINOS ERROR" << ilax << "REQUIRES MORE THAN" << nfmxin << "FUNCTION CALLS.";
       }
       eros = 0;
       goto L480;
 L460:
       if (mISW[4] >= 1) {
          qInfo() << "                         " << csig << "TIVE MINOS ERROR NOT CALCULATED FOR PARAMETER" << ilax;
       }
       eros = 0;

 L480:
       if (mISW[4] > 1) {
          qInfo() << "     **************************************************************************";
       }
       if (sig < 0) {
          mErn[it-1] = eros;
          if (ilax2 > 0 && ilax2 <= mNu) val2mi = mU[ilax2-1];
       } else {
          mErp[it-1] = eros;
          if (ilax2 > 0 && ilax2 <= mNu) val2pl = mU[ilax2-1];
       }
    }
 //*-*-                                       . . parameter finished. reset v
 //*-*-                      normal termination */
    mItaur = 1;
    qmnfree(1);
    i__1 = mpar*(mpar + 1) / 2;
    for (j = 1; j <= i__1; ++j) { mVhmat[j-1] = mVthmat[j-1]; }
    for (i = 1; i <= mpar; ++i) {
       mWerr[i-1]   = mMNOTw[i-1];
       mGlobcc[i-1] = mMNOTgcc[i-1];
       mX[i-1]      = mXt[i-1];
    }
    qmninex(mX);
    mEDM    = sigsav;
    mAmin   = abest;
    mISW[1] = isw2;
    mISW[3] = isw4;
    mDcovar = dc;
    goto L700;
 //*-*-                      new minimum
 L650:
    mLnewmn = true;
    mISW[1] = 0;
    mDcovar = 1;
    mISW[3] = 0;
    sav     = mU[ilax-1];
    mItaur  = 1;
    qmnfree(1);
    mU[ilax-1] = sav;
    qmnexin(mX);
    mEDM = mBigedm;
 //*-*-                      in any case
 L700:
    mItaur  = 0;
    mNfcnmx = nfmxin;
    mIstrat = istrav;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*Implements one parameter definition*-*-*-*-*-*-*-*-*-*-*-*
///*-*              ===================================
///*-*        Called from mNParS and user-callable
///*-*    Implements one parameter definition, that is:
///*-*          K     (external) parameter number
///*-*          CNAMK parameter name
///*-*          UK    starting value
///*-*          WK    starting step size or uncertainty
///*-*          A, B  lower and upper physical parameter limits
///*-*    and sets up (updates) the parameter lists.
///*-*    Output: IERFLG=0 if no problems
///*-*                  >0 if mNParM unable to implement definition
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmNParm(qint32 k1, QString cnamj, double uk, double wk, double a, double b, qint32 &ierflg)
{
    /* Local variables */
    double vplu, a_small, gsmin, pinti, vminu, danger, sav, sav2;
    qint32 ierr, kint, in, ix, ktofix, lastin, kinfix, nvl;
    QString cnamk, chbufi;

    qint32 k = k1+1;
    cnamk   = cnamj;
    kint   = mNPar;
    if (k < 1 || k > mMaxExtr) {
 //*-*-                    parameter number exceeds allowed maximum value
       qInfo() << " MINUIT USER ERROR.  PARAMETER NUMBER IS" << k << "ALLOWED RANGE IS ONE TO" << mMaxExtr;
       goto L800;
    }
 //*-*-                    normal parameter request
    ktofix = 0;
    if (mNvarl[k-1] < 0) goto L50;
 //*-*-        previously defined parameter is being redefined
 //*-*-                                    find if parameter was fixed
    for (ix = 1; ix <= mNPFix; ++ix) {
       if (mIpfix[ix-1] == k) ktofix = k;
    }
    if (ktofix > 0) {
       qmnwarn("W", "PARAM DEF", "REDEFINING A FIXED PARAMETER.");
       if (kint>= mMaxInt) {
          qInfo() << " CANNOT RELEASE. MAX NPAR EXCEEDED.";
          goto L800;
       }
       qmnfree(-k);
    }
 //*-*-                      if redefining previously variable parameter
    if (mNiofex[k-1] > 0) kint= mNPar - 1;
 L50:

 //*-*-                                     . . .prqint32heading
    if (mLphead && mISW[4] >= 0) {
       qInfo() << " PARAMETER DEFINITIONS:";
       qInfo() << "    NO.   NAME         VALUE      STEP SIZE      LIMITS";
       mLphead = false;
    }
    if (wk > 0) goto L122;
 //*-*-                                       . . .constant parameter . . . .
    if (mISW[4] >= 0) {
       qInfo() << k << cnamk << uk << "constant";
    }
    nvl = 0;
    goto L200;
 L122:
    if (a == 0 && b == 0) {
 //*-*-                                     variable parameter without limits
       nvl = 1;
       if (mISW[4] >= 0) {
          qInfo() << k << cnamk << uk << wk << "     no limits";
       }
    } else {
 //*-*-                                        variable parameter with limits
       nvl = 4;
       mLnolim = false;
       if (mISW[4] >= 0) {
          qInfo() << k << cnamk << uk << wk << a << b;
       }
    }
 //*-*-                            . . request for another variable parameter
    ++kint;
    if (kint> mMaxInt) {
       qInfo() << " MINUIT USER ERROR.   TOO MANY VARIABLE PARAMETERS.";
       goto L800;
    }
    if (nvl == 1) goto L200;
    if (a == b) {
       qInfo() << " USER ERROR IN MINUIT PARAMETER";
       qInfo() << " DEFINITION";
       qInfo() << " UPPER AND LOWER LIMITS EQUAL.";
       goto L800;
    }
    if (b < a) {
       sav = b;
       b = a;
       a = sav;
       qmnwarn("W", "PARAM DEF", "PARAMETER LIMITS WERE REVERSED.");
       if (mLwarn) mLphead = true;
    }
    if (b - a > 1e7) {
       QString ctemp(QString("LIMITS ON PARAM%1 TOO FAR APART.").arg(k));
       qmnwarn("W", "PARAM DEF", ctemp.toStdString().data());
       if (mLwarn) mLphead = true;
    }
    danger = (b - uk)*(uk - a);
    if (danger < 0) {
       qmnwarn("W", "PARAM DEF", "STARTING VALUE OUTSIDE LIMITS.");
    }
    if (danger == 0) {
       qmnwarn("W", "PARAM DEF", "STARTING VALUE IS AT LIMIT.");
    }
 L200:
 //*-*-                          . . . input OK, set values, arrange lists,
 //*-*-                                   calculate step sizes GSTEP, DIRIN
    mCfrom      = "PARAMETR";
    mNfcnfr     = mNfcn;
    mCstatu     = "NEW VALUES";
    mNu         = qMax(mNu,k);
    mCpnam[k-1] = cnamk;
    mU[k-1]     = uk;
    mAlim[k-1]  = a;
    mBlim[k-1]  = b;
    mNvarl[k-1] = nvl;
    qmnrset(1);
 //*-*-                            K is external number of new parameter
 //*-*-          LASTIN is the number of var. params with ext. param. no.< K
    lastin = 0;
    for (ix = 1; ix <= k-1; ++ix) { if (mNiofex[ix-1] > 0) ++lastin; }
 //*-*-                Kqint32is new number of variable params, NPAR is old
    if (kint== mNPar) goto L280;
    if (kint> mNPar) {
 //*-*-                         insert new variable parameter in list
       for (in = mNPar; in >= lastin + 1; --in) {
          ix            = mNexofi[in-1];
          mNiofex[ix-1] = in + 1;
          mNexofi[in]   = ix;
          mX[in]        = mX[in-1];
          mXt[in]       = mXt[in-1];
          mDirin[in]    = mDirin[in-1];
          mG2[in]       = mG2[in-1];
          mGstep[in]    = mGstep[in-1];
          mWerr[in]     = mWerr[in-1];
          mGrd[in]      = mGrd[in-1];
       }
    } else {
 //*-*-                         remove variable parameter from list
       for (in = lastin + 1; in <= kint; ++in) {
          ix            = mNexofi[in];
          mNiofex[ix-1] = in;
          mNexofi[in-1] = ix;
          mX[in-1]      = mX[in];
          mXt[in-1]     = mXt[in];
          mDirin[in-1]  = mDirin[in];
          mG2[in-1]     = mG2[in];
          mGstep[in-1]  = mGstep[in];
          mWerr[in-1]   = mWerr[in];
          mGrd[in-1]    = mGrd[in];
       }
    }
 L280:
    ix = k;
    mNiofex[ix-1] = 0;
    mNPar = kint;
 //*-*-                                      lists are now arranged . . . .
    if (nvl > 0) {
       in            = lastin + 1;
       mNexofi[in-1] = ix;
       mNiofex[ix-1] = in;
       sav           = mU[ix-1];
       qmnpint(sav, ix-1, pinti);
       mX[in-1]    = pinti;
       mXt[in-1]   = mX[in-1];
       mWerr[in-1] = wk;
       sav2        = sav + wk;
       qmnpint(sav2, ix-1, pinti);
       vplu = pinti - mX[in-1];
       sav2 = sav - wk;
       qmnpint(sav2, ix-1, pinti);
       vminu = pinti - mX[in-1];
       mDirin[in-1] = (qAbs(vplu) + qAbs(vminu))*.5;
       mG2[in-1] = mUp*2 / (mDirin[in-1]*mDirin[in-1]);
       gsmin = mEpsma2*8*qAbs(mX[in-1]);
       mGstep[in-1] = qMax(gsmin,mDirin[in-1]*.1);
       if (mAmin != mUndefi) {
          a_small      = qSqrt(mEpsma2*(mAmin + mUp) / mUp);
          mGstep[in-1] = qMax(gsmin,a_small*mDirin[in-1]);
       }
       mGrd[in-1] = mG2[in-1]*mDirin[in-1];
 //*-*-                  if parameter has limits
       if (mNvarl[k-1] > 1) {
          if (mGstep[in-1] > .5) mGstep[in-1] = .5;
          mGstep[in-1] = -mGstep[in-1];
       }
    }
    if (ktofix > 0) {
       ierr = 0;
       kinfix = mNiofex[ktofix-1];
       if (kinfix > 0) qmnfixp(kinfix-1, ierr);
       if (ierr > 0)   goto L800;
    }
    ierflg = 0;
    return;
 //*-*-                  error on input, unable to implement request  . . . .
 L800:
    ierflg = 1;

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*Implements one parameter definition*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*            =========== =======================
///*-*        Called from MNREAD and user-callable
///*-*    Implements one parameter definition, that is:
///*-*       parses the string CRDBUF and calls mNParM
///*-*
///*-* output conditions:
///*-*        ICONDN = 0    all OK
///*-*        ICONDN = 1    error, attempt to define parameter is ignored
///*-*        ICONDN = 2    end of parameter definitions
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmNPars(QString &crdbuf, qint32 &icondn)
{
    /* Local variables */
    double a=0, b=0, fk=0, uk=0, wk=0, xk=0;
    qint32 ierr, kapo1, kapo2;
    qint32 k, llist, ibegin, lenbuf, istart, lnc, icy;
    QString cnamk, comand, celmnt, ctemp;
    char stmp[128];

    lenbuf = strlen(crdbuf.toStdString().data());
 //*-*-                    find out whether fixed or free-field format
    kapo1 = strspn(crdbuf.toStdString().data(), "'");
    if (kapo1 == 0) goto L150;
    kapo2 = strspn(crdbuf.toStdString().data() + kapo1, "'");
    if (kapo2 == 0) goto L150;
 //*-*-         new (free-field) format
    kapo2 += kapo1;
 //*-*-                            skip leading blanks if any
    for (istart = 1; istart <= kapo1-1; ++istart) {
       if (crdbuf.midRef(istart-1,1) != " ") goto L120;
    }
    goto L210;
 L120:
 //*-*-                              parameter number integer
    celmnt = QString(crdbuf.midRef(istart-1, kapo1-istart).toString().data());
    if (scanf(celmnt.toStdString().data(),&fk)) {;}
    k = qint32(fk);
    if (k <= 0) goto L210;
    cnamk = "PARAM " + celmnt;
    if (kapo2 - kapo1 > 1) {
       cnamk = QString(crdbuf.midRef(kapo1, kapo2-1-kapo1).toString().data());
    }
 //*-*  special handling if comma or blanks and a comma follow 'name'
    for (icy = kapo2 + 1; icy <= lenbuf; ++icy) {
       if (crdbuf.midRef(icy-1,1) == ",") goto L139;
       if (crdbuf.midRef(icy-1,1) != " " ) goto L140;
    }
    uk = 0;
    wk = 0;
    a  = 0;
    b  = 0;
    goto L170;
 L139:
    ++icy;
 L140:
    ibegin = icy;
    ctemp = QString(crdbuf.midRef(ibegin-1,lenbuf-ibegin).toString().data());
    qmncrck(ctemp, 20, comand, lnc, mMaxPar, mPARSplist, llist, ierr, mIsyswr);
    if (ierr > 0) goto L180;
    uk = mPARSplist[0];
    wk = 0;
    if (llist >= 2) wk = mPARSplist[1];
    a = 0;
    if (llist >= 3) a = mPARSplist[2];
    b = 0;
    if (llist >= 4) b = mPARSplist[3];
    goto L170;
 //*-*-         old (fixed-field) format
 L150:
    if (scanf(crdbuf.toStdString().data(),&xk,stmp,&uk,&wk,&a,&b)) {;}
    cnamk = stmp;
    k = qint32(xk);
    if (k == 0)    goto L210;
 //*-*-         parameter format cracked, implement parameter definition
 L170:
    qmNParm(k-1, cnamk, uk, wk, a, b, ierr);
    icondn = ierr;
    return;
 //*-*-         format or other error
 L180:
    icondn = 1;
    return;
 //*-*-       end of data
 L210:
    icondn = 2;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*To fit a parabola to npar2p points*-*-*-*-*-*-*-*-*-*-*
///*-*                ==================================
///*-*   npar2p   no. of points
///*-*   parx2p(i)   x value of point i
///*-*   pary2p(i)   y value of point i
///*-*
///*-*   coef2p(1...3)  coefficients of the fitted parabola
///*-*   y=coef2p(1) + coef2p(2)*x + coef2p(3)*x**2
///*-*   sdev2p= variance
///*-*   method : chi**2 = min equation solved explicitly
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnpfit(double *parx2p, double *pary2p, qint32 npar2p, double *coef2p, double &sdev2p)
{
    /* Local variables */
    double a, f, s, t, y, s2, x2, x3, x4, y2, cz[3], xm, xy, x2y;
    x2 = x3 = 0;
    qint32 i;

    /* Parameter adjustments */
    --coef2p;
    --pary2p;
    --parx2p;

    /* Function Body */
    for (i = 1; i <= 3; ++i) { cz[i-1] = 0; }
    sdev2p = 0;
    if (npar2p < 3) goto L10;
    f = (double) (npar2p);
 //*-* --- center x values for reasons of machine precision
    xm  = 0;
    for (i = 1; i <= npar2p; ++i) { xm += parx2p[i]; }
    xm /= f;
    x2  = 0;
    x3  = 0;
    x4  = 0;
    y   = 0;
    y2  = 0;
    xy  = 0;
    x2y = 0;
    for (i = 1; i <= npar2p; ++i) {
       s    = parx2p[i] - xm;
       t    = pary2p[i];
       s2   = s*s;
       x2  += s2;
       x3  += s*s2;
       x4  += s2*s2;
       y   += t;
       y2  += t*t;
       xy  += s*t;
       x2y += s2*t;
    }
    a = (f*x4 - x2*x2)*x2 - f*(x3*x3);
    if (a == 0) goto L10;
    cz[2] = (x2*(f*x2y - x2*y) - f*x3*xy) / a;
    cz[1] = (xy - x3*cz[2]) / x2;
    cz[0] = (y - x2*cz[2]) / f;
    if (npar2p == 3) goto L6;
    sdev2p = y2 - (cz[0]*y + cz[1]*xy + cz[2]*x2y);
    if (sdev2p < 0) sdev2p = 0;
    sdev2p /= f - 3;
 L6:
    cz[0] += xm*(xm*cz[2] - cz[1]);
    cz[1] -= xm*2*cz[2];
 L10:
    for (i = 1; i <= 3; ++i) { coef2p[i] = cz[i-1]; }

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*Calculates the internal parameter value PINTI*-*-*-*-*-*-*-*
///*-*          =============================================
///*-*        corresponding  to the external value PEXTI for parameter I.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnpint(double &pexti, qint32 i1, double &pinti)
{
    /* Local variables */
    double a, alimi, blimi, yy, yy2;
    qint32 igo;
    QString chbuf2, chbufi;

    qint32 i = i1+1;
    pinti   = pexti;
    igo     = mNvarl[i-1];
    if (igo == 4) {
 //*-* --                          there are two limits
       alimi = mAlim[i-1];
       blimi = mBlim[i-1];
       yy = (pexti - alimi)*2 / (blimi - alimi) - 1;
       yy2 = yy*yy;
       if (yy2 >= 1 - mEpsma2) {
          if (yy < 0) {
             a      = mVlimlo;
             chbuf2 = " IS AT ITS LOWER ALLOWED LIMIT.";
          } else {
             a      = mVlimhi;
             chbuf2 = " IS AT ITS UPPER ALLOWED LIMIT.";
          }
          pinti   = a;
          pexti   = alimi + (blimi - alimi)*.5*(qSin(a) + 1);
          mLimset = true;
          if (yy2 > 1) chbuf2 = " BROUGHT BACK INSIDE LIMITS.";
          qmnwarn("W", mCfrom.toStdString().data(), QString("VARIABLE%1%2").arg(i).arg(chbuf2).toStdString().data());
       } else {
          pinti = qAsin(yy);
       }
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Plots points in array xypt onto one page with labelled axes*-*-*-*-*
///*-*    ===========================================================
///*-*        NXYPT is the number of points to be plotted
///*-*        XPT(I) = x-coord. of ith point
///*-*        YPT(I) = y-coord. of ith point
///*-*        CHPT(I) = character to be plotted at this position
///*-*        the input point arrays XPT, YPT, CHPT are destroyed.
///*-*
///*-*
///*-*   If mGraphicsMode is true (default), a TGraph object is produced
///*-*   via the Plug-in handler. To get the plot, you can do:
///*-*       TGraph *gr = (TGraph*)gMinuit->GetPlot();
///*-*       gr->Draw("al");
///*-*
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnplot(double *xpt, double *ypt, char *chpt, qint32 nxypt, qint32 npagwd, qint32 npagln)
{
    ypt = NULL;
    xpt = NULL;
    qWarning() << Q_FUNC_INFO << "NOT IMPLEMENTED" << xpt << ypt << chpt << nxypt << npagwd << npagln;
    qFatal("77");
//    if (mGraphicsMode) {
//       TPluginHandler *h;
//       if ((h = gROOT->GetPluginManager()->FindHandler("QMinuitGraph"))) {
//          //remove the first two points
//          if (h->LoadPlugin() != -1)
//          mPlot = (TObject*)h->ExecPlugin(3,nxypt-2,&xpt[2],&ypt[2]);
//       }
//       return;
//    }

//    static QString cdot   = ".";
//    static QString cslash = "/";

//    /* Local variables */
//    double xmin, ymin, xmax, ymax, savx, savy, yprt;
//    double bwidx, bwidy, xbest, ybest, ax, ay, bx, by;
//    double xvalus[12], any, dxx, dyy;
//    qint32 iten, i, j, k, maxnx, maxny, iquit, ni, linodd;
//    qint32 nxbest, nybest, km1, ibk, isp1, nx, ny, ks, ix;
//    QString chmess, ctemp;
//    Bool_t overpr;
//    char cline[120];
//    char chsav, chbest;

//    /* Function Body */
//  //*-*  Computing MIN
//    maxnx = qMin(npagwd-20,100);
//    if (maxnx < 10) maxnx = 10;
//    maxny = npagln;
//    if (maxny < 10) maxny = 10;
//    if (nxypt <= 1) return;
//    xbest  = xpt[0];
//    ybest  = ypt[0];
//    chbest = chpt[0];
//  //*-*-        order the points by decreasing y
//    km1 = nxypt - 1;
//    for (i = 1; i <= km1; ++i) {
//       iquit = 0;
//       ni    = nxypt - i;
//       for (j = 1; j <= ni; ++j) {
//          if (ypt[j-1] > ypt[j]) continue;
//          savx     = xpt[j-1];
//          xpt[j-1] = xpt[j];
//          xpt[j]   = savx;
//          savy     = ypt[j-1];
//          ypt[j-1] = ypt[j];
//          ypt[j]   = savy;
//          chsav    = chpt[j-1];
//          chpt[j-1]= chpt[j];
//          chpt[j]  = chsav;
//          iquit    = 1;
//       }
//       if (iquit == 0) break;
//    }
//  //*-*-        find extreme values
//    xmax = xpt[0];
//    xmin = xmax;
//    for (i = 1; i <= nxypt; ++i) {
//       if (xpt[i-1] > xmax) xmax = xpt[i-1];
//       if (xpt[i-1] < xmin) xmin = xpt[i-1];
//    }
//    dxx   = (xmax - xmin)*.001;
//    xmax += dxx;
//    xmin -= dxx;
//    qmnbins(xmin, xmax, maxnx, xmin, xmax, nx, bwidx);
//    ymax = ypt[0];
//    ymin = ypt[nxypt-1];
//    if (ymax == ymin) ymax = ymin + 1;
//    dyy   = (ymax - ymin)*.001;
//    ymax += dyy;
//    ymin -= dyy;
//    qmnbins(ymin, ymax, maxny, ymin, ymax, ny, bwidy);
//    any = (double) ny;
//  //*-*-        if first point is blank, it is an 'origin'
//    if (chbest == ' ') goto L50;
//    xbest = (xmax + xmin)*.5;
//    ybest = (ymax + ymin)*.5;
// L50:
//  //*-*-        find scale constants
//    ax = 1 / bwidx;
//    ay = 1 / bwidy;
//    bx = -ax*xmin + 2;
//    by = -ay*ymin - 2;
//  //*-*-        convert points to grid positions
//    for (i = 1; i <= nxypt; ++i) {
//       xpt[i-1] = ax*xpt[i-1] + bx;
//       ypt[i-1] = any - ay*ypt[i-1] - by;
//    }
//    nxbest = qint32((ax*xbest + bx));
//    nybest = qint32((any - ay*ybest - by));
//  //*-*-        print the points
//    ny += 2;
//    nx += 2;
//    isp1 = 1;
//    linodd = 1;
//    overpr = false;
//    for (i = 1; i <= ny; ++i) {
//       for (ibk = 1; ibk <= nx; ++ibk) { cline[ibk-1] = ' '; }
//       cline[nx] = '\0';
//       cline[nx+1] = '\0';
//       cline[0]        = '.';
//       // not needed - but to avoid a wrongly reported compiler warning (see ROOT-6496)
//       if (nx>0) cline[nx-1]     = '.';
//       cline[nxbest-1] = '.';
//       if (i != 1 && i != nybest && i != ny) goto L320;
//       for (j = 1; j <= nx; ++j) { cline[j-1] = '.'; }
// L320:
//       yprt = ymax - double(i-1)*bwidy;
//       if (isp1 > nxypt) goto L350;
//  //*-*-        find the points to be plotted on this line
//       for (k = isp1; k <= nxypt; ++k) {
//          ks = qint32(ypt[k-1]);
//          if (ks > i) goto L345;
//          ix = qint32(xpt[k-1]);
//          if (cline[ix-1] == '.')   goto L340;
//          if (cline[ix-1] == ' ') goto L340;
//          if (cline[ix-1] == chpt[k-1])   continue;
//          overpr = true;
//  //*-*-        OVERPR is true if one or more positions contains more than
//  //*-*-           one point
//          cline[ix-1] = '&';
//          continue;
// L340:
//          cline[ix-1] = chpt[k-1];
//       }
//       isp1 = nxypt + 1;
//       goto L350;
// L345:
//       isp1 = k;
// L350:
//       if (linodd == 1 || i == ny) goto L380;
//       linodd = 1;
//       ctemp  = cline;
//       Printf("                  %s",(const char*)ctemp);
//       goto L400;
// L380:
//       ctemp = cline;
//       Printf(" %14.7g ..%s",yprt,(const char*)ctemp);
//       linodd = 0;
// L400:
//       ;
//    }
//  //*-*-        print labels on x-axis every ten columns
//    for (ibk = 1; ibk <= nx; ++ibk) {
//       cline[ibk-1] = ' ';
//       if (ibk % 10 == 1) cline[ibk-1] = '/';
//    }
//    Printf("                  %s",cline);

//    for (ibk = 1; ibk <= 12; ++ibk) {
//       xvalus[ibk-1] = xmin + double(ibk-1)*10*bwidx;
//    }
//    iten = (nx + 9) / 10;
//    for (ibk = 1; ibk <= iten && ibk <= 12; ++ibk) {
//       snprintf(cline + (ibk-1)*10, 11-(ibk == 12), "%#9.3g ", xvalus[ibk-1]);
//    }
//    Printf("           %s", cline);
//    chmess = " ";
//    if (overpr) chmess = "   Overprint character is &";
    //    Printf("                         ONE COLUMN=%13.7g%s",bwidx,(const char*)chmess);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Provides the user with information concerning the current status*-*-*
///*-*    ================================================================
///*-*          of parameter number IUEXT. Namely, it returns:
///*-*        CHNAM: the name of the parameter
///*-*        VAL: the current (external) value of the parameter
///*-*        ERR: the current estimate of the parameter uncertainty
///*-*        XLOLIM: the lower bound (or zero if no limits)
///*-*        XUPLIM: the upper bound (or zero if no limits)
///*-*        IUINT: the internal parameter number (or zero if not variable,
///*-*           or negative if undefined).
///*-*  Note also:  If IUEXT is negative, then it is -internal parameter
///*-*           number, and IUINT is returned as the EXTERNAL number.
///*-*     Except for IUINT, this is exactly the inverse of mNParM
///*-*     User-called
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnpout(qint32 iuext1, QString &chnam, double &val, double &err, double &xlolim, double &xuplim, qint32 &iuint) const
{
    /* Local variables */
    qint32 iint, iext, nvl;

    qint32 iuext = iuext1 + 1;
    xlolim = 0;
    xuplim = 0;
    err    = 0;
    if (iuext == 0) goto L100;
    if (iuext < 0) {
 //*-*-                  internal parameter number specified
       iint  = -(iuext);
       if (iint > mNPar) goto L100;
       iext  = mNexofi[iint-1];
       iuint = iext;
    } else {
 //*-*-                   external parameter number specified
       iext = iuext;
       if (iext > mNu) goto L100;
       iint  = mNiofex[iext-1];
       iuint = iint;
    }
 //*-*-                    in both cases
    nvl = mNvarl[iext-1];
    if (nvl < 0) goto L100;
    chnam = mCpnam[iext-1];
    val   = mU[iext-1];
    if (iint > 0) err = mWerr[iint-1];
    if (nvl == 4) {
       xlolim = mAlim[iext-1];
       xuplim = mBlim[iext-1];
    }
    return;
 //*-*-               parameter is undefined
 L100:
    iuint = -1;
    chnam = "undefined";
    val = 0;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Prints the values of the parameters at the time of the call*-*-*-*-*
///*-*    ===========================================================
///*-*        also prints other relevant information such as function value,
///*-*        estimated distance to minimum, parameter errors, step sizes.
///*-*
///*-*         According to the value of IKODE, the printout is:/
///*-*    IKODE=INKODE= 0    only info about function value
///*-*                  1    parameter values, errors, limits
///*-*                  2    values, errors, step sizes, internal values
///*-*                  3    values, errors, step sizes, first derivs.
///*-*                  4    values, parabolic errors, MINOS errors
///*-*    when INKODE=5, MNPRIN chooses IKODE=1,2, or 3, according to mISW[1]
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnprin(qint32 inkode, double fval)
{
    /* Initialized data */

    static QString cblank = "           ";
    static QString cnambf = "           ";

    /* Local variables */
    double dcmax, x1, x2, x3, dc;
    x2 = x3 = 0;
    qint32 nadd, i, k, l, m, ikode, ic, nc, ntrail, lbl;
    QString chedm;
    QString colhdl[6], colhdu[6], cx2, cx3, cheval;

    if (mNu == 0) {
       qInfo() << " THERE ARE CURRENTLY NO PARAMETERS DEFINED";
       return;
    }
 //*-*-                 get value of IKODE based in INKODE, mISW[1]
    ikode = inkode;
    if (inkode == 5) {
       ikode = mISW[1] + 1;
       if (ikode > 3) ikode = 3;
    }
 //*-*-                 set 'default' column headings
    for (k = 1; k <= 6; ++k) {
       colhdu[k-1] = "UNDEFINED";
       colhdl[k-1] = "COLUMN HEAD";
    }
 //*-*-             print title if Minos errors, and title exists.
    if (ikode == 4 && mCtitl != mCundef) {
       qInfo() << " MINUIT TASK: " << mCtitl;
    }
 //*-*-             report function value and status
    if (fval == mUndefi) cheval = " unknown       ";
    else                 cheval = QString("%1").arg(fval);

    if (mEDM == mBigedm) chedm = " unknown  ";
    else                 chedm = QString("%1").arg(mEDM);

    nc = mNfcn - mNfcnfr;
    qInfo() << " FCN=" << cheval << "FROM " << mCfrom << "STATUS=" << mCstatu << nc <<  "CALLS" << mNfcn << "TOTAL";
    m = mISW[1];
    if (m == 0 || m == 2 || mDcovar == 0) {
       qInfo() << "                     EDM=" << chedm << "    STRATEGY=" << mIstrat << "      " << mCovmes[m];
    } else {
       dcmax = 1;
       dc    = qMin(mDcovar,dcmax)*100;
       qInfo() << "                     EDM=" << chedm << "    STRATEGY=" << mIstrat << "  ERROR MATRIX UNCERTAINTY " << dc << "per cent";
    }

    if (ikode == 0) return;
 //*-*-              find longest name (for Rene!)
    ntrail = 10;
    for (i = 1; i <= mNu; ++i) {
       if (mNvarl[i-1] < 0) continue;
       for (ic = 10; ic >= 1; --ic) {
          if (mCpnam[i-1].midRef(ic-1,1) != " ") goto L16;
       }
       ic = 1;
 L16:
       lbl = 10 - ic;
       if (lbl < ntrail) ntrail = lbl;
    }
    nadd = ntrail / 2 + 1;
    if (ikode == 1) {
       colhdu[0] = "              ";
       colhdl[0] = "      ERROR   ";
       colhdu[1] = "      PHYSICAL";
       colhdu[2] = " LIMITS       ";
       colhdl[1] = "    NEGATIVE  ";
       colhdl[2] = "    POSITIVE  ";
    }
    if (ikode == 2) {
       colhdu[0] = "              ";
       colhdl[0] = "      ERROR   ";
       colhdu[1] = "    INTERNAL  ";
       colhdl[1] = "    STEP SIZE ";
       colhdu[2] = "    INTERNAL  ";
       colhdl[2] = "      VALUE   ";
    }
    if (ikode == 3) {
       colhdu[0] = "              ";
       colhdl[0] = "      ERROR   ";
       colhdu[1] = "       STEP   ";
       colhdl[1] = "       SIZE   ";
       colhdu[2] = "      FIRST   ";
       colhdl[2] = "   DERIVATIVE ";
    }
    if (ikode == 4) {
       colhdu[0] = "    PARABOLIC ";
       colhdl[0] = "      ERROR   ";
       colhdu[1] = "        MINOS ";
       colhdu[2] = "ERRORS        ";
       colhdl[1] = "   NEGATIVE   ";
       colhdl[2] = "   POSITIVE   ";
    }

    if (ikode != 4) {
       if (mISW[1] < 3) colhdu[0] = "  APPROXIMATE ";
       if (mISW[1] < 1) colhdu[0] = " CURRENT GUESS";
    }
    qInfo() << "  EXT PARAMETER              " << colhdu[0] << colhdu[1] << colhdu[2];
    qInfo() << "  NO.   NAME      VALUE      " << colhdl[0] << colhdl[1] << colhdl[2];
 //*-*-                                       . . . loop over parameters . .
    for (i = 1; i <= mNu; ++i) {
       if (mNvarl[i-1] < 0) continue;
       l = mNiofex[i-1];
       cnambf = cblank.left(nadd) + mCpnam[i-1];
       if (l == 0) goto L55;
 //*-*-             variable parameter.
       x1  = mWerr[l-1];
       cx2 = "PLEASE GET X..";
       cx3 = "PLEASE GET X..";
       if (ikode == 1) {
          if (mNvarl[i-1] <= 1) {
             qInfo() << i << cnambf << mU[i-1] << x1;
             continue;
          } else {
             x2 = mAlim[i-1];
             x3 = mBlim[i-1];
          }
       }
       if (ikode == 2) {
          x2 = mDirin[l-1];
          x3 = mX[l-1];
       }
       if (ikode == 3) {
          x2 = mDirin[l-1];
          x3 = mGrd[l-1];
          if (mNvarl[i-1] > 1 && qAbs(qCos(mX[l-1])) < .001) {
             cx3 = "** at limit **";
          }
       }
       if (ikode == 4) {
          x2 = mErn[l-1];
          if (x2 == 0)        cx2 = " ";
          if (x2 == mUndefi)  cx2 = "   at limit   ";
          x3 = mErp[l-1];
          if (x3 == 0)        cx3 = " ";
          if (x3 == mUndefi)         cx3 = "   at limit   ";
       }
       if (cx2 == "PLEASE GET X..")  cx2 = QString("%1").arg(x2);
       if (cx3 == "PLEASE GET X..")  cx3 = QString("%1").arg(x3);
       qInfo() << i << cnambf << mU[i-1] << x1 << cx2 << cx3;

 //*-*-              check if parameter is at limit
       if (mNvarl[i-1] <= 1 || ikode == 3) continue;
       if (qAbs(qCos(mX[l-1])) < .001) {
          qInfo() << "                                 WARNING -   - ABOVE PARAMETER IS AT LIMIT.";
       }
       continue;

 //*-*-                               print constant or fixed parameter.
 L55:
       colhdu[0] = "   constant   ";
       if (mNvarl[i-1] > 0)  colhdu[0] = "     fixed    ";
       if (mNvarl[i-1] == 4 && ikode == 1) {
          qInfo() << i << cnambf << mU[i-1] << colhdu[0] << mAlim[i-1] << mBlim[i-1];
       } else {
          qInfo() << i << cnambf << mU[i-1] << colhdu[0];
       }
    }

    if (mUp != mUpdflt) {
       qInfo() << "                               ERR DEF= " << mUp;
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*Calculates the eigenvalues of v to see if positive-def*-*-*-*-*
///*-*        ======================================================
///*-*        if not, adds constant along diagonal to make positive.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnpsdf()
{
    /* Local variables */
    double dgmin, padd, pmin, pmax, dg, epspdf, epsmin;
    qint32 ndex, i, j, ndexd, ip, ifault;
    QString chbuff, ctemp;

    epsmin = 1e-6;
    epspdf = qMax(epsmin,mEpsma2);
    dgmin  = mVhmat[0];
 //*-*-                       Check if negative or zero on diagonal
    for (i = 1; i <= mNPar; ++i) {
       ndex = i*(i + 1) / 2;
       if (mVhmat[ndex-1] <= 0) {
          qmnwarn("W", mCfrom.toStdString().data(), QString("Negative diagonal element %1 in Error Matrix").arg(i).toStdString().data());
       }
       if (mVhmat[ndex-1] < dgmin) dgmin = mVhmat[ndex-1];
    }
    if (dgmin <= 0) {
       dg    = epspdf + 1 - dgmin;
       qmnwarn("W", mCfrom.toStdString().data(), QString("%1 added to diagonal of error matrix").arg(dg).toStdString().data());
    } else {
       dg = 0;
    }
 //*-*-                   Store VHMAT in P, make sure diagonal pos.
    for (i = 1; i <= mNPar; ++i) {
       ndex  = i*(i-1) / 2;
       ndexd = ndex + i;
       mVhmat[ndexd-1] += dg;
       if (mVhmat[ndexd-1]==0) {
          mPSDFs[i-1] = 1 / 1e-19; // a totally arbitrary silly small value
       } else {
          mPSDFs[i-1] = 1 / qSqrt(mVhmat[ndexd-1]);
       }
       for (j = 1; j <= i; ++j) {
          ++ndex;
          mP[i + j*mMaxPar - mMaxPar-1] = mVhmat[ndex-1]*mPSDFs[i-1]*mPSDFs[j-1];
       }
    }
 //*-*-     call eigen (p,p,maxint,npar,pstar,-npar)
    qmneig(mP, mMaxInt, mNPar, mMaxInt, mPstar, epspdf, ifault);
    pmin = mPstar[0];
    pmax = mPstar[0];
    for (ip = 2; ip <= mNPar; ++ip) {
       if (mPstar[ip-1] < pmin) pmin = mPstar[ip-1];
       if (mPstar[ip-1] > pmax) pmax = mPstar[ip-1];
    }
    pmax = qMax(qAbs(pmax),double(1));
    if ((pmin <= 0 && mLwarn) || mISW[4] >= 2) {
       qInfo() << " EIGENVALUES OF SECOND-DERIVATIVE MATRIX:";
       ctemp = "       ";
       for (ip = 1; ip <= mNPar; ++ip) {
          ctemp += QString(" %1").arg(mPstar[ip-1]);
       }
       qInfo() << ctemp;
    }
    if (pmin > epspdf*pmax) return;
    if (mISW[1] == 3) mISW[1] = 2;
    padd = pmax*.001 - pmin;
    for (ip = 1; ip <= mNPar; ++ip) {
       ndex = ip*(ip + 1) / 2;
       mVhmat[ndex-1] *= padd + 1;
    }
    mCstatu = "NOT POSDEF";
    qmnwarn("W", mCfrom.toStdString().data(), QString("MATRIX FORCED POS-DEF BY ADDING %1 TO DIAGONAL.").arg(padd).toStdString().data());


}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Called only by MNSIMP (and MNIMPR) to add a new point*-*-*-*-*-*-*
///*-*      =====================================================
///*-*        and remove an old one from the current simplex, and get the
///*-*        estimated distance to minimum.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnrazz(double ynew, double *pnew, double *y, qint32 &jh, qint32 &jl)
{
    /* Local variables */
    double pbig, plit;
    qint32 i, j, nparp1;

    /* Function Body */
    for (i = 1; i <= mNPar; ++i) { mP[i + jh*mMaxPar - mMaxPar-1] = pnew[i-1]; }
    y[jh-1] = ynew;
    if (ynew < mAmin) {
       for (i = 1; i <= mNPar; ++i) { mX[i-1] = pnew[i-1]; }
       qmninex(mX);
       mAmin   = ynew;
       mCstatu = "PROGRESS  ";
       jl      = jh;
    }
    jh     = 1;
    nparp1 = mNPar + 1;
    for (j = 2; j <= nparp1; ++j) { if (y[j-1] > y[jh-1]) jh = j; }
    mEDM = y[jh-1] - y[jl-1];
    if (mEDM <= 0) goto L45;
    for (i = 1; i <= mNPar; ++i) {
       pbig = mP[i-1];
       plit = pbig;
       for (j = 2; j <= nparp1; ++j) {
          if (mP[i + j*mMaxPar - mMaxPar-1] > pbig) pbig = mP[i + j*mMaxPar - mMaxPar-1];
          if (mP[i + j*mMaxPar - mMaxPar-1] < plit) plit = mP[i + j*mMaxPar - mMaxPar-1];
       }
       mDirin[i-1] = pbig - plit;
    }
 L40:
    return;
 L45:
    qInfo() << QString("  FUNCTION VALUE DOES NOT SEEM TO DEPEND ON ANY OF THE %1 VARIABLE PARAMETERS.").arg(mNPar);
    qInfo() << "          VERIFY THAT STEP SIZES ARE BIG ENOUGH AND CHECK FCN LOGIC.";
    qInfo() << " *******************************************************************************";
    qInfo() << " *******************************************************************************";
    goto L40;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*This is a super-portable random number generator*-*-*-*-*-*-*
///*-*          ================================================
///*-*         It should not overflow on any 32-bit machine.
///*-*         The cycle is only ~10**9, so use with care!
///*-*         Note especially that VAL must not be undefined on input.
///*-*                    Set Default Starting Seed
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnrn15(double &val, qint32 &inseed)
{
    /* Initialized data */

    static qint32 iseed = 12345;

    qint32 k;

    if (val == 3) goto L100;
    inseed = iseed;
    k      = iseed / 53668;
    iseed  = (iseed - k*53668)*40014 - k*12211;
    if (iseed < 0) iseed += 2147483563;
    val = double(iseed*4.656613e-10);
    return;
 //*-*               "entry" to set seed, flag is VAL=3
 L100:
    iseed = inseed;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*Resets function value and errors to UNDEFINED*-*-*-*-*-*-*-*
///*-*            =============================================
///*-*    If IOPT=1,
///*-*    If IOPT=0, sets only MINOS errors to undefined
///*-*        Called from MNCLER and whenever problem changes, for example
///*-*        after SET LIMITS, SET PARAM, CALL FCN 6
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnrset(qint32 iopt)
{
    qint32 iext, i;

    mCstatu = "RESET     ";
    if (iopt >= 1) {
       mAmin   = mUndefi;
       mFval3  = qAbs(mAmin)*2 + 1;
       mEDM    = mBigedm;
       mISW[3] = 0;
       mISW[1] = 0;
       mDcovar = 1;
       mISW[0] = 0;
    }
    mLnolim = true;
    for (i = 1; i <= mNPar; ++i) {
       iext = mNexofi[i-1];
       if (mNvarl[iext-1] >= 4) mLnolim = true;
       mErp[i-1] = 0;
       mErn[i-1] = 0;
       mGlobcc[i-1] = 0;
    }
    if (mISW[1] >= 1) {
       mISW[1] = 1;
       mDcovar = qMax(mDcovar,.5);
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Writes current parameter values and step sizes onto file ISYSSA*-*-*
///*-*    ===============================================================
///*-*          in format which can be reread by Minuit for restarting.
///*-*       The covariance matrix is also output if it exists.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnsave()
{
    qInfo() << "qmnsave is dummy in QMinuit";

}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Scans the values of FCN as a function of one parameter*-*-*-*-*-*
///*-*      ======================================================
///*-*        and plots the resulting values as a curve using MNPLOT.
///*-*        It may be called to scan one parameter or all parameters.
///*-*        retains the best function and parameter values found.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnscan()
{
    /* Local variables */
    double step, uhigh, xhreq, xlreq, ubest, fnext, unext, xh, xl;
    qint32 ipar, iint, icall, ncall, nbins, nparx;
    qint32 nxypt, nccall, iparwd;

    xlreq = qMin(mWord7[2],mWord7[3]);
    xhreq = qMax(mWord7[2],mWord7[3]);
    ncall = qint32((mWord7[1] + .01));
    if (ncall <= 1)  ncall = 41;
    if (ncall > 98) ncall = 98;
    nccall = ncall;
    if (mAmin == mUndefi) qmnamin();
    iparwd  = qint32((mWord7[0] + .1));
    ipar    = qMax(iparwd,0);
    mCstatu = "NO CHANGE";
    if (iparwd > 0) goto L200;

 //*-*-        equivalent to a loop over parameters requested
 L100:
    ++ipar;
    if (ipar > mNu) goto L900;
    iint = mNiofex[ipar-1];
    if (iint <= 0) goto L100;
 //*-*-        set up range for parameter IPAR
 L200:
    iint    = mNiofex[ipar-1];
    ubest    = mU[ipar-1];
    mXpt[0]  = ubest;
    mYpt[0]  = mAmin;
    mChpt[0] = ' ';
    mXpt[1]  = ubest;
    mYpt[1]  = mAmin;
    mChpt[1] = 'X';
    nxypt    = 2;
    if (mNvarl[ipar-1] > 1) goto L300;

 //*-*-        no limits on parameter
    if (xlreq == xhreq) goto L250;
    unext = xlreq;
    step = (xhreq - xlreq) / double(ncall-1);
    goto L500;
 L250:
    xl = ubest - mWerr[iint-1];
    xh = ubest + mWerr[iint-1];
    qmnbins(xl, xh, ncall, unext, uhigh, nbins, step);
    nccall = nbins + 1;
    goto L500;
 //*-*-        limits on parameter
 L300:
    if (xlreq == xhreq) goto L350;
 //*-*  Computing MAX
    xl = qMax(xlreq,mAlim[ipar-1]);
 //*-*  Computing MIN
    xh = qMin(xhreq,mBlim[ipar-1]);
    if (xl >= xh) goto L700;
    unext = xl;
    step  = (xh - xl) / double(ncall-1);
    goto L500;
 L350:
    unext = mAlim[ipar-1];
    step = (mBlim[ipar-1] - mAlim[ipar-1]) / double(ncall-1);
 //*-*-        main scanning loop over parameter IPAR
 L500:
    for (icall = 1; icall <= nccall; ++icall) {
       mU[ipar-1] = unext;
       nparx = mNPar;
       Eval(nparx, mGin, fnext, mU, 4);        ++mNfcn;
       ++nxypt;
       mXpt[nxypt-1]  = unext;
       mYpt[nxypt-1]  = fnext;
       mChpt[nxypt-1] = '*';
       if (fnext < mAmin) {
          mAmin   = fnext;
          ubest   = unext;
          mCstatu = "IMPROVED  ";
       }
       unext += step;
    }
    mChpt[nccall] = 0;

 //*-*-        finished with scan of parameter IPAR
    mU[ipar-1] = ubest;
    qmnexin(mX);
    if (mISW[4] >= 1)
    qInfo() << QString("%1SCAN OF PARAMETER NO. %2,  %3").arg(mNewpag).arg(ipar).arg(mCpnam[ipar-1]);
    qmnplot(mXpt, mYpt, mChpt, nxypt, mNpagwd, mNpagln);
    goto L800;
 L700:
    qInfo() << QString(" REQUESTED RANGE OUTSIDE LIMITS FOR PARAMETER  %1").arg(ipar);
 L800:
    if (iparwd <= 0) goto L100;
 //*-*-        finished with all parameters
 L900:
    if (mISW[4] >= 0) qmnprin(5, mAmin);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*Performs a rough (but global) minimization by monte carlo search*-*
///*-*    ================================================================
///*-*        Each time a new minimum is found, the search area is shifted
///*-*        to be centered at the best value.  Random points are chosen
///*-*        uniformly over a hypercube determined by current step sizes.
///*-*   The Metropolis algorithm accepts a worse point with probability
///*-*      exp(-d/UP), where d is the degradation.  Improved points
///*-*      are of course always accepted.  Actual steps are random
///*-*      multiples of the nominal steps (DIRIN).
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnseek()
{
    /* Local variables */
    double dxdi, rnum, ftry, rnum1, rnum2, alpha;
    double flast, bar;
    qint32 ipar, iext, j, ifail, iseed=0, nparx, istep, ib, mxfail, mxstep;

    mxfail = qint32(mWord7[0]);
    if (mxfail <= 0) mxfail = mNPar*20 + 100;
    mxstep = mxfail*10;
    if (mAmin == mUndefi) qmnamin();
    alpha = mWord7[1];
    if (alpha <= 0) alpha = 3;
    if (mISW[4] >= 1) {
       qInfo() << " MNSEEK: MONTE CARLO MINIMIZATION USING METROPOLIS ALGORITHM";
       qInfo() << QString(" TO STOP AFTER %1 SUCCESSIVE FAILURES, OR %2 STEPS").arg(mxfail).arg(mxstep);
       qInfo() << QString(" MAXIMUM STEP SIZE IS %1 ERROR BARS.").arg(alpha);
    }
    mCstatu = "INITIAL  ";
    if (mISW[4] >= 2) qmnprin(2, mAmin);
    mCstatu = "UNCHANGED ";
    ifail   = 0;
    rnum    = 0;
    rnum1   = 0;
    rnum2   = 0;
    nparx   = mNPar;
    flast   = mAmin;
 //*-*-             set up step sizes, starting values
    for (ipar = 1; ipar <= mNPar; ++ipar) {
       iext = mNexofi[ipar-1];
       mDirin[ipar-1] = alpha*2*mWerr[ipar-1];
       if (mNvarl[iext-1] > 1) {
 //*-*-             parameter with limits
          qmndxdi(mX[ipar-1], ipar-1, dxdi);
          if (dxdi == 0) dxdi = 1;
          mDirin[ipar-1] = alpha*2*mWerr[ipar-1] / dxdi;
          if (qAbs(mDirin[ipar-1]) > 6.2831859999999997) {
             mDirin[ipar-1] = 6.2831859999999997;
          }
       }
       mSEEKxmid[ipar-1]  = mX[ipar-1];
       mSEEKxbest[ipar-1] = mX[ipar-1];
    }
 //*-*-                             search loop
    for (istep = 1; istep <= mxstep; ++istep) {
       if (ifail >= mxfail) break;
       for (ipar = 1; ipar <= mNPar; ++ipar) {
          qmnrn15(rnum1, iseed);
          qmnrn15(rnum2, iseed);
          mX[ipar-1] = mSEEKxmid[ipar-1] + (rnum1 + rnum2 - 1)*.5*mDirin[ipar-1];
       }
       qmninex(mX);
       Eval(nparx, mGin, ftry, mU, 4);        ++mNfcn;
       if (ftry < flast) {
          if (ftry < mAmin) {
             mCstatu = "IMPROVEMNT";
             mAmin = ftry;
             for (ib = 1; ib <= mNPar; ++ib) { mSEEKxbest[ib-1] = mX[ib-1]; }
             ifail = 0;
             if (mISW[4] >= 2) qmnprin(2, mAmin);
          }
          goto L300;
       } else {
          ++ifail;
 //*-*-                  Metropolis algorithm
          bar = (mAmin - ftry) / mUp;
          qmnrn15(rnum, iseed);
          if (bar < qLn(rnum)/qLn(10.)) continue;
       }
 //*-*-                   Accept new point, move there
 L300:
       for (j = 1; j <= mNPar; ++j) { mSEEKxmid[j-1] = mX[j-1];        }
       flast = ftry;
    }
 //*-*-                              end search loop
    if (mISW[4] > 1) {
       qInfo() << QString(" MNSEEK: %1 SUCCESSIVE UNSUCCESSFUL TRIALS.").arg(ifail);
    }
    for (ib = 1; ib <= mNPar; ++ib) { mX[ib-1] = mSEEKxbest[ib-1]; }
    qmninex(mX);
    if (mISW[4] >= 1) qmnprin(2, mAmin);
    if (mISW[4] == 0) qmnprin(0, mAmin);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Interprets the commands that start with SET and SHOW*-*-*-*-*-*-*
///*-*      ====================================================
///*-*        Called from MNEXCM
///*-*        file characteristics for SET INPUT
///*-*       'SET ' or 'SHOW',  'ON ' or 'OFF', 'SUPPRESSED' or 'REPORTED  '
///*-*        explanation of print level numbers -1:3  and strategies 0:2
///*-*        identification of debug options
///*-*        things that can be set or shown
///*-*        options not intended for normal users
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnset()
{
    /* Initialized data */

    static const char *cname[30] = {
       "FCN value ",
       "PARameters",
       "LIMits    ",
       "COVariance",
       "CORrelatio",
       "PRInt levl",
       "NOGradient",
       "GRAdient  ",
       "ERRor def ",
       "INPut file",
       "WIDth page",
       "LINes page",
       "NOWarnings",
       "WARnings  ",
       "RANdom gen",
       "TITle     ",
       "STRategy  ",
       "EIGenvalue",
       "PAGe throw",
       "MINos errs",
       "EPSmachine",
       "OUTputfile",
       "BATch     ",
       "INTeractiv",
       "VERsion   ",
       "reserve   ",
       "NODebug   ",
       "DEBug     ",
       "SHOw      ",
       "SET       "};

    static qint32 nname = 25;
    static qint32 nntot = 30;
    static QString cprlev[5] = {
       "-1: NO OUTPUT EXCEPT FROM SHOW    ",
       " 0: REDUCED OUTPUT                ",
       " 1: NORMAL OUTPUT                 ",
       " 2: EXTRA OUTPUT FOR PROBLEM CASES",
       " 3: MAXIMUM OUTPUT                "};

    static QString cstrat[3] = {
       " 0: MINIMIZE THE NUMBER OF CALLS TO FUNCTION",
       " 1: TRY TO BALANCE SPEED AGAINST RELIABILITY",
       " 2: MAKE SURE MINIMUM TRUE, ERRORS CORRECT  "};

    static QString cdbopt[7] = {
       "REPORT ALL EXCEPTIONAL CONDITIONS      ",
       "MNLINE: LINE SEARCH MINIMIZATION       ",
       "MNDERI: FIRST DERIVATIVE CALCULATIONS  ",
       "MNHESS: SECOND DERIVATIVE CALCULATIONS ",
       "MNMIGR: COVARIANCE MATRIX UPDATES      ",
       "MNHES1: FIRST DERIVATIVE UNCERTAINTIES ",
       "MNCONT: MNCONTOUR PLOT (MNCROS SEARCH) "};

    /* System generated locals */
    //qint32 f_inqu();

    /* Local variables */
    double val;
    qint32 iset, iprm, i, jseed, kname, iseed, iunit, id, ii, kk;
    qint32 ikseed, idbopt, igrain=0, iswsav, isw2;
    QString  cfname, cmode, ckind,  cwarn, copt, ctemp, ctemp2;
    bool lname=false;

    for (i = 1; i <= nntot; ++i) {
       ctemp  = cname[i-1];
       ckind  = ctemp.left(3);
       ctemp2 = QString(mCword.midRef(4,6).toString().data());
       if (strstr(ctemp2.toStdString().data(),ckind.toStdString().data())) goto L5;
    }
    i = 0;
 L5:
    kname = i;

 //*-*-          Command could be SET xxx, SHOW xxx,  HELP SET or HELP SHOW
    ctemp2 = mCword.left(3);
    if ( ctemp2.contains("HEL"))  goto L2000;
    if ( ctemp2.contains("SHO"))  goto L1000;
    if (!ctemp2.contains("SET"))  goto L1900;
 //*-*-                          ---
    ckind = "SET ";
 //*-*-                                       . . . . . . . . . . set unknown
    if (kname <= 0) goto L1900;
 //*-*-                                       . . . . . . . . . . set known
    switch ((int)kname) {
       case 1:  goto L3000;
       case 2:  goto L20;
       case 3:  goto L30;
       case 4:  goto L40;
       case 5:  goto L3000;
       case 6:  goto L60;
       case 7:  goto L70;
       case 8:  goto L80;
       case 9:  goto L90;
       case 10:  goto L100;
       case 11:  goto L110;
       case 12:  goto L120;
       case 13:  goto L130;
       case 14:  goto L140;
       case 15:  goto L150;
       case 16:  goto L160;
       case 17:  goto L170;
       case 18:  goto L3000;
       case 19:  goto L190;
       case 20:  goto L3000;
       case 21:  goto L210;
       case 22:  goto L220;
       case 23:  goto L230;
       case 24:  goto L240;
       case 25:  goto L3000;
       case 26:  goto L1900;
       case 27:  goto L270;
       case 28:  goto L280;
       case 29:  goto L290;
       case 30:  goto L300;
    }

 //*-*-                                       . . . . . . . . . . set param
 L20:
    iprm = qint32(mWord7[0]);
    if (iprm > mNu) goto L25;
    if (iprm <= 0) goto L25;
    if (mNvarl[iprm-1] < 0) goto L25;
    mU[iprm-1] = mWord7[1];
    qmnexin(mX);
    isw2 = mISW[1];
    qmnrset(1);
 //*-*-       Keep approximate covariance matrix, even if new param value
    mISW[1] = qMin(isw2,1);
    mCfrom  = "SET PARM";
    mNfcnfr = mNfcn;
    mCstatu = "NEW VALUES";
    return;
 L25:
    qInfo() << " UNDEFINED PARAMETER NUMBER.  IGNORED.";
    return;
 //*-*-                                       . . . . . . . . . . set limits
 L30:
    qmnlims();
    return;
 //*-*-                                       . . . . . . . . . . set covar
 L40:
 //*-*   this command must be handled by MNREAD, and is not Fortran-callable
    goto L3000;
 //*-*-                                       . . . . . . . . . . set print
 L60:
    mISW[4] = qint32(mWord7[0]);
    return;
 //*-*-                                       . . . . . . . . . . set nograd
 L70:
    mISW[2] = 0;
    return;
 //*-*-                                       . . . . . . . . . . set grad
 L80:
    qmngrad();
    return;
 //*-*-                                       . . . . . . . . . . set errdef
 L90:
    if (mWord7[0] == mUp) return;
    if (mWord7[0] <= 0) {
       if (mUp == mUpdflt) return;
       mUp = mUpdflt;
    } else {
       mUp = mWord7[0];
    }
    for (i = 1; i <= mNPar; ++i) {
       mErn[i-1] = 0;
       mErp[i-1] = 0;
    }
    qmnwerr();
    return;
 //*-*-                                       . . . . . . . . . . set input
 //*-* This command must be handled by MNREAD. If it gets this far,
 //*-*-        it is illegal.
 L100:
    goto L3000;
 //*-*-                                       . . . . . . . . . . set width
 L110:
    mNpagwd = qint32(mWord7[0]);
    mNpagwd = qMax(mNpagwd,50);
    return;

 L120:
    mNpagln = qint32(mWord7[0]);
    return;
 //*-*-                                       . . . . . . . . . . set nowarn

 L130:
    mLwarn = false;
    return;
 //*-*-                                       . . . . . . . . . . set warn
 L140:
    mLwarn = true;
    qmnwarn("W", "SHO", "SHO");
    return;
 //*-*-                                       . . . . . . . . . . set random
 L150:
    jseed = qint32(mWord7[0]);
    val = 3;
    qmnrn15(val, jseed);
    if (mISW[4] > 0) {
       qInfo() << QString(" MINUIT RANDOM NUMBER SEED SET TO %1").arg(jseed);
    }
    return;
 //*-*-                                       . . . . . . . . . . set title
 L160:
 //*-*   this command must be handled by MNREAD, and is not Fortran-callable
    goto L3000;
 //*-*-                                       . . . . . . . . . set strategy
 L170:
    mIstrat = qint32(mWord7[0]);
    mIstrat = qMax(mIstrat,0);
    mIstrat = qMin(mIstrat,2);
    if (mISW[4] > 0) goto L1172;
    return;
 //*-*-                                      . . . . . . . . . set page throw
 L190:
    mNewpag = qint32(mWord7[0]);
    goto L1190;
 //*-*-                                       . . . . . . . . . . set epsmac
 L210:
    if (mWord7[0] > 0 && mWord7[0] < .1) {
       mEpsmac = mWord7[0];
    }
    mEpsma2 = qSqrt(mEpsmac);
    goto L1210;
 //*-*-                                      . . . . . . . . . . set outputfile
 L220:
    iunit = qint32(mWord7[0]);
    mIsyswr = iunit;
    mIstkwr[0] = iunit;
    if (mISW[4] >= 0) goto L1220;
    return;
 //*-*-                                       . . . . . . . . . . set batch
 L230:
    mISW[5] = 0;
    if (mISW[4] >= 0) goto L1100;
    return;
 //*-*-                                      . . . . . . . . . . set interactive
 L240:
    mISW[5] = 1;
    if (mISW[4] >= 0) goto L1100;
    return;
 //*-*-                                       . . . . . . . . . . set nodebug
 L270:
    iset = 0;
    goto L281;
 //*-*-                                       . . . . . . . . . . set debug
 L280:
    iset = 1;
 L281:
    idbopt = qint32(mWord7[0]);
    if (idbopt > 6) goto L288;
    if (idbopt >= 0) {
       mIdbg[idbopt] = iset;
       if (iset == 1) mIdbg[0] = 1;
    } else {
 //*-*-            SET DEBUG -1  sets all debug options
       for (id = 0; id <= 6; ++id) { mIdbg[id] = iset; }
    }
    mLrepor = mIdbg[0] >= 1;
    qmnwarn("D", "SHO", "SHO");
    return;
 L288:
    qInfo() << QString(" UNKNOWN DEBUG OPTION %1 REQUESTED. IGNORED").arg(idbopt);
    return;
 //*-*-                                       . . . . . . . . . . set show
 L290:
 //*-*-                                       . . . . . . . . . . set set
 L300:
    goto L3000;
 //*-*-               -----------------------------------------------------
 L1000:
 //*-*-              at this point, CWORD must be 'SHOW'
    ckind = "SHOW";
    if (kname <= 0) goto L1900;

    switch ((int)kname) {
       case 1:  goto L1010;
       case 2:  goto L1020;
       case 3:  goto L1030;
       case 4:  goto L1040;
       case 5:  goto L1050;
       case 6:  goto L1060;
       case 7:  goto L1070;
       case 8:  goto L1070;
       case 9:  goto L1090;
       case 10:  goto L1100;
       case 11:  goto L1110;
       case 12:  goto L1120;
       case 13:  goto L1130;
       case 14:  goto L1130;
       case 15:  goto L1150;
       case 16:  goto L1160;
       case 17:  goto L1170;
       case 18:  goto L1180;
       case 19:  goto L1190;
       case 20:  goto L1200;
       case 21:  goto L1210;
       case 22:  goto L1220;
       case 23:  goto L1100;
       case 24:  goto L1100;
       case 25:  goto L1250;
       case 26:  goto L1900;
       case 27:  goto L1270;
       case 28:  goto L1270;
       case 29:  goto L1290;
       case 30:  goto L1300;
    }

 //*-*-                                       . . . . . . . . . . show fcn
 L1010:
    if (mAmin == mUndefi) qmnamin();
    qmnprin(0, mAmin);
    return;
 //*-*-                                       . . . . . . . . . . show param
 L1020:
    if (mAmin == mUndefi) qmnamin();
    qmnprin(5, mAmin);
    return;
 //*-*-                                       . . . . . . . . . . show limits
 L1030:
    if (mAmin == mUndefi) qmnamin();
    qmnprin(1, mAmin);
    return;
 //*-*-                                       . . . . . . . . . . show covar
 L1040:
    qmnmatu(1);
    return;
 //*-*-                                       . . . . . . . . . . show corre
 L1050:
    qmnmatu(0);
    return;
 //*-*-                                       . . . . . . . . . . show print
 L1060:
    if (mISW[4] < -1) mISW[4] = -1;
    if (mISW[4] > 3)  mISW[4] = 3;
    qInfo() << " ALLOWED PRINT LEVELS ARE:";
    qInfo() << QString("                           %1").arg(cprlev[0].toStdString().data());
    qInfo() << QString("                           %1").arg(cprlev[1].toStdString().data());
    qInfo() << QString("                           %1").arg(cprlev[2].toStdString().data());
    qInfo() << QString("                           %1").arg(cprlev[3].toStdString().data());
    qInfo() << QString("                           %1").arg(cprlev[4].toStdString().data());
    qInfo() << QString(" CURRENT PRINTOUT LEVEL IS %1").arg(cprlev[mISW[4]+1].toStdString().data());
    return;
 //*-*-                                       . . . . . . . show nograd, grad
 L1070:
    if (mISW[2] <= 0) {
       qInfo() << " NOGRAD IS SET.  DERIVATIVES NOT COMPUTED IN FCN.";
    } else {
       qInfo() << "   GRAD IS SET.  USER COMPUTES DERIVATIVES IN FCN.";
    }
    return;
 //*-*-                                      . . . . . . . . . . show errdef
 L1090:
    qInfo() << QString(" ERRORS CORRESPOND TO FUNCTION CHANGE OF %1").arg(mUp);
    return;
 //*-*-                                      . . . . . . . . . . show input,
 //*-*-                                               batch, or interactive
 L1100:
 //    ioin__1.inerr = 0;
 //    ioin__1.inunit = mIsysrd;
 //    ioin__1.infile = 0;
 //    ioin__1.inex = 0;
 //    ioin__1.inopen = 0;
 //    ioin__1.innum = 0;
 //    ioin__1.innamed = &lname;
 //    ioin__1.innamlen = 64;
 //    ioin__1.inname = cfname;
 //    ioin__1.inacc = 0;
 //    ioin__1.inseq = 0;
 //    ioin__1.indir = 0;
 //    ioin__1.infmt = 0;
 //    ioin__1.inform = 0;
 //    ioin__1.inunf = 0;
 //    ioin__1.inrecl = 0;
 //    ioin__1.innrec = 0;
 //    ioin__1.inblank = 0;
 //    f_inqu(&ioin__1);
    cmode = "BATCH MODE      ";
    if (mISW[5] == 1) cmode  = "INTERACTIVE MODE";
    if (! lname)      cfname = "unknown";
    qInfo() << QString(" INPUT NOW BEING READ IN %1 FROM UNIT NO. %2 FILENAME: %3").arg(cmode).arg(mIsysrd).arg(cfname);
    return;
 //*-*-                                      . . . . . . . . . . show width
 L1110:
    qInfo() << QString("          PAGE WIDTH IS SET TO %1 COLUMNS").arg(mNpagwd);
    return;
 //*-*-                                      . . . . . . . . . . show lines
 L1120:
    qInfo() << QString("          PAGE LENGTH IS SET TO %1 LINES").arg(mNpagln);
    return;
 //*-*-                                      . . . . . . .show nowarn, warn
 L1130:
    cwarn = "SUPPRESSED";
    if (mLwarn) cwarn = "REPORTED  ";
    qInfo() << cwarn;
    if (! mLwarn) qmnwarn("W", "SHO", "SHO");
    return;
 //*-*-                                     . . . . . . . . . . show random
 L1150:
    val = 0;
    qmnrn15(val, igrain);
    ikseed = igrain;
    qInfo() << QString(" MINUIT RNDM SEED IS CURRENTLY=%1").arg(ikseed);
    val   = 3;
    iseed = ikseed;
    qmnrn15(val, iseed);
    return;
 //*-*-                                       . . . . . . . . . show title
 L1160:
    qInfo() << QString(" TITLE OF CURRENT TASK IS:%1").arg(mCtitl);
    return;
 //*-*-                                       . . . . . . . show strategy
 L1170:
    qInfo() << " ALLOWED STRATEGIES ARE:";
    qInfo() << QString("                    %1").arg(cstrat[0].toStdString().data());
    qInfo() << QString("                    %1").arg(cstrat[1].toStdString().data());
    qInfo() << QString("                    %1").arg(cstrat[2].toStdString().data());
 L1172:
    qInfo() << QString(" NOW USING STRATEGY %1").arg(cstrat[mIstrat]);
    return;
 //*-*-                                         . . . . . show eigenvalues
 L1180:
    iswsav = mISW[4];
    mISW[4] = 3;
    if (mISW[1] < 1) {
       qInfo() << mCovmes[0];
    } else {
       qmnpsdf();
    }
    mISW[4] = iswsav;
    return;
 //*-*-                                           . . . . . show page throw
 L1190:
    qInfo() << QString(" PAGE THROW CARRIAGE CONTROL = %1").arg(mNewpag);
    if (mNewpag == 0) {
       qInfo() << " NO PAGE THROWS IN MINUIT OUTPUT";
    }
    return;
 //*-*-                                       . . . . . . show minos errors
 L1200:
    for (ii = 1; ii <= mNPar; ++ii) {
       if (mErp[ii-1] > 0 || mErn[ii-1] < 0) goto L1204;
    }
    qInfo() << "       THERE ARE NO MINOS ERRORS CURRENTLY VALID.";
    return;
 L1204:
    qmnprin(4, mAmin);
    return;
 //*-*-                                       . . . . . . . . . show epsmac
 L1210:
    qInfo() << QString(" FLOATING-POINT NUMBERS ASSUMED ACCURATE TO %1").arg(mEpsmac);
    return;
 //*-*-                                       . . . . . . show outputfiles
 L1220:
    qInfo() << QString("  MINUIT PRIMARY OUTPUT TO UNIT %1").arg(mIsyswr);
    return;
 //*-*-                                       . . . . . . show version
 L1250:
    qInfo() << QString(" THIS IS MINUIT VERSION:%1").arg(mCvrsn);
    return;
 //*-*-                                       . . . . . . show nodebug, debug
 L1270:
    for (id = 0; id <= 6; ++id) {
       copt = "OFF";
       if (mIdbg[id] >= 1) copt = "ON ";
       qInfo() << QString("          DEBUG OPTION %1 IS %2 :%3").arg(id).arg(copt).arg(cdbopt[id]);
    }
    if (! mLrepor) qmnwarn("D", "SHO", "SHO");
    return;
 //*-*-                                       . . . . . . . . . . show show
 L1290:
    ckind = "SHOW";
    goto L2100;
 //*-*-                                       . . . . . . . . . . show set
 L1300:
    ckind = "SET ";
    goto L2100;
 //*-*-               -----------------------------------------------------
 //*-*-                             UNKNOWN COMMAND
 L1900:
    qInfo() << QString(" THE COMMAND:%1 IS UNKNOWN.").arg(mCword);
    goto L2100;
 //*-*-               -----------------------------------------------------
 //*-*-                   HELP SHOW,  HELP SET,  SHOW SET, or SHOW SHOW
 L2000:
    ckind = "SET ";
    ctemp2 = QString(mCword.midRef(3,7).toString().data());
    if (strcmp(ctemp2.toStdString().data(), "SHO")) ckind = "SHOW";
 L2100:
    qInfo() << QString(" THE FORMAT OF THE %1 COMMAND IS:").arg(ckind);
    qInfo() << QString(" %1 xxx    [numerical arguments if any]").arg(ckind);
    qInfo() << " WHERE xxx MAY BE ONE OF THE FOLLOWING:";
    for (kk = 1; kk <= nname; ++kk) {
       qInfo() << cname[kk-1];
    }
    return;
 //*-*-               -----------------------------------------------------
 //*-*-                              ILLEGAL COMMAND
 L3000:
    qInfo() << " ABOVE COMMAND IS ILLEGAL.   IGNORED";
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Minimization using the simplex method of Nelder and Mead*-*-*-*-*
///*-*      ========================================================
///*-*        Performs a minimization using the simplex method of Nelder
///*-*        and Mead (ref. -- Comp. J. 7,308 (1965)).
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnsimp()
{
    /* Initialized data */

    static double alpha = 1;
    static double beta = .5;
    static double gamma = 2;
    static double rhomin = 4;
    static double rhomax = 8;

    /* Local variables */
    double dmin_, dxdi, yrho, f, ynpp1, aming, ypbar;
    double bestx, ystar, y1, y2, ystst, pb, wg;
    double absmin, rho, sig2, rho1, rho2;
    qint32 npfn, i, j, k, jhold, ncycl, nparx;
    qint32 nparp1, kg, jh, nf, jl, ns;

    if (mNPar <= 0) return;
    if (mAmin == mUndefi) qmnamin();
    mCfrom  = "SIMPLEX ";
    mNfcnfr = mNfcn;
    mCstatu = "UNCHANGED ";
    npfn    = mNfcn;
    nparp1  = mNPar + 1;
    nparx   = mNPar;
    rho1    = alpha + 1;
    rho2    = rho1 + alpha*gamma;
    wg      = 1 / double(mNPar);
    if (mISW[4] >= 0) {
       qInfo() << QString(" START SIMPLEX MINIMIZATION.    CONVERGENCE WHEN EDM .LT. %1").arg(mEpsi);
    }
    for (i = 1; i <= mNPar; ++i) {
       mDirin[i-1] = mWerr[i-1];
       qmndxdi(mX[i-1], i-1, dxdi);
       if (dxdi != 0) mDirin[i-1] = mWerr[i-1] / dxdi;
       dmin_ = mEpsma2*qAbs(mX[i-1]);
       if (mDirin[i-1] < dmin_) mDirin[i-1] = dmin_;
    }
 //*-* **       choose the initial simplex using single-parameter searches
 L1:
    ynpp1 = mAmin;
    jl = nparp1;
    mSIMPy[nparp1-1] = mAmin;
    absmin = mAmin;
    for (i = 1; i <= mNPar; ++i) {
       aming      = mAmin;
       mPbar[i-1] = mX[i-1];
       bestx      = mX[i-1];
       kg         = 0;
       ns         = 0;
       nf         = 0;
 L4:
       mX[i-1] = bestx + mDirin[i-1];
       qmninex(mX);
       Eval(nparx, mGin, f, mU, 4);        ++mNfcn;
       if (f <= aming) goto L6;
 //*-*-        failure
       if (kg == 1) goto L8;
       kg = -1;
       ++nf;
       mDirin[i-1] *= -.4;
       if (nf < 3) goto L4;
       ns = 6;
 //*-*-        success
 L6:
       bestx        = mX[i-1];
       mDirin[i-1] *= 3;
       aming        = f;
       mCstatu      = "PROGRESS  ";
       kg           = 1;
       ++ns;
       if (ns < 6) goto L4;
 //*-*-        local minimum found in ith direction
 L8:
       mSIMPy[i-1] = aming;
       if (aming < absmin) jl = i;
       if (aming < absmin) absmin = aming;
       mX[i-1] = bestx;
       for (k = 1; k <= mNPar; ++k) { mP[k + i*mMaxPar - mMaxPar-1] = mX[k-1]; }
    }
    jh    = nparp1;
    mAmin = mSIMPy[jl-1];
    qmnrazz(ynpp1, mPbar, mSIMPy, jh, jl);
    for (i = 1; i <= mNPar; ++i) { mX[i-1] = mP[i + jl*mMaxPar - mMaxPar-1]; }
    qmninex(mX);
    mCstatu = "PROGRESS  ";
    if (mISW[4] >= 1) qmnprin(5, mAmin);
    mEDM  = mBigedm;
    sig2  = mEDM;
    ncycl = 0;
 //*-*-                                       . . . . .  start main loop
 L50:
    if (sig2 < mEpsi && mEDM < mEpsi) goto L76;
    sig2 = mEDM;
    if (mNfcn - npfn > mNfcnmx) goto L78;
 //*-*-        calculate new point * by reflection
    for (i = 1; i <= mNPar; ++i) {
       pb = 0;
       for (j = 1; j <= nparp1; ++j) { pb += wg*mP[i + j*mMaxPar - mMaxPar-1]; }
       mPbar[i-1]  = pb - wg*mP[i + jh*mMaxPar - mMaxPar-1];
       mPstar[i-1] = (alpha + 1)*mPbar[i-1] - alpha*mP[i + jh*mMaxPar - mMaxPar-1];
    }
    qmninex(mPstar);
    Eval(nparx, mGin, ystar, mU, 4);    ++mNfcn;
    if (ystar >= mAmin) goto L70;
 //*-*-        point * better than jl, calculate new point **
    for (i = 1; i <= mNPar; ++i) {
       mPstst[i-1] = gamma*mPstar[i-1] + (1 - gamma)*mPbar[i-1];
    }
    qmninex(mPstst);
    Eval(nparx, mGin, ystst, mU, 4);    ++mNfcn;
 //*-*-        try a parabola through ph, pstar, pstst.  min = prho
    y1 = (ystar - mSIMPy[jh-1])*rho2;
    y2 = (ystst - mSIMPy[jh-1])*rho1;
    rho = (rho2*y1 - rho1*y2)*.5 / (y1 - y2);
    if (rho < rhomin) goto L66;
    if (rho > rhomax) rho = rhomax;
    for (i = 1; i <= mNPar; ++i) {
       mPrho[i-1] = rho*mPbar[i-1] + (1 - rho)*mP[i + jh*mMaxPar - mMaxPar-1];
    }
    qmninex(mPrho);
    Eval(nparx, mGin, yrho, mU, 4);    ++mNfcn;
    if (yrho  < mSIMPy[jl-1] && yrho < ystst) goto L65;
    if (ystst < mSIMPy[jl-1]) goto L67;
    if (yrho  > mSIMPy[jl-1]) goto L66;
 //*-*-        accept minimum point of parabola, PRHO
 L65:
    qmnrazz(yrho, mPrho, mSIMPy, jh, jl);
    goto L68;
 L66:
    if (ystst < mSIMPy[jl-1]) goto L67;
    qmnrazz(ystar, mPstar, mSIMPy, jh, jl);
    goto L68;
 L67:
    qmnrazz(ystst, mPstst, mSIMPy, jh, jl);
 L68:
    ++ncycl;
    if (mISW[4] < 2) goto L50;
    if (mISW[4] >= 3 || ncycl % 10 == 0) {
       qmnprin(5, mAmin);
    }
    goto L50;
 //*-*-        point * is not as good as jl
 L70:
    if (ystar >= mSIMPy[jh-1]) goto L73;
    jhold = jh;
    qmnrazz(ystar, mPstar, mSIMPy, jh, jl);
    if (jhold != jh) goto L50;
 //*-*-        calculate new point **
 L73:
    for (i = 1; i <= mNPar; ++i) {
       mPstst[i-1] = beta*mP[i + jh*mMaxPar - mMaxPar-1] + (1 - beta)*mPbar[i-1];
    }
    qmninex(mPstst);
    Eval(nparx, mGin, ystst, mU, 4);    ++mNfcn;
    if (ystst > mSIMPy[jh-1]) goto L1;
 //*-*-    point ** is better than jh
    if (ystst < mAmin) goto L67;
    qmnrazz(ystst, mPstst, mSIMPy, jh, jl);
    goto L50;
 //*-*-                                       . . . . . .  end main loop
 L76:
    if (mISW[4] >= 0) {
       qInfo() << " SIMPLEX MINIMIZATION HAS CONVERGED.";
    }
    mISW[3] = 1;
    goto L80;
 L78:
    if (mISW[4] >= 0) {
       qInfo() << " SIMPLEX TERMINATES WITHOUT CONVERGENCE.";
    }
    mCstatu = "CALL LIMIT";
    mISW[3] = -1;
    mISW[0] = 1;
 L80:
    for (i = 1; i <= mNPar; ++i) {
       pb = 0;
       for (j = 1; j <= nparp1; ++j) { pb += wg*mP[i + j*mMaxPar - mMaxPar-1]; }
       mPbar[i-1] = pb - wg*mP[i + jh*mMaxPar - mMaxPar-1];
    }
    qmninex(mPbar);
    Eval(nparx, mGin, ypbar, mU, 4);    ++mNfcn;
    if (ypbar < mAmin)         qmnrazz(ypbar, mPbar, mSIMPy, jh, jl);
    qmninex(mX);
    if (mNfcnmx + npfn - mNfcn < mNPar*3) goto L90;
    if (mEDM > mEpsi*2) goto L1;
 L90:
    if (mISW[4] >= 0) qmnprin(5, mAmin);
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*Returns concerning the current status of the minimization*-*-*-*-*
///*-*      =========================================================
///*-*       User-called
///*-*          Namely, it returns:
///*-*        FMIN: the best function value found so far
///*-*        mEDM: the estimated vertical distance remaining to minimum
///*-*        ERRDEF: the value of UP defining parameter uncertainties
///*-*        NPARI: the number of currently variable parameters
///*-*        NPARX: the highest (external) parameter number defined by user
///*-*        ISTAT: a status integer indicating how good is the covariance
///*-*           matrix:  0= not calculated at all
///*-*                    1= approximation only, not accurate
///*-*                    2= full matrix, but forced positive-definite
///*-*                    3= full accurate covariance matrix
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnstat(double &fmin, double &fedm, double &errdef, qint32 &npari, qint32 &nparx, qint32 &istat)
{
    fmin   = mAmin;
    fedm   = mEDM;
    errdef = mUp;
    npari  = mNPar;
    nparx  = mNu;
    istat  = mISW[1];
    if (mEDM == mBigedm) mEDM = mUp;
    if (mAmin == mUndefi) {
       fmin  = 0;
       mEDM  = mUp;
       istat = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*To find the machine precision*-*-*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*            =============================
///*-*        Compares its argument with the value 1.0, and returns
///*-*        the value .TRUE. if they are equal.  To find EPSMAC
///*-*        safely by foiling the Fortran optimizer
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmntiny(volatile double epsp1, double &epsbak)
{
    epsbak = epsp1 - 1;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*Returns .TRUE. if CFNAME contains unprintable characters*-*-*-*
///*-*        ========================================================
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

bool QMinuit::qmnunpt(QString &cfname)
{
    qint32 i, l, ic;
    bool ret_val;
    static QString cpt = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890./;:[]$%*_!@#&+()";

    ret_val = false;
    l       = cfname.length();
    for (i = 1; i <= l; ++i) {
       for (ic = 1; ic <= 80; ++ic) {
          if (cfname[i-1] == cpt[ic-1]) goto L100;
       }
       return true;
 L100:
       ;
    }
    return ret_val;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*-*-*Inverts a symmetric matrix*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*                    ==========================
///*-*        inverts a symmetric matrix.   matrix is first scaled to
///*-*        have all ones on the diagonal (equivalent to change of units)
///*-*        but no pivoting is done since matrix is positive-definite.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnvert(double *a, qint32 l, qint32 /*m*/, qint32 n, qint32 &ifail)
{
    /* System generated locals */
    qint32 a_offset;

    /* Local variables */
    double si;
    qint32 i, j, k, kp1, km1;

    /* Parameter adjustments */
    a_offset = l + 1;
    a -= a_offset;

    /* Function Body */
    ifail = 0;
    if (n < 1) goto L100;
    if (n > mMaxInt) goto L100;
 //*-*-                  scale matrix by sqrt of diag elements
    for (i = 1; i <= n; ++i) {
       si = a[i + i*l];
       if (si <= 0) goto L100;
       mVERTs[i-1] = 1 / qSqrt(si);
    }
    for (i = 1; i <= n; ++i) {
       for (j = 1; j <= n; ++j) {
          a[i + j*l] = a[i + j*l]*mVERTs[i-1]*mVERTs[j-1];
       }
    }
 //*-*-                                       . . . start main loop . . . .
    for (i = 1; i <= n; ++i) {
       k = i;
 //*-*-                  preparation for elimination step1
       if (a[k + k*l] != 0) mVERTq[k-1] = 1 / a[k + k*l];
       else goto L100;
       mVERTpp[k-1] = 1;
       a[k + k*l] = 0;
       kp1 = k + 1;
       km1 = k - 1;
       if (km1 < 0) goto L100;
       else if (km1 == 0) goto L50;
       else               goto L40;
 L40:
       for (j = 1; j <= km1; ++j) {
          mVERTpp[j-1] = a[j + k*l];
          mVERTq[j-1]  = a[j + k*l]*mVERTq[k-1];
          a[j + k*l]   = 0;
       }
 L50:
       if (k - n < 0) goto L51;
       else if (k - n == 0) goto L60;
       else                goto L100;
 L51:
       for (j = kp1; j <= n; ++j) {
          mVERTpp[j-1] = a[k + j*l];
          mVERTq[j-1]  = -a[k + j*l]*mVERTq[k-1];
          a[k + j*l]   = 0;
       }
 //*-*-                  elimination proper
 L60:
       for (j = 1; j <= n; ++j) {
          for (k = j; k <= n; ++k) { a[j + k*l] += mVERTpp[j-1]*mVERTq[k-1]; }
       }
    }
 //*-*-                  elements of left diagonal and unscaling
    for (j = 1; j <= n; ++j) {
       for (k = 1; k <= j; ++k) {
          a[k + j*l] = a[k + j*l]*mVERTs[k-1]*mVERTs[j-1];
          a[j + k*l] = a[k + j*l];
       }
    }
    return;
 //*-*-                  failure return
 L100:
    ifail = 1;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*-*-*-*-*Prints Warning messages*-*-*-*-*-*-*-*-*-*-*-*-*-*
///*-*                    =======================
///*-*     If COPT='W', CMES is a WARning message from CORG.
///*-*     If COPT='D', CMES is a DEBug message from CORG.
///*-*         If SET WARnings is in effect (the default), this routine
///*-*             prints the warning message CMES coming from CORG.
///*-*         If SET NOWarnings is in effect, the warning message is
///*-*             stored in a circular buffer of length kMAXMES.
///*-*         If called with CORG=CMES='SHO', it prints the messages in
///*-*             the circular buffer, FIFO, and empties the buffer.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

void QMinuit::qmnwarn(const char *copt1, const char *corg1, const char *cmes1)
{
    QString copt = copt1;
    QString corg = corg1;
    QString cmes = cmes1;

    const qint32 kMAXMES = 10;
    qint32 ityp, i, ic, nm;
    QString englsh, ctyp;

    if (corg.left(3) != "SHO" || cmes.left(3) != "SHO") {

 //*-*-            Either print warning or put in buffer
       if (copt == "W") {
          ityp = 1;
          if (mLwarn) {
             qInfo() << QString(" MINUIT WARNING IN %1").arg(corg);
             qInfo() << QString(" ============== %1").arg(cmes);
             return;
          }
       } else {
          ityp = 2;
          if (mLrepor) {
             qInfo() << QString(" MINUIT DEBUG FOR %1").arg(corg);
             qInfo() << QString(" =============== %1 ").arg(cmes);
             return;
          }
       }
 //*-*-                if appropriate flag is off, fill circular buffer
       if (mNwrmes[ityp-1] == 0) mIcirc[ityp-1] = 0;
       ++mNwrmes[ityp-1];
       ++mIcirc[ityp-1];
       if (mIcirc[ityp-1] > 10)         mIcirc[ityp-1] = 1;
       ic = mIcirc[ityp-1];
       mOrigin[ic] = corg;
       mWarmes[ic] = cmes;
       mNfcwar[ic] = mNfcn;
       return;
    }

 //*-*-            'SHO WARnings', ask if any suppressed mess in buffer
    if (copt == "W") {
       ityp = 1;
       ctyp = "WARNING";
    } else {
       ityp = 2;
       ctyp = "*DEBUG*";
    }
    if (mNwrmes[ityp-1] > 0) {
       englsh = " WAS SUPPRESSED.  ";
       if (mNwrmes[ityp-1] > 1) englsh = "S WERE SUPPRESSED.";
       qInfo() << QString(" %1 MINUIT %2 MESSAGE%3").arg(mNwrmes[ityp-1]).arg(ctyp).arg(englsh);
       nm = mNwrmes[ityp-1];
       ic = 0;
       if (nm > kMAXMES) {
          qInfo() << " ONLY THE MOST RECENT 10 WILL BE LISTED BELOW.";
          nm = kMAXMES;
          ic = mIcirc[ityp-1];
       }
       qInfo() << "  CALLS  ORIGIN         MESSAGE";
       for (i = 1; i <= nm; ++i) {
          ++ic;
          if (ic > kMAXMES) ic = 1;
          qInfo() << QString(" %1  %2  %3").arg(mNfcwar[ic]).arg(mOrigin[ic]).arg(mWarmes[ic]);
       }
       mNwrmes[ityp-1] = 0;
       qInfo() << " ";
    }
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*-*Calculates the WERR, external parameter errors*-*-*-*-*-*-*
///*-*            ==============================================
///*-*      and the global correlation coefficients, to be called
///*-*      whenever a new covariance matrix is available.
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*


void QMinuit::qmnwerr()
{
    double denom, ba, al, dx, du1, du2;
    qint32 ndex, ierr, i, j, k, l, ndiag, k1, iin;

 //*-*-                        calculate external error if v exists
    if (mISW[1] >= 1) {
       for (l = 1; l <= mNPar; ++l) {
          ndex = l*(l + 1) / 2;
          dx = qSqrt(qAbs(mVhmat[ndex-1]*mUp));
          i = mNexofi[l-1];
          if (mNvarl[i-1] > 1) {
             al = mAlim[i-1];
             ba = mBlim[i-1] - al;
             du1 = al + 0.5*(qSin(mX[l-1] + dx) + 1)*ba - mU[i-1];
             du2 = al + 0.5*(qSin(mX[l-1] - dx) + 1)*ba - mU[i-1];
             if (dx > 1) du1 = ba;
             dx = 0.5*(qAbs(du1) + qAbs(du2));
          }
          mWerr[l-1] = dx;
       }
    }
 //*-*-                         global correlation coefficients
    if (mISW[1] >= 1) {
       for (i = 1; i <= mNPar; ++i) {
          mGlobcc[i-1] = 0;
          k1 = i*(i-1) / 2;
          for (j = 1; j <= i; ++j) {
             k = k1 + j;
             mP[i + j*mMaxPar - mMaxPar-1] = mVhmat[k-1];
             mP[j + i*mMaxPar - mMaxPar-1] = mP[i + j*mMaxPar - mMaxPar-1];
          }
       }
       qmnvert(mP, mMaxInt, mMaxInt, mNPar, ierr);
       if (ierr == 0) {
          for (iin = 1; iin <= mNPar; ++iin) {
             ndiag = iin*(iin + 1) / 2;
             denom = mP[iin + iin*mMaxPar - mMaxPar-1]*mVhmat[ndiag-1];
             if (denom <= 1 && denom >= 0) mGlobcc[iin-1] = 0;
             else                          mGlobcc[iin-1] = qSqrt(1 - 1 / denom);
          }
       }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// release a parameter

qint32 QMinuit::release(qint32 parNo)
{
    qint32 err;
    double tmp[1];
    tmp[0] = parNo+1; //set internal Minuit numbering

    qmnexcm( "RELEASE", tmp, 1, err );

    return err;
}

////////////////////////////////////////////////////////////////////////////////
/// To get the n-sigma contour the error def parameter "up" has to set to n^2.

qint32 QMinuit::setErrorDef(double up)
{
    qint32 err;

    qmnexcm( "SET ERRDEF", &up, 1, err );

    return err;
}

////////////////////////////////////////////////////////////////////////////////
///*-*-*-*-*-*-*To set the address of the minimization function*-*-*-*-*-*-*-*
///*-*          ===============================================
///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

//void QMinuit::setFCN(void */*fcn*/)
//{
////    fFCN = fcn;
//}

//////////////////////////////////////////////////////////////////////////////////
/////*-*-*-*-*-*-*Static function called when setFCN is called in interactive mode
/////*-*          ===============================================

//void QMinuit::setFCN(void /*(*fcn)(qint32 &, double *, double &, double *, int)*/)
//{
////    TMethodCall *m  = gMinuit->GetMethodCall();
////    if (!m) return;

////    Long_t args[5];
////    args[0] = (Long_t)&npar;
////    args[1] = (Long_t)gin;
////    args[2] = (Long_t)&f;
////    args[3] = (Long_t)u;
////    args[4] = (Long_t)flag;
////    m->SetParamPtrs(args);
////    Double_t result;
////    m->Execute(result);
//// }

// ////////////////////////////////////////////////////////////////////////////////
// ///*-*-*-*-*-*-*To set the address of the minimization function*-*-*-*-*-*-*-*
// ///*-*          ===============================================
// ///     this function is called by CINT instead of the function above
// ///*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

// void QMinuit::setFCN(void *fcn)
// {
//    if (!fcn) return;

//    const char *funcname = gCling->Getp2f2funcname(fcn);
//    if (funcname) {
//       mMethodCall = new TMethodCall();
//       mMethodCall->InitWithPrototype(funcname,"Int_t&,Double_t*,Double_t&,Double_t*,Int_t");
//    }
//    fFCN = InteractiveFCNm;
    //    gMinuit = this; //required by InteractiveFCN
//}

////////////////////////////////////////////////////////////////////////////////
///set Minuit print level
/// printlevel = -1  quiet (also suppresse all warnings)
///            =  0  normal
///            =  1  verbose

qint32 QMinuit::setPrintLevel(qint32 printLevel)
{
    qint32    err;
    double tmp[1];
    tmp[0] = printLevel;

    qmnexcm( "SET PRINT", tmp, 1, err );

    if (printLevel <=-1) qmnexcm("SET NOWarnings",tmp,0,err);

    return err;
}
