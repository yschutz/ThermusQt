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
// ---------------------------------- minuit.h



//////////////////////////////////////////////////////////////////////////
//                                                                      //
// QMinuit                                                              //
//                                                                      //
// The MINUIT minimisation package (base class)                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// copied by Yves Schutz November 2016

#ifndef QMinuit_H
#define QMinuit_H

#include <QObject>

class QMinuit : public QObject
{
    Q_OBJECT

private:
    QMinuit(const QMinuit &m);
    QMinuit& operator=(const QMinuit &m); // Not implemented

public:
    enum{kMAXWARN=100};

    double     *mAlim;            // [mMaxPar2] Lower limits for parameters. If zero no limits
    double     mAmin;             // Minimum value found for FCN
    double     mApsi;             //
    double     mBigedm;           // Big EDM = 123456
    double     *mBlim;            // [mMaxPar2] Upper limits for parameters
    QString    mCfrom;            //
    char       *mChpt;            // !Character to be plotted at the X,Y contour positions
    double     *mCOMDplist;       // [mMaxPar] array used in mncomd
    double     *mCONTgcc;         // [mMaxPar] array used in mncont
    double     *mCONTw;           // [mMaxPar] array used in mncont
    QString    mCovmes[4];        //
    QString    *mCpnam;           // [mMaxPar2] Array of parameters names
    QString    mCstatu;           //
    QString    mCtitl;            //
    QString    mCword;            //
    QString    mCvrsn;            //
    QString    mCundef;           //
    double     mDcovar;           // Relative change in covariance matrix
    double     *mDirin;           // [mMaxPar] (Internal) step sizes for current step
    double     *mDirins;          // [mMaxPar] (Internal) step sizes for current step for fixed params
    double     *mDgrd;            // [mMaxPar] Uncertainties
    double     mEDM;              // Estimated vertical distance to the minimum
    qint32     mEmpty;            // Initialization flag (1 = Minuit initialized)
    double     mEpsi;             //
    double     mEpsmac;           // machine precision for floating points:
    double     mEpsma2;           // sqrt(mEpsmac)
    double     *mErn;             // [mMaxPar] Negative Minos errors if calculated
    double     *mErp;             // [mMaxPar] Positive Minos errors if calculated
    qint32     *mIpfix;           // [mMaxPar] List of fixed parameters
    double     *mFIXPyy;          // [mMaxPar] array used in mnfixp
    double     mFval3;            //
    double     *mG2;              // [mMaxPar]
    double     *mG2s;             // [mMaxPar]
    double     *mGin;             // [mMaxPar2]
    double     *mGlobcc;          // [mMaxPar] Global Correlation Coefficients
    double     *mGRADgf;          // [mMaxPar] array used in mngrad
    bool       mGraphicsMode;     // true if graphics mode on (default)
    double     *mGrd;             // [mMaxPar] First derivatives
    double     *mGrds;            // [mMaxPar]
    double     *mGstep;           // [mMaxPar] Step sizes
    double     *mGsteps;          // [mMaxPar]
    double     *mHESSyy;          // [mMaxPar] array used in mnhess
    qint32     mIcirc[2];         //
    qint32     mIcomnd;           // Number of commands
    qint32     mIdbg[11];         // Array of internal debug switches
    double     *mIMPRdsav;        // [mMaxPar] array used in mnimpr
    double     *mIMPRy;           // [mMaxPar] array used in mnimpr
    //    qint32   mIstkrd[10];       //
    qint32     mIstkwr[10];       //
    qint32     mISW[7];           // Array of switches
    qint32     mIsysrd;           // standardInput unit
    qint32     mIsyssa;           //
    qint32     mIsyswr;           // standard output unit
    qint32     mItaur;            //
    qint32     mIstrat;           //
    qint32     mKe1cr;            //
    qint32     mKe2cr;            //
    bool       mLimset;           // true if a parameter is up against limits (for MINOS)
    bool       mLnewmn;           // true if the previous process has unexpectedly improved FCN
    bool       mLnolim;           // true if there are no limits on any parameters (not yet used)
    bool       mLphead;           // true if a heading should be put out for the next parameter definition
    bool       mLrepor;           // true if exceptional conditions are put out (default=false)
    bool       mLwarn;            // true if warning messges are to be put out (default=true)
    double     *mMATUvline;       // [mMaxPar] array used in mnmatu
    qint32       mMaxPar;           // Maximum number of parameters
    qint32     mMaxCptns;
    qint32     mMaxExtr;          // Maximum number of external parameters
    qint32     mMaxInt;           // Maximum number of internal parameters
    qint32     mMaxIterations;    // Maximum number of iterations
    qint32     mMaxPar1;          // mMaxPar*(mMaxPar+1)
    qint32     mMaxPar2;          // mMaxPar*mMaxPar
    qint32     mMaxPar5;          // mMaxPar*(mMaxPar+1)/2
    //    TMethodCall  *mMethodCall;      // Pointer to MethodCall in case of interpreted function
    double     *mMIGRflnu;        // [mMaxPar] array used in mnmigr
    double     *mMIGRgs;          // [mMaxPar] array used in mnmigr
    double     *mMIGRstep;        // [mMaxPar] array used in mnmigr
    double     *mMIGRvg;          // [mMaxPar] array used in mnmigr
    double     *mMIGRxxs;         // [mMaxPar] array used in mnmigr
    double     *mMNOTgcc;         // [mMaxPar] array used in mnmnot
    double     *mMNOTw;           // [mMaxPar] array used in mnmnot
    double     *mMNOTxdev;        // [mMaxPar] array used in mnmnot
    qint32     mNblock;           // Number of Minuit data blocks
    qint32     *mNexofi;          // [mMaxPar] External parameters number for currently variable parameters
    qint32     mNewpag;           //
    qint32     mNfcn;             // Number of calls to FCN
    qint32     mNfcnlc;           //
    qint32     mNfcnfr;           //
    qint32     mNfcnmx;           // Maximum number of calls to FCN
    qint32     mNfcwar[20];       //
    qint32     *mNiofex;          // [mMaxPar2] Internal parameters number, or zero if not currently variable
    qint32     mNpagln;           // Number of lines per page
    qint32     mNpagwd;           // Page width
    qint32       mNPar;             // Number of free parameters (total number of pars = mNPar + fNfix)
    double     *mPARSplist;       // [mMaxPar] array used in mNPars
    qint32     mNPFix;            // Number of fixed parameters
    qint32     mNstkrd;           //
    qint32     mNstkwr;           //
    qint32     mNu;               //
    qint32     mNwrmes[2];        //
    QObject    *mObjectFit;       // Pointer to object being fitted
    QString    mOrigin[kMAXWARN]; //
    double     *mP;               // [mMaxPar1]
    double     *mPbar;            // [mMaxPar]
    QObject    *mPlot;            // Pointer to TGraph object created by mncont
    double     *mPrho;            // [mMaxPar] Minimum poqint32 of parabola
    double     *mPSDFs;           // [mMaxPar] array used in mnpsdf
    double     *mPstar;           // [mMaxPar2]
    double     *mPstst;           // [mMaxPar]
    double     *mSEEKxbest;       // [mMaxPar] array used in mnseek
    double     *mSEEKxmid;        // [mMaxPar] array used in mnseek
    double     *mSIMPy;           // [mMaxPar] array used in mnsimp
    qint32     mStatus;           // Status flag for the last called Minuit function
    double     *mU;               // [mMaxPar2] External (visible to user in FCN) value of parameters
    double     mUndefi;           // Undefined number = -54321
    double     mUp;               // FCN+-UP defines errors (for chisquare fits UP=1)
    double     mUpdflt;           //
    qint32     *mNvarl;           // [mMaxPar2] parameters flag (-1=undefined, 0=constant..)
    double     *mVERTq;           // [mMaxPar] array used in mnvert
    double     *mVERTs;           // [mMaxPar] array used in mnvert
    double     *mVERTpp;          // [mMaxPar] array used in mnvert
    double     *mVhmat;           // [mMaxPar5] (Internal) error matrix stored as Half MATrix, since it is symmetric
    double     mVlimhi;           //
    double     mVlimlo;           //
    double     *mVthmat;          // [mMaxPar5] VHMAT is sometimes saved in VTHMAT, especially in MNMNOT
    QString    mWarmes[kMAXWARN]; //
    double     *mWerr;            // [mMaxPar] External parameters error (standard deviation, defined by UP)
    double     *mWord7;           // [mMaxPar]
    double     *mX;               // [mMaxPar] Internal parameters values
    double     mXdircr;           //
    double     mXmidcr;           //
    double     *mXpt;             // [mMaxCptns] X array of points for contours
    double     *mXs;              // [mMaxPar] Internal parameters values saved for fixed params
    double     *mXt;              // [mMaxPar] Internal parameters values X saved as Xt
    double     *mXts;             // [mMaxPar] Internal parameters values X saved as Xt for fixed params
    double     *mYpt;             // [mMaxCptns] Y array of points for contours
    double     mYdircr;           //
    double     mYmidcr;           //

