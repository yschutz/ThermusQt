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

// adapted for Qt by Yves Schutz November 2016
// implemented as singleton

#ifndef QMinuit_H
#define QMinuit_H

#include <QDebug>
#include <QObject>

class QMinuit : public QObject
{
    Q_OBJECT

public:
    virtual void     buildArrays(int maxpar=15);
    virtual int      command(const char *command);
    virtual void         contour(QVector<double> &xcoor, QVector<double> &ycoor, int npoints = 10, int pa1 = 0, int pa2 = 1);
    virtual int      defineParameter( int parNo, const char *name, double initVal, double initErr, double lowerLimit, double upperLimit );
    virtual void     deleteArrays();
    virtual int      eval(int npar, double *grad, double &fval, double *par, int flag);
    virtual int      fixParameter( int parNo );
    QObject*         geQObjectFit() const {return mObjectFit;}
    int              getMaxIterations() const {return mMaxIterations;}
    virtual int      getNumFixedPars() const {return mNPFix;}
    virtual int      getNumFreePars() const {return mNPar;}
    virtual int      getNumPars() const {return mNPar + mNPFix;}
    virtual int      getParameter( int parNo, double &currentValue, double &currentError ) const;
    virtual QObject* getPlot() const {return mPlot;}
    int              getStatus() const {return mStatus;}
    static QMinuit&  instance();
    virtual int      migrad();
    virtual void     qmnamin();
    virtual void     qmnbins(double a1, double a2, int naa, double &bl, double &bh, int &nb, double &bwid);
    virtual void     qmncalf(double *pvec, double &ycalf);
    virtual void     qmncler();
    virtual void     qmncntr(int ike1, int ike2, int &ierrf);
    virtual void     qmncomd(const char *crdbin, int &icondn);
    virtual void     qmncont(int ke1, int ke2, int nptu, QVector<double> &xptu, QVector<double> &yptu, int &ierrf);
    virtual void     qmncrck(QString crdbuf, int maxcwd, QString &comand, int &lnc, int mxp, double *plist, int &llist, int &ierr, int isyswr);
    virtual void     qmncros(double &aopt, int &iercr);
    virtual void     qmncuve();
    virtual void     qmnderi();
    virtual void     qmndxdi(double pint, int ipar, double &dxdi);
    virtual void     qmneig(double *a, int ndima, int n, int mits, double *work, double precis, int &ifault);
    virtual void     qmnemat(double *emat, int ndim);
    virtual void     qmnerrs(int number, double &eplus, double &eminus, double &eparab, double &gcc);
    virtual void     qmneval(double anext, double &fnext, int &ierev);
    virtual void     qmnexcm(const char *comand, double *plist, int llist, int &ierflg) ;
    virtual void     qmnexin(double *pint);
    virtual void     qmnfixp(int iint, int &ierr);
    virtual void     qmnfree(int k);
    virtual void     qmngrad();
    virtual void     qmnhelp(QString comd);
    virtual void     qmnhelp(const char *command="");
    virtual void     qmnhess();
    virtual void     qmnhes1();
    virtual void     qmnimpr();
    virtual void     qmninex(double *pint);
    virtual void     qmninit(int i1, int i2, int i3);
    virtual void     qmnlims();
    virtual void     qmnline(double *start, double fstart, double *step, double slope, double toler);
    virtual void     qmnmatu(int kode);
    virtual void     qmnmigr();
    virtual void     qmnmnos();
    virtual void     qmnmnot(int ilax, int ilax2, double &val2pl, double &val2mi);
    virtual void     qmNParm(int k, QString cnamj, double uk, double wk, double a, double b, int &ierflg);
    virtual void     qmNPars(QString &crdbuf, int &icondn);
    virtual void     qmnpfit(double *parx2p, double *pary2p, int npar2p, double *coef2p, double &sdev2p);
    virtual void     qmnpint(double &pexti, int i, double &pinti);
    virtual void     qmnplot(double *xpt, double *ypt, char *chpt, int nxypt, int npagwd, int npagln);
    virtual void     qmnpout(int iuext, QString &chnam, double &val, double &err, double &xlolim, double &xuplim, int &iuint) const;
    virtual void     qmnprin(int inkode, double fval);
    virtual void     qmnpsdf();
    virtual void     qmnrazz(double ynew, double *pnew, double *y, int &jh, int &jl);
    virtual void     qmnrn15(double &val, int &inseed);
    virtual void     qmnrset(int iopt);
    virtual void     qmnsave();
    virtual void     qmnscan();
    virtual void     qmnseek();
    virtual void     qmnset();
    virtual void     qmnsimp();
    virtual void     qmnstat(double &fmin, double &mEDM, double &errdef, int &npari, int &nparx, int &istat);
    virtual void     qmntiny(volatile double epsp1, double &epsbak);
    bool             qmnunpt(QString &cfname);
    virtual void     qmnvert(double *a, int l, int m, int n, int &ifail);
    virtual void     qmnwarn(const char *copt, const char *corg, const char *cmes);
    virtual void     qmnwerr();
    virtual int      release( int parNo );
    virtual int      setErrorDef( double up );
    virtual void     setFCN(void (*fcn)(int &, double *, double &f, double *, int));
    virtual void     setGraphicsMode(bool mode=true) {mGraphicsMode = mode;}
    virtual void     setMaxIterations(int maxiter=500) {mMaxIterations = maxiter;}
    void             setMaxParameters(int max);
    virtual void     seQObjectFit(QObject *obj) {mObjectFit=obj;}
    virtual int      setPrintLevel( int printLevel=0 );

private:
    QMinuit(QObject* parent = nullptr);
    ~QMinuit();

