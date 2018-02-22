// Y. Schutz October 2017

#include <gsl/gsl_roots.h>

#include <QDebug>
#include "FncsConstrain.h"

double brent(double xLow, double xHigh, double step, int& status, PARAMETERSS p, double (*myfunction)(double x, void* p))
{
    int iter = 0, max_iter = 100;
    const gsl_root_fsolver_type *T;
    gsl_root_fsolver *s;
    double result = 0;

    gsl_function F;
    F.function = myfunction;
    F.params = &p;

    T = gsl_root_fsolver_brent;
    s = gsl_root_fsolver_alloc (T);
    gsl_root_fsolver_set (s, &F, xLow, xHigh);
    do
      {
        iter++;
        status = gsl_root_fsolver_iterate (s);
        result = gsl_root_fsolver_root (s);
        xLow = gsl_root_fsolver_x_lower (s);
        xHigh = gsl_root_fsolver_x_upper (s);
        status = gsl_root_test_interval (xLow, xHigh, 0, step);
      }
    while (status == GSL_CONTINUE && iter < max_iter);

    gsl_root_fsolver_free (s);

    return result;
}