    void      (*fFCN)(qint32 &npar, double *gin, double &f, double *u, qint32 flag); // !


    //  methods performed on QMinuit class
public:

    explicit      QMinuit(QObject *parent = 0);
                  QMinuit(qint32 maxpar);
    virtual       ~QMinuit();

    virtual void     BuildArrays(qint32 maxpar=15);
    virtual QObject* Clone(const char *newname="") const;   //Clone-Method to copy the function-pointer fFCN
    virtual qint32   Command(const char *command);
    virtual QObject* Contour(qint32 npoints=10, qint32 pa1=0, qint32 pa2=1);
    virtual qint32   DefineParameter( qint32 parNo, const char *name, double initVal, double initErr, double lowerLimit, double upperLimit );
    virtual void     DeleteArrays();
    virtual qint32   Eval(qint32 npar, double *grad, double &fval, double *par, qint32 flag);
    virtual qint32   FixParameter( qint32 parNo );
//    TMethodCall   *GetMethodCall() const {return mMethodCall;}
    QObject*         GeQObjectFit() const {return mObjectFit;}
    qint32           GetMaxIterations() const {return mMaxIterations;}
    virtual qint32   GetNumFixedPars() const {return mNPFix;}
    virtual qint32   GetNumFreePars() const {return mNPar;}
    virtual qint32   GetNumPars() const {return mNPar + mNPFix;}
    virtual qint32   GetParameter( qint32 parNo, double &currentValue, double &currentError ) const;
    virtual QObject* GetPlot() const {return mPlot;}
    qint32           GetStatus() const {return mStatus;}
    virtual qint32   Migrad();
    virtual void     qmnamin();
    virtual void     qmnbins(double a1, double a2, qint32 naa, double &bl, double &bh, qint32 &nb, double &bwid);
    virtual void     qmncalf(double *pvec, double &ycalf);
    virtual void     qmncler();
    virtual void     qmncntr(qint32 ike1, qint32 ike2, qint32 &ierrf);
    virtual void     qmncomd(const char *crdbin, qint32 &icondn);
    virtual void     qmncont(qint32 ke1, qint32 ke2, qint32 nptu, double *xptu, double *yptu, qint32 &ierrf);
    virtual void     qmncrck(QString crdbuf, qint32 maxcwd, QString &comand, qint32 &lnc
                     ,  qint32 mxp, double *plist, qint32 &llist, qint32 &ierr, qint32 isyswr);
    virtual void     qmncros(double &aopt, qint32 &iercr);
    virtual void     qmncuve();
    virtual void     qmnderi();
    virtual void     qmndxdi(double pint, qint32 ipar, double &dxdi);
    virtual void     qmneig(double *a, qint32 ndima, qint32 n, qint32 mits, double *work, double precis, qint32 &ifault);
    virtual void     qmnemat(double *emat, qint32 ndim);
    virtual void     qmnerrs(qint32 number, double &eplus, double &eminus, double &eparab, double &gcc);
    virtual void     qmneval(double anext, double &fnext, qint32 &ierev);
    virtual void     qmnexcm(const char *comand, double *plist, qint32 llist, qint32 &ierflg) ;
    virtual void     qmnexin(double *pint);
    virtual void     qmnfixp(qint32 iint, qint32 &ierr);
    virtual void     qmnfree(qint32 k);
    virtual void     qmngrad();
    virtual void     qmnhelp(QString comd);
    virtual void     qmnhelp(const char *command="");
    virtual void     qmnhess();
    virtual void     qmnhes1();
    virtual void     qmnimpr();
    virtual void     qmninex(double *pint);
    virtual void     qmninit(qint32 i1, qint32 i2, qint32 i3);
    virtual void     qmnlims();
    virtual void     qmnline(double *start, double fstart, double *step, double slope, double toler);
    virtual void     qmnmatu(qint32 kode);
    virtual void     qmnmigr();
    virtual void     qmnmnos();
    virtual void     qmnmnot(qint32 ilax, qint32 ilax2, double &val2pl, double &val2mi);
    virtual void     qmNParm(qint32 k, QString cnamj, double uk, double wk, double a, double b, qint32 &ierflg);
    virtual void     qmNPars(QString &crdbuf, qint32 &icondn);
    virtual void     qmnpfit(double *parx2p, double *pary2p, qint32 npar2p, double *coef2p, double &sdev2p);
    virtual void     qmnpint(double &pexti, qint32 i, double &pinti);
    virtual void     qmnplot(double *xpt, double *ypt, char *chpt, qint32 nxypt, qint32 npagwd, qint32 npagln);
    virtual void     qmnpout(qint32 iuext, QString &chnam, double &val, double &err, double &xlolim, double &xuplim, qint32 &iuint) const;
    virtual void     qmnprin(qint32 inkode, double fval);
    virtual void     qmnpsdf();
    virtual void     qmnrazz(double ynew, double *pnew, double *y, qint32 &jh, qint32 &jl);
    virtual void     qmnrn15(double &val, qint32 &inseed);
    virtual void     qmnrset(qint32 iopt);
    virtual void     qmnsave();
    virtual void     qmnscan();
    virtual void     qmnseek();
    virtual void     qmnset();
    virtual void     qmnsimp();
    virtual void     qmnstat(double &fmin, double &mEDM, double &errdef, qint32 &npari, qint32 &nparx, qint32 &istat);
    virtual void     qmntiny(volatile double epsp1, double &epsbak);
    bool             qmnunpt(QString &cfname);
    virtual void     qmnvert(double *a, qint32 l, qint32 m, qint32 n, qint32 &ifail);
    virtual void     qmnwarn(const char *copt, const char *corg, const char *cmes);
    virtual void     qmnwerr();
    virtual qint32   release( qint32 parNo );
    virtual qint32   setErrorDef( double up );
//    virtual void   SetFCN(void *fcn);
//    virtual void   SetFCN(void (*fcn)(qint32 &, double *, double &f, double *, int));
    virtual void     setGraphicsMode(bool mode=true) {mGraphicsMode = mode;}
    virtual void     setMaxIterations(qint32 maxiter=500) {mMaxIterations = maxiter;}
    virtual void     seQObjectFit(QObject *obj) {mObjectFit=obj;}
    virtual qint32   setPrintLevel( qint32 printLevel=0 );

};

extern QMinuit *gMinuit;

#endif // QMinuit_H
