#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_vector.h>

#include <QDebug>

struct papa { double a; double b;};

#include <QMessageBox>

int funcTest(const gsl_vector* x, void* params, gsl_vector* f)
{
    int rv = 0;

    bool check = true;
    if (check) {
    double a = ((struct papa *) params)->a;
    double b = ((struct papa *) params)->b;

    const double x0 = gsl_vector_get (x, 0);
    const double x1 = gsl_vector_get (x, 1);

    const double y0 = a * (1 - x0);
    const double y1 = b * (x1 - x0 * x0);

    gsl_vector_set (f, 0, y0);
    gsl_vector_set (f, 1, y1);

} else {

        rv = 1;
        QMessageBox msg(QMessageBox::Critical, Q_FUNC_INFO, Q_FUNC_INFO);
        msg.setInformativeText("Prim part dens problems!");
        msg.exec();
        exit(1);
    }
    return rv;
}
