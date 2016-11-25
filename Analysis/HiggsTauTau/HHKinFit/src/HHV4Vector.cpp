#include "../include/HHV4Vector.h"

#include <TArrow.h>
#include <iostream>

#include "../include/PSTools.h"

HHV4Vector::HHV4Vector(Double_t e, Double_t eta, Double_t phi, Double_t m)
    : m_e(e), m_eta(eta), m_phi(phi), m_m(m), m_dE(0), m_dEta(0), m_dPhi(0),
      m_id(HHPID::undef), m_id2(HHPID::undef), m_mother(-1), m_nDaughter(0), m_name("init"), m_cov_manually_set(false),
      m_cov_transversal(TMatrixD(2,2))
{
  m_cov_transversal(0,0)=0;
  m_cov_transversal(0,1)=0;
  m_cov_transversal(1,0)=0;
  m_cov_transversal(1,1)=0;
}

HHV4Vector::HHV4Vector(const HHV4Vector & v, TString suffix)
{
  m_e = v.E();
  m_eta = v.Eta();
  m_phi = v.Phi();
  m_m = v.M();
  m_dE = v.dE();
  m_dEta = v.dEta();
  m_dPhi = v.dPhi();
  m_name = v.Name() + suffix;
  m_id = v.ID();
  m_id2 = v.ID2();
  m_mother = v.Mother();
  m_nDaughter = v.nDaughter();
  m_cov_manually_set = false;
  for (int i = 0; i < m_nDaughter; i++)
    m_daughter[i] = v.Daughter(i);
}
  
void
HHV4Vector::SetPxPyPzE(Double_t px, Double_t py, Double_t pz, Double_t e)
{
  m_e = e;
  Double_t p2 = px * px + py * py + pz * pz;
  if (p2 == 0.) {
    m_eta = 0.;
    m_phi = 0.;
  }
  else {
    Double_t p = sqrt(p2);
    if (e - p < 0.) {
      p = e;
      m_m = 0;
    }
    else
      m_m = sqrt((e - p) * (e + p));

    m_phi = atan2(py, px);
    Double_t theta = acos(pz / p);
    m_eta = -log(tan(theta / 2.));
  }
}

void
HHV4Vector::PrintEPxPyPzM(int pos) const
{
  if (pos >= 0) {
    PSTools::coutf(4, pos);
    std::cout << " ";
  }
  PSTools::coutf(10, m_name);
  PSTools::coutf(4, ID());
  PSTools::coutf(9, 2, E());
  PSTools::coutf(9, 2, Px());
  PSTools::coutf(9, 2, Py());
  PSTools::coutf(9, 2, Pz());
  PSTools::coutf(9, 2, M());
  PSTools::coutf(5, Mother());
  for (int i = 0; i < nDaughter(); i++) {
    PSTools::coutf(4, Daughter(i));
  }
  std::cout << std::endl;
  //  cout << Name << "  " << ID << "  ";
//  Printf(" (EPxPyPzM)=  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f", E(), Px(), Py(),
//         Pz(), M());
}

void
HHV4Vector::PrintEPtEtaPhiM(int pos) const
{
  if (pos >= 0) {
    PSTools::coutf(4, pos);
    std::cout << " ";
  }
  PSTools::coutf(10, m_name);
  PSTools::coutf(4, ID());
  PSTools::coutf(9, 2, E());
  PSTools::coutf(9, 2, Pt());
  PSTools::coutf(9, 2, Eta());
  PSTools::coutf(9, 2, Phi());
  PSTools::coutf(9, 2, M());
  PSTools::coutf(5, Mother());
  for (int i = 0; i < nDaughter(); i++) {
    PSTools::coutf(4, Daughter(i));
  }
  std::cout << std::endl;
  //  cout << Name << "  " << ID << "  ";
//  Printf(" (EPxPyPzM)=  %8.2f  %8.2f  %8.2f  %8.2f  %8.2f", E(), Px(), Py(),
//         Pz(), M());
}

void
HHV4Vector::PrintErrors() const
{
  std::cout << Name() << "  ";
  Printf(" (Errors)    %8.2f  %8.4f  %8.4f  ", dE(), dEta(), dPhi());
}

void
HHV4Vector::Sum(const HHV4Vector & q1, const HHV4Vector & q2)
{
  Double_t Etemp = q1.E() + q2.E();
  Double_t Pxtemp = q1.Px() + q2.Px();
  Double_t Pytemp = q1.Py() + q2.Py();
  Double_t Pztemp = q1.Pz() + q2.Pz();
  SetPxPyPzE(Pxtemp, Pytemp, Pztemp, Etemp);
}

void
HHV4Vector::Subtract(const HHV4Vector & q1, const HHV4Vector & q2)
{
  Double_t Etemp  = q1.E()  - q2.E();
  Double_t Pxtemp = q1.Px() - q2.Px();
  Double_t Pytemp = q1.Py() - q2.Py();
  Double_t Pztemp = q1.Pz() - q2.Pz();
  SetPxPyPzE(Pxtemp, Pytemp, Pztemp, Etemp);
}

