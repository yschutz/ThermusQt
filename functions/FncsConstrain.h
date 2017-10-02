#include <gsl/gsl_vector.h>

#include "main/TTMThermalModelBQ.h"
//#include "main/TTMThermalModelBSQ.h"

struct PARAMETERS { TTMThermalModelBQ* p0; double p1;};

//int BSQConstrainSQ(TTMThermalModelBSQ *model);
//int BSQConstrainSQC(TTMThermalModelBSQ *model);
//int BSQConstrainSQCb(TTMThermalModelBSQ *model);
//int BSQConstrainBSQ(TTMThermalModelBSQ *model, double B, double S, double Q);
//int BSQConstrainQQ(TTMThermalModelBSQ *model, double Q);
//int BSQConstrainS(TTMThermalModelBSQ *model);
//int BSQConstrainQ(TTMThermalModelBSQ *model);
//int BSQConstrainSQEN(TTMThermalModelBSQ *model, double eovern);
//int BSQConstrainSQPercolation(TTMThermalModelBSQ *model);
//int BSQConstrainSPercolation(TTMThermalModelBSQ *model);
//int BSQConstrainSEN(TTMThermalModelBSQ *model, double eovern);
//int BSQConstrainSQST3(TTMThermalModelBSQ *model, double sovert3);
//int BSQConstrainSST3(TTMThermalModelBSQ *model, double sovert3);
int BQConstrainBDens(TTMThermalModelBQ *model, double nb);
int BQConstrainEN(TTMThermalModelBQ *model, double eovern);
int BQConstrainQ(TTMThermalModelBQ *model);
int BQConstrainQBDens(TTMThermalModelBQ *model, double nb);
int BQConstrainQEN(TTMThermalModelBQ *model, double eovern);
int BQConstrainQNetBDens(TTMThermalModelBQ *model, double nb);
int BQConstrainQPercolation(TTMThermalModelBQ *model);
int BQConstrainQST3(TTMThermalModelBQ *model, double SoverT3);
int BQConstrainST3(TTMThermalModelBQ *model, double SoverT3);
int BQConstrainQBDens(TTMThermalModelBQ *model, double nb);
//int BSQConstrainSBDens(TTMThermalModelBSQ *model, double nb);
//int BSQConstrainSQBDens(TTMThermalModelBSQ *model, double nb);
//int BSQConstrainSQNetBDens(TTMThermalModelBSQ *model, double nb);

int BQfuncBDens       (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncEN          (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncQ           (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncQBDens      (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncQBDens      (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncQEN         (const gsl_vector* x, void *p, gsl_vector *f);
int BQfuncQNetBDens   (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncQPercolation(const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncQST3        (const gsl_vector* x, void* p, gsl_vector* f);
int BQfuncST3         (const gsl_vector* x, void* p, gsl_vector* f);

void broyden(gsl_vector* x, size_t n, int& status, PARAMETERS p, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));


int funcTest(const gsl_vector* x, void* p, gsl_vector* f);
