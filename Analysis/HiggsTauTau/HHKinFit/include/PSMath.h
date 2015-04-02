#ifndef PSMATH_H_
#define PSMATH_H_

#include <Rtypes.h>

class PSMath {
 public:
  PSMath();
  static Int_t PSfit(Int_t iloop, Int_t &iter, Int_t &method, Int_t &mode,  
	      Bool_t &noNewtonShifts, Int_t printlevel,
	      Int_t np, Double_t a[], Double_t astart[], Double_t alimit[][2], 
	      Double_t aprec[], Double_t daN[], Double_t h[], Double_t aMemory[][5],
	      Double_t chi2, Double_t chi2iter[], Double_t g[], Double_t H[], Double_t Hinv[] );

  static void PSNewtonLimitShift(Int_t sign, Int_t np, Double_t a[], Double_t alimit[][2], Double_t aprec[],
			  Double_t daN[], Double_t h[], Double_t g[], Double_t H[]);
  
  static Double_t PSNewtonAnalyzer(Int_t np, Double_t a[], Double_t alimit[][2], Double_t aprec[],
			    Double_t daN[], Double_t h[], 
			    Double_t g[], Double_t H[], Double_t Hinv[], Double_t chi2, Bool_t noNewtonShifts, Int_t printlevel=1);

  static void PSfitShow(Int_t iloop, Int_t convergence, Int_t iter, 
		 Int_t method, Int_t mode, 
		 Int_t printlevel, Int_t graphiklevel,
		 Int_t np, Double_t a[], Double_t astart[], Double_t alimit[][2], 
		 Double_t aprec [], Double_t daN[], Double_t h[], Double_t chi2,
		 Double_t g[], Double_t H[] );

  static Double_t PSLineSearch(Int_t & mode, Double_t hh, Double_t xlimit[], 
			Double_t epsx, Double_t epsf, Double_t x[4], Double_t f[], 
			Double_t chi2 , Int_t printlevel);

  static void PSLineLimit(Int_t np, Double_t astart[], Double_t daN[], Double_t alimit[][2], Double_t xlimit[] );

  static Double_t  PSVnorm(Double_t x[], Int_t n);

  static void PSVprint(const char* text, Double_t x[], Int_t n);
  static void PSMprint(const char* text, Double_t A[], Int_t ni, Int_t nj);
  static void PSM2print(const char* text, Double_t A[][2], Int_t ni);

  static Double_t PSMinverse(Double_t H[], Double_t Hinv[], Int_t p);

  static Double_t PSMCholtest();

  static Double_t PSMmultiply(Double_t A[],Double_t B[],Double_t C[],Int_t n1,Int_t n2);

  static Double_t PSMmultiplyMRRT(Double_t A[], Int_t n1, Int_t n2);

  static Double_t PSMmultiplyMT(Double_t A[], Double_t B[], Int_t n1, Int_t n2);

  static Double_t PSMRTrianInvert2(Double_t R[], Int_t n);

  static Double_t PSMRTrianInvert(Double_t R[], Double_t Rinv[],  Int_t n);
  
  static Double_t PSMCholesky(Double_t M[], Double_t R[],  Int_t n);

  static Double_t PSfitCheckLimits(Int_t np, Double_t a[], Double_t h[], 
			    Double_t alimit[][2], Double_t aprecision[], 
			    Double_t daN[], Double_t g[], Double_t d);
  
  static Double_t PSminIterate(Double_t a[], Double_t daN[], Double_t h[], Int_t p,  
			Double_t g[], Double_t H[], Double_t Hinv[], Double_t X0);
  
  static Double_t PSfuncQuadratic(Double_t a[], Double_t amean[], Double_t F0, 
			   Double_t g[], Double_t H[], Int_t np);


 
  static Int_t PSderivative(Int_t icall, Int_t np, Double_t a[], Double_t h[],
		     Double_t chi2, Double_t chi2iter[],
		     Double_t g[], Double_t H[]);

  static Int_t PSderivative1(Int_t icall, Double_t a[], Double_t h[], 
			       Double_t chi2, Double_t g[], Double_t H[]);

  static Double_t PSfitMinStep(Int_t np, Double_t a[], Double_t h[], 
			Double_t chi2iter[],
			Double_t g[], Double_t H[], Double_t Hinv[], Double_t daN[]);

  static Int_t PSfitconstrain0(Double_t F, Double_t g, Double_t H, Double_t Fix, Double_t aix[]);


};

#endif
