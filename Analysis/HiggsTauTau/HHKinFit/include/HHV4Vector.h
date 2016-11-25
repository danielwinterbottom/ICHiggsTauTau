/*
 * class for 4-vectors
 */

#ifndef V4_H_
#define V4_H_

#include <Rtypes.h>
#include <TString.h>
#include <cmath>
#include "HHPID.h"
#include "TLorentzVector.h"
#include "TMatrixD.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"


class HHV4Vector {
public:
  HHV4Vector(Double_t e=0., Double_t eta=0., Double_t phi=0., Double_t m=0.);
  HHV4Vector(const HHV4Vector& v, TString suffix="");

  Double_t E() const    {return(m_e);}
  Double_t Eta() const  {return(m_eta);}
  Double_t Theta() const{return(2.*atan(exp(-m_eta)));}
  Double_t Phi() const  {return(m_phi);}
  Double_t M() const    {return(m_m);}

  Double_t dE() const   {return(m_dE);}
  Double_t dEta() const {return(m_dEta);}
  Double_t dPhi() const {return(m_dPhi);}

  Double_t P() const   {return(sqrt((m_e-m_m)*(m_e+m_m)));}
  Double_t Px() const  {return(P() * sin(2.*atan(exp(-m_eta))) * cos(m_phi));}
  Double_t Py() const  {return(P() * sin(2.*atan(exp(-m_eta))) * sin(m_phi));}
  Double_t Pz() const  {return(P() * cos(2.*atan(exp(-m_eta))));}
  Double_t Pt() const  {return(P() * sin(2.*atan(exp(-m_eta))));}

  TString Name() const       {return(m_name);}
  HHPID::pid ID() const      {return(m_id);}
  HHPID::pid ID2() const     {return(m_id2);}
  Int_t Mother() const           {return(m_mother);}
  Int_t nDaughter() const        {return(m_nDaughter);}
  Int_t Daughter (Int_t i) const {return(m_daughter[i]);}

  void GetAllData(const HHV4Vector & q);

  void SetVector(ROOT::Math::XYZTVector v) {m_e = v.E(); m_eta = v.Eta(); m_phi=v.Phi(); m_m=v.M();};
  void SetVector(TLorentzVector v)         {m_e = v.E(); m_eta = v.Eta(); m_phi=v.Phi(); m_m=v.M();};
  void SetPxPyPzE(Double_t px, Double_t py, Double_t pz, Double_t e);
  void SetEEtaPhiM(Double_t e,Double_t eta,Double_t phi,Double_t m) { m_e=e; m_eta=eta; m_phi=phi; m_m=m ; }
  void SetErrors(Double_t de,Double_t deta,Double_t dphi)           { m_dE=de; m_dEta=deta; m_dPhi=dphi; }
  void SetEkeepM(Double_t e) {m_e=e;}
  void SetM(Double_t m)      {m_m=m;}
  void SetName(TString name) {m_name=name;}
  void SetID(HHPID::pid id)       {m_id = id;}
  void SetMother(Int_t i)    {m_mother = i;}
  void SetDaughters()        {m_nDaughter=0;}
  void SetDaughters(Int_t i1, Int_t i2) { m_nDaughter=2; m_daughter[0]=i1; m_daughter[1]=i2;}
  void AddDaughter(Int_t i1) {m_daughter[m_nDaughter]=i1; m_nDaughter++;}

  void CalcCov();
  void SetCov(Double_t xx, Double_t yy, Double_t xy);
  void SetCov(TMatrixD m);
  TMatrixD GetCov();

//  V4Vector operator+ (const V4Vector &) const;
//  V4Vector operator- (const V4Vector &) const;
  //  Double_t Dot (const V4 &q) const;
  //  V4 operator * (const V4 &) const;

  void PrintEPxPyPzM(int pos=-1) const;
  void PrintEPtEtaPhiM(int pos=-1) const;

  void Print() const;
  void PrintErrors () const;
  void Draw(Int_t color=1, Int_t style=1) const;

  void Boost(Double_t bx, Double_t by, Double_t bz);
  void Rotate(const HHV4Vector & q);
  void Sum(const HHV4Vector & q1, const HHV4Vector & q2);
  void Subtract(const HHV4Vector & q1, const HHV4Vector & q2);


private:

  Double_t m_e, m_eta, m_phi, m_m;
  Double_t m_dE, m_dEta, m_dPhi;
  HHPID::pid  m_id, m_id2;
  
  Int_t m_mother, m_nDaughter;
  TString m_name;  
  Int_t m_daughter[10];
  Bool_t m_cov_manually_set;

  TMatrixD m_cov_transversal;
};

#endif /* V4_H_ */
