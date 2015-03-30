#include "../include/PSMath.h"

#include <TMarker.h>
#include <TPolyMarker.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

// Collection of mathematical tools
//-----------------------------
//  PSfit            Fit tool
//  PSVprint         print a vector
//  PSMprint         print a matrix
//  PSM2print        print a 2x2 matrix
//  PSMinverse       invert a symmetric matrix
//  PSMCholtest      test inversion of symmetric matrix via cholesky LR decomposition
//  PSMmultiply      multiply two matrices
//  PSMmultiplyMRRT  multiply triangular matrix R with transpose, 
//  PSMmultiplyMT    multiply matrix  A = B * B^T 
//  PSMRTrianInvert2 invert a triangular R - matrix
//  PSMRTrianInvert  invert a triangular R - matrix 
//  PSMCholesky      Cholesky decomposition of  M = n*n  symmetric matrix
//  PSfuncQuadratic  compute multi-dim quadratic function (a,g,H)
//  PSfitTest           generic test function for fits
//  PSminIterate     iteration step for minimisation (new a,h,d)
//  PSfitMinStep        calcuate Newton Step for minimisation

//  PSfitconstrain0Test test function extrapolation to F=constrain
//  PSfitconstrain0     calculate a1,a2 for constraint F(a^2)=c 
//  PSfitconstrain1     constrain function F(a) with derivative and Hesse

//  PSderivative1       1-dim case of PSderivative,  see documentation there
//  PSderivative        Tool for numerical calc. of derivative and Hesse matrix

