// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: September 2017

#include <gsl/gsl_sf_bessel.h>

#include <QObject>
#include <QtMath>


double Test(double *x, double *par)
{
    double a = par[0];
    double b = par[1];

    return a * x[0] + b * x[1];
}

//__________________________________________________________________________
double FcnDistr(double *x, double *par)
{
  // x[0] : p/T

  double mu   = par[0];
  double mass = par[1];
  double G    = par[2];
  double Stat = par[3];

  double E = qSqrt(x[0] * x[0] + qPow(mass, 2));

  return  G * qExp(-E + mu) / (1 + Stat * G * qExp(-E + mu));
}

//__________________________________________________________________________
double FcnDens(double *x, double *par)
{
  // x[0] : p/T
  // This function returns 1/T^3*dn/d(p/T)- it is calculated based on 
  // n_i = -d(Omega_GC)/d(mu_i)
  //

  double mu   = par[0];
  double mass = par[1];
  double G    = par[2];
  double Stat = par[3];
  double Deg  = par[4];

  double E = qSqrt(x[0] * x[0] + qPow(mass, 2));

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * qExp(-E + mu) / (1 + Stat * G * qExp(-E + mu));
}

//__________________________________________________________________________
double FcnEnergyDens(double *x, double *par)
{
  // x[0] : p/T
  // This function returns 1/T^4*de/d(p/T)- it follows from the definition 
  // of average energy
  //

  double mu   = par[0];
  double mass = par[1];
  double G    = par[2];
  double Stat = par[3];
  double Deg  = par[4];

  double E = qSqrt(x[0] * x[0] + qPow(mass, 2));

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * E * qExp(-E + mu) / (1 + Stat * G * qExp(-E + mu));
}

//__________________________________________________________________________
double FcnEntropyDens(double *x, double *par)
{
  // x[0] : p/T
  // This function returns 1/T^3*ds/d(p/T)- it is calculated based on
  // s = -d(Omega_GC)/dT
  //
 
  double mu   = par[0];
  double mass = par[1];
  double G    = par[2];
  double Stat = par[3];
  double Deg  = par[4];

  double E = qSqrt(x[0] * x[0] + qPow(mass, 2));

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * qExp(-E + mu) / (1 + Stat * G * qExp(-E + mu)) * (x[0] * x[0] / 3. / E + E - mu);
}

//__________________________________________________________________________
double FcnPressure(double *x, double *par)
{
  // x[0] : p/T
  // This function returns 1/T^4*dP/d(p/T)- it is calculated based on
  // P = -d(Omega_GC)/dV
  //

  double mu   = par[0];
  double mass = par[1];
  double G    = par[2];
  double Stat = par[3];
  double Deg  = par[4];

  double E = qSqrt(x[0] * x[0] + qPow(mass, 2));

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * qExp(-E + mu) / (1 + Stat * G * qExp(-E + mu)) * x[0] * x[0] / 3. / E;
}

//__________________________________________________________________________
double FcnDensWidth(double *x, double *par)
{
  // x[0] : p/T
  // x[1] : m/T
  // This function returns 1/T^3 * d^2n/d(p/T)d(m/T) un-normalised
  // consistent with the no-width density
  //

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Stat  = par[3];
  double Deg   = par[4];
  double width = par[5];	// width/T

  double E = qSqrt(x[0] * x[0] + x[1] * x[1]);

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * qExp(-E + mu) / (1. + G * qExp(-E + mu) * Stat) / M_PI * mass * width * 2. * x[1] / (qPow((x[1] * x[1] - mass * mass), 2) + mass * mass * width * width);
}

//__________________________________________________________________________
double FcnEnergyDensWidth(double *x, double *par)
{
  // x[0] : p/T
  // x[1] : m/T
  // This function returns 1/T^4 * d^2e/d(p/T)d(m/T) un-normalised
  // consistent with the no-width energy density
  //

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Stat  = par[3];
  double Deg   = par[4];
  double width = par[5];	// width/T

  double E = qSqrt(x[0] * x[0] + x[1] * x[1]);

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * E * G * qExp(-E + mu) / (1. + G * qExp(-E + mu) * Stat) / M_PI * mass * width * 2. * x[1] / (qPow((x[1] * x[1] - mass * mass), 2) + mass * mass * width * width);
}

//__________________________________________________________________________
double FcnEntropyDensWidth(double *x, double *par)
{
  // x[0] : p/T
  // x[1] : m/T
  // This function returns 1/T^3 * d^2s/d(p/T)d(m/T) un-normalised
  // consistent with the no-width entropy density
  //

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Stat  = par[3];
  double Deg   = par[4];
  double width = par[5];	// width/T

  double E = qSqrt(x[0] * x[0] + x[1] * x[1]);

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * qExp(-E + mu) / (1. + G * qExp(-E + mu) * Stat) * (x[0] * x[0] / 3. / E + E - mu) / M_PI * mass * width * 2. * x[1] / ( qPow((x[1] * x[1] - mass * mass), 2) + mass * mass * width * width);
}

//__________________________________________________________________________
double FcnPressureWidth(double *x, double *par)
{
  // x[0] : p/T
  // x[1] : m/T
  // This function returns 1/T^4 * d^2P/d(p/T)d(m/T) un-normalised
  // consistent with the no-width pressure
  //

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Stat  = par[3];
  double Deg   = par[4];
  double width = par[5];	// width/T

  double E = qSqrt(x[0] * x[0] + x[1] * x[1]);

  return 1. / (2. * qPow(M_PI, 2)) * Deg * x[0] * x[0] * G * qExp(-E + mu) / (1. + G * qExp(-E + mu) * Stat) * x[0] * x[0] / 3. / E  / M_PI * mass * width * 2. * x[1] / ( qPow((x[1] * x[1] - mass * mass), 2) + mass * mass * width * width);
}

