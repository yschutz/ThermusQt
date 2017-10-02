#include <gsl/gsl_multiroots.h>

#include <QDebug>

#include "functions/FncsConstrain.h"

void broyden(gsl_vector* x, size_t ndim, int& status, PARAMETERS p, int (*myfunction)(const gsl_vector* x, void* p, gsl_vector* f))
{
    // the broyden function to solve mutidimentional equations

    gsl_multiroot_function function;
    function.f = myfunction;
    function.n = ndim;
    function.params = &p;

    for (size_t i = 0; i < ndim; i++)
        gsl_vector_set (x, i, 0.);

    const gsl_multiroot_fsolver_type *T;
    T = gsl_multiroot_fsolver_broyden;

    gsl_multiroot_fsolver *s;
    s = gsl_multiroot_fsolver_alloc(T, ndim);
    gsl_multiroot_fsolver_set (s, &function, x);

    size_t iter = 0;
    do
    {
        iter++;
        status = gsl_multiroot_fsolver_iterate (s);
        if (status)   /* check if solver is stuck */
            break;
        status = gsl_multiroot_test_residual (s->f, 1e-7);
    } while (status == GSL_CONTINUE && iter < 10);

    for (size_t i = 0; i < ndim; i++)
       gsl_vector_set(x, i, gsl_vector_get(s->x, i));

    gsl_multiroot_fsolver_free (s);
}
