/*
 * HHTauInputKinFit.cpp
 *
 *  Created on: Jun 17, 2014
 */

#include "../include/HHTauInputKinFit.h"
#include "../include/PSMath.h"
#include "../include/PSTools.h"

#include "TString.h"
#include "TPad.h"
#include "TPolyMarker.h"
#include "TMarker.h"
#include "TMatrixD.h"
#include "TMatrixDEigen.h"
#include "TLatex.h"
#include "TH2F.h"
#include <iostream>

HHTauInputKinFit::HHTauInputKinFit(HHEventRecord* recrecord)
    : m_chi2(-1), m_chi2_b1(-1), m_chi2_b2(-1), m_chi2_tau1(-1), m_chi2_tau2(-1),
      m_convergence(0), m_fittedmH(-1),
      m_printlevel(1), m_graphicslevel(0),
      m_maxloops(100),
      m_advancedBalance(true),m_logLevel(0),
      m_recrecord(recrecord), m_fitrecord (new HHEventRecord(*recrecord, "Fit")),
      m_particlelist(m_recrecord->GetParticleList())
{
}

HHTauInputKinFit::~HHTauInputKinFit(){
  delete m_fitrecord;
}


void
HHTauInputKinFit::Fit()
{
//  V4Vector* ptmissrec = PTmissCalc(fitrecord);
//   HHV4Vector* ptmissrec = m_recrecord->GetEntry(HHEventRecord::MET);

  //  ----------  for PSfit -----
  const Int_t np = 2;
  Double_t a[np];
  Double_t astart[np];
  Double_t alimit[np][2];
  Double_t aprec[np];
  Double_t daN[np];
  Double_t h[np];
  Double_t chi2iter[1], aMemory[np][5], g[np], H[np * np], Hinv[np * np];
  Bool_t noNewtonShifts = false;
  Int_t iter = 0;             //  number of iterations
  Int_t method = 1;           //  initial fit method, see PSfit()
  Int_t mode = 1;             //  mode =1 for start of a new fit by PSfit()


  // fill initial fit parameters
  astart[0] = m_fitrecord->GetEntry(HHEventRecord::b1)->E();         // energy of first b-jet
  astart[1] = m_fitrecord->GetEntry(HHEventRecord::tau1)->E();       // energy of first tau
  aprec[0] = 0.01;   //0.1                 // precision for fit
  aprec[1] = 0.01;   //0.1                 // precision for fit

  // fill initial step width
  h[0] = 0.5*m_fitrecord->GetEntry(HHEventRecord::b1)->dE();      // step width = 50% of bjet uncertainty
  h[1] = 0.5*m_fitrecord->GetEntry(HHEventRecord::tau1)->dE();    // step width = 50% of tau uncertainty

  daN[0] = 1.0;   //0.0                 // initial search direction in Eb diagonal
  daN[1] = 0.0;   //1.0

  // fit range
  alimit[0][0] = astart[0] - m_fitrecord->GetEntry(HHEventRecord::b1)->dE() * 5.; // b-jets: 5 sigma
  if (alimit[0][0]<0) alimit[0][0]=0;
  alimit[0][1] = astart[0] + m_fitrecord->GetEntry(HHEventRecord::b1)->dE() * 5.;
  
  alimit[1][0] = astart[1] - m_fitrecord->GetEntry(HHEventRecord::tau1)->dE() * 5.; // taus: 5 sigma
  if (alimit[1][0]<0) alimit[1][0]=0;
  alimit[1][1] = astart[1] + m_fitrecord->GetEntry(HHEventRecord::tau1)->dE() * 5.;  
  
  // tau: check initial values against fit range
  if (astart[1] - h[1] < alimit[1][0]) {
    astart[1] = alimit[1][0] + h[1];
  }
  else if (astart[1] + h[1] > alimit[1][1]) {
    astart[1] = alimit[1][1] - h[1];
  }

  for (Int_t ip = 0; ip < np; ip++) {
    a[ip] = astart[ip];
  }

  static const Int_t nloopmax = 100;
  Double_t Xa[nloopmax],   Ya[nloopmax];
  Double_t Xa1[nloopmax],  Ya1[nloopmax];
  Double_t HPx[nloopmax],  HPy[nloopmax];
  Double_t HPx1[nloopmax], HPy1[nloopmax];

  for (Int_t iloop = 0; iloop < m_maxloops * 10 && iter < m_maxloops; iloop++) { // FIT loop
    //    chi2        = testfunction(a,np) ;
    //    std::cout << "V4KinFit  in loop iloop " << iloop << std::endl;

//    fitrecord->Print("before",0);
//    std::cout << "new values:" << "(" << alimit[0][0] << "-" << alimit[0][1] << ") " << a[0] << "--- (" << alimit[1][0] << "-" << alimit[1][1] << ") " << a[1] << std::endl;
    m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEkeepM(a[0]); // update 4-vectors with fit parameters
    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEkeepM(a[1]);
    ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
    ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);

    m_chi2_b1 = Chi2V4(HHEventRecord::b1);
    m_chi2_b2 = Chi2V4(HHEventRecord::b2);
    m_chi2_tau1 = Chi2V4(HHEventRecord::tau1);
    m_chi2_tau2 = Chi2V4(HHEventRecord::tau2);

    m_chi2 = m_chi2_b1 + m_chi2_b2 + m_chi2_tau1 + m_chi2_tau2 ; // chi2 calculation

    if (m_logLevel>1){
      std::cout << "chi2 b1: " << m_chi2_b1 << std::endl;
      std::cout << "chi2 b2: " << m_chi2_b2 << std::endl;
      std::cout << "chi2 tau1: " << m_chi2_tau1 << std::endl;
      std::cout << "chi2 tau2: " << m_chi2_tau2 << std::endl;
      std::cout << "------------------" << std::endl;
      std::cout << "chi2 : " << m_chi2 << std::endl;
      std::cout << "------------------" << std::endl;
      std::cout << "------------------" << std::endl;
    }

    if (iloop >= 0 && iloop < nloopmax) {
      Xa1[iloop] = m_fitrecord->GetEntry(HHEventRecord::b1)->E();
      Ya1[iloop] = m_fitrecord->GetEntry(HHEventRecord::tau1)->E();
    }
    if (iter >= 0 && iter < nloopmax) {
      Xa[iter] = m_fitrecord->GetEntry(HHEventRecord::b1)->E();
      Ya[iter] = m_fitrecord->GetEntry(HHEventRecord::tau1)->E();
    }
    if (iloop >= 0 && iloop < nloopmax) {
      HPx1[iloop] = m_fitrecord->GetEntry(HHEventRecord::H)->Px();
      HPy1[iloop] = m_fitrecord->GetEntry(HHEventRecord::H)->Py();
    }
    if (iter >= 0 && iter < nloopmax) {
      HPx[iter] = m_fitrecord->GetEntry(HHEventRecord::H)->Px();
      HPy[iter] = m_fitrecord->GetEntry(HHEventRecord::H)->Py();
    }

    PSMath::PSfitShow(iloop, m_convergence, iter, method, mode, m_printlevel,
                      m_graphicslevel, np, a, astart, alimit, aprec, daN, h,
                      m_chi2, g, H);

    if (m_convergence != 0) {
      break;
    }
    m_convergence = PSMath::PSfit(iloop, iter, method, mode, noNewtonShifts, m_printlevel,
                                  np, a, astart, alimit, aprec,
                                  daN, h, aMemory, m_chi2, chi2iter, g, H,
                                  Hinv);
  }
  // ------ end of FIT loop
  m_fittedmH = m_fitrecord->GetEntry(HHEventRecord::H)->M();
}