Int_t
PSMath::PSfit (Int_t iloop, Int_t &iter, Int_t &method, Int_t &mode,
               Bool_t &noNewtonShifts, Int_t printlevel, Int_t np, Double_t a[],
               Double_t astart[], Double_t alimit[][2], Double_t aprec[],
               Double_t daN[], Double_t h[], Double_t aMemory[][5], Double_t chi2,
               Double_t chi2iter[], Double_t g[], Double_t H[], Double_t Hinv[])
{
  // generic fitter using Newton method and Line Search within parameter limits
  // iter:     set iter=0 at start of a new fit, 
  //           iter<0 during construction of numerical derivatives
  //           iter>0 after each iteration
  // method:   preferred initial fit method: 
  //           =1 line search in direction of daN[],  
  //           =2 Newtown in all dimensions 
  // mode:     mode within method; 
  // printlevel: 0: quite mode, 1: one line with fit result
  //             2: full fit result, 3: one line per iteration, 4: more and more
  // np:         number of fit parameters
  // a[np]:      fit parameters, fill start values at beginning when iloop=0, 
  //             contains actual fit parameters for chi2 calculation,
  //             will contain final result
  // astart[np]: start values for a[], will not be changed
  // alimit[np]: upper and lower limits for a[]
  // aprec[np]:  precision requested for a[], also minimum value for h[]
  //             and minimum distance of a[] to alimit[] (needed to calculate derivatives) 
  // daN[]:      step vector from iteration to iteration,
  //             may contain initial search direction if method= line search
  // h[]:        for Newton method: initial step width for calculation of num. derivatives
  // chi2:       must contain function value at input value of a to be minimized
  // chi2iter[]  Function (chi2) with nominal a[], i.e. no shift of a[]
  // g[np]       derivative vector  g[np]
  // H[np*np]    Hesse matrix,   g[] and H[] are also used as 
  //                             intermediate storage of chi2
  // Hinv[np*np] Inverse of Hesse matrix

  static Int_t icallNewton, iterMemory;
  static Double_t chi2Memory;
  static Double_t x[4], f[4];
  static Double_t xx, xlimit[2];
  static Double_t xh, daNabs;
  static Double_t epsx = 0.1, epsf = 0.1;
  Int_t convergence;

  Int_t itemp, ready;
  Double_t temp, d;

  //  std::cout << "PSfit ==== iloop " << iloop << "  chi2 " << chi2 << std::endl;

  convergence = 0;
  if (iloop == 0) {                                       // start of a new fit
    if( PSVnorm (daN, np) == 0)
      std::cout << "WARNING! No initial search direction given! Start with Newton Method not yet supported!" << std::endl;
    iterMemory = 0;
    iter = 0;
    chi2Memory = 123456.;
    for (Int_t ip = 0; ip < np; ip++) {                          // check limits
      alimit[ip][0] = fmax (alimit[ip][0], -pow (10.0, 10.0));
      alimit[ip][1] = fmin (alimit[ip][1], pow (10.0, 10.0));
      if (alimit[ip][0] >= alimit[ip][1]) {
        alimit[ip][0] = -pow (10.0, 10.0);
        alimit[ip][1] = pow (10.0, 10.0);
      }
      if (astart[ip] < alimit[ip][0] || astart[ip] > alimit[ip][1]) { // NOT WORKING astart has no effect once the loop started!
        astart[ip] = 0.5 * (alimit[ip][0] + alimit[ip][1]); // set to mean in between limits
        return convergence;
      }
    }
  }

  if (method == 3) {
    if(fabs(chi2Memory - chi2) < epsf) {      // test convergence
      convergence = 1;
      iterMemory = iterMemory + 1;
      iter = iterMemory;
      chi2Memory = chi2;
    }
    else{
      if(printlevel >=2)
	  std::cout << "No Convergence! Back to LineSearch!" << std::endl;
      method = 1;                       // switch to line search
      mode = 1;
      for (Int_t ip = 0; ip < np; ip++) {
	astart[ip] = a[ip];
      }
    }
  }

  iter = -1; // only positive if a new iteration step (LineSearch, ..) is finished
  if (method == 1) {                // ----------------------------- line search
    if(printlevel >=2)
      std::cout << "In Linesearch!" << std::endl;
    if (mode == 1) {                     // start of a new line search
      daNabs = PSVnorm (daN, np);      // check initial search direction
      //      xh     = PSVnorm(h, np) ;        // initial step width for line search
      xh = 1.0/PSVnorm (daN, np);        // initial step width for line search

      if (daNabs == 0.)
        return 5;                            //Minimum found at both limits
      
      else {
        PSLineLimit (np, astart, daN, alimit, xlimit);
      }      // limits for line search
    }

    Double_t epsxLS = epsx;

    for(Int_t ip = 0; ip < np; ip++) {
      if(aprec[ip]*0.5/fabs(daN[ip]) < epsxLS){
	epsxLS = aprec[ip]*0.5/fabs(daN[ip]);
      }
    }

    xx = PSLineSearch (mode, xh, xlimit, epsxLS, epsf, x, f, chi2, printlevel);
    for (Int_t ip = 0; ip < np; ip++) {
      a[ip] = astart[ip] + xx * daN[ip];
    }

    if (mode <= 0) {                                            // Minimum found
      if(printlevel >=2){
	std::cout << "Minimum found between:" << std::endl;
	std::cout << "Eb1 = " << astart[0] + x[1] * daN[0] << " ETau1 = " << astart[1] + x[1] * daN[1] << std::endl;
	std::cout << "and" << std::endl;
	std::cout << "Eb1 = " << astart[0] + x[3] * daN[0] << " ETau1 = " << astart[1] + x[3] * daN[1] << std::endl;
	std::cout << "Finished Linesearch! Test Convergence" << std::endl;
      }
      //      PSVprint("aMemory ",aMemory,np) ;
      //      PSVprint("      a ", a, np) ;
      bool didConverge = true;
      for (Int_t ip = 0; ip < np; ip++) { // check for progress w.r.t. previous iteration
        if (fabs(a[ip] - aMemory[ip][0]) > aprec[ip]) {
	  if(printlevel >=2)
	    std::cout << "No Convergence as Parameter " << np << " is " << fabs(a[ip] - aMemory[ip][0]) << " away from previous iteration." << std::endl;
	  didConverge = false;
          break;
        }
      }
      if(didConverge){
	convergence = 2;
	iterMemory = iterMemory + 1;
	iter = iterMemory;
	return convergence;
      }

 
      for (Int_t previousIter = 1; previousIter <= 4; previousIter++){
	bool weHaveBeenHereBefore = true;
	for (Int_t ip = 0; ip < np; ip++) {
	  if(fabs(aMemory[ip][previousIter] - a[ip]) > aprec[ip]*0.01){
	    weHaveBeenHereBefore = false;
	    if(printlevel >=2)
	      std::cout << "We have not been here before!" << std::endl;
	    break;
	  }
	}
	if(weHaveBeenHereBefore){
	  if(printlevel >=1){
	    std::cout << "Pattern recognized! We have been here " << previousIter+1 << " iterations ago! Disable NewtonShifts!" << std::endl;
	  }
	  noNewtonShifts = true;
	}
      }

      for (Int_t ip = 0; ip < np; ip++) {
	aMemory[ip][4] = aMemory[ip][3];
	aMemory[ip][3] = aMemory[ip][2];
	aMemory[ip][2] = aMemory[ip][1];
	aMemory[ip][1] = aMemory[ip][0];
        aMemory[ip][0] = a[ip];
      }

      if(printlevel >=2)
	std::cout << "No Convergence! Switch to Newton!" << std::endl;
      method = 2;
      icallNewton = -1;                          // switch to Newton Method
      iterMemory = iterMemory + 1;
      iter = iterMemory;
      chi2Memory = chi2;
    }
  }

  if (method == 2) {                                    // Newton method
    //    std::cout << "Newton Method ---------------------- iter " << iter << std::endl;
    if (icallNewton == -1) {
      PSNewtonLimitShift (1, np, a, alimit, aprec, daN, h, g, H);
      icallNewton = 0;
      return convergence;
    }
    if(printlevel >=2)
	std::cout << "Newton Method! Calc. derivative!" << std::endl;
    if(np > 1)
      ready = PSderivative(icallNewton, np, a, h, chi2, chi2iter, g, H);
    else
      ready = PSderivative1(icallNewton, a, h, chi2, g, H);

    icallNewton = icallNewton + 1;
    d = 99.;
    if (ready == 1) {
      if(printlevel >=2)
	std::cout << "Newton Method! Got derivative! Get Minimum!" << std::endl;
      PSNewtonLimitShift (-1, np, a, alimit, aprec, daN, h, g, H);

      d = PSNewtonAnalyzer (np, a, alimit, aprec, daN, h, g, H, Hinv, chi2, noNewtonShifts, printlevel);
      //      std::cout << "d " << d << "   chi2Memory " << chi2Memory << "  chi2 " << chi2 << std::endl;
      if (d < epsx) {      // test convergence in next run by checking if deltaChi2 is small enough
        method = 3;
	if(printlevel >=2)
	  std::cout << "Small Shift! Check for Convergence (small Chi2) in next Loop!" << std::endl;
      }
      else {
	if(printlevel >=2)
	  std::cout << "No Minimum! Back to Linesearch!" << std::endl;
	method = 1;
	mode = 1;                        // switch to line search
	for (Int_t ip = 0; ip < np; ip++) {
          astart[ip] = a[ip];
        }
      }
    }
  }
  return convergence;
}
//----------------------------------------------------------------
void
PSMath::PSNewtonLimitShift (Int_t sign, Int_t np, Double_t a[],
                            Double_t alimit[][2], Double_t aprec[],
                            Double_t daN[], Double_t h[], Double_t g[],
                            Double_t H[])
{
  // ------- for Newton Method: if close to limit shift central point by less than precision
  if (sign > 0) {
    for (Int_t ip = 0; ip < np; ip++) {                 // check limits
      daN[ip] = 0.;
      //      cout << "a[ip] " << a[ip] << "  alimit[ip][0] " << alimit[ip][0]
      //	   << "  aprec[ip] " << aprec[ip] << endl;
      if (a[ip] < alimit[ip][0] + aprec[ip]) {
        daN[ip] = aprec[ip] * 0.9;
        a[ip] = alimit[ip][0] + daN[ip];
        h[ip] = aprec[ip] * 0.8;
      }
      else if (a[ip] < alimit[ip][0] + h[ip]) {
        h[ip] = fabs(alimit[ip][0] - a[ip]) * 0.99;
      }
      if (a[ip] > alimit[ip][1] - aprec[ip]) {
        daN[ip] = -aprec[ip] * 0.9;
        a[ip] = alimit[ip][1] + daN[ip];
        h[ip] = aprec[ip] * 0.8;
      }
      else if (a[ip] > alimit[ip][1] - h[ip]) {
        h[ip] = fabs(alimit[ip][1] - a[ip]) * 0.99;
      }
    }
  }
  else if (sign < 0) {     //  ------------------    shift back to limit
    //    PSMprint("in Shift  vorher  H ",H,np,np) ;
    //    PSVprint("in Shift  vorher  daN ",daN,np) ;
    //    PSVprint("in Shift  vorher  a ",a,np) ;
    //    PSVprint("in Shift  vorher  g ",g,np) ;
    for (Int_t ip = 0; ip < np; ip++) {
      daN[ip] = -daN[ip];
      a[ip] = a[ip] + daN[ip];
      for (Int_t jp = 0; jp < np; jp++) {
        g[ip] = g[ip] + H[ip * np + jp] * daN[jp]; // also shift derivative
      }
    }
    //    PSVprint("in Shift  nacher   a ",a,np) ;
    //    PSVprint("in Shift  nachher  g ",g,np) ;
  }
}
//----------------------------------------------------------------
Double_t
PSMath::PSNewtonAnalyzer (Int_t np, Double_t a[], Double_t alimit[][2],
                          Double_t aprec[], Double_t daN[], Double_t h[],
                          Double_t g[], Double_t H[], Double_t Hinv[],
                          Double_t chi2, Bool_t noNewtonShifts, Int_t printlevel)
{
  //  cout << "   PSNewtonAnalyzer  chi2 = " << chi2 << "\n" ;

  Double_t d;                    // convergence test value
  Int_t iNewton;                 // =1 if Newton Method can be used
  Double_t gnorm, hnorm;
  Double_t xlimit[2];         // straight line distance from a[] to alimits[][2]

  d = 100.;
  for (Int_t ip = 0; ip < np; ip++) {
    daN[ip] = 0.;
  }

  //  --------- test validity of Hesse for Newton method --------
  iNewton = 1;
  if (np == 2) {
    if (H[0] * H[3] <= H[1] * H[2]) {
      iNewton = 0;
      if (printlevel>0) std::cout << "PSNewton Analyzer check 1 (det<0)" << std::endl;
    }
  } // check only works for np=2
  for (Int_t ip = 0; ip < np; ip++) {    // check for positive Diagonal of Hesse
    if (H[ip * np + ip] < 0.) {
      iNewton = 0;
      if (printlevel>0) std::cout << "PSNewton Analyzer check 2 (negative diagonal entries)" << std::endl;
      break;
    }
    if (H[ip * np + ip] * h[ip] < 0.07 * fabs(g[ip])) {
      iNewton = 0;
      if (printlevel>0) std::cout << "PSNewton Analyzer check 3" << std::endl;
      //      cout << "ip  " << ip << "  H " << H[ip*np+ip] << "  h " << h[ip] << "  g " << g[ip]
      //	   << "   H[ip*np+ip] * h[ip] " << H[ip*np+ip] * h[ip]
      //	   << "  0.07 * abs(g[ip]) " << 0.07 * abs(g[ip]) << endl;
      break;
    }
  }
  if (iNewton == 1) {                      // Newton Step width
    Double_t temp = PSMinverse (H, Hinv, np);
    for (Int_t ii = 0; ii < np; ii++) {
      for (Int_t jj = 0; jj < np; jj++) {
        daN[ii] = daN[ii] - Hinv[ii * np + jj] * g[jj];
      }
    }
    d = 0.;
    for (Int_t ip = 0; ip < np; ip++) {
      d = d - g[ip] * daN[ip];
    }   // convergence test value
    //    cout <<   "    d    = " << d << endl;
    if (d < 0.) {
      iNewton = 0;
    }              // daN wrong since not in direction of -g
  }

  // ------------- Newton not ok ------------------------ 
  if (iNewton != 1 || noNewtonShifts) {
    if (printlevel>0 && iNewton != 1){ 
      std::cout << "PSNewton Analyzer ===== WARNING ========== H is not positive definit" << std::endl;
      if (printlevel>4){ 
	PSVprint( "    a      ",a     ,np) ;
	PSM2print("    alimit ",alimit,np) ;
	PSVprint( "    aprec  ",aprec ,np) ;
	PSVprint( "    daN    ",daN   ,np) ;
	PSVprint( "    h      ",h     ,np) ;
	PSMprint( "    H      ",H     ,np,np) ;
      }
    }
    if(printlevel>0 && noNewtonShifts)
      std::cout << "Newton Shifts are disabled!" << std::endl;
    gnorm = PSVnorm (g, np);            // continue in direction of derivative
    hnorm = PSVnorm (h, np);              // continue with previous step width
    for (Int_t ip = 0; ip < np; ip++) {
      daN[ip] = -g[ip] * hnorm / gnorm;
      if(printlevel>2)
	std::cout << "daN[" << np << "] set to " << daN[ip] << std::endl;
    }
    for (Int_t ip = 0; ip < np; ip++) {                 // check limits
      if (a[ip] < alimit[ip][0] + aprec[ip]) {
        h[ip] = aprec[ip];
        daN[ip] = fmax (daN[ip], 0.);
	if (printlevel>0) std::cout << "At lower limit for parameter " << ip << std::endl;
      }
      else if (a[ip] > alimit[ip][1] - aprec[ip]) {
        h[ip] = aprec[ip];
        daN[ip] = fmin (daN[ip], 0.);
	if (printlevel>0) std::cout << "At upper limit for parameter " << ip << std::endl;
	//if (daN[ip]*g[ip]<0.) {daN[ip] = 0.; }
      }
    }
    if (printlevel>2)
      std::cout << "daN set to: daN[0]: " << daN[0] << " daN[1]: " << daN[1] << std::endl;
    //    PSVprint( "      g    ",g   ,np) ;
    //    PSVprint( "    daN    ",daN   ,np) ;
    d = 110.;
    return d;
  }

  // ------------ Newton step seems to be fine --------------
  PSLineLimit (np, a, daN, alimit, xlimit);  // get distance to limit
  
  if (printlevel>0) std::cout << "daN shift for Newton in daN[0]: " << daN[0] << " daN[1]: " << daN[1] << std::endl;
  
  Double_t xmax = fmin (1., xlimit[1]); //  Newton step would lead out of limits

  for (Int_t ip = 0; ip < np; ip++) {                       // apply Newton step
    a[ip] = a[ip] + daN[ip] * xmax;                  // update parameter value
    h[ip] = 100. * sqrt (0.000001 * fabs(chi2 / H[ip * np + ip])); // update step width
  }

  if (printlevel>0) std::cout << "Newton shifted to: a[0] = " << a[0] << " a[1] = " << a[1] << std::endl;

  //  PSVprint("in Analyzer  vor limits  daN ",daN,np) ;
  for (Int_t ip = 0; ip < np; ip++) {                 // check limits
    if (a[ip] < alimit[ip][0] + aprec[ip]) {
      h[ip] = aprec[ip];
      //      cout << " at limit ?  ip " << ip << "  " ;  PSVprint("    g ",g,  np) ;
      //      cout << " at limit ?  ip " << ip << "  " ;  PSVprint("  daN ",daN,np) ;
      //      if (daN[ip]*g[ip]<0.) {daN[ip] = 0.; }
      daN[ip] = fmax (daN[ip], 0.);
      if (printlevel>0) std::cout << "At lower limit for parameter " << ip << std::endl;
      //      cout << " at limit ?  ip " << ip << "  " ;  PSVprint("  daN ",daN,np) ;
    }
    else if (a[ip] > alimit[ip][1] - aprec[ip]) {
      h[ip] = aprec[ip];
      //      cout << " at limit ?  ip " << ip << "  " ;  PSVprint("    g ",g,  np) ;
      //      cout << " at limit ?  ip " << ip << "  " ;  PSVprint("  daN ",daN,np) ;
      //      if (daN[ip]*g[ip]<0.) {daN[ip] = 0.; }
      daN[ip] = fmin (daN[ip], 0.);
      if (printlevel>0) std::cout << "At upper limit for parameter " << ip << std::endl;
      //      cout << " at limit ?  ip " << ip << "  " ;  PSVprint("  daN ",daN,np) ;
    }
  }
  //  PSVprint("in Analyzer  vor d   daN ",daN,np) ;
  d = 0.;
  for (Int_t ip = 0; ip < np; ip++) {                 // check limits
    d = d - g[ip] * daN[ip]; // convergence test value (only when not at limit)
  }
  if (d < 0.) {
    //    PSVprint("d<0  g   ",g,  np) ;
    //    PSVprint("d<0  daN ",daN,np) ;
    //    PSMprint("d<0  H "  ,H,  np,np) ;
    d = 111.;
  }
  
  Double_t d2 = 0.;
  for (Int_t ip = 0; ip < np; ip++) {
    for (Int_t jp = 0; jp < np; jp++) {
      d2 = d2 + daN[ip] * H[ip * np + jp] * daN[jp];
    }
  }

  if (printlevel>2)
      std::cout << "daN set to: dan[0]: " << daN[0] << " daN[1]: " << daN[1] << std::endl;
  
  return d;
}
//-------------------------------------------------------
void
PSMath::PSfitShow (Int_t iloop, Int_t convergence, Int_t iter, Int_t method,
                   Int_t mode, Int_t printlevel, Int_t graphiklevel, Int_t np,
                   Double_t a[], Double_t astart[], Double_t alimit[][2],
                   Double_t aprec[], Double_t daN[], Double_t h[],
                   Double_t chi2, Double_t g[], Double_t H[])
{
  // printlevel: 0: quite mode,      1: one line with fit result
  //             2: full fit result, 3: one line per iteration, 4: more and more

  static Double_t chi2Memory;
  if (printlevel > 0) {
    if (printlevel >= 2 && iloop == 0) {
      std::cout << "---------- PSfitShow ----------- starts with  " << iloop
          << "  " << iter << "  method " << method << "  mode " << mode
          << std::endl;
      PSVprint ("    a      ", a, np);
      PSVprint ("    astart ", astart, np);
      PSVprint ("    aprec  ", aprec, np);
      PSVprint ("    daN    ", daN, np);
      PSVprint ("    h      ", h, np);
      PSM2print ("    alimit ", alimit, np);
      chi2Memory = 12345.;
    }
    if ((printlevel >= 3 && iter >= 0) || printlevel >= 4) {
      std::cout << "PSfit loop " << iloop << "  ";
      if (method == 1) {
        std::cout << "LineSearch ";
      }
      else if (method == 2) {
        std::cout << "Newton     ";
      }
      std::cout << "  mode " << mode << "   iter " << iter << "  chi2 " << chi2
          << "  " << "  convergence " << convergence;
      PSVprint ("    a ", a, np);
    }
    if (convergence != 0) {
      if (printlevel >= 2) {   // print full fit result
        std::cout << "------------------ PSfit result ------------------ \n ";
        if (convergence == 1) {
          std::cout << "Convergence after  ";
        }
        else if (convergence == 2) {
          std::cout << "No further progress after  ";
        }
        else {
          std::cout << "No convergence after  ";
        }
        std::cout << "  iloop " << iloop << "  iter " << iter
            << "  convergence " << convergence << std::endl;
        std::cout << "               chi2 = " << chi2 << "     (previous chi2= "
            << chi2Memory << ")" << std::endl;
        PSVprint (" Fit result    a", a, np);
        PSVprint (" Fit result  daN", daN, np);
        PSVprint (" Fit result    h", h, np);
        PSM2print ("    alimit ", alimit, np);
        PSVprint (" Fit result    g", g, np);
        PSMprint (" Fit result    H", H, np, np);
      }
      if (printlevel >= 1) {   // print one line with fit result
        std::cout << "PSfit iloop " << iloop << "  iter " << iter
            << "  convergence " << convergence << "   chi2= " << chi2;
        PSVprint ("   a", a, np);
      }
    }
  }
  chi2Memory = chi2;
  if (graphiklevel > 0) { // ------------------------ Plots iterations in plane a[0] - a[1]
    //    c1->Clear();
    static const Int_t nloopmax = 100;
    static Double_t Xloop[nloopmax];
    static Double_t Yloop[nloopmax];
    static TPolyMarker * iterMarker = NULL;
    static TPolyMarker * loopMarker = NULL;
    if (iloop == 0) {
      iterMarker = new TPolyMarker (100);
      iterMarker->SetMarkerColor (kBlue);
      iterMarker->SetMarkerSize (1.5);

      loopMarker = new TPolyMarker (100);
      loopMarker->SetMarkerColor (kBlack);
      loopMarker->SetMarkerSize (0.5);
      //      static Double_t *Xloop = loopMarker->GetX() ;
      //      static Double_t *Yloop = loopMarker->GetY() ;

      //      TF2 * f2 = new TF2("Test-Function","(x**2 + y**2 -1.9*x*y)**2 + x**2 + y**2 -1.9*x*y",
      //			 alimit[0][0], alimit[0][1], alimit[1][0], alimit[1][1] ) ;
      //      f2->SetMinimum( 0.);
      //      f2->SetMaximum(20.);
      //      Double_t levels[] = {1.,2.,3.,4.,5.,6.,7.,8.,9.,10.,11.,12.,13.,14.,15.,16.,17.,18.,19.,20.}
      //      f2->SetContour(20,levels) ;
      //      f2->Draw("CONT1Z");

      //      TArrow  *ar=new TArrow(a[0], a[1], a[0]+daN[0],  a[1]+daN[1], 0.02,"|>") ;
      //      ar->SetLineColor(kRed);   ar->SetFillColor(kRed);
      //      ar->DrawClone();
    }

    if (iter >= 0 && iter < nloopmax) {
      iterMarker->SetPoint (iter, a[0], a[1]);
    }
    if (iloop >= 0 && iloop < nloopmax) {
      Xloop[iloop] = a[0];
      Yloop[iloop] = a[1];
    }

    if (convergence != 0) {

      Double_t *Xiter = iterMarker->GetX ();
      Double_t *Yiter = iterMarker->GetY ();
      loopMarker->DrawPolyMarker (iloop, Xloop, Yloop, "same");
      iterMarker->DrawPolyMarker (iter, Xiter, Yiter, "same");

      TMarker *mi = new TMarker (Xiter[0], Yiter[0], 20);
      mi->SetMarkerColor (kRed);
      TMarker *mf = new TMarker (Xiter[iter], Yiter[iter], 20);
      mf->SetMarkerColor (kGreen);
      TMarker *ma = new TMarker (a[0], a[1], 20);
      ma->SetMarkerColor (kMagenta);
      mi->Draw ();
      mf->Draw ();
      ma->Draw ();
    }
  }
}
//------------------------------------------------------------------
Double_t
PSMath::PSLineSearch (Int_t & mode, Double_t hh, Double_t xlimit[],
                      Double_t epsx, Double_t epsf, Double_t x[4], Double_t f[],
                      Double_t chi2, Int_t printlevel)
{      // 1-dim Line-Search, Method from Blobel textbook p. 252
  static Double_t xt, ft;
  Double_t d31, d32, d21;
  Double_t g, H;
  Double_t tau = 0.618034;
  Double_t taufac = tau - tau * tau;
  Double_t close = 0.004;
  Double_t h;
  Double_t temp;
  Int_t ilimit;

  //  cout << "chi2 " << chi2 << endl;

  if (mode == 1) {
    if(printlevel > 2)
      std::cout << "In Linesearch Mode 1!" << std::endl;
    //    PSVprint("  xlimit ",xlimit,2) ;
    //    PSVprint("  x      ",x,4) ;
    // set up for "+x" search direction
    h = fmin (fabs(hh), 0.25 * (xlimit[1] - xlimit[0]));  // limit h to xlimit
    x[1] = 0.;
    f[1] = chi2;
    x[2] = x[1] + h;
    if (x[2] > xlimit[1] - epsx) {
      x[2] = x[1] - h;
    }     // too close to limit: invert direction
    x[3] = 0.;
    f[2] = 0.;
    f[3] = 0.;
    //    cout << "mode " << mode << endl;
    //    coutf(14,4, x[1]) ;   coutf(14,4, x[2]) ;   coutf(14,4, x[3]) ;   cout << endl;
    //    coutf(14,4, f[1]) ;   coutf(14,4, f[2]) ;   coutf(14,4, f[3]) ;   cout << endl;
    mode = 2;
    return x[2];
  }

  else if (mode == 2) {
    if(printlevel > 2)
      std::cout << "In Linesearch Mode 2!" << std::endl;
    f[2] = chi2;
    x[3] = x[2] + x[2] - x[1];      // step in same direction as previous step
    if (x[3] > xlimit[1] - epsx) {
      x[3] = x[1] + x[1] - x[2];
    }   // too close to limit: invert direction
    //    cout << "mode " << mode << endl;
    //    coutf(14,4, x[1]) ;   coutf(14,4, x[2]) ;   coutf(14,4, x[3]) ;   cout << endl;
    //    coutf(14,4, f[1]) ;   coutf(14,4, f[2]) ;   coutf(14,4, f[3]) ;   cout << endl;
    mode = 3;
    return x[3];
  }

  else if (mode == 3) {
    if(printlevel > 2)
      std::cout << "In Linesearch Mode 3!" << std::endl;
    f[3] = chi2;
    // due to limits, the order x1->x2->x3 might be screwed up: repair this
    //    cout << "PSLineSearch in mode 3 before sort  " ;
    //    coutf(14,4, x[1]) ;   coutf(14,4, x[2]) ;   coutf(14,4, x[3]) ;   cout << endl;
    //    cout << "PSLineSearch in mode 3 before sort  " ;
    //    coutf(14,4, f[1]) ;   coutf(14,4, f[2]) ;   coutf(14,4, f[3]) ;   cout << endl;

    // choose f1 to be the worst point
    if (f[1] < f[2]) {   //Switch points 1 and 2
      temp = f[2];
      f[2] = f[1];
      f[1] = temp;
      temp = x[2];
      x[2] = x[1];
      x[1] = temp;
    } // order f1>f2
    if (f[1] < f[3]) {    //Switch points 1 and 3
      temp = f[3];
      f[3] = f[1];
      f[1] = temp;
      temp = x[3];
      x[3] = x[1];
      x[1] = temp;
    } // order f1>f3
    if( (x[1] > x[2] && x[1] < x[3]) || (x[1] < x[2] && x[1] > x[3]) ){    //Local maximum between x[2] and x[3]! Continue in direction of steepest descent.
      if(printlevel > 2)
	std::cout << "Local Maximum!" << std::endl;
      if( (f[1] - f[2]) > (f[1] - f[3]) ){  //Continue in direction of x[2]
	temp = x[3];
	x[3] = x[2] + x[2] - x[1];
	if( (x[3] > xlimit[1] - epsx) || (x[3] < xlimit[0] + epsx)) {      //To close to limit. Other direction.
	  x[3] = temp;
	  x[2] = x[3];
	  f[2] = f[3];
	  x[3] = x[2] + x[2] - x[1];
	  return x[3];
	}
	return x[3];
      }
      else{                                //Continue in direction of x[3]
	temp = x[2];
	Double_t tempf = f[2];
	x[2] = x[3];
	f[2] = f[3];
	x[3] = x[2] + x[2] - x[1];
	if( (x[3] > xlimit[1] - epsx) || (x[3] < xlimit[0] + epsx)) {      //To close to limit. Other direction.
	  x[2] = temp;
	  f[2] = tempf;
	  x[3] = x[2] + x[2] - x[1];
	  return x[3];
	}
	return x[3];
      }
    }
    // sort x1 -> x2 -> x3 in one direction
    else if (fabs(x[1] - x[2]) > fabs(x[1] - x[3])) {  // exchange x2 <-> x3
      temp = f[3];
      f[3] = f[2];
      f[2] = temp;
      temp = x[3];
      x[3] = x[2];
      x[2] = temp;
    }
    //    cout << "PSLineSearch in mode 3 after  sort  " ;
    //    coutf(14,4, x[1]) ;   coutf(14,4, x[2]) ;   coutf(14,4, x[3]) ;   cout << endl;
    //    cout << "PSLineSearch in mode 3 after  sort  " ;
    //    coutf(14,4, f[1]) ;   coutf(14,4, f[2]) ;   coutf(14,4, f[3]) ;   cout << endl;

    // check if minimum found
    if (f[2] < f[3]) {    // minimum found
      xt = 0.5 * (x[3] + x[2]); // simple step, and switch to normal "+x" direction
      if (x[1] > x[2]) {
        temp = f[1];
        f[1] = f[3];
        f[3] = temp;
        temp = x[1];
        x[1] = x[3];
        x[3] = temp;
      }
      mode = 5;
    }
    else {            // no minimum found, check if x3 is close to limit
      xt = x[3] + x[3] - x[2];
      ilimit = -1;                                       // check limits
      if (xt < xlimit[0]) {
        ilimit = 0;
      }
      else if (xt > xlimit[1]) {
        ilimit = 1;
      }
      if (ilimit > -1) {                 // limit xt to xlimit0 <= xt <= xlimit1
        xt = xlimit[ilimit] - taufac * (xlimit[ilimit] - x[3]);
      }
      mode = 4;
      x[3] = xt;
    }
    //    cout << "in mode 3,   next mode " << mode << endl;
    //    coutf(14,4, x[1]) ;   coutf(14,4, x[2]) ;   coutf(14,4, x[3]) ;   cout << endl;
    //    coutf(14,4, f[1]) ;   coutf(14,4, f[2]) ;   coutf(14,4, f[3]) ;   cout << endl;
    return xt;
  }

  else if (mode == 4) {             //  find intervall around minimum
    if(printlevel > 2)
	std::cout << "Mode 4!" << std::endl;
    f[3] = chi2;
    if (f[3] <= f[2]) { //  no minimum found:  shift x1 <- x2 <- x3 and get next x3
      f[1] = f[2];
      f[2] = f[3];
      x[1] = x[2];
      x[2] = x[3];
      x[3] = x[2] + (1 + tau) * (x[2] - x[1]);
      f[3] = 0;    // Goldener Schnitt
      // check limits
      ilimit = -1;                                       // check limits
      if (x[3] < xlimit[0]) {
        ilimit = 0;
      }
      else if (x[3] > xlimit[1]) {
        ilimit = 1;
      }
      if (ilimit > -1) {                 // limit x3 to xlimit0 <= x3 <= xlimit1
        //	cout << "search intervall at ilimit " << ilimit << endl;
        if (fabs(x[2] - xlimit[ilimit]) < epsx) { // Minimum found at limit : finish !
	  if(printlevel > 2)
	    std::cout << "Minimum found near limit!" << std::endl;
          //	  cout << "Minimum found near limit "
          //	       << x[2] << " ~ " << xlimit[ilimit] <<  " ~ " << x[3] << endl;
          f[1] = f[2];
          f[2] = f[3];
          x[1] = x[2];
          x[2] = xlimit[ilimit];
          x[3] = xlimit[ilimit];
          mode = -1;
	  return xlimit[ilimit];
        }
        x[3] = xlimit[ilimit] - taufac * (xlimit[ilimit] - x[2]);
	if(printlevel > 2)
	    std::cout << "Searching close to limit!" << std::endl;
        return x[3];
      }
      return x[3];
    }                // case when not at limit and no minimum found
    else {                           // search intervall found
      //      cout << "search intervall found " << endl;
      mode = 5;
      xt = 0.5 * (x[3] + x[2]);  // ?  step not needed but easier to code...
      // switch to normal "+x" direction
      if (x[1] > x[2]) {
        temp = f[1];
        f[1] = f[3];
        f[3] = temp;
        temp = x[1];
        x[1] = x[3];
        x[3] = temp;
      }
    }
    return xt;
  }

  else if (mode == 5) { // ------------ find minimum within search range --------------
    if(printlevel > 2)
	std::cout << "Mode 5!" << std::endl;
    ft = chi2;
    Double_t deltaChi2 = fabs(ft - f[2]);
    //    cout <<" in mode 5    xt  " << xt << "   ft " << ft << endl;
    //    PSVprint("  x ",x,4) ;
    
    if (ft < f[2]) {                     // better point found: narrow intervall
      if (xt < x[2]) {
        f[3] = f[2];
        x[3] = x[2];
        f[2] = ft;
        x[2] = xt;
      }
      else {
        f[1] = f[2];
        x[1] = x[2];
        f[2] = ft;
        x[2] = xt;
      }
    }
    else {                           // worse point
      if (xt < x[2]) {
        f[1] = ft;
        x[1] = xt;
      }
      else {
        f[3] = ft;
        x[3] = xt;
      }
    }

    if(printlevel > 2){
      std::cout << "x[1]: " << x[1] <<  "  f[1]: " << f[1] << std::endl;
      std::cout << "x[2]: " << x[2] <<  "  f[2]: " << f[2] << std::endl;
      std::cout << "x[3]: " << x[3] <<  "  f[3]: " << f[3] << std::endl;
    }
    if( (fabs(x[1] - x[3]) < epsx && deltaChi2 < epsf) || fabs(x[1] - x[3]) < 0.00001) {// CONVERGENCE reached   Requirement of small deltaChi2 difficult in very steep regions
      mode = 0;
      return x[2];
    }
    
    //    if (x[3]-x[1]<epsx && f[1]-f[2]<epsf && f[3]-f[2]<epsf ) {    // CONVERGENCE reached
    d21 = x[2] - x[1];
    d31 = x[3] - x[1];
    d32 = x[3] - x[2];
    g = 1. / d31 * ((f[3] - f[2]) * d21 / d32 + (f[2] - f[1]) * d32 / d21); // derivative
    H = 2. / d31 * ((f[3] - f[2]) / d32 - (f[2] - f[1]) / d21);       // Hesse
    xt = x[2] - (g / H) * 1.02;                               // Newton Method
    //    PSVprint("  x ",x,4) ;
    //    cout << "   d21 " << d21 << "   d31 " << d31 << "   d32 " << d32 << "  xt " << xt << endl;

    if (fabs(x[2] - xt) < close * d31) {       // safety for numerical precision
    //      cout << "close" << xt << endl;
      if (d21 < d32) {
        xt = x[2] + taufac * d31;
      }                // and numerical precision
      else {
        xt = x[2] - taufac * d31;
      }
    }
    return xt;
  }
  return -1; //default return should never be reached
}