//__________________________________________________________________________
double FcnDensNormWidth(double *x, double *par)
{
  // x[0] : m/T
  // This function when integrated gives the normalisation of the 
  // Breit-Wigner distribution
  //

  double mass  = par[0];	// mass/T
  double width = par[1];	// width/T

  return 1. / M_PI * mass * width * 2. * x[0] / (qPow((x[0] * x[0] - mass * mass), 2) + mass * mass * width * width);
}

//__________________________________________________________________________
double FcnDensBoltzmannWidth(double *x, double *par)
{
  // x[0] : m/T
  // This function returns 1/T^3 * dn/d(m/T)

  double mu    = par[0];		// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Deg   = par[3];
  double width = par[4];	// width/T

  return 1. / (2. * qPow(M_PI, 2)) * G * Deg * qExp(mu) * x[0] * x[0] * gsl_sf_bessel_Kn(2, x[0]) /
    M_PI * mass * width * 2. * x[0] / (qPow((x[0] * x[0] - mass * mass), 2) + mass * mass * width * width);

}

//__________________________________________________________________________
double FcnEnergyBoltzmannWidth(double *x, double *par)
{
  // x[0] : m/T
  // This function returns 1/T^4 * de/d(m/T)

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Deg   = par[3];
  double width = par[4];	// width/T

  return 1. / (2. * qPow(M_PI, 2)) * G * Deg * qExp(mu) * x[0] * x[0] * x[0] * (gsl_sf_bessel_Kn(1, x[0]) + 3. / x[0] * gsl_sf_bessel_Kn(2, x[0])) / M_PI * mass * width * 2. * x[0] / (qPow((x[0] * x[0] - mass * mass), 2) + mass * mass * width * width);

}

//__________________________________________________________________________
double FcnEntropyBoltzmannWidth(double *x, double *par)
{
  // x[0] : m/T
  // This function returns 1/T^3 * ds/d(m/T)

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Deg   = par[3];
  double width = par[4];	// width/T

  return 1. / (2. * qPow(M_PI, 2)) * G * Deg * qExp(mu) * x[0] * x[0] * (x[0] * gsl_sf_bessel_Kn(1, x[0]) + (4. - mu) * gsl_sf_bessel_Kn(2, x[0])) / M_PI * mass * width * 2. * x[0] / (qPow((x[0] * x[0] - mass * mass), 2) + mass * mass * width * width);

}

//__________________________________________________________________________
double FcnEnergyFlucBoltzmannWidth(double *x, double *par)
{
  // x[0] : m/T
  // This function returns 1/T^4 * integrand

  double mu    = par[0];	// mu/T
  double mass  = par[1];	// mass/T
  double G     = par[2];
  double Deg   = par[3];
  double width = par[4];	// width/T

  return 1. / (2. * qPow(M_PI, 2)) * G * Deg * qExp(mu) * x[0] * x[0] * x[0] *
      (gsl_sf_bessel_Kn(0, x[0]) + 5. / x[0] * gsl_sf_bessel_Kn(1, x[0]) + 12. * qPow(1. / x[0], 2.) * gsl_sf_bessel_Kn(2, x[0]) )
      / M_PI * mass * width * 2. * x[0] / (qPow((x[0] * x[0] - mass * mass), 2) + mass * mass * width * width);
}

//__________________________________________________________________________
double GenericQStatDeriv(double *x, double *par)
{
    // this function returns
    // 1/T^3 * g/2/Pi^2 (e^{-E/T+mu/T})^a / (1+-e^{-E/T+mu/T})^b * E^c 

    // x[0] : p/T

    double mu   = par[0];   // mu/T
    double mass = par[1];   // mass/T
    double G    = par[2];   // gamma
    double Stat = par[3];
    double Deg  = par[4];
    double a    = par[5];
    double b    = par[6];
    double c    = par[7];

    double E = qSqrt(x[0] * x[0] + qPow(mass, 2));

    return  1. / (2.* qPow(M_PI, 2)) * qPow(x[0], 2) * Deg * qPow(G, a) * qPow(E, c)*
    qPow(Stat, a+1.) * qExp(-a * E + a * mu) / qPow(1 + Stat * G * qExp(-E + mu), b);
}

//__________________________________________________________________________
double GenericQStatDerivWidth(double *x, double *par)
{
    // this function returns
    // 1/T^3 * g/2/Pi^2 (e^{-E/T+mu/T})^a / (1+-e^{-E/T+mu/T})^b * E^c * BW Factor

    // x[0] : p/T
    // x[1] : m/T

    double mu    = par[0];   // mu/T
    double mass  = par[1];   // mass/T
    double G     = par[2];   // gamma
    double Stat  = par[3];
    double Deg   = par[4];
    double a     = par[5];
    double b     = par[6];
    double c     = par[7];
    double width = par[8]; // in fact width/T

    double E = qSqrt(x[0] * x[0] + x[1] * x[1]);  // in fact E/T

    return  1. / (2.* qPow(M_PI, 2)) * qPow(x[0], 2) * Deg * qPow(G, a) * qPow(E, c) *
    qPow(Stat, a+1.) * qExp(-a * E + a * mu) / qPow(1 + Stat * G * qExp(-E + mu), b)/
    M_PI * mass * width * 2. * x[1] / (qPow((x[1] * x[1] - mass * mass), 2) + mass * mass * width * width);
}