void
HHTauInputKinFit::ConstrainE2(Int_t iv4, Int_t iv41, Int_t iv42)
{
  m_fitrecord->UpdateMothers(iv41);
  Double_t M1, M2, M, Mc, E1, E2, b2, beta2;
  M = m_fitrecord->GetEntry(iv4)->M();
  Mc = m_particlelist->GetParticleInfo( m_fitrecord->GetEntry(iv4)->ID() )->M();
  E1 = m_fitrecord->GetEntry(iv41)->E();
  M1 = m_fitrecord->GetEntry(iv41)->M();
  E2 = m_fitrecord->GetEntry(iv42)->E();
  M2 = m_fitrecord->GetEntry(iv42)->M();

  if (M2 == 0.) { // massless case
    m_fitrecord->UpdateEntry(iv42)->SetEkeepM(E2 * (Mc / M) * (Mc / M)); // only changes absolute value and keeps eta, phi, m untouched
                                                                         // such that invariant mass of 1 and 2 gives correct mass Mc
    m_fitrecord->UpdateMothers(iv42);
    return;
  }

  Int_t ID2jet = -1;
  Int_t ID2 = m_fitrecord->GetEntry(iv42)->ID();

  if (ID2 == HHPID::q || ID2 == HHPID::c
      || ID2 == HHPID::b || ID2 == HHPID::gluon)
    ID2jet = 1;

  beta2 = sqrt(E2 * E2 - M2 * M2) / E2;
  if (ID2jet < 0) { // is not a jet
    b2 = (M2 / E2) * (M2 / E2); // isn't the no jet case identical to the jet case? 1/gamma**2 = m**2/E**2 = 1-beta**2
  }
  else { // is a jet
    b2 = 1 - beta2 * beta2;
  }

  Double_t d = (M * M - M1 * M1 - M2 * M2) / (2. * E1 * E2);
  Double_t E2lin = (Mc * Mc - M1 * M1) / (2. * E1 * d);
  Double_t E2N = E1 * d / b2;
  Double_t E2new = E2N * (-1. + sqrt(1. + 2. * E2lin / E2N));

  if (ID2jet < 0) { // is not a jet
    m_fitrecord->UpdateEntry(iv42)->SetEkeepM(E2new);
  }
  else { // is a jet
    HHV4Vector* entry = m_fitrecord->GetEntry(iv42);
    m_fitrecord->UpdateEntry(iv42)->SetEEtaPhiM(E2new, entry->Eta(), entry->Phi(), M2 * E2new / E2);
  }

  m_fitrecord->UpdateMothers(iv42);
}

