#include <gsl/gsl_multiroots.h>

#include <QDebug>

struct params {int dummy;};
void broyden(gsl_vector* x, size_t n, int& status, int (*f)(const gsl_vector* x, void* p, gsl_vector* f))
{

    gsl_multiroot_function function;
    function.f = f;
    function.n = n;
    params p = {0};
    function.params = &p;

    double x_init[2] = {-10.0, -5.0};
//    gsl_vector *x = gsl_vector_alloc(n);

    gsl_vector_set (x, 0, x_init[0]);
    gsl_vector_set (x, 1, x_init[1]);

    const gsl_multiroot_fsolver_type *T;
    T = gsl_multiroot_fsolver_broyden;

    gsl_multiroot_fsolver *s;
    s = gsl_multiroot_fsolver_alloc(T, n);
    gsl_multiroot_fsolver_set (s, &function, x);

    size_t iter = 0;
    do
    {
        iter++;
        status = gsl_multiroot_fsolver_iterate (s);

        //        print_state (iter, s);

        if (status)   /* check if solver is stuck */
            break;

        status =
                gsl_multiroot_test_residual (s->f, 1e-7);
    }
    while (status == GSL_CONTINUE && iter < 1000);

    qDebug() << "status = " << gsl_strerror (status);
    qDebug() << gsl_vector_get(s->x, 0) << gsl_vector_get(s->x, 1);
    qDebug() << gsl_vector_get(s->f, 0) << gsl_vector_get(s->f, 1);

    gsl_multiroot_fsolver_free (s);
    gsl_vector_free (x);
}
