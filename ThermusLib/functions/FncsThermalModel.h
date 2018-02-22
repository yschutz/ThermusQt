class TTMParticle;
class TTMThermalModel;
class TTMThermalModelBSQ;
class TTMThermalFit;
class F1;
class F2;

double Test(double *x, double *par);

double Norm(double *x, double *par);

double Partition(double *x, double *par);
double CorrPiplus(double *x, double *par);
double CorrPiminus(double *x, double *par);
double CorrKminus(double *x, double *par);
double CorrKplus(double *x, double *par);
double CorrKzero(double *x, double *par);
double CorrAKzero(double *x, double *par);
double CorrProton(double *x, double *par);
double CorrAProton(double *x, double *par);
double CorrNeutron(double *x, double *par);
double CorrANeutron(double *x, double *par);
double CorrLa(double *x, double *par);
double CorrALa(double *x, double *par);
double CorrSigmaplus(double *x, double *par);
double CorrASigmaplus(double *x, double *par);
double CorrSigmaminus(double *x, double *par);
double CorrASigmaminus(double *x, double *par);
double CorrDeltaminus(double *x, double *par);
double CorrADeltaminus(double *x, double *par);
double CorrDeltaplusplus(double *x, double *par);
double CorrADeltaplusplus(double *x, double *par);
double CorrKsiminus(double *x, double *par);
double CorrAKsiminus(double *x, double *par);
double CorrKsi0(double *x, double *par);
double CorrAKsi0(double *x, double *par);
double CorrOmega(double *x, double *par);
double CorrAOmega(double *x, double *par);

double FcnAbsOmega(double *x, double *par);
double FcnArgOmega(double *x, double *par);

double FcnDistr(double *x, double *par);
double FcnDens(double *x, double *par);
double FcnEnergyDens(double *x, double *par);
double FcnEntropyDens(double *x, double *par);
double FcnPressure(double *x, double *par);
double FcnDensWidth(double *x, double *par);
double FcnEnergyDensWidth(double *x, double *par);
double FcnEntropyDensWidth(double *x, double *par);
double FcnPressureWidth(double *x, double *par);
double FcnDensNormWidth(double *x, double *par);
double FcnDensBoltzmannWidth(double *x, double *par);
double FcnEnergyBoltzmannWidth(double *x, double *par);
double FcnEntropyBoltzmannWidth(double *x, double *par);

double IntegrateLegendre32(F1* func, double a, double b);
double IntegrateLegendre20(F1* func, double a, double b);
double IntegrateLegendre40(F1* func, double a, double b);

double IntegrateLaguerre32(F1* func);
double Integrate2DLaguerre32Legendre32(F2* func, double ay, double by);

double FindExclVolPressure(TTMThermalModelBSQ *model, double limit);

void   Minuit_fcn(int &npar, double *gin, double &f, double *par,
        int iflag);
void   fit_function(TTMThermalModel *mod,char *file,int flag = 0);

double FcnEnergyFlucBoltzmannWidth(double *x, double *par);
double GenericQStatDeriv(double *x, double *par);
double GenericQStatDerivWidth(double *x, double *par);