    QMinuit(const QMinuit& m) :QObject(m.parent()) {} // cannot copy QMinuit
    QMinuit& operator = (const QMinuit&) { return *this; }
    static QMinuit mMinuit; // the unique instance
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
    int        mEmpty;            // Initialization flag (1 = Minuit initialized)
    double     mEpsi;             //
    double     mEpsmac;           // machine precision for floating points:
    double     mEpsma2;           // sqrt(mEpsmac)
    double     *mErn;             // [mMaxPar] Negative Minos errors if calculated
    double     *mErp;             // [mMaxPar] Positive Minos errors if calculated
    int        *mIpfix;           // [mMaxPar] List of fixed parameters
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
    int        mIcirc[2];         //
    int        mIcomnd;           // Number of commands
    int        mIdbg[11];         // Array of internal debug switches
    double     *mIMPRdsav;        // [mMaxPar] array used in mnimpr
    double     *mIMPRy;           // [mMaxPar] array used in mnimpr
    //    int   mIstkrd[10];       //
    int        mIstkwr[10];       //
    int        mISW[7];           // Array of switches
    int        mIsysrd;           // standardInput unit
    int        mIsyssa;           //
    int        mIsyswr;           // standard output unit
    int        mItaur;            //
    int        mIstrat;           //
    int        mKe1cr;            //
    int        mKe2cr;            //
    bool       mLimset;           // true if a parameter is up against limits (for MINOS)
    bool       mLnewmn;           // true if the previous process has unexpectedly improved FCN
    bool       mLnolim;           // true if there are no limits on any parameters (not yet used)
    bool       mLphead;           // true if a heading should be put out for the next parameter definition
    bool       mLrepor;           // true if exceptional conditions are put out (default=false)
    bool       mLwarn;            // true if warning messges are to be put out (default=true)
    double     *mMATUvline;       // [mMaxPar] array used in mnmatu
    int        mMaxPar;           // Maximum number of parameters
    int        mMaxCptns;
    int        mMaxExtr;          // Maximum number of external parameters
    int        mMaxInt;           // Maximum number of internal parameters
    int        mMaxIterations;    // Maximum number of iterations
    int        mMaxPar1;          // mMaxPar*(mMaxPar+1)
    int        mMaxPar2;          // mMaxPar*mMaxPar
    int        mMaxPar5;          // mMaxPar*(mMaxPar+1)/2
    double     *mMIGRflnu;        // [mMaxPar] array used in mnmigr
    double     *mMIGRgs;          // [mMaxPar] array used in mnmigr
    double     *mMIGRstep;        // [mMaxPar] array used in mnmigr
    double     *mMIGRvg;          // [mMaxPar] array used in mnmigr
    double     *mMIGRxxs;         // [mMaxPar] array used in mnmigr
    double     *mMNOTgcc;         // [mMaxPar] array used in mnmnot
    double     *mMNOTw;           // [mMaxPar] array used in mnmnot
    double     *mMNOTxdev;        // [mMaxPar] array used in mnmnot
    int        mNblock;           // Number of Minuit data blocks
    int        *mNexofi;          // [mMaxPar] External parameters number for currently variable parameters
    int        mNewpag;           //
    int        mNfcn;             // Number of calls to FCN
    int        mNfcnlc;           //
    int        mNfcnfr;           //
    int        mNfcnmx;           // Maximum number of calls to FCN
    int        mNfcwar[20];       //
    int        *mNiofex;          // [mMaxPar2] Internal parameters number, or zero if not currently variable
    int        mNpagln;           // Number of lines per page
    int        mNpagwd;           // Page width
    int        mNPar;             // Number of free parameters (total number of pars = mNPar + fNfix)
    int        mNPFix;            // Number of fixed parameters
    int        mNstkrd;           //
    int        mNstkwr;           //
    int        mNu;               //
    int        mNwrmes[2];        //
    QObject    *mObjectFit;       // Pointer to object being fitted
    QString    mOrigin[kMAXWARN]; //
    double     *mP;               // [mMaxPar1]
    double     *mPARSplist;       // [mMaxPar] array used in mNPars
    double     *mPbar;            // [mMaxPar]
    QObject    *mPlot;            // Pointer to TGraph object created by mncont
    double     *mPrho;            // [mMaxPar] Minimum point of parabola
    double     *mPSDFs;           // [mMaxPar] array used in mnpsdf
    double     *mPstar;           // [mMaxPar2]
    double     *mPstst;           // [mMaxPar]
    double     *mSEEKxbest;       // [mMaxPar] array used in mnseek
    double     *mSEEKxmid;        // [mMaxPar] array used in mnseek
    double     *mSIMPy;           // [mMaxPar] array used in mnsimp
    int        mStatus;           // Status flag for the last called Minuit function
    double     *mU;               // [mMaxPar2] External (visible to user in FCN) value of parameters
    double     mUndefi;           // Undefined number = -54321
    double     mUp;               // FCN+-UP defines errors (for chisquare fits UP=1)
    double     mUpdflt;           //
    int        *mNvarl;           // [mMaxPar2] parameters flag (-1=undefined, 0=constant..)
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
    void      (*mFCN)(int &npar, double *gin, double &f, double *u, int flag); // !
};
#endif // QMinuit_H
