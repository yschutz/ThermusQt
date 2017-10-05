#include <gsl/gsl_vector.h>

#include "main/TTMThermalModelBQ.h"
#include "main/TTMThermalModelBSQ.h"

struct PARAMETERS  { TTMThermalModelBQ* p0; double p1;};
struct PARAMETERSS { TTMThermalModelBSQ* p0; double p1; double p2; double p3;};


int BQConstrainBDens       (TTMThermalModelBQ *model, double nb);
int BQConstrainEN          (TTMThermalModelBQ *model, double eovern);
int BQConstrainQ           (TTMThermalModelBQ *model);
int BQConstrainQBDens      (TTMThermalModelBQ *model, double nb);
int BQConstrainQEN         (TTMThermalModelBQ *model, double eovern);
int BQConstrainQNetBDens   (TTMThermalModelBQ *model, double nb);
int BQConstrainQPercolation(TTMThermalModelBQ *model);
int BQConstrainQST3        (TTMThermalModelBQ *model, double SoverT3);
int BQConstrainST3         (TTMThermalModelBQ *model, double SoverT3);
int BQConstrainQBDens      (TTMThermalModelBQ *model, double nb);

int BSQConstrainBSQ          (TTMThermalModelBSQ *model, double B, double S, double Q);
int BSQConstrainQQ           (TTMThermalModelBSQ *model, double qq);
int BSQConstrainS            (TTMThermalModelBSQ *model);
int BSQConstrainSQ           (TTMThermalModelBSQ *model);
int BSQConstrainSQC          (TTMThermalModelBSQ *model);
int BSQConstrainSQCb         (TTMThermalModelBSQ *model);
int BSQConstrainQ            (TTMThermalModelBSQ *model);
int BSQConstrainSQEN         (TTMThermalModelBSQ *model, double eovern);
int BSQConstrainSQPercolation(TTMThermalModelBSQ *model);
int BSQConstrainSPercolation (TTMThermalModelBSQ *model);
int BSQConstrainSEN          (TTMThermalModelBSQ *model, double eovern);
int BSQConstrainSQST3        (TTMThermalModelBSQ *model, double sovert3);
int BSQConstrainSST3         (TTMThermalModelBSQ *model, double sovert3);
int BSQConstrainSBDens       (TTMThermalModelBSQ *model, double nb);
int BSQConstrainSQBDens      (TTMThermalModelBSQ *model, double nb);
int BSQConstrainSQNetBDens   (TTMThermalModelBSQ *model, double nb);

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

int BSQfuncBSQ        (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncQ          (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncQQ         (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncS          (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSBDens     (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSEN        (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSQ         (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSQBDens    (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSQC        (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSQCb       (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSQEN       (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSQST3      (const gsl_vector* x, void* p, gsl_vector* f);
int BSQfuncSST3       (const gsl_vector* x, void* p, gsl_vector* f);


void broyden(gsl_vector* x, size_t n, int& status, PARAMETERS p, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));
void broyden(gsl_vector* x, size_t n, int& status, PARAMETERSS p, int (*f)(const gsl_vector* x, void* p, gsl_vector* f));


int funcTest(const gsl_vector* x, void* p, gsl_vector* f);
