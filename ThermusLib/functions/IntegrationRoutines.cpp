// Author: Spencer Wheaton 14 July 2004 //
// Adapted for Qt by Yves Schutz: September 2017

#include "f1.h"


//__________________________________________________________________________
double IntegrateSimpson(F1* func, double a, double b, double h)
{
  // Integrate function from a to b with step-size h using Simpson's method
  // In the last step h is recalculated if required.

  double sum = 0.;

  double x0 = a;
  double x1 = x0 + h;
  double x2 = x0 + 2.*h;

  do{
    double term = h / 3. * (func->eval(x0) + 4. * func->eval(x1) + func->eval(x2));
    sum += term;

    x0 = x2;
    x1 = x0 + h;
    x2 = x0 + 2. * h;
  } while(x2 <= b);

  if (x0 < b && x2 > b) {

    h = (b - x0) / 2.;

    x1 = x0 + h;
    x2 = x0 + 2. * h;

    double term = h / 3. * (func->eval(x0) + 4. * func->eval(x1) + func->eval(x2));
    sum += term;
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateBode(F1* func, double a, double b, double h)
{
  // Integrate function from a to b with step-size h using Bode's method
  //

  double sum = 0.;

  double x0  = a;
  double x1  = x0 + h;
  double x2  = x0 + 2.  * h;
  double x3  = x0 + 3.  * h;
  double x4  = x0 + 4.  * h;
  double x5  = x0 + 5.  * h;
  double x6  = x0 + 6.  * h;
  double x7  = x0 + 7.  * h;
  double x8  = x0 + 8.  * h;
  double x9  = x0 + 9.  * h;
  double x10 = x0 + 10. * h;

  do{
    double f0  = func->eval(x0);
    double f1  = func->eval(x1);
    double f2  = func->eval(x2);
    double f3  = func->eval(x3);
    double f4  = func->eval(x4);
    double f5  = func->eval(x5);
    double f6  = func->eval(x6);
    double f7  = func->eval(x7);
    double f8  = func->eval(x8);
    double f9  = func->eval(x9);
    double f10 = func->eval(x10);

    double term = 5. * h/ 299376.* (16067 * (f0 + f10) + 106300 * (f1 + f9) - 48525 * ( f2 + f8) + 272400 * (f3 + f7) - 260550 * (f4 + f6) + 427368 * f5);

    sum += term;
    
    x0  = x10;
    x1  = x0 + h;
    x2  = x0 + 2.  * h;
    x3  = x0 + 3.  * h;
    x4  = x0 + 4.  * h;
    x5  = x0 + 5.  * h;
    x6  = x0 + 6.  * h;
    x7  = x0 + 7.  * h;
    x8  = x0 + 8.  * h;
    x9  = x0 + 9.  * h;
    x10 = x0 + 10. * h;

  } while(x10 <= b);

  if(x0 < b && x10 > b) {

    double hh = (b - x0) / 10.;

    x1  = x0 + hh;
    x2  = x0 + 2.  * hh;
    x3  = x0 + 3.  * hh;
    x4  = x0 + 4.  * hh;
    x5  = x0 + 5.  * hh;
    x6  = x0 + 6.  * hh;
    x7  = x0 + 7.  * hh;
    x8  = x0 + 8.  * hh;
    x9  = x0 + 9.  * hh;
    x10 = x0 + 10. * hh;

    double f0  = func->eval(x0);
    double f1  = func->eval(x1);
    double f2  = func->eval(x2);
    double f3  = func->eval(x3);
    double f4  = func->eval(x4);
    double f5  = func->eval(x5);
    double f6  = func->eval(x6);
    double f7  = func->eval(x7);
    double f8  = func->eval(x8);
    double f9  = func->eval(x9);
    double f10 = func->eval(x10);

    double term = 5. * hh / 299376. * (16067 * (f0 + f10) + 106300 * (f1 + f9) - 48525 * (f2 + f8) + 272400 * (f3 + f7) - 260550 * (f4 + f6) + 427368 * f5);

    sum += term;
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateLegendre32(F1* func, double a, double b)
{
  // Integrate function from a to b using Legendre-Gaussian integration 
  // with 32 points.
  //

  double x[32];
  double w[32];

  x[0]  = -0.997263861849;
  x[1]  = -0.985611511545;
  x[2]  = -0.964762255588;
  x[3]  = -0.934906075938;
  x[4]  = -0.896321155766;
  x[5]  = -0.849367613733;
  x[6]  = -0.794483795968;
  x[7]  = -0.732182118740;
  x[8]  = -0.663044266930;
  x[9]  = -0.587715757241;
  x[10] = -0.506899908932; 
  x[11] = -0.421351276131; 
  x[12] = -0.331868602282; 
  x[13] = -0.239287362252; 
  x[14] = -0.144471961583; 
  x[15] = -0.048307665688; 
  x[16] = 0.048307665688; 	
  x[17] = 0.144471961583; 	
  x[18] = 0.239287362252; 	
  x[19] = 0.331868602282; 	
  x[20] = 0.421351276131; 	
  x[21] = 0.506899908932; 	
  x[22] = 0.587715757241;	
  x[23] = 0.663044266930; 	
  x[24] = 0.732182118740; 	
  x[25] = 0.794483795968; 	
  x[26] = 0.849367613733; 	
  x[27] = 0.896321155766; 	
  x[28] = 0.934906075938; 	
  x[29] = 0.964762255588; 	
  x[30] = 0.985611511545; 	
  x[31] = 0.997263861849; 	

  w[0]  = 0.007018610009;
  w[1]  = 0.016274394716;
  w[2]  = 0.025392065309;
  w[3]  = 0.034273862913;
  w[4]  = 0.042835898022;
  w[5]  = 0.050998059262;
  w[6]  = 0.058684093479;
  w[7]  = 0.065822222776;
  w[8]  = 0.072345794109;
  w[9]  = 0.078193895787;
  w[10] = 0.083311924227;
  w[11] = 0.087652093004;
  w[12] = 0.091173878696;
  w[13] = 0.093844399081;
  w[14] = 0.095638720079;
  w[15] = 0.096540088515;
  w[16] = 0.096540088515;
  w[17] = 0.095638720079;
  w[18] = 0.093844399081;
  w[19] = 0.091173878696;
  w[20] = 0.087652093004;
  w[21] = 0.083311924227;
  w[22] = 0.078193895787;
  w[23] = 0.072345794109;
  w[24] = 0.065822222776;
  w[25] = 0.058684093479;
  w[26] = 0.050998059262;
  w[27] = 0.042835898022;
  w[28] = 0.034273862913;
  w[29] = 0.025392065309;
  w[30] = 0.016274394716;
  w[31] = 0.007018610009;

  double sum = 0.;

  for(int i = 0 ; i < 32 ; i++) {
    w[i] = (b - a) / 2. * w[i];
    x[i] = (b - a) / 2. * x[i] + (b + a) / 2.;
    sum += w[i] * func->eval(x[i]);
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateLaguerre32(F1* func)
{
  // Integrate function from 0 to infinity using Gauss-Laguerre integration
  // with 32 points
  //

  double x[32];
  double w[32];

  x[0]  = 0.044489365833;
  x[1]  = 0.234526109520;
  x[2]  = 0.576884629302;
  x[3]  = 1.072448753818;
  x[4]  = 1.722408776445;
  x[5]  = 2.528336706426;
  x[6]  = 3.492213273022;
  x[7]  = 4.616456769750;
  x[8]  = 5.903958504174;
  x[9]  = 7.358126733186;
  x[10] = 8.982940924213; 		
  x[11] = 10.783018632540; 	
  x[12] = 12.763697986743; 	
  x[13] = 14.931139755523; 	
  x[14] = 17.292454336715; 	
  x[15] = 19.855860940336; 	
  x[16] = 22.630889013197; 	
  x[17] = 25.628636022459; 	
  x[18] = 28.862101816323; 	
  x[19] = 32.346629153965; 	
  x[20] = 36.100494805752; 	
  x[21] = 40.145719771539; 	
  x[22] = 44.509207995755; 	
  x[23] = 49.224394987309; 	
  x[24] = 54.333721333397; 	
  x[25] = 59.892509162134; 	
  x[26] = 65.975377287935; 	
  x[27] = 72.687628090663;	
  x[28] = 80.187446977914;	
  x[29] = 88.735340417892; 	
  x[30] = 98.829542868284;	
  x[31] = 111.751398097938; 	

  w[0]  = 0.114187105768;
  w[1]  = 0.266065216898;
  w[2]  = 0.418793137325;
  w[3]  = 0.572532846500;
  w[4]  = 0.727648788381;
  w[5]  = 0.884536719340;
  w[6]  = 1.043618875892;
  w[7]  = 1.205349274152;
  w[8]  = 1.370221338522;
  w[9]  = 1.538777256469;
  w[10] = 1.711619352686;
  w[11] = 1.889424063449;
  w[12] = 2.072959340247;
  w[13] = 2.263106633997;
  w[14] = 2.460889072488;
  w[15] = 2.667508126397;
  w[16] = 2.884392092922;
  w[17] = 3.113261327040;
  w[18] = 3.356217692596;
  w[19] = 3.615869856484;
  w[20] = 3.895513044949;
  w[21] = 4.199394104712;
  w[22] = 4.533114978534;
  w[23] = 4.904270287611;
  w[24] = 5.323500972024;
  w[25] = 5.806333214234;
  w[26] = 6.376614674160;
  w[27] = 7.073526580707;
  w[28] = 7.967693509296;
  w[29] = 9.205040331278;
  w[30] = 11.163013090768;
  w[31] = 15.390180415261;

  double sum = 0.;

  for(int i = 0 ; i < 32 ; i++) {
    sum += w[i] * func->eval(x[i]);
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateLegendre20(F1* func, double a, double b)
{
  // Integrate function from a to b using Legendre-Gaussian integration 
  // with 20 points.
  //

  double x[20];
  double w[20];

  x[0]  = -0.993128599185;
  x[1]  = -0.963971927278;
  x[2]  = -0.912234428251;
  x[3]  = -0.839116971822;
  x[4]  = -0.746331906460;
  x[5]  = -0.636053680727;
  x[6]  = -0.510867001951;
  x[7]  = -0.373706088715;
  x[8]  = -0.227785851142;
  x[9]  = -0.076526521133;
  x[10] = 0.076526521133;
  x[11] = 0.227785851142;
  x[12] = 0.373706088715;
  x[13] = 0.510867001951;
  x[14] = 0.636053680727;
  x[15] = 0.746331906460;
  x[16] = 0.839116971822;
  x[17] = 0.912234428251;
  x[18] = 0.963971927278;
  x[19] = 0.993128599185;

  w[0]  = 0.017614007139;
  w[1]  = 0.040601429768;
  w[2]  = 0.062672048333;
  w[3]  = 0.083276741577;
  w[4]  = 0.101930119817;
  w[5]  = 0.118194531962;
  w[6]  = 0.131688638449;
  w[7]  = 0.142096109318;
  w[8]  = 0.149172986473;
  w[9]  = 0.152753387131;
  w[10] = 0.152753387131;
  w[11] = 0.149172986473;
  w[12] = 0.142096109318;
  w[13] = 0.131688638449;
  w[14] = 0.118194531962;
  w[15] = 0.101930119817;
  w[16] = 0.083276741577;
  w[17] = 0.062672048333;
  w[18] = 0.040601429768;
  w[19] = 0.017614007139;

  double sum = 0.;

  for(int i = 0 ; i < 20 ; i++) {
    double w1 = (b - a) / 2. * w[i];
    double x1 = (b - a) / 2. * x[i] + (b + a) / 2.;
    sum += w1 * func->eval(x1);
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateLaguerre15(F1* func)
{
  // Integrate function from 0 to infinity using Gauss-Laguerre integration
  // with 15 points
  //

  double x[15];
  double w[15];

  x[0]  = 0.093307812017;
  x[1]  = 0.492691740302;
  x[2]  = 1.215595412071;
  x[3]  = 2.269949526204;
  x[4]  = 3.667622721751;
  x[5]  = 5.425336627414;
  x[6]  = 7.565916226613;
  x[7]  = 10.120228568019;
  x[8]  = 13.130282482176;
  x[9]  = 16.654407708330;
  x[10] = 20.776478899449;
  x[11] = 25.623894226729;
  x[12] = 31.407519169754;
  x[13] = 38.530683306486;
  x[14] = 48.026085572686;

  w[0]  = 0.239578170311;
  w[1]  = 0.560100842793;
  w[2]  = 0.887008262919;
  w[3]  = 1.223664402148;
  w[4]  = 1.574448721630;
  w[5]  = 1.944751976530;
  w[6]  = 2.341502056636;
  w[7]  = 2.774041926826;
  w[8]  = 3.255643346398;
  w[9]  = 3.806311714226;
  w[10] = 4.458477753837;
  w[11] = 5.270017784430;
  w[12] = 6.359563469731;
  w[13] = 8.031787632117;
  w[14] = 11.527772100941;

  double sum = 0.;

  for(int i = 0 ; i < 15 ; i++) {
    sum += w[i] * func->eval(x[i]);
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateLegendre64(F1* func, double a, double b)
{
  // Integrate function from a to b using Legendre-Gaussian integration 
  // with 64 points.
  //

  double x[64];
  double w[64];

  x[0]  = -0.999305041736;
  x[1]  = -0.996340116772;
  x[2]  = -0.991013371477;
  x[3]  = -0.983336253885;
  x[4]  = -0.973326827790;
  x[5]  = -0.961008799652;
  x[6]  = -0.946411374858;
  x[7]  = -0.929569172132;
  x[8]  = -0.910522137079;
  x[9]  = -0.889315445995;
  x[10] = -0.865999398154; 
  x[11] = -0.840629296253; 
  x[12] = -0.813265315123; 
  x[13] = -0.783972358943; 
  x[14] = -0.752819907261; 
  x[15] = -0.719881850172; 
  x[16] = -0.685236313054; 
  x[17] = -0.648965471255; 
  x[18] = -0.611155355172; 
  x[19] = -0.571895646203; 
  x[20] = -0.531279464020; 
  x[21] = -0.489403145707; 
  x[22] = -0.446366017253; 
  x[23] = -0.402270157964; 
  x[24] = -0.357220158338; 
  x[25] = -0.311322871990; 
  x[26] = -0.264687162209; 
  x[27] = -0.217423643740; 
  x[28] = -0.169644420424; 
  x[29] = -0.121462819296; 
  x[30] = -0.072993121788; 
  x[31] = -0.024350292663; 
  x[32] = 0.024350292663; 	
  x[33] = 0.072993121788; 	
  x[34] = 0.121462819296; 	
  x[35] = 0.169644420424; 	
  x[36] = 0.217423643740; 	
  x[37] = 0.264687162209; 	
  x[38] = 0.311322871990; 	
  x[39] = 0.357220158338; 	
  x[40] = 0.402270157964; 	
  x[41] = 0.446366017253; 	
  x[42] = 0.489403145707; 	
  x[43] = 0.531279464020; 	
  x[44] = 0.571895646203; 	
  x[45] = 0.611155355172; 	
  x[46] = 0.648965471255; 	
  x[47] = 0.685236313054; 	
  x[48] = 0.719881850172; 	
  x[49] = 0.752819907261; 	
  x[50] = 0.783972358943; 	
  x[51] = 0.813265315123; 	
  x[52] = 0.840629296253; 	
  x[53] = 0.865999398154; 	
  x[54] = 0.889315445995; 	
  x[55] = 0.910522137079; 	
  x[56] = 0.929569172132; 	
  x[57] = 0.946411374858; 	
  x[58] = 0.961008799652;	
  x[59] = 0.973326827790; 	
  x[60] = 0.983336253885; 	
  x[61] = 0.991013371477; 	
  x[62] = 0.996340116772; 	
  x[63] = 0.999305041736; 	


  w[0]  = 0.001783280722;
  w[1]  = 0.004147033257;
  w[2]  = 0.006504457969;
  w[3]  = 0.008846759826;
  w[4]  = 0.011168139460;
  w[5]  = 0.013463047897;
  w[6]  = 0.015726030476;
  w[7]  = 0.017951715776;
  w[8]  = 0.020134823154;
  w[9]  = 0.022270173808;
  w[10] = 0.024352702569;
  w[11] = 0.026377469715;
  w[12] = 0.028339672614;
  w[13] = 0.030234657072;
  w[14] = 0.032057928355;
  w[15] = 0.033805161837;
  w[16] = 0.035472213257;
  w[17] = 0.037055128540;
  w[18] = 0.038550153179;
  w[19] = 0.039953741133;
  w[20] = 0.041262563243;
  w[21] = 0.042473515124;
  w[22] = 0.043583724529;
  w[23] = 0.044590558164;
  w[24] = 0.045491627927;
  w[25] = 0.046284796579;
  w[26] = 0.046968182815;
  w[27] = 0.047540165714;
  w[28] = 0.047999388596;
  w[29] = 0.048344762235;
  w[30] = 0.048575467441;
  w[31] = 0.048690957009;
  w[32] = 0.048690957009;
  w[33] = 0.048575467441;
  w[34] = 0.048344762235;
  w[35] = 0.047999388596;
  w[36] = 0.047540165714;
  w[37] = 0.046968182815;
  w[38] = 0.046284796579;
  w[39] = 0.045491627927;
  w[40] = 0.044590558164;
  w[41] = 0.043583724529;
  w[42] = 0.042473515124;
  w[43] = 0.041262563243;
  w[44] = 0.039953741133;
  w[45] = 0.038550153179;
  w[46] = 0.037055128540;
  w[47] = 0.035472213257;
  w[48] = 0.033805161837;
  w[49] = 0.032057928355;
  w[50] = 0.030234657072;
  w[51] = 0.028339672614;
  w[52] = 0.026377469715;
  w[53] = 0.024352702569;
  w[54] = 0.022270173808;
  w[55] = 0.020134823154;
  w[56] = 0.017951715776;
  w[57] = 0.015726030476;
  w[58] = 0.013463047897;
  w[59] = 0.011168139460;
  w[60] = 0.008846759826;
  w[61] = 0.006504457969;
  w[62] = 0.004147033257;
  w[63] = 0.001783280722;

  double sum = 0.;

  for(int i = 0 ; i < 64 ; i++) {
    w[i] = (b - a) / 2. * w[i];
    x[i] = (b - a) / 2. * x[i] + (b + a) / 2.;
    sum += w[i] * func->eval(x[i]);
  }
  return sum;
}

//__________________________________________________________________________
double IntegrateLegendre40(F1* func, double a, double b)
{
  // Integrate function from a to b using Legendre-Gaussian integration 
  // with 40 points.
  //

  static double x[40];
  static double w[40];

  x[0]  = -0.998237709711;
  x[1]  = -0.990726238699;
  x[2]  = -0.977259949984;
  x[3]  = -0.957916819214;
  x[4]  = -0.932812808279;
  x[5]  = -0.902098806969;
  x[6]  = -0.865959503212;
  x[7]  = -0.824612230833;
  x[8]  = -0.778305651427;
  x[9]  = -0.727318255190;
  x[10] = -0.671956684614;
  x[11] = -0.612553889668;  
  x[12] = -0.549467125095;  
  x[13] = -0.483075801686;  
  x[14] = -0.413779204372;  
  x[15] = -0.341994090826;  
  x[16] = -0.268152185007;  
  x[17] = -0.192697580701;  
  x[18] = -0.116084070675;  
  x[19] = -0.038772417506;  
  x[20] = 0.038772417506;	 
  x[21] = 0.116084070675; 	 
  x[22] = 0.192697580701; 	 
  x[23] = 0.268152185007;	 
  x[24] = 0.341994090826; 	 
  x[25] = 0.413779204372; 	 
  x[26] = 0.483075801686; 	 
  x[27] = 0.549467125095; 	 
  x[28] = 0.612553889668; 	 
  x[29] = 0.671956684614; 	 
  x[30] = 0.727318255190; 	 
  x[31] = 0.778305651427; 	 
  x[32] = 0.824612230833; 	 
  x[33] = 0.865959503212; 	 
  x[34] = 0.902098806969; 	 
  x[35] = 0.932812808279; 	 
  x[36] = 0.957916819214; 	 
  x[37] = 0.977259949984; 	 
  x[38] = 0.990726238699; 	 
  x[39] = 0.998237709711; 	 

  w[0]  = 0.004521277099;
  w[1]  = 0.010498284521;
  w[2]  = 0.016421058382;
  w[3]  = 0.022245849194;
  w[4]  = 0.027937006980;
  w[5]  = 0.033460195283;
  w[6]  = 0.038782167974;
  w[7]  = 0.043870908186;
  w[8]  = 0.048695807635;
  w[9]  = 0.053227846984;
  w[10] = 0.057439769099;
  w[11] = 0.061306242493;
  w[12] = 0.064804013457;
  w[13] = 0.067912045815;
  w[14] = 0.070611647391;
  w[15] = 0.072886582396;
  w[16] = 0.074723169058;
  w[17] = 0.076110361901;
  w[18] = 0.077039818164;
  w[19] = 0.077505947978;
  w[20] = 0.077505947978;
  w[21] = 0.077039818164;
  w[22] = 0.076110361901;
  w[23] = 0.074723169058;
  w[24] = 0.072886582396;
  w[25] = 0.070611647391;
  w[26] = 0.067912045815;
  w[27] = 0.064804013457;
  w[28] = 0.061306242493;
  w[29] = 0.057439769099;
  w[30] = 0.053227846984;
  w[31] = 0.048695807635;
  w[32] = 0.043870908186;
  w[33] = 0.038782167974;
  w[34] = 0.033460195283;
  w[35] = 0.027937006980;
  w[36] = 0.022245849194;
  w[37] = 0.016421058382;
  w[38] = 0.010498284521;
  w[39] = 0.004521277099;

  double sum = 0.;

  for(int i = 0 ; i < 40 ; i++) {
    double w1 = (b - a) / 2. * w[i];
    double x1 = (b - a) / 2. * x[i] + (b + a) / 2.;
    sum += w1 * func->eval(x1);
  }
  return sum;
}