//inline V4 V4::operator + (const V4 & q) const {
//  //  cout << "+  " << E << "  " << Px()  << q.E() << q.Px() << endl;
//  Double_t Etemp  = E    + q.E() ;
//  Double_t Pxtemp = Px() + q.Px() ;
//  Double_t Pytemp = Py() + q.Py() ;
//  Double_t Pztemp = Pz() + q.Pz() ;
//  Double_t Ptemp  = sqrt(Pxtemp**2 + Pytemp**2 + Pztemp**2) ;
//  Double_t Etatemp ; Etatemp = 0. ;
//  if (Ptemp-Pztemp > 0.) {
//    Etatemp = (Ptemp+Pztemp) / (Ptemp-Pztemp) ;
//    if (Etatemp > 0.) {Etatemp = 0.5 * log( Etatemp) ;}
//  }
//  //  cout << "+  " << Etemp << "  " << Pxtemp  << Pytemp  << Pztemp  << Ptemp << " " << Etatemp << endl;
//  Double_t Phitemp = atan2( Pytemp, Pxtemp ) ;
//  Double_t Mtemp   = sqrt((Etemp-Ptemp) * (Etemp+Ptemp) ) ;
//  return   V4(Etemp, Etatemp, Phitemp, Mtemp) ;
//}
//inline Double_t V4::operator * (const V4 & q) const
//{  return Dot(q) ; }
//inline Double_t V4::Dot(const V4 & q) const
//{  return E*q.E() - Px()*q.Px() - Py()*q.Py() - Pz()*q.Pz() ; }

void
HHV4Vector::Boost(Double_t bx, Double_t by, Double_t bz)
{
  //Boost this Lorentz vector
  //cout << "Boost: beta " << bx << "  " << by << "  " << bz << "  " << bx**2+by**2+bz**2 << endl;
  //  cout << "Boost:  P    " << Px()<<"  " << Py()<<"  " << Pz()<<"  " << endl;
  Double_t b2 = bx * bx + by * by + bz * bz;
  Double_t gamma = 1.0 / sqrt(1.0 - b2);
  Double_t bp = bx * Px() + by * Py() + bz * Pz();
  Double_t gamma2 = b2 > 0 ? (gamma - 1.0) / b2 : 0.0;
  Double_t px, py, pz;
  //  cout << "Boost: b2,g,bp,g2: " << b2 << "  " << gamma << "  " << bp << "  " << gamma2 << endl;
  px = Px() + gamma2 * bp * bx + gamma * bx * E();
  py = Py() + gamma2 * bp * by + gamma * by * E();
  pz = Pz() + gamma2 * bp * bz + gamma * bz * E();
  m_e = gamma * (E() + bp);
  m_phi = atan2(py, px);
  m_eta = -log(tan(0.5 * acos(pz / sqrt(px * px + py * py + pz * pz))));
  //  cout << "Boost:  E " << E << "  px " << px<< "  py " << py<< "  pz " << pz
  //       << "  Phi " <<Phi<<" Eta " <<Eta << endl;
}

void
HHV4Vector::Rotate(const HHV4Vector & q)
{
  Double_t ex, ey, ez, en;
  Double_t a, b, c, d, px1, py1, pz1, px2, py2, pz2, px3, py3, pz3;
  px1 = Px();
  py1 = Py();
  pz1 = Pz();
  ex = q.Px();
  ey = q.Py();
  ez = q.Pz();
  en = sqrt(ex * ex + ey * ey + ez * ez);
  ex = ex / en;
  ey = ey / en;
  ez = ez / en;
  //  cout << "Rotate e: " << ex << "  " << ey << "  " << ez << endl;
  a = ez;
  b = sqrt(1. - a * a);
  c = ex / b;
  d = -ey / b;
  px2 = a * px1 + b * pz1;   // rotation around y-axis
  py2 = py1;
  pz2 = -b * px1 + a * pz1;
  //  cout << "Rotate p2: " << px2 << "  " << py2 << "  " << pz2 << endl;
  px3 = c * px2 + d * py2;   // rotation around z-axis
  py3 = -d * px2 + c * py2;
  pz3 = pz2;
  //  cout << "Rotate p3: " << px3 << "  " << py3 << "  " << pz3 << endl;
  SetPxPyPzE(px3, py3, pz3, E());
}

//inline void V4::V4transform (V4 *q);