//------------------------------------
void
PSMath::PSLineLimit (Int_t np, Double_t astart[], Double_t daN[],
                     Double_t alimit[][2], Double_t xlimit[])
{
  // calculate xlimit for line search along direction daN
  Double_t temp0, temp1, temp;
  xlimit[0] = -pow (10.0, 10.0);
  xlimit[1] = pow (10.0, 10.0);

  //  cout << "    xlimit[0] "  << xlimit[0] << "    xlimit[1] " << xlimit[1] << endl; 
  //  PSVprint( "PSLineLimit    astart ",astart,np) ;
  //  PSM2print("PSLineLimit    alimit ",alimit,np) ;
  for (Int_t ip = 0; ip < np; ip++) {                // limits
    if (daN[ip] != 0.) {
      temp0 = (alimit[ip][0] - astart[ip]) / daN[ip];
      temp1 = (alimit[ip][1] - astart[ip]) / daN[ip];
      //      cout << "temp0 " << temp0 << "   temp1 " << temp1 << endl;
      if (temp0 > temp1) {
        temp = temp0;
        temp0 = temp1;
        temp1 = temp;
      }
      //      cout << "temp0 " << temp0 << "   temp1 " << temp1 << endl;
      xlimit[0] = fmax (xlimit[0], temp0);
      xlimit[1] = fmin (xlimit[1], temp1);
    }
  }
  //  cout << "    xlimit[0] " << xlimit[0] << "    xlimit[1] " << xlimit[1] << endl; 
}
//---------------------------------
Double_t
PSMath::PSVnorm (Double_t x[], Int_t n)
{
  Double_t xnorm = 0.;
  for (int i = 0; i < n; i++) {
    xnorm = xnorm + pow (x[i], 2.0);
  }
  return sqrt (xnorm);
}
//----------------------------------------------------------------
void
PSMath::PSVprint (const char* text, Double_t x[], Int_t n)
{
  //  cout << "   PS Vector print  " << "  " << n << "\n" ;
  std::cout << text << "[" << n << "] = ";
  for (int i = 0; i < n; i++) {
    std::cout << "  " << x[i];
  };
  //  cout << "x[0]" << x[0] << "\n" ;
  std::cout << "\n";
}
//----------------------------------------------------------------
void
PSMath::PSMprint (const char* text, Double_t A[], Int_t ni, Int_t nj)
{
  std::cout << text << "[" << ni << "]" << "[" << nj << "]" << "\n";
  for (int i = 0; i < ni; i++) {
    std::cout << "      " << i << "    ";
    for (int j = 0; j < nj; j++) {
      std::cout << "  " << A[i * nj + j];
    };
    std::cout << "\n";
  }
}
//----------------------------------------------------------------
void
PSMath::PSM2print (const char* text, Double_t A[][2], Int_t ni)
{
  for (int i = 0; i < ni; i++) {
    std::cout << text << "[" << i << "]" << "[]";
    for (int j = 0; j < 2; j++) {
      printf ("%*.*f ", 12, 4, A[i][j]);
    };
    std::cout << "\n";
  }
}
//------------------------------------------------------------------
Double_t
PSMath::PSMinverse (Double_t H[], Double_t Hinv[], Int_t p)
{    // invert a symmetric matrix H[p,p]
  // for 2*2 and 3*3 inversion also works for a non-symmetric matrix
  Double_t temp;
  if (p == 2) {
    Double_t det = H[0] * H[3] - H[1] * H[2];
    Hinv[0] = H[3] / det;
    Hinv[3] = H[0] / det;
    Hinv[1] = -H[1] / det;
    Hinv[2] = -H[2] / det;
  }
  else if (p == 3) {
    //    cout <<" Minverse p=3 " << "\n" ;
    Hinv[0 * p + 0] = H[1 * p + 1] * H[2 * p + 2] - H[1 * p + 2] * H[2 * p + 1];
    Hinv[0 * p + 1] = H[1 * p + 2] * H[2 * p + 0] - H[1 * p + 0] * H[2 * p + 2];
    Hinv[0 * p + 2] = H[1 * p + 0] * H[2 * p + 1] - H[1 * p + 1] * H[2 * p + 0];
    Hinv[1 * p + 0] = H[2 * p + 1] * H[0 * p + 2] - H[2 * p + 2] * H[0 * p + 1];
    Hinv[1 * p + 1] = H[2 * p + 2] * H[0 * p + 0] - H[2 * p + 0] * H[0 * p + 2];
    Hinv[1 * p + 2] = H[2 * p + 0] * H[0 * p + 1] - H[2 * p + 1] * H[0 * p + 0];
    Hinv[2 * p + 0] = H[0 * p + 1] * H[1 * p + 2] - H[0 * p + 2] * H[1 * p + 1];
    Hinv[2 * p + 1] = H[0 * p + 2] * H[1 * p + 0] - H[0 * p + 0] * H[1 * p + 2];
    Hinv[2 * p + 2] = H[0 * p + 0] * H[1 * p + 1] - H[0 * p + 1] * H[1 * p + 0];
    Double_t det = H[0 * p + 0] * Hinv[0 * p + 0]
        + H[1 * p + 0] * Hinv[0 * p + 1] + H[2 * p + 0] * Hinv[0 * p + 2];
    ;
    for (int ii = 0; ii < p; ii++) {
      for (int jj = 0; jj < p; jj++) {
        Hinv[ii * p + jj] = Hinv[ii * p + jj] / det;
      }
    }
  }
  else {
    temp = PSMCholesky (H, Hinv, p);
    temp = PSMRTrianInvert2 (Hinv, p);
    temp = PSMmultiplyMRRT (Hinv, p, p);
  };
  return 0.;
}
//-----------------------------
Double_t
PSMath::PSMCholtest ()
{ // test inversion of symmetric matrix via cholesky LR decomposition
  Double_t M[4 * 4], R[4 * 4], Rinv[4 * 4], Mtest[4 * 4];
  // test for  R- right triangular matrix
  Double_t Test1[1 * 1] = { 9. };
  Double_t Test2[2 * 2] = { 9., 3., 0., 5. };
  Double_t Test3[3 * 3] = { 9., 3., 2., 0., 5., -4., 0., 0., -9. };
  Double_t Test4[4 * 4] = { 9., 3., 2., 0., 0., 5., -4., 1., 0., 0., 60., -9.,
      0., 0., 0., 40. };

  Int_t n = 4;
  Double_t temp;
  temp = PSMmultiplyMT (M, Test4, n, n);
  //  for (int ii=0; ii < n ; ii++) {
  //    for (int jj=0; jj < n ; jj++) {
  //      M[ii*n+jj] = 0. ; 
  //      for (int kk=0; kk < n ; kk++) {
  //	M[ii*n+jj] = M[ii*n+jj] + Test[kk*n+ii] * Test[kk*n+jj] ;
  //      } ;
  //    } ;
  //  } ;
  PSMprint ("Test input R", Test4, n, n);
  PSMprint ("Test   M", M, n, n);

  temp = PSMCholesky (M, R, n);
  PSMprint ("Test   R", R, n, n);
  temp = PSMRTrianInvert2 (R, n);

  temp = PSMmultiplyMT (Mtest, R, n, n);
  PSMprint ("Test Inverse 1  ", Mtest, n, n);
  temp = PSMmultiply (R, M, Mtest, n, n);
  PSMprint ("Test should be 1", R, n, n);

  std::cout << "self copy version   \n";
  temp = PSMCholesky (M, R, n);
  temp = PSMRTrianInvert2 (R, n);
  temp = PSMmultiplyMRRT (R, n, n);
  PSMprint ("Test   inverse R", R, n, n);
  temp = PSMmultiply (Mtest, M, R, n, n);
  PSMprint ("Test should be 1", Mtest, n, n);

  temp = PSMinverse (M, Rinv, n);
  PSMprint ("Test   inverse Rinv", Rinv, n, n);
  return 0.;
}
//-----------------------------
Double_t
PSMath::PSMmultiply (Double_t A[], Double_t B[], Double_t C[], Int_t n1,
                     Int_t n2)
{ // multiply matrix  A = B * C,  Aij = Bik * Ckj,
//  A(n1,n1)   B(n1,n2)  C(n2,n1)
  for (int ii = 0; ii < n1; ii++) {
    for (int jj = 0; jj < n1; jj++) {
      A[ii * n1 + jj] = 0.;
      for (int kk = 0; kk < n2; kk++) {
        A[ii * n1 + jj] = A[ii * n1 + jj] + B[ii * n1 + kk] * C[kk * n2 + jj];
      }
    }
  }
  return 0.;
}
//-----------------------------
Double_t
PSMath::PSMmultiplyMRRT (Double_t A[], Int_t n1, Int_t n2)
{  // multiply triangular matrix R=A  with transpose,
//  store result in input, B = R * R^T,  A = B,
//  Aij= Bik * Bjk,  A(n1,n1)   B(n1,n2)
  for (int ii = 0; ii < n1; ii++) {
    for (int jj = ii + 1; jj < n1; jj++) {
      for (int kk = jj; kk < n2; kk++) {
        A[jj * n1 + ii] = A[jj * n1 + ii] + A[ii * n1 + kk] * A[jj * n2 + kk];
      }
    }
    A[ii * n1 + ii] = pow (A[ii * n1 + ii], 2.0);
    for (int ll = ii + 1; ll < n2; ll++) {
      A[ii * n1 + ii] = A[ii * n1 + ii] + pow (A[ii * n1 + ll], 2);
    }
    for (int jj = ii + 1; jj < n1; jj++) {
      A[ii * n1 + jj] = A[jj * n1 + ii];
    }
  }
  return 0;
}
//-----------------------------
Double_t
PSMath::PSMmultiplyMT (Double_t A[], Double_t B[], Int_t n1, Int_t n2)
{  // multiply matrix  A = B * B^T,  Aij = Bik * Bjk,  A(n1,n1)   B(n1,n2)
  for (int ii = 0; ii < n1; ii++) {
    for (int jj = 0; jj < n1; jj++) {
      A[ii * n1 + jj] = 0.;
      for (int kk = 0; kk < n2; kk++) {
        A[ii * n1 + jj] = A[ii * n1 + jj] + B[ii * n1 + kk] * B[jj * n2 + kk];
      }
    }
  }
  return 0.;
}
//-----------------------------
Double_t
PSMath::PSMRTrianInvert2 (Double_t R[], Int_t n)
{  // invert a triangular R - matrix (n*n)
//  store result in position of input (R)
  for (int ii = 0; ii < n; ii++) {
    for (int jj = ii + 1; jj < n; jj++) {
      R[jj * n + ii] = R[ii * n + jj];
      R[ii * n + jj] = 0.;
    }
    for (int jj = ii + 1; jj < n; jj++) {
      R[ii * n + jj] = R[ii * n + jj] - 1. / R[ii * n + ii] * R[jj * n + ii];
      for (int kk = ii + 1; kk < jj; kk++) {
        R[ii * n + jj] = R[ii * n + jj] - R[ii * n + kk] * R[kk * n + jj];
      }
      R[ii * n + jj] = R[ii * n + jj] / R[jj * n + jj];
    }
    R[ii * n + ii] = 1. / R[ii * n + ii];
    for (int jj = ii + 1; jj < n; jj++) {
      R[jj * n + ii] = 0.;
    }
  }
  return 0.;
}
//-----------------------------
Double_t
PSMath::PSMRTrianInvert (Double_t R[], Double_t Rinv[], Int_t n)
{  // invert a triangular R - matrix (n*n)       R * Rinv = 1
  for (int ii = 0; ii < n; ii++) {
    for (int jj = 0; jj < n; jj++) {
      Rinv[ii * n + jj] = 0.;
    };
    Rinv[ii * n + ii] = 1. / R[ii * n + ii];
    for (int jj = ii + 1; jj < n; jj++) {
      for (int kk = ii; kk < jj; kk++) {
        Rinv[ii * n + jj] = Rinv[ii * n + jj]
            - Rinv[ii * n + kk] * R[kk * n + jj];
        std::cout << "i,j,k,R = " << ii << "  " << jj << "  " << kk << "   "
            << Rinv[ii * n + jj] << "\n";
      }
      Rinv[ii * n + jj] = Rinv[ii * n + jj] / R[jj * n + jj];
    }
  }
  //	cout << "i,j,R = " << ii << "  " << jj << "  " <<"   " << Rinv[ii*n+jj] <<"\n" ;
  //      cout << "i,j,R = " << ii << "  " << jj << "  " <<"   " << Rinv[ii*n+jj] <<"\n" ;
  //      cout << "i,j,R = " << ii << "  " << jj << "  " <<"   " << Rinv[ii*n+jj] <<"\n" ;
  return 0.;
}

