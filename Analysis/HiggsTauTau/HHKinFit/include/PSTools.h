/*
 * Pstools.h
 */

#ifndef PSTOOLS_H_
#define PSTOOLS_H_

#include "TCanvas.h"
#include "TH1F.h"
#include "TPad.h"
#include "TString.h"

class PSTools {
 public:
	PSTools(int size);
	static void ListHistos();
	static TH1F* HistPointer(const char* histname);

	static void wait(TPad* c1);
	static void next(TPad* c1);

	static TCanvas* Pcanvas (int flag=0);
	static void Platex (Float_t x , Float_t y , Float_t size);
	static void Pstyle();
	static void Pprint(const char* filename, TCanvas* c1);

	static void Pcolor(int icol = 1);
	static void white()   {Pcolor(0);}
	static void black()   {Pcolor(1);}
	static void red()     {Pcolor(2);}
	static void green()   {Pcolor(3);}
	static void blue()    {Pcolor(4);}
	static void yellow()  {Pcolor(5);}
	static void magenta() {Pcolor(6);}
	static void turqouis(){Pcolor(7);}
	static void green2()  {Pcolor(8);}
	static void blue2()   {Pcolor(9);}

	static void coutf(Int_t w,TString s);
	static void coutf(Int_t w,Int_t i);
	static void coutf(int w,std::string s);
	static void coutf(int w,double x);
	static void coutf(int w,int p,double x);
	static void PSDrawErrorEllipse(Double_t x=0., Double_t y=0., Double_t sx=3., Double_t sy=2., Double_t rho=0.5,Int_t nsigma=3);
//	static Double_t fmax(Double_t a, Double_t b);
//	static Double_t fmin(Double_t a, Double_t b);
//	static void fsortup(Double_t *a, Double_t *b);
};

#endif /* PSTOOLS_H_ */