//void Ptransform (Double_t E, Double_t Px, Double_t Py, Double_t Pz,
//		 Double_t Mb, Double_t Pxb, Double_t Pyb, Double_t Pzb)
//{
//  // boost vector from a system, in which particle q is at rest,
//  // into the rest frame of  system, in which particle p will have momentum P1,P2,P3
//
//  // 1) first boost along   z-axis such that total momentum is along z
//  // 2) then  rotate around y-axis such that Pz is o.k.
//  // 3) then  rotate around z-axis such that all components are ok.
//
//  Double_t Mb = q.M();
//  Double_t E1,Px1,Py1,Pz1 ;
//  Double_t E2,Px2,Py2,Pz2 ;
//  Double_t E3,Px3,Py3,Pz3 ;
//  Double_t Pb, Ptb ;
//  Pb  = sqrt(q.Pxb()*q.Pxb()+q.Pyb*q.Pyb+Pzb*Pzb) ;
//  Ptb = sqrt(Pxb*Pxb+Pyb*Pyb) ;
//
//  // 1) first boost along   z-axis such that total momentum is along z
//  E1  = Pboost(E ,Pz, Mb,Pb) ;
//  Px1 = Px;
//  Py1 = Py;
//  Pz1 = Pboost(Pz,E , Mb,Pb) ;
//
//  // 2) then  rotate around y-axis such that Pz and Pt are o.k.
//  Double_t costh , sinth ;
//  if (Pb == 0.) {
//    costh = 1. ;
//    sinth = 0. ;
//  }
//  else {
//    costh = Pzb / Pb ;
//    sinth = Ptb / Pb ;
//  }
//  E2  =  E1 ;
//  Px2 =  Pz1 * sinth + Px1 * costh ;
//  Py2 =  Py1 ;
//  Pz2 =  Pz1 * costh - Px1 * sinth ;
//
//  //  cout <<"E,px,py,pz  W 2 " <<E << " " <<Ptb << " " <<0. << " " <<Pzb << endl;
//
//  // 3) then  rotate around z-axis such that all components are ok.
//  Double_t cosph , sinph ;
//  if (Ptb == 0.) {
//    sinph = 0. ;
//    cosph = 1. ;
//  }
//  else {
//    cosph = Pxb / Ptb ;
//    sinph = Pyb / Ptb ;
//  }
//  E3  =   E2 ;
//  Px3 =   Px2 * cosph - Py2 * sinph ;
//  Py3 =   Px2 * sinph + Py2 * cosph ;
//  Pz3 =   Pz2 ;
//  // output into '3temp' vector
//  Double_t E3temp  = E3 ;
//  Double_t Px3temp = Px3 ;
//  Double_t Py3temp = Py3 ;
//  Double_t Pz3temp = Pz3 ;
//  //  TString sp ;
//  //  sp = "        " ;
//  //  cout << "E,Px,Py,Pz W   = " <<sqrt(Mb**2+Pxb**2+Pyb**2+Pzb**2)
//  //                                  << sp <<Pxb<< sp <<Pyb<< sp <<Pzb<<endl;
//  //  cout << "E,Px,Py,Pz in  = " <<E << sp <<Px << sp <<Py << sp <<Pz <<endl;
//  //  cout << "E,Px,Py,Pz 1   = " <<E1<< sp <<Px1<< sp <<Py1<< sp <<Pz1<<endl;
//  //  cout << "E,Px,Py,Pz 2   = " <<E2<< sp <<Px2<< sp <<Py2<< sp <<Pz2<<endl;
//  //  cout << "E,Px,Py,Pz 3   = " <<E3<< sp <<Px3<< sp <<Py3<< sp <<Pz3<<endl;
//  //  cout << "E,Px,Py,Pztemp = " <<E3temp<< sp <<Px3temp<< sp <<Py3temp<< sp <<Pz3temp<< endl;
//}

void
HHV4Vector::Draw(Int_t color, Int_t style) const
{  // draw particle for event display in x-y view
  TArrow *Ar = new TArrow(0, 0, Px(), Py(), 0.001, "|>");
  Ar->SetLineColor(color);
  Ar->SetFillColor(color);
  Ar->SetLineWidth(3);
  Ar->SetLineStyle(style);
  Ar->Draw();
}

void
HHV4Vector::CalcCov()
{
  m_cov_manually_set = false;

  if (P()>0){
    //NOTE: only dE is used for Cov calculation!
    Double_t dp = E()/P()*dE(); // error propagation p=sqrt(e^2-m^2)
    Double_t dpt = sin(Theta())*dp;

    m_cov_transversal(0,0) = pow(cos(Phi())*dpt,2);
    m_cov_transversal(1,1) = pow(sin(Phi())*dpt,2);
    m_cov_transversal(0,1) = sin(Phi())*cos(Phi())*dpt*dpt;
    m_cov_transversal(1,0) = sin(Phi())*cos(Phi())*dpt*dpt;
  }

}

void
HHV4Vector::SetCov(Double_t xx, Double_t yy, Double_t xy)
{
  m_cov_manually_set = true;
  m_cov_transversal(0,0)=xx;
  m_cov_transversal(0,1)=xy;
  m_cov_transversal(1,0)=xy;
  m_cov_transversal(1,1)=yy;
}

void
HHV4Vector::SetCov(TMatrixD m)
{
  m_cov_manually_set = true;
  m_cov_transversal(0,0)=m(0,0);
  m_cov_transversal(0,1)=m(0,1);
  m_cov_transversal(1,0)=m(1,0);
  m_cov_transversal(1,1)=m(1,1);
}

TMatrixD
HHV4Vector::GetCov()
{
  if (!m_cov_manually_set) CalcCov();
  return m_cov_transversal;
}