//-----------------------------
Double_t
PSMath::PSMCholesky (Double_t M[], Double_t R[], Int_t n)
{  // Cholesky decomposition of  M = n*n  symmetric matrix
// M = R^T * R
  for (int ii = 0; ii < n; ii++) {
    R[ii * n + ii] = M[ii * n + ii];
    for (int ll = 0; ll < ii; ll++) {
      R[ii * n + ii] = R[ii * n + ii] - pow (R[ll * n + ii], 2.0);
    };
    if (R[ii * n + ii] <= 0.) {
      return -1.;
    }
    R[ii * n + ii] = sqrt (R[ii * n + ii]);
    //
    for (int jj = ii + 1; jj < n; jj++) {
      R[ii * n + jj] = 0.;
      //      cout << "i,j = " << ii << "  " << jj << "  " << "\n" ;
      for (int kk = 0; kk < ii; kk++) {
        //	cout << "i,j,k = " << ii << "  " << jj << "  " << kk << "\n" ;
        R[ii * n + jj] = R[ii * n + jj] + R[kk * n + ii] * R[kk * n + jj];
        //	cout << "i,j,k = " << ii << "  " << jj << "  " << kk << "  " << R[ii*n+jj] << "\n" ;
      };
      R[ii * n + jj] = 1. / R[ii * n + ii] * (M[ii * n + jj] - R[ii * n + jj]);
    }
    for (int jj = 0; jj < ii; jj++) {
      R[ii * n + jj] = 0.;
    }
  }
  return 0.;
}
//----------------------------------------------------------------
Double_t
PSMath::PSfitCheckLimits (Int_t np, Double_t a[], Double_t h[],
                          Double_t alimit[][2], Double_t aprecision[],
                          Double_t daN[], Double_t g[], Double_t d)
{
  // check limits of fit parameters
  std::cout << "PSfitCheckLimits " << std::endl;

  Double_t dnew;
  Double_t z, z0, z1;
  z0 = 0.;
  z1 = 0.;
  for (Int_t ip = 0; ip < np; ip++) {
    if (a[ip] < alimit[ip][0] + aprecision[ip]) {
      z = (a[ip] - (alimit[ip][0] + aprecision[ip])) / daN[ip];
      if (z0 < z) {
        z0 = z;
        h[ip] = aprecision[ip];
      }
      std::cout << "PSfitCheckLimits: input:  ip " << ip << " a " << a[ip]
          << "  h " << aprecision[ip] << "  Limits:" << alimit[ip][0] << "  "
          << alimit[ip][1] << std::endl;
      std::cout << "PSfitCheckLimits ip " << ip << "  z " << z << "  z0 " << z0
          << std::endl;
    }
    if (a[ip] > alimit[ip][1] - aprecision[ip]) {
      z = (a[ip] - (alimit[ip][1] - aprecision[ip])) / daN[ip];
      if (z1 < z) {
        z1 = z;
        h[ip] = aprecision[ip];
      }
      std::cout << "PSfitCheckLimits: input:  ip " << ip << " a " << a[ip]
          << "  h " << aprecision[ip] << "  Limits:" << alimit[ip][0] << "  "
          << alimit[ip][1] << std::endl;
      std::cout << "PSfitCheckLimits ip " << ip << "  z " << z << "  z1 " << z1
          << std::endl;
    }
  }
  if (z0 > 0. || z1 > 0.) {
    std::cout << "====================WARNING: out of range: " << "  z0 " << z0
        << "  z1 " << z1 << std::endl;
    for (Int_t ip = 0; ip < np; ip++) {
      std::cout << "PSfitCheckLimits: input:  ip " << ip << " a " << a[ip]
          << "  h " << aprecision[ip] << "  Limits:" << alimit[ip][0] << "  "
          << alimit[ip][1] << std::endl;
    }
    if (z0 > z1) {
      z = z0;
    }
    else {
      z = z1;
    }
    for (Int_t ip = 0; ip < np; ip++) {
      if (d < 100) {
        dnew = d + g[ip] * daN[ip];
      }
      else {
        dnew = d;
      }
      a[ip] = a[ip] - z * daN[ip];
      daN[ip] = (1. - z) * daN[ip];

      std::cout << "New: ip " << ip << "  d " << d << "  daN " << daN[ip]
          << "  a " << a[ip] << "  h " << h[ip] << std::endl;
    }
    if (fabs(z - 1.) < 0.0001) {
      return 0.;
    }     //    no change of parmeters: end iterations
    else {
      return 99.;
    }                        //    try again
  }
  else {
    dnew = d;
  }
  return dnew;
}
//----------------------------------------------------------------
Double_t
PSMath::PSminIterate (Double_t a[], Double_t daN[], Double_t h[], Int_t p,
                      Double_t g[], Double_t H[], Double_t Hinv[], Double_t X0)
{
  //  cout << "   PSminIterate  X0 = " << X0 << "\n" ;

  Double_t d;                   // convergence test value
  Int_t iNewton;                 // =1 if Newton Method can be used

  for (Int_t ii = 0; ii < p; ii++) {
    daN[ii] = 0.;
  }

  if (H[0] * H[3] <= H[1] * H[2]) {    // check only works for 2 parameters
    std::cout
        << "PSminIterate ===== WARNING ==================== H ist not positiv definit"
        << std::endl;
  }

  iNewton = 1;
  for (Int_t ii = 0; ii < p; ii++) {     // check for positive Diagonal of Hesse
    if (H[ii * p + ii] < 0.) {
      iNewton = 0;
      break;
    }
    if (H[ii * p + ii] * h[ii] < 0.1 * fabs(g[ii])) {
      iNewton = 0;
      break;
    }
  }
  if (iNewton == 1) {               // update parameter value with Newton method
    for (Int_t ii = 0; ii < p; ii++) {
      for (Int_t jj = 0; jj < p; jj++) {
        daN[ii] = daN[ii] - Hinv[ii * p + jj] * g[jj]; // Newton Step width
      }
    }
    d = 0.;
    for (Int_t ii = 0; ii < p; ii++) {
      d = d - g[ii] * daN[ii];
    }       // convergence test value

    if (d > 0.) {                // update parameter value in case of positive d
      for (Int_t ii = 0; ii < p; ii++) {
        a[ii] = a[ii] + daN[ii];                 // update parameter value
        h[ii] = 100. * sqrt (0.00001 * fabs(X0 / H[ii * p + ii])); // update step width
      }
    }
    else {                              // d < 0   Hesse not positive definit ?!
      for (Int_t ii = 0; ii < p; ii++) {
        daN[ii] = -h[ii] * g[ii] / fabs(g[ii]); // simple step in direction of ngative slope
        a[ii] = a[ii] + daN[ii];                 // update parameter value
        //    h[ii] = h[ii]  ;                         // reduce step width
        d = 110;
      }
    }
  }
  else {                                         // Hesse not positiv definit
    for (Int_t ii = 0; ii < p; ii++) {
      daN[ii] = -h[ii] * g[ii] / fabs(g[ii]); // simple step in direction of ngative slope
      a[ii] = a[ii] + daN[ii];                     // update parameter value
      //      h[ii] = h[ii]  ;                           // update step width
      d = 110.;
    }
  }
  return d;
}
//----------------------------------------------------------------
Double_t
PSMath::PSfuncQuadratic (Double_t a[], Double_t amean[], Double_t F0,
                         Double_t g[], Double_t H[], Int_t np)
{   // compute multi-dim quadratic function from mean, derivative and Hesse
  Double_t F = F0;
  Double_t temp;
  //  cout << F << "\n" ;
  //  PSVprint(" a",a,np) ;
  //  PSVprint(" amean",amean,np) ;
  //  PSVprint(" g",g,np) ;
  for (int ii = 0; ii < np; ii++) {
    F = F + g[ii] * (a[ii] - amean[ii]);
    //    cout << ii << "   " << F << "\n" ;
    for (int jj = 0; jj < np; jj++) {
      F = F + 0.5 * (a[ii] - amean[ii]) * H[ii * np + jj] * (a[jj] - amean[jj]);
      //      cout << ii << "   " << jj << "  " << F << "\n" ;
    };
  };
  return F;
}
//----------------------------------------------------------------
Int_t PSMath::PSderivative1 (Int_t icall, Double_t a[], Double_t h[], 
		      Double_t F, Double_t g[], Double_t H[])
{
  // 1-dim case of PSderivative,  see documentation there
  // g,H will contain derivative and second derivative of F(a)
  // ready = 1 if derivatives are ready

  Int_t nstep = 3;
  Int_t iter  ;                       // number of finished iterations
  Int_t icalc ;                       // current number of function calculations 
                                      //     within current iteration
  Int_t ready = -1 ;
  iter  = icall / nstep ;
  icalc = icall - iter * nstep ;
  if (icalc == 0)  { 
    a[0] = a[0] + h[0];
  } else if (icalc == 1) {
    a[0] = a[0] - 2.* h[0] ;
  } else if (icalc == 2) {
    a[0] = a[0] + h[0] ;
  } ;

  if (icalc == 0 ) {
    g[0] = 0.     ;        // build up derivative
    H[0] = -2.*F  ;        // build up Hesse 
  } else if (icalc == 1) {
    g[0] = g[0] + F  ;        // build up derivative
    H[0] = H[0] + F  ;        // build up Hesse 
  } else if (icalc == 2) {
    g[0] = g[0] - F  ;        // build up derivative
    H[0] = H[0] + F  ;        // build up Hesse 
    g[0] = g[0] / (2.*h[0]) ;    // at last step: divide by step width
    H[0] = H[0] / pow(h[0],2) ;      // at last step: divide by step width
    ready = 1 ;
  }
  return ready ;
}
//----------------------------------------------------------------
Int_t
PSMath::PSderivative (Int_t icall, Int_t np, Double_t a[], Double_t h[],
                      Double_t chi2, Double_t chi2iter[], Double_t g[],
                      Double_t H[])
{
  // Tool for numerical calculation of derivative and Hesse matrix
  //  depending on icall, the components of a[] are shifted up and down.
  //  In the following call the corresponding function (chi2) value is stored
  //  and the next shift to a[] is applied.
  //  After all shifts are done the derivative and Hesse matrix are calculated
  //  and the step size is recalculated (reduced).

  //  Int_t icall  ;                 //     current call number
  //  Int_t np  ;                    //     number of fit parameters
  //  Double_t a[np], Double_t h[np] ; //  fit parameters and step width 
  //                                 //  (will be updated during the fit)
  //  Double_t chi2                   //  Function (chi2) value for current a[]
  //  Double_t chi2iter               //  Function (chi2) with nominal a[]
  //                                 //     (i.e. no shift of a[])
  //  Double_t g[4] ;                 //  derivative vector  g[np]
  //  Double_t H[4*4] ;               //  Hesse matrix       H[np*np]
  //                                 //  g[] and H[] are also used as 
  //                                 //    intermediate storage of chi2
  //     icall = 0             for nominal a[]
  //     icall = 1...np        for + shift of a
  //     icall = np+1...2np    for - shift of a
  //     icall = 2np+1 ... (np*np+3np+2)/2        for ++ shift of a[0]
  //     icall = (np*np+3np+2)/2 ... np*np+np+1   for -- shift of a[0]

  //  Double_t chi2,  d ;             // chi2 value and stop criteria for fit
  if(np == 1)
    std::cout << "WARNING! Using PSderivative for 1-dim Case! Use PSderivative1 instead!" << std::endl;

  Int_t ready = -1;
  Int_t nstep = 1 + 2 * np + np * (np - 1);
  Int_t iter;                       // number of finished iteration
  Int_t icalc;                       // current number of chi2 calculations
  //     within current iteration
  Int_t ia;                          // index of fit parameter to be changed
  Int_t iai, iaj;                    // same for Hesse_ij

  Int_t shiftnom = 0;        //     icall = 0             for nominal a[] //0
  Int_t shiftp1 = 1;        //     etc see above                          //1
  Int_t shiftp2 = shiftp1 + np - 1;                                       //1
  Int_t shiftm1 = shiftp2 + 1;                                            //2
  Int_t shiftm2 = shiftm1 + np - 1;                                       //2
  Int_t shiftpp1 = shiftm2 + 1;                                           //3
  Int_t shiftpp2 = shiftm2 + np * (np - 1) / 2;                           //3
  Int_t shiftmm1 = shiftpp2 + 1;                                          //4
  Int_t shiftmm2 = shiftpp2 + np * (np - 1) / 2;                          //3

  iter = icall / nstep;
  icalc = icall - iter * nstep;
  
  Int_t iaold = -1, ia1old = -1, iaiold = -1, iajold = -1;
  Int_t ianew = -1, ia1new = -1, iainew = -1, iajnew = -1;
  Double_t signold = 0., signnew = 0.;

  if (icalc == shiftnom) { //cout <<" Nominal "<< chi2 << "\n" ;
    iaold = -1;
    ianew = 0;
    signold = 0.;
    signnew = +1.;
    //  if (icalc == shiftp1) { //cout<<"Derivative + start  "<< chi2 << "\n" ;
    //   iaold = -1   ;    ianew = 0 ;      ;  signold = 0.  ;  signnew = +1. ;
  }
  else if (icalc < shiftp2) {  //cout <<" Derivative + "<< chi2 << "\n" ;
    iaold = icalc - shiftp1;
    ianew = iaold + 1;
    signold = -1.;
    signnew = +1.;
  }
  else if (icalc == shiftp2) {  // cout <<" Derivative + end "<< chi2 << "\n" ;
    iaold = np - 1;
    ianew = 0;
    signold = -1.;
    signnew = -1.;
  }
  else if (icalc < shiftm2) {  // cout <<" Derivative - "<< chi2 << "\n" ;
    iaold = icalc - shiftm1;
    ianew = iaold + 1;
    signold = +1.;
    signnew = -1.;

  }
  else if (icalc == shiftm2) {  //cout<<"Derivative -end "<<chi2<<"\n";//Hesse
    iaold = np - 1;
    signold = +1.;
    iainew = 0;
    iajnew = 1;
    signnew = +1.;
  }
  else if (icalc < shiftpp2) {  // cout <<" Hesse ++ "<< chi2 << "\n" ;
    ia = icalc - shiftm2 - 1;
    Int_t sum = 0;
    Int_t test = -1;
    for (iai = 0; test < 0; iai++) {
      sum = sum + np - 1 - iai;
      if (ia < sum) {
        iai = iai - 1;
        iaj = ia - sum + np;
        test = 1;
      };
    };
    //    cout << " ready ia " <<ia<< "   iai "<<iai<< "   iaj "<<iaj<<  "\n" ;
    iaiold = iai;
    iajold = iaj;
    signold = -1.;
    if (iaj < np - 1) {
      iainew = iai;
      iajnew = iaj + 1;
      signnew = +1.;
    }
    else {
      iainew = iai + 1;
      iajnew = iai + 2;
      signnew = +1.;
    };

  }
  else if (icalc == shiftpp2) {  // cout <<" Hesse ++ end "<< chi2 << "\n" ;
    iaiold = np - 2;
    iajold = np - 1;
    signold = -1.;
    iainew = 0;
    iajnew = 1;
    signnew = -1.;

  }
  else if (icalc < shiftmm2) {  // cout <<" Hesse -- "<< chi2 << "\n" ;
    ia = icalc - shiftpp2 - 1;
    Int_t sum = 0;
    Int_t test = -1;
    for (iai = 0; test < 0; iai++) {
      sum = sum + np - 1 - iai;
      if (ia < sum) {
        iai = iai - 1;
        iaj = ia - sum + np;
        test = 1;
      };
    };
    //    cout << " ready ia " <<ia<< "   iai "<<iai<< "   iaj "<<iaj<<  "\n" ;
    iaiold = iai;
    iajold = iaj;
    signold = +1.;
    if (iaj < np - 1) {
      iainew = iai;
      iajnew = iaj + 1;
      signnew = -1.;
    }
    else {
      iainew = iai + 1;
      iajnew = iai + 2;
      signnew = -1.;
    };
  }
  else if (icalc == shiftmm2) {  // cout <<" Hesse -- end   "<< chi2 << "\n" ;
    iaiold = np - 2;
    iajold = np - 1;
    signold = +1.;
  }
  else {
    std::cout << "ERROR \n";
  };

  if (iaold >= 0) {
    a[iaold] = a[iaold] + signold * h[iaold];
  };
  if (ianew >= 0) {
    a[ianew] = a[ianew] + signnew * h[ianew];
  };
  if (iaiold >= 0) {
    a[iaiold] = a[iaiold] + signold * h[iaiold];
  };
  if (iainew >= 0) {
    a[iainew] = a[iainew] + signnew * h[iainew];
  };
  if (iajold >= 0) {
    a[iajold] = a[iajold] + signold * h[iajold];
  };
  if (iajnew >= 0) {
    a[iajnew] = a[iajnew] + signnew * h[iajnew];
  };

  if (icalc == shiftnom) {
    chi2iter[0] = chi2;
    for (int ii = 0; ii < np; ii++) {
      g[ii] = 0.;                             // build up derivative
      for (int jj = 0; jj < np; jj++) {
        H[ii * np + jj] = -2. * chi2;      // build up Hesse
      };
    };
  };

  if (iaold >= 0) {
    g[iaold] = g[iaold] - signold * chi2;  // build up derivative
    H[iaold * np + iaold] = H[iaold * np + iaold] + chi2; // build up Hesse diagonal
  }
  if (iaiold >= 0) {
    H[iaiold * np + iajold] = H[iaiold * np + iajold] + chi2; //Hesse (non-diagonal)
  }
  if (icalc == shiftmm2) {
    for (int ii = 0; ii < np; ii++) {
      g[ii] = g[ii] / (2. * h[ii]);       // final derivative
      H[ii * np + ii] = H[ii * np + ii] / (pow (h[ii], 2.0)); // final Hesse diagonal
    };
    for (int ii = 0; ii < np - 1; ii++) {                 // Hesse non-diagonal
      for (int jj = ii + 1; jj < np; jj++) {
        H[ii * np + jj] = H[ii * np + jj] - pow (h[ii], 2.0) * H[ii * np + ii]
            - pow (h[jj], 2.0) * H[jj * np + jj];
      };
    };
    for (int ii = 0; ii < np - 1; ii++) {             //final Hesse non-diagonal
      for (int jj = ii + 1; jj < np; jj++) {
        H[ii * np + jj] = H[ii * np + jj] / (2. * h[ii] * h[jj]);
        H[jj * np + ii] = H[ii * np + jj];
      };
    };
    ready = 1;
  };
  return ready;
}
//----------------------------------------------------------------
Double_t
PSMath::PSfitMinStep (Int_t np, Double_t a[], Double_t h[], Double_t chi2iter[],
                      Double_t g[], Double_t H[], Double_t Hinv[],
                      Double_t daN[])
{
  // calcuate Newton Step for minimisation

  Double_t d = 99, temp;
  PSVprint (" Parameters a", a, np);
  PSVprint (" StepSize   h", h, np);
  PSVprint (" Derivative g", g, np);
  PSMprint (" Hesse H", H, np, np);
  std::cout << "now calculate dN ! \n";
  temp = PSMinverse (H, Hinv, np);
  PSMprint (" Hesse Hinv", Hinv, np, np);
  d = PSminIterate (a, daN, h, np, g, H, Hinv, chi2iter[0]);
  std::cout << "chi2iter = " << chi2iter[0] << "  d = " << d << "\n";
  PSVprint (" New daN", daN, np);
  PSVprint (" New   a", a, np);
  PSVprint (" New   h", h, np);
  return d;
}
//----------------------------------------------------------------
Int_t
PSMath::PSfitconstrain0 (Double_t F, Double_t g, Double_t H, Double_t Fix,
                         Double_t aix[])
{
  //  constrain function F(a) with derivative g(a) and 2nd derivative H(a)
  //  The result assumes a quadratic function 
  //  to Fix = F + g * a + 1/2 * H * a^2
  //  result: The (in general) two solutions are turned in aix[0] and aix[1]
  //  return = number of solutions (0,1,2)
  Int_t nsolutions = 0;
  Double_t alin, amin;
  aix[0] = 0.;
  aix[1] = 0.;
  if (g == 0) {
    // cout << "g=0 \n";
    if (H == 0) {
      //cout << "H=0 \n";
      nsolutions = 0;
    }
    else {
      amin = 2. * (Fix - F) / H;
      if (amin < 0.) {
        nsolutions = 0;
      }
      else {
        aix[0] = sqrt (amin);
        aix[1] = -aix[0];
        nsolutions = 2;
      }
    }
  }
  else {
    alin = (Fix - F) / g;
    //    cout << "alin = " << alin << "\n";
    if (H == 0) {
      aix[0] = alin;
      aix[1] = alin;
      //      cout << "Solutions = " << aix[0] << "    " << aix[1] << "\n" ;
      nsolutions = 1;
      return nsolutions;
    };
    //    cout << "amin = " << amin << "\n";
    amin = -g / H;
    Double_t temp = 1. - 2 * alin / amin;
    //    cout << "temp = " << temp << "\n";
    if (temp < 0.) {
      nsolutions = 0;
      return nsolutions;
    };
    //    cout << "amin = " << amin << "    alin = " << alin << "\n" ;
    if (alin > 0.01 * amin) {
      aix[0] = amin * (1. - sqrt (temp));
      aix[1] = amin * (1. + sqrt (temp));
      nsolutions = 2;
    }
    else {
      //      cout <<"approximation \n" ;
      aix[0] = alin - pow (alin, 2.0) / amin / 2.;
      aix[1] = 2. * amin - aix[0];
      nsolutions = 2;
    }
  };
  //  cout << "Solutions = " << aix[0] << "    " << aix[1] << "\n" ;
  return nsolutions;
}
//-------------------------------------------
