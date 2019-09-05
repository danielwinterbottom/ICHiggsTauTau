// -*- C++ -*-
//
// 
/**\class PolarimetricA1.h PolarimetricA1.cc
 Description: 
*/
//
// Original Author:  Vladimir Cherepanov 
//         Created:  Mon May 1 13:49:02 CET 2017
//
//

#ifndef PolarimetricA1_h
#define PolarimetricA1_h


#include <vector>
#include "TLorentzVector.h"
#include "TComplex.h"
#include "TMatrixT.h"
#include "TVectorT.h"
#include "TMatrixTSym.h"
#include <string.h>
#include <vector>
#include "TLorentzVector.h"
using namespace std;


class PolarimetricA1 {
 
 public:
  PolarimetricA1();
  PolarimetricA1(vector<TLorentzVector> TauA1andProd);
  PolarimetricA1(vector<TLorentzVector> TauA1andProd, TLorentzVector RefernceFrame);
  ~PolarimetricA1();
  void Configure(vector<TLorentzVector> TauA1andProd, int taucharge);
  void Configure(vector<TLorentzVector> TauA1andProd, TLorentzVector RefernceFrame, int taucharge);
  bool isConfigured();
  void Setup(vector<TLorentzVector> TauA1andProd, TLorentzVector ReferenceFrame, int taucharge );
  void subSetup(double s1, double s2, double s3, double Q);


  std::vector<TLorentzVector> getBoosted(){return TauA1andProd_RF;}



  TLorentzVector Boost(TLorentzVector pB, TLorentzVector frame);


  //====================
  double costhetaLF(); 
  double sinthetaLF();

  double cosbetaLF();
  double cospsiLF();
  double sinpsiLF();
  double ultrarel_cospsiLF();
  double cosgammaLF();
  double singammaLF();
  double cosalpha();
  double sinalpha();
  double cos2gamma();
  double sin2gamma();
  double singamma();
  double cosgamma();
  double cosbeta();
  double sinbeta();
  //====================
  double getg();
  double getf();

 double vgetg(TString type="");
 double vgetf(TString type="");
 double vgetgscalar(TString type="");
 double vgetfscalar(TString type="");

 double vgetA1omega(TString type="");
 double vgetA1omegascalar(TString type="");

 double getOmegaA1();
 double getOmegaA1Bar();

//========== TRF  =======
 double TRF_vgetf(TString type="");
 double TRF_vgetg(TString type="");
 double TRF_vgetA1omega(TString type="");
 double TRF_cosbeta();      double  TRF_cosalpha();   double  TRF_cosgamma();  
 double TRF_sinbeta();        double TRF_sinalpha();    double  TRF_singamma();  
//========== TRF  =======
  double lambda(double x, double y, double z);
  double Scalar(TLorentzVector p1, TLorentzVector p2);

  double MomentSFunction(double s,string type="WA");
  double K1(double ct, double QQ, int hel);
  double K1bar(double ct, double QQ, int hel);
  double K2(double ct, double QQ, int hel);
  double K2bar(double ct, double QQ, int hel);
  double K3(double ct, double QQ, int hel);
  double K3bar(double ct, double QQ, int hel);
  double getMoment(double ct, string type, int hel);
  //--------------------------- Hadronic current ---------------------
  //  only 9 structure fucbntions are non-zero in 3pions case

  double WA();
  double WC();
  double WD();
  double WE();
  double WSA();
  double WSB();
  double WSD();
  double WSC();
  double WSE();

  double V1();
  double V2();
  double VV12();
  double JJ();
  //double JN();
  TComplex f3(double Q);
  TLorentzVector JRe(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1);
  TLorentzVector JIm(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1);

  TLorentzVector JCRe(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1);
  TLorentzVector JCIm(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1);
  TComplex JN(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1, TLorentzVector N);
  TComplex JCN(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1, TLorentzVector N);

  TLorentzVector PTZ5(TLorentzVector aR, TLorentzVector aI, TLorentzVector bR, TLorentzVector bI, TLorentzVector c);
  TLorentzVector PTZ(TLorentzVector q1, TLorentzVector q2, TLorentzVector q3, TLorentzVector a1, TLorentzVector N); 
  TLorentzVector PVC(); 

  TLorentzVector CLVEC(std::vector<TComplex> H, std::vector<TComplex> HC, TLorentzVector N);
  TLorentzVector CLAXI(std::vector<TComplex> H, std::vector<TComplex> HC, TLorentzVector N);

  double VV1();
  double VV2();
  double V1V2();
  double h0();
  double h();

  TVector3 nL();
  TVector3 nT();
  TVector3 nTZLFr();
  TVector3 nPerp();
  TVector3 ns();
  TLorentzVector sLV();



  TComplex  BreitWigner(double Q, string type="rho");
  TComplex  BRho(double Q);
  TComplex F1();
  TComplex F2();
  TComplex F4();
  TComplex Conjugate(TComplex a);
  TVector3 Rotate(TVector3 LVec, TVector3 Rot);
  double  Widths(double Q, string type="rho");
  double ppi(double QQ);
  double ga1(double  Q);
  double Mass(string type="rho");

  TComplex BWIGML(double S, double M,  double G, double m1, double m2, int L);
  TComplex FPIKM(double W, double XM1, double XM2);
  TComplex F3PI(double IFORM,double QQ,double SA,double SB);
  TComplex FA1A1P(double XMSQ);
  double WGA1(double QQ);
  double WGA1C(double S);
  double WGA1N(double S);


  TComplex  BWa1(double QQ);
  TComplex  BWrho(double QQ);
  TComplex  BWrhoPrime(double QQ);
  double GammaA1(double QQ);
  double gForGammaA1(double QQ);
  double GammaRho(double QQ);
  double  GammaRhoPrime(double QQ);



  double GetOmegaA1();



 private:
  double mpi;
  double mpi0;
  double mtau;
  double coscab;
  double mrho;
  double mrhoprime;
  double ma1;
  double mpiprime;
  double Gamma0rho; 
  double Gamma0rhoprime; 
  double Gamma0a1;
  double Gamma0piprime;
  double fpi;
  double fpiprime;
  double gpiprimerhopi;
  double grhopipi;
  double beta;
  double COEF1;
  double COEF2;
  double COEF3;
  int SIGN;

  const TLorentzVector a1pos;
  const TLorentzVector a1pss1;
  const TLorentzVector a1pss2;

  bool debug;

  vector<TLorentzVector> TauA1andProd_RF;
  TLorentzVector _osPionLV;
  TLorentzVector _ss1pionLV;
  TLorentzVector _ss2pionLV;
  TLorentzVector _a1LV;
  TLorentzVector _tauLV;
  TLorentzVector _nuLV;
  TLorentzVector _s12;
  TLorentzVector _s13;
  TLorentzVector _s23;
  TLorentzVector _tauAlongZLabFrame;

  double _s1; 
  double _s2; 
  double _s3; 
  double _Q;


  double LFQ;
  TLorentzVector   LFosPionLV;
  TLorentzVector   LFss1pionLV;
  TLorentzVector   LFss2pionLV;
  TLorentzVector   LFa1LV;
  TLorentzVector   LFtauLV;

  bool doSystematic;
  TString systType;
  TMatrixT<double> convertToMatrix(TVectorT<double> V);


};
#endif