Double_t
HHTauInputKinFit::Chi2V4(Int_t iv4)
{
  Double_t chi2 = 0;
  Double_t chi2_E = 0;
  Double_t chi2_Eta = 0;
  Double_t chi2_Phi = 0;
  HHV4Vector* fitobject = m_fitrecord->GetEntry(iv4);
  HHV4Vector* recobject = m_recrecord->GetEntry(iv4);

  //  std::cout << "V4chi2 dE " ; V4fit[iv4].Print();
  if (fitobject->dE() > 0.) {
    chi2_E = ((recobject->E() - fitobject->E()) / fitobject->dE())
        * ((recobject->E() - fitobject->E()) / fitobject->dE());
  }

  if (fitobject->dEta() > 0.) {
    chi2_Eta = ((recobject->Eta() - fitobject->Eta()) / fitobject->dEta())
        * ((recobject->Eta() - fitobject->Eta()) / fitobject->dEta());
  }
  if (fitobject->dPhi() > 0.) {
    chi2_Phi = ((recobject->Phi() - fitobject->Phi()) / fitobject->dPhi())
        * ((recobject->Phi() - fitobject->Phi()) / fitobject->dPhi());
  }

  chi2 = chi2_E + chi2_Eta + chi2_Phi;
  if (m_logLevel>1){
    PSTools::coutf(5, TString("E"));
    PSTools::coutf(9, 2, recobject->E());
    PSTools::coutf(9, 2, fitobject->E());
    PSTools::coutf(9, 2, recobject->E()-fitobject->E());
    PSTools::coutf(3, TString("|"));
    PSTools::coutf(9, 2, fitobject->dE());
    PSTools::coutf(3, TString("->"));
    PSTools::coutf(9, 2, chi2_E);
    std::cout << std::endl;

    PSTools::coutf(5, TString("Eta"));
    PSTools::coutf(9, 2, recobject->Eta());
    PSTools::coutf(9, 2, fitobject->Eta());
    PSTools::coutf(9, 2, recobject->Eta()-fitobject->Eta());
    PSTools::coutf(3, TString("|"));
    PSTools::coutf(9, 2, fitobject->dEta());
    PSTools::coutf(3, TString("->"));
    PSTools::coutf(9, 2, chi2_Eta);
    std::cout << std::endl;

    PSTools::coutf(5, TString("Phi"));
    PSTools::coutf(9, 2, recobject->Phi());
    PSTools::coutf(9, 2, fitobject->Phi());
    PSTools::coutf(9, 2, recobject->Phi()-fitobject->Phi());
    PSTools::coutf(3, TString("|"));
    PSTools::coutf(9, 2, fitobject->dPhi());
    PSTools::coutf(3, TString("->"));
    PSTools::coutf(9, 2, chi2_Phi);
    std::cout << std::endl;
  }

  return chi2;
}
void
HHTauInputKinFit::SetPrintLevel(Int_t level){
  m_printlevel = level;
}

void
HHTauInputKinFit::SetGraphicsLevel(Int_t level){
  m_graphicslevel=level;
}

void
HHTauInputKinFit::SetMaxLoops(Int_t loops){
  m_maxloops = loops;
}

Double_t
HHTauInputKinFit::GetChi2()
{
  return m_chi2;
}

Double_t
HHTauInputKinFit::GetChi2_b1()
{
  return m_chi2_b1;
}

Double_t
HHTauInputKinFit::GetChi2_b2()
{
  return m_chi2_b2;
}

Double_t
HHTauInputKinFit::GetChi2_tau1()
{
  return m_chi2_tau1;
}

Double_t
HHTauInputKinFit::GetChi2_tau2()
{
  return m_chi2_tau2;
}


Double_t
HHTauInputKinFit::GetFittedMH()
{
  return m_fittedmH;
}

Double_t
HHTauInputKinFit::GetConvergence()
{
  return m_convergence;
}

HHEventRecord*
HHTauInputKinFit::GetFitrecord(){
  return m_fitrecord;
}

void
HHTauInputKinFit::SetLogLevel(Int_t level)
{
  m_logLevel = level;
}


double
HHTauInputKinFit::GetPullE(Int_t iv4){
  HHV4Vector* fv = m_fitrecord->GetEntry(iv4);
  HHV4Vector* rv = m_recrecord->GetEntry(iv4);
  return (fv->E()-rv->E())/rv->dE();
}
