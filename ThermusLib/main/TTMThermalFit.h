// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: Novembre 2016

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TTMThermalFit                                                        //
//                                                                      //
// Base class for thermal fit objects.                                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#ifndef TTMTHERMALFIT_H
#define TTMTHERMALFIT_H

#include "TTMYield.h"

#include <QObject>

class QMinuit;
class QTableView;

class TTMParameterSet;
class TTMThermalModel;

class TTMThermalFit : public QObject
{
    Q_OBJECT
public:
    explicit TTMThermalFit(QObject *parent = nullptr);
    ~TTMThermalFit();

    void                     addYield(TTMYield *yield);
    void                     fitData(int flag = 0);
    virtual TTMThermalModel* generateThermalModel()                                   = 0;
    void                     generateYields();
    double                   getChiSquare() const                                     { return mChiSquare; }
    QString                  getDescriptor() const                                    { return mDescriptor; }
    virtual TTMParameterSet* getParameterSet() const                                  = 0;
    double                   getQuadDev() const                                       { return mQuadDev; }
    QString                  getTMName(int id1, int id2, const QString& descr) const;
    TTMYield*                getYield(int id1, int id2, const QString& descr) const;
    QList<TTMYield*>         getYields() const { return mYields; }
    void                     inputExpYields(QString &fileName);
    void                     listMinuitInfo() const;
    void                     listYields(bool debug = false);
    void                     removeYield(int id1, int id2, const QString& descr);

protected:
    double           mChiSquare;   // chi-squared
    QString          mDescriptor;  // string describing fit
    double           mQuadDev;     // quadratic deviation
    QList<TTMYield*> mYields;      // Container for yields of interest
    QTableView*      mYieldView;   // to display the yields
};

#endif // TTMTHERMALFIT_H
