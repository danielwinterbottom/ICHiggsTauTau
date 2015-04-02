/*
 * HHKinFit.cpp
 *
 *  Created on: Jun 17, 2014
 */

#include "../include/HHKinFit.h"
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

HHKinFit::HHKinFit(HHEventRecord* recrecord)
    : m_chi2(-1), m_chi2_b1(-1), m_chi2_b2(-1), m_chi2_balance(-1),
      m_convergence(0), m_fittedmH(-1),
      m_printlevel(1), m_graphicslevel(0),
      m_maxloops(500),
      m_advancedBalance(kTRUE),
      m_logLevel(0),
      m_keepMassesConst(0),
      m_recrecord(recrecord), m_fitrecord (new HHEventRecord(*recrecord, "Fit")),
      m_covRecoil(2,2)
{
  m_particlelist = m_recrecord->GetParticleList();
}


HHKinFit::~HHKinFit(){
  delete m_fitrecord;
}


void
HHKinFit::Fit()
{
//  V4Vector* ptmissrec = PTmissCalc(fitrecord);
  HHV4Vector* ptmissrec = m_recrecord->GetEntry(HHEventRecord::MET);

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
//   Int_t icallNewton = -1;     //  init start of Newton Method
//   Int_t iloop = 0;            // counter for falls to fit function

  // calculate htau from tauvis; recombine leaves measured entries in event record untouched
  m_recrecord->Recombine();

  Double_t bjet1UpperLimit = m_recrecord->GetEntry(HHEventRecord::b1)->E() + 5.0 * m_recrecord->GetEntry(HHEventRecord::b1)->dE();
  Double_t bjet1LowerLimit = m_recrecord->GetEntry(HHEventRecord::b1)->E() - 5.0 * m_recrecord->GetEntry(HHEventRecord::b1)->dE();
  Double_t bjet2UpperLimit = m_recrecord->GetEntry(HHEventRecord::b2)->E() + 5.0 * m_recrecord->GetEntry(HHEventRecord::b2)->dE();
  Double_t bjet2LowerLimit = m_recrecord->GetEntry(HHEventRecord::b2)->E() - 5.0 * m_recrecord->GetEntry(HHEventRecord::b2)->dE();

  Double_t tau1LowerLimit = m_recrecord->GetEntry(HHEventRecord::tauvis1)->E();
  Double_t tau2LowerLimit = m_recrecord->GetEntry(HHEventRecord::tauvis2)->E();

  m_keepMassesConst = false;

  Double_t mhtauHypo =  m_particlelist->GetParticleInfo(HHPID::h1)->M();
  Double_t mhtauReco  = m_recrecord->GetEntry(HHEventRecord::htau)->M();
  Double_t mtau  = m_particlelist->GetParticleInfo(HHPID::tau)->M();

  //Calculate Recoil CovMatrix
  TMatrixD Cov_MET     = m_recrecord->GetEntry(HHEventRecord::MET)->GetCov();
  TMatrixD Cov_b1      = m_recrecord->GetEntry(HHEventRecord::b1)->GetCov();
  TMatrixD Cov_b2      = m_recrecord->GetEntry(HHEventRecord::b2)->GetCov();
  TMatrixD Cov_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->GetCov();
  TMatrixD Cov_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->GetCov();
  m_covRecoil = Cov_MET - (Cov_b1 + Cov_b2 + Cov_tauvis1 + Cov_tauvis2);
  
  TMatrixDEigen eigenmatrix(m_covRecoil);

  if (eigenmatrix.GetEigenValues()(0,0)<0 || eigenmatrix.GetEigenValues()(1,1)<0){
    m_fixedCovMatrix = true;
    m_covRecoil(0,0) = 100;
    m_covRecoil(1,1) = 100;
    m_covRecoil(1,0) = 0;
    m_covRecoil(0,1) = 0;
  }
  
  TMatrixDEigen eigenmatrix2(m_covRecoil);

  // initialise tau1 and tau2 vectors
  if(m_recrecord->GetEntry(HHEventRecord::tauvis1)->E() > mtau){
    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEEtaPhiM(m_recrecord->GetEntry(HHEventRecord::tauvis1)->E(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Phi(),
							       mtau);
  }
  else{
    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEEtaPhiM(pow(m_recrecord->GetEntry(HHEventRecord::tauvis1)->P(),2)+pow(mtau,2),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Phi(),
							       mtau);
  }

  if(tau2LowerLimit > mtau){
    m_fitrecord->UpdateEntry(HHEventRecord::tau2)->SetEEtaPhiM(tau2LowerLimit,
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Phi(),
							       mtau);
  }
  else{
    m_fitrecord->UpdateEntry(HHEventRecord::tau2)->SetEEtaPhiM(pow(m_recrecord->GetEntry(HHEventRecord::tauvis2)->P(),2)+pow(mtau,2),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Phi(),
							       mtau);
  }
                                    

  //firstly, compute upper limit of E(tau1) by having set E(tau2)=E(tau2)_min and compute E(tau1)
  ConstrainE2(HHEventRecord::htau, HHEventRecord::tau2, HHEventRecord::tau1);
  Double_t maxEtau1 = m_fitrecord->GetEntry(HHEventRecord::tau1)->E();


  /*
  //secondly, compute lower limit of E(tau1) by setting E(tau1)=E(tauvis1) and compute E(tau2)
  //and check whether E(tau2)<E(tauvis2)
  if(m_recrecord->GetEntry(HHEventRecord::tauvis1)->E() > mtau){
    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEEtaPhiM(m_recrecord->GetEntry(HHEventRecord::tauvis1)->E(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Phi(),
							       mtau);
  }
  else{
    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEEtaPhiM(pow(m_recrecord->GetEntry(HHEventRecord::tauvis1)->P(),2)+pow(mtau,2),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Phi(),
							       mtau);
  }
  ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);
  */
  
  Double_t minEtau1 = tau1LowerLimit;


  //Reset taus
  if(m_recrecord->GetEntry(HHEventRecord::tauvis2)->E() > mtau){
    m_fitrecord->UpdateEntry(HHEventRecord::tau2)->SetEEtaPhiM(m_recrecord->GetEntry(HHEventRecord::tauvis2)->E(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Phi(),
							       mtau);
  }
  else{
    m_fitrecord->UpdateEntry(HHEventRecord::tau2)->SetEEtaPhiM(pow(m_recrecord->GetEntry(HHEventRecord::tauvis2)->P(),2)+pow(mtau,2),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis2)->Phi(),
							       mtau);
  }

  if(m_recrecord->GetEntry(HHEventRecord::tauvis1)->E() > mtau){
    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEEtaPhiM(m_recrecord->GetEntry(HHEventRecord::tauvis1)->E(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Phi(),
							       mtau);
  }
  else{
    m_fitrecord->UpdateEntry(HHEventRecord::tau2)->SetEEtaPhiM(pow(m_recrecord->GetEntry(HHEventRecord::tauvis1)->P(),2)+pow(mtau,2),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Eta(),
							       m_recrecord->GetEntry(HHEventRecord::tauvis1)->Phi(),
							       mtau);
  }
  
  
  ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);


  m_fitrecord->UpdateMothers(HHEventRecord::b1);
  m_fitrecord->UpdateMothers(HHEventRecord::tau1);

  if (!(maxEtau1>=m_recrecord->GetEntry(HHEventRecord::tauvis1)->E()) ){
    if(m_printlevel > 0)
      std::cout << "tautau mass constraint cannot be fulfilled -> reconstructed visible tau energy greater/smaller than maximal/minimal allowed total tau energy." << std::endl;
    m_convergence=-1;
    m_chi2=9999;
    m_chi2_b1=9999;
    m_chi2_b2=9999;
    m_chi2_balance=9999;
    m_fittedmH=-1;
    return;
  }

  // fill initial tau fit parameters
  astart[1] = m_fitrecord->GetEntry(HHEventRecord::tau1)->E();       // energy of first tau
  aprec[1] = 0.1;               //0.1                 // precision for fit
  astart[0] = m_fitrecord->GetEntry(HHEventRecord::b1)->E();         // energy of first b-jet
  aprec[0] = astart[0]*0.002;   //0.1                 // precision for fit
  m_tauStartEnergy = astart[1];
  m_bStartEnergy = astart[1];
  // fill initial step width
  h[0] = 0.5*m_fitrecord->GetEntry(HHEventRecord::b1)->dE();
  h[1] = 0.1*minEtau1;   //                

  daN[0] = 1.0;   //0.0                 // initial search direction in Eb-Etau diagonal
  daN[1] = 1.0;   //1.0

  // fit range
  alimit[0][0] = bjet1LowerLimit; 
  if (alimit[0][0]<0.01) alimit[0][0]=0.01;
  alimit[0][1] = bjet1UpperLimit;
  
  
  //EBJet2 Constraints
  HHV4Vector* bjet2_entry = m_fitrecord->GetEntry(HHEventRecord::b2);
  double E_bjet2 = bjet2_entry->E();
  double E_bjet2Low = bjet2LowerLimit;
  double E_bjet2High = bjet2UpperLimit;
  if( E_bjet2Low <= 0.01)
    E_bjet2Low = 0.01;

    if (m_logLevel>1)
    std::cout << "Setting bjet 2 Energy to " << E_bjet2Low << std::endl;

  if(m_keepMassesConst)
    m_fitrecord->UpdateEntry(HHEventRecord::b2)->SetEkeepM(E_bjet2Low);
  else
    m_fitrecord->UpdateEntry(HHEventRecord::b2)->SetEEtaPhiM(E_bjet2Low,                              //Set b2 to lower b2 limit
							     bjet2_entry->Eta(),
							     bjet2_entry->Phi(),
							     bjet2_entry->M() * E_bjet2Low/E_bjet2 );

  ConstrainE2(HHEventRecord::hb, HHEventRecord::b2, HHEventRecord::b1);                             //Calculate b1 with b2 fixed
  if(alimit[0][1] > m_fitrecord->GetEntry(HHEventRecord::b1)->E()){
    alimit[0][1] = m_fitrecord->GetEntry(HHEventRecord::b1)->E();                                   //Modify b1 limit if b2 limit is tighter
    if (m_logLevel>1)
      std::cout << "Upper bjet 1 limit is now " << alimit[0][1] << std::endl;
  }

  if (m_logLevel>1)
    std::cout << "Setting bjet 2 Energy to " << E_bjet2High << std::endl;

  if(m_keepMassesConst)
    m_fitrecord->UpdateEntry(HHEventRecord::b2)->SetEkeepM(E_bjet2High);
  else
    m_fitrecord->UpdateEntry(HHEventRecord::b2)->SetEEtaPhiM(E_bjet2High,                             //Set b2 to upper b2 limit
							     bjet2_entry->Eta(),
							     bjet2_entry->Phi(),
							     bjet2_entry->M() * E_bjet2High/E_bjet2Low );
  
  ConstrainE2(HHEventRecord::hb, HHEventRecord::b2, HHEventRecord::b1);                             //Calculate b1 with b2 fixed
  if(alimit[0][0] < m_fitrecord->GetEntry(HHEventRecord::b1)->E()){
    alimit[0][0] = m_fitrecord->GetEntry(HHEventRecord::b1)->E();                                   //Modify b1 limit if b2 limit is tighter
    if (m_logLevel>1)
      std::cout << "Lower bjet 1 limit is now " << alimit[0][0] << std::endl;
  }
  
  //Fill initial bjet fit parameters
  if(alimit[0][1] - alimit[0][0] > 0.5*m_fitrecord->GetEntry(HHEventRecord::b1)->dE()){
    astart[0] = alimit[0][0] + (alimit[0][1] - alimit[0][0])/2.0;         // energy of first b-jet
  }
  else{
    m_convergence=-2;
    m_chi2=9999;
    m_chi2_b1=9999;
    m_chi2_b2=9999;
    m_chi2_balance=9999;
    m_fittedmH=-1;
    return;
  }
  
  HHV4Vector* bjet1_entry = m_fitrecord->GetEntry(HHEventRecord::b1);
  Double_t bjet1_entryEnergy = bjet1_entry->E();
  if(m_keepMassesConst)
    m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEkeepM(astart[0]);
  else
    m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEEtaPhiM(astart[0], bjet1_entry->Eta(), bjet1_entry->Phi(), bjet1_entry->M()*astart[0]/bjet1_entryEnergy); 
  ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
  
  alimit[1][0] = minEtau1;              // tau: minimum is visible tau1 energy
  alimit[1][1] = maxEtau1;              //      maximum as computed above

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
  for (Int_t ip = 0; ip < np; ip++) {
    aMemory[ip][0] = -999.0;
    aMemory[ip][1] = -995.0;
    aMemory[ip][2] = -990.0;
    aMemory[ip][3] = -985.0;
    aMemory[ip][3] = -980.0;
  }

  static const Int_t nloopmax = 100;
  static Double_t Xa[nloopmax], Ya[nloopmax];
  static Double_t Xa1[nloopmax], Ya1[nloopmax];
  static Double_t HPx[nloopmax], HPy[nloopmax];
  static Double_t HPx1[nloopmax], HPy1[nloopmax];

  ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);
  ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);

  if (m_logLevel>1){
    std::cout << "Starting FitLoop! Start-Values: " << std::endl;
    std::cout << "Fit Params: " << std::endl;
    std::cout << "Eb1: " << m_fitrecord->GetEntry(HHEventRecord::b1)->E() << " Eb2: " << m_fitrecord->GetEntry(HHEventRecord::b2)->E() << std::endl;
    std::cout << "Eb1 Lower Limit: " << alimit[0][0] << "Eb1 Upper Limit: " << alimit[0][1] << std::endl; 
    std::cout << "Eb1 Precision is : " << aprec[0] << std::endl;
    std::cout << "Etau1: " << m_fitrecord->GetEntry(HHEventRecord::tau1)->E() << " Etau2: " << m_fitrecord->GetEntry(HHEventRecord::tau2)->E() << std::endl;
    std::cout << "ETau1 Lower Limit: " << alimit[1][0] << "ETau1 Upper Limit: " << alimit[1][1] << std::endl; 
    std::cout << "Etau1 Precision is : " << aprec[1] << std::endl;
    std::cout << "HH Px: " << m_fitrecord->GetEntry(HHEventRecord::H)->Px() << " HH Py: " << m_fitrecord->GetEntry(HHEventRecord::H)->Py() << std::endl;
  }

  for (Int_t iloop = 0; iloop < m_maxloops * 10 && iter < m_maxloops; iloop++) { // FIT loop
    
    if(m_keepMassesConst)
      m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEkeepM(a[0]); // update 4-vectors with fit parameters (No Mass Change)
    else{
      HHV4Vector* entry = m_fitrecord->GetEntry(HHEventRecord::b1);
      m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEEtaPhiM(a[0], entry->Eta(), entry->Phi(), entry->M()*a[0]/entry->E()); //Change b2 Mass
    }

    m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEkeepM(a[1]);
    ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
    ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);

    m_chi2_b1 = Chi2V4(HHEventRecord::b1);
    m_chi2_b2 = Chi2V4(HHEventRecord::b2);
    m_chi2_balance = Chi2Balance();
    m_chi2 = m_chi2_b1 + m_chi2_b2 + m_chi2_balance; // chi2 calculation

    if (m_logLevel>1){
      std::cout << std::setprecision(9);
      std::cout << "Fit Params: " << std::endl;
      std::cout << "Eb1:   " << a[0] << " Etau1:   " << a[1] << std::endl;
      if (m_logLevel>3){
	std::cout << "Eb1FR: " << m_fitrecord->GetEntry(HHEventRecord::b1)->E() << " Etau1FR: " << m_fitrecord->GetEntry(HHEventRecord::tau1)->E() << std::endl;
	std::cout << "Eb2FR: " << m_fitrecord->GetEntry(HHEventRecord::b2)->E() << " Etau2FR: " << m_fitrecord->GetEntry(HHEventRecord::tau2)->E() << std::endl;
      }
      std::cout << std::setprecision(6);
      std::cout << "chi2 b1: " << m_chi2_b1 << std::endl;
      std::cout << "chi2 b2: " << m_chi2_b2 << std::endl;
      std::cout << "chi2 missing: " << m_chi2_balance << std::endl;
      std::cout << "------------------" << std::endl;
      std::cout << "chi2 : " << m_chi2 << std::endl;
      std::cout << "------------------" << std::endl;
      std::cout << "------------------" << std::endl;
    }

//    chi2 = Chi2V4(recrecord, fitrecord, V4EventRecord::b1)
//        + Chi2V4(recrecord, fitrecord, V4EventRecord::b2)
//        + Chi2PTmiss(*ptmissrec, fitrecord);  // chi2 calculation

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

    //    if (iloop>=0 && iloop<nloopmax) {Xloop[iloop] = a[0] ;  Yloop[iloop] = a[1] ; }
    //    if (iter>=0) { nIterations = Iterations->SetNextPoint( V4fit[V4EventRecord::b1].E(), V4fit[V4EventRecord::tau1].E() ) ;   }
    //    if (iter>=0) { nIterations = Iterations->SetNextPoint( a[0], a[1] ) ;   }

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
  
  if(m_convergence != 0 && m_convergence != 5){
    if(a[0] < (alimit[0][0] + 2*aprec[0]) ){
      if (m_logLevel>1)
	std::cout << "Convergence at lower bjet limit!" << std::endl;
      m_convergence = 3;
    }
    if(a[0] > (alimit[0][1] - 2*aprec[0]) ){
      if (m_logLevel>1)
	std::cout << "Convergence at upper bjet limit!" << std::endl;
      m_convergence = 3;
    }
    if(a[1] < (alimit[1][0] + 2*aprec[1]) ){
      if(m_convergence == 3)
	m_convergence = 5;
      else{
	if (m_logLevel>1)
	  std::cout << "Convergence at lower tau limit!" << std::endl;
	m_convergence = 4;
      }
    }
    if(a[1] > (alimit[1][1] - 2*aprec[1]) ){
      if(m_convergence == 3)
	m_convergence = 5;
      else{
	if (m_logLevel>1)
	  std::cout << "Convergence at upper tau limit!" << std::endl;
	m_convergence = 4;
      }
    }
  }
  if (m_logLevel>1)
    std::cout << "Convergence is " << m_convergence << std::endl;
  
  /*
  TMatrixD Cov_MET     = m_recrecord->GetEntry(HHEventRecord::MET)->GetCov();
  TMatrixD Cov_b1      = m_recrecord->GetEntry(HHEventRecord::b1)->GetCov();
  TMatrixD Cov_b2      = m_recrecord->GetEntry(HHEventRecord::b2)->GetCov();
  TMatrixD Cov_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->GetCov();
  TMatrixD Cov_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->GetCov();
  
  TMatrixDEigen eigenmatrixMET(Cov_MET);
  
  
  if (eigenmatrix2.GetEigenValues()(0,0)<0 || eigenmatrix2.GetEigenValues()(1,1)<0){
    std::cout << "Recoil covariance matrix has negative Eigenvalues:" << std::endl;
    m_recrecord->Print("",1);
    std::cout << "COV_MET:    " << std::endl;
    PSTools::coutf(9, Cov_MET(0,0));
    PSTools::coutf(9, Cov_MET(0,1)); std::cout << std::endl;
    PSTools::coutf(9, Cov_MET(1,0));
    PSTools::coutf(9, Cov_MET(1,1)); std::cout << std::endl;

    std::cout << "COV_b1:     dE=" << m_recrecord->GetEntry(HHEventRecord::b1)->dE() << std::endl;
    PSTools::coutf(9, Cov_b1(0,0));
    PSTools::coutf(9, Cov_b1(0,1)); std::cout << std::endl;
    PSTools::coutf(9, Cov_b1(1,0));
    PSTools::coutf(9, Cov_b1(1,1)); std::cout << std::endl;

    std::cout << "COV_b2:     dE=" << m_recrecord->GetEntry(HHEventRecord::b2)->dE() << std::endl;
    PSTools::coutf(9, Cov_b2(0,0));
    PSTools::coutf(9, Cov_b2(0,1)); std::cout << std::endl;
    PSTools::coutf(9, Cov_b2(1,0));
    PSTools::coutf(9, Cov_b2(1,1)); std::cout << std::endl;

    std::cout << "COV_recoil: " << std::endl;
    PSTools::coutf(9, m_covRecoil(0,0));
    PSTools::coutf(9, m_covRecoil(0,1)); std::cout << std::endl;
    PSTools::coutf(9, m_covRecoil(1,0));
    PSTools::coutf(9, m_covRecoil(1,1)); std::cout << std::endl;

    std::cout << "eigenvalues COV_MET:" << std::endl;
    PSTools::coutf(9, eigenmatrixMET.GetEigenValues()(0,0));
    PSTools::coutf(9, eigenmatrixMET.GetEigenValues()(1,1)); std::cout << std::endl;

    std::cout << "eigenvalues COV_recoil:" << std::endl;
    PSTools::coutf(9, eigenmatrix.GetEigenValues()(0,0));
    PSTools::coutf(9, eigenmatrix.GetEigenValues()(1,1)); std::cout << std::endl;
  }
  */

  
  if (m_logLevel>0){
    Double_t px_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Px();
    Double_t py_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Py();

    Double_t px_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Px();
    Double_t py_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Py();

    Double_t px_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Px();
    Double_t py_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Py();

    Double_t px_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Px();
    Double_t py_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Py();

    Double_t px_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Px();
    Double_t py_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Py();

    Double_t px_H_reco = px_MET + px_b1 + px_b2 + px_tauvis1 + px_tauvis2;
    Double_t py_H_reco = py_MET + py_b1 + py_b2 + py_tauvis1 + py_tauvis2;

    std::cout << "MET:" << std::endl;
    PSTools::coutf(9, px_MET);
    PSTools::coutf(9, py_MET); std::cout << std::endl;

    std::cout << "recoil:" << std::endl;
    PSTools::coutf(9, -px_H_reco);
    PSTools::coutf(9, -py_H_reco); std::cout << std::endl;


  
    std::cout << "chi2 b1:      " << m_chi2_b1 << std::endl;
    std::cout << "chi2 b2:      " << m_chi2_b2 << std::endl;
    std::cout << "chi2 balance: " << m_chi2_balance << std::endl;
    std::cout << "CHI2 TOTAL:   " << m_chi2 << std::endl;
  }
}

void
HHKinFit::ConstrainE2(Int_t iv4, Int_t iv41, Int_t iv42)
{
  Double_t M1, M2, M, Mc, E1, E2, b2, beta2;
  m_fitrecord->UpdateMothers(iv41);
  Mc = m_particlelist->GetParticleInfo( m_fitrecord->GetEntry(iv4)->ID() )->M();
  M = m_fitrecord->GetEntry(iv4)->M();

  int loopCount = 0;
  
  while(fabs(M-Mc) > 0.000001){
    loopCount++;
    Int_t ID2 = m_fitrecord->GetEntry(iv42)->ID();
    if (ID2 == HHPID::tau && m_logLevel > 3)
      m_fitrecord->Print("Fit Record Before Constraining:");
    E1 = m_fitrecord->GetEntry(iv41)->E();
    M1 = m_fitrecord->GetEntry(iv41)->M();
    E2 = m_fitrecord->GetEntry(iv42)->E();
    M2 = m_fitrecord->GetEntry(iv42)->M();
    
    if (M2 == 0.) { // massless case
      if(m_logLevel > 0)
	std::cout << "Massless case!" << std::endl;
      m_fitrecord->UpdateEntry(iv42)->SetEkeepM(E2 * (Mc / M) * (Mc / M)); // only changes absolute value and keeps eta, phi, m untouched
      // such that invariant mass of 1 and 2 gives correct mass Mc
      m_fitrecord->UpdateMothers(iv42);
      return;
    }
    
    Int_t ID2jet = -1;
    
    if (ID2 == HHPID::q || ID2 == HHPID::c
	|| ID2 == HHPID::b || ID2 == HHPID::gluon)
      ID2jet = 1;
    
    if(m_keepMassesConst)
    ID2jet = -1;
    
    beta2 = sqrt(E2 * E2 - M2 * M2) / E2;
    if (ID2jet < 0) { // is not a jet
      b2 = (M2 / E2) * (M2 / E2); // isn't the no jet case identical to the jet case? 1/gamma**2 = m**2/E**2 = 1-beta**2
    }
    else { // is a jet
      b2 = 1 - beta2 * beta2;
    }

    //std::cout << "beta2   b2: " << beta2 << "  " << b2 << std::endl;  
    
    Double_t d = (M * M - M1 * M1 - M2 * M2) / (2. * E1 * E2);
    Double_t E2lin = (Mc * Mc - M1 * M1) / (2. * E1 * d);
    Double_t E2N = E1 * d / b2;
    Double_t E2new = E2N * (-1. + sqrt(1. + 2. * E2lin / E2N));
    
    //std::cout << "d, E2lin, E2N, E2new: " << d << "  " << E2lin << "  " << E2N << "  " << E2new << std::endl;  
    
    if (ID2jet < 0) { // is not a jet
      m_fitrecord->UpdateEntry(iv42)->SetEkeepM(E2new);
    }
    else { // is a jet
      HHV4Vector* entry = m_fitrecord->GetEntry(iv42);
      m_fitrecord->UpdateEntry(iv42)->SetEEtaPhiM(E2new, entry->Eta(), entry->Phi(), M2 * E2new / E2);
      if(m_logLevel > 2)
	std::cout << "Jet Masses are now - BJet1 Mass: " << m_fitrecord->GetEntry(iv41)->M() << " BJet2 Mass: " << m_fitrecord->GetEntry(iv42)->M() << std::endl;
    }
    
    m_fitrecord->UpdateMothers(iv42);
    M = m_fitrecord->GetEntry(iv4)->M();

    if (ID2 == HHPID::tau && m_logLevel > 3 )
      m_fitrecord->Print("Fit Record After Constraining:");
  }

  if(m_logLevel > 1){
    std::cout << "Did Mass Constraint Loop " << loopCount << " times." << std::endl;
  }

}

Double_t
HHKinFit::Chi2V4(Int_t iv4)
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

Double_t
HHKinFit::Chi2Balance()
{

  //fit objets
  Double_t px_H_fit   = m_fitrecord->GetEntry(HHEventRecord::H)->Px();
  Double_t py_H_fit   = m_fitrecord->GetEntry(HHEventRecord::H)->Py();


  //reco objects
  Double_t px_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Px();
  Double_t py_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Py();

  Double_t px_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Px();
  Double_t py_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Py();

  Double_t px_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Px();
  Double_t py_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Py();

  Double_t px_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Px();
  Double_t py_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Py();

  Double_t px_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Px();
  Double_t py_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Py();
 
  Double_t px_H_reco = px_MET + px_b1 + px_b2 + px_tauvis1 + px_tauvis2 + 1.0;   //+ 1.0 just a test. Remove!
  Double_t py_H_reco = py_MET + py_b1 + py_b2 + py_tauvis1 + py_tauvis2 + 1.0;   //+ 1.0 just a test. Remove!
  Double_t pt_H_reco = sqrt(pow(px_H_reco,2) + pow(py_H_reco,2));

  Double_t res[2];
  res[0] = px_H_fit - px_H_reco;    // residuum in Pt_H
  res[1] = py_H_fit - py_H_reco;    // residuum in Pt_H
  
  Double_t Vxx = m_covRecoil(0,0);
  Double_t Vyy = m_covRecoil(1,1);
  Double_t Vxy = m_covRecoil(0,1);

  Double_t det, Vinvxx, Vinvyy, Vinvxy;
  det = Vxx * Vyy - Vxy * Vxy;
  Vinvxx = Vyy / det;
  Vinvyy = Vxx / det;
  Vinvxy = -Vxy / det;

  Double_t Vinv[2 * 2];
  Vinv[0] = Vinvxx;
  Vinv[1] = Vinvxy;
  Vinv[2] = Vinv[1];
  Vinv[3] = Vinvyy;

  Double_t chi2=-1;
  if (m_advancedBalance){
    chi2 = res[0] * (Vinv[0] * res[0] + Vinv[1] * res[1]) // chi2 = res_transponiert * Vinv * res
         + res[1] * (Vinv[2] * res[0] + Vinv[3] * res[1]);
  }
  else{
    chi2 =  pow(( pt_H_reco - m_fitrecord->GetEntry(HHEventRecord::H)->Pt()), 2)
          / pow((m_recrecord->GetEntry(HHEventRecord::MET)->dE()), 2);
  }

  
  if (m_logLevel>2){
    std::cout << "P_H_Fit - x: " << px_H_fit << "  y: " << py_H_fit << std::endl;
    std::cout << "P_H_reco - x: " << px_H_reco << "  y: " << py_H_reco << std::endl;
    std::cout << "res[0] " << res[0] << "  res[1] " << res[1] << std::endl;
    std::cout << "V[0] " << Vxx << "  V[1] " << Vxy << std::endl;
    std::cout << "V[2] " << Vxy << "  V[3] " << Vyy << std::endl;
    std::cout << "Vinv[0] " << Vinv[0] << "  Vinv[1] " << Vinv[1] << std::endl;
    std::cout << "Vinv[2] " << Vinv[2] << "  Vinv[3] " << Vinv[3] << std::endl;
  }

  if (m_logLevel>1){
    PSTools::coutf(5, TString("pmx"));
    PSTools::coutf(9, 2, px_H_fit);
    PSTools::coutf(9, 2, px_H_reco);
    PSTools::coutf(9, 2, res[0]);
    PSTools::coutf(3, TString("|"));
    PSTools::coutf(9, 2, sqrt(Vxx));
    std::cout << std::endl;

    PSTools::coutf(5, TString("pmy"));
    PSTools::coutf(9, 2, py_H_fit);
    PSTools::coutf(9, 2, py_H_reco);
    PSTools::coutf(9, 2, res[1]);
    PSTools::coutf(3, TString("|"));
    PSTools::coutf(9, 2, sqrt(Vyy));
    PSTools::coutf(3, TString("->"));
    PSTools::coutf(9, 2, chi2);
    std::cout << std::endl;
    std::cout << "--------------" << std::endl;
  }

  return chi2;
}

Chi2Map HHKinFit::CreateChi2Map(int NbVariations, int NtauVariations){
  Chi2Map returnMap;

  HHV4Vector* b1entry = m_fitrecord->GetEntry(HHEventRecord::b1);
  double b1FitEnergy = b1entry->E();

  HHV4Vector* tau1entry = m_fitrecord->GetEntry(HHEventRecord::tau1);
  double tau1FitEnergy = tau1entry->E();


  for(int bVariation = 0; bVariation <= NbVariations; ++bVariation){
    for(int tauVariation = 0; tauVariation <= NtauVariations; ++tauVariation){
      double b1Energy;
      if(NbVariations != 0)
	b1Energy = b1FitEnergy + (2.0*(float)bVariation/(float)NbVariations - 1.0) * 5.0* m_fitrecord->GetEntry(HHEventRecord::b1)->dE();
      else
	b1Energy = b1FitEnergy;
      if(b1Energy < 0.01)
	b1Energy = 0.01;

      double tau1Energy;
      if(NtauVariations != 0)
	tau1Energy = tau1FitEnergy + (2.0*(float)tauVariation/(float)NtauVariations - 1.0) * 0.7 * tau1FitEnergy;
      else
	tau1Energy = tau1FitEnergy;

      std::pair<double, double> energies = std::pair<double, double>(b1Energy, tau1Energy);

      m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEEtaPhiM(b1Energy, b1entry->Eta(), b1entry->Phi(), b1entry->M()*b1Energy/b1entry->E()); //Change b2 Mass
      m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEkeepM(tau1Energy);
      ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
      ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);

      double chi2_b1 = Chi2V4(HHEventRecord::b1);
      double chi2_b2 = Chi2V4(HHEventRecord::b2);
      double chi2_balance = Chi2Balance();
      double chi2 = chi2_b1 + chi2_b2 + chi2_balance;

      if (m_logLevel>1){
	std::cout << "Fit Params: " << std::endl;
	std::cout << std::setprecision(9);
	std::cout << "Eb1: " << b1Energy << " Etau1: " << tau1Energy << std::endl;
	std::cout << "Eb1FR: " << m_fitrecord->GetEntry(HHEventRecord::b1)->E() << " Etau1FR: " << m_fitrecord->GetEntry(HHEventRecord::tau1)->E() << std::endl;
	std::cout << "Eb2FR: " << m_fitrecord->GetEntry(HHEventRecord::b2)->E() << " Etau2FR: " << m_fitrecord->GetEntry(HHEventRecord::tau2)->E() << std::endl;
	std::cout << std::setprecision(6);
	std::cout << "chi2 b1: " << chi2_b1 << std::endl;
	std::cout << "chi2 b2: " << chi2_b2 << std::endl;
	std::cout << "chi2 missing: " << chi2_balance << std::endl;
	std::cout << "------------------" << std::endl;
	std::cout << "chi2 : " << chi2 << std::endl;
	std::cout << "------------------" << std::endl;
	std::cout << "------------------" << std::endl;
      }

      returnMap.insert(std::pair<std::pair<double, double>, double>(energies, chi2));
    }
  }

  m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEEtaPhiM(b1FitEnergy, b1entry->Eta(), b1entry->Phi(), b1entry->M()*b1FitEnergy/b1entry->E()); //Change b2 Mass
  m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEkeepM(tau1FitEnergy);
  ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
  ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);

  return returnMap;
}

Chi2Map HHKinFit::CreateChi2MapAroundStartvalues(int NbVariations, int NtauVariations){
  Chi2Map returnMap;

  HHV4Vector* b1entry = m_fitrecord->GetEntry(HHEventRecord::b1);
  double b1FitEnergy = b1entry->E();

  HHV4Vector* tau1entry = m_fitrecord->GetEntry(HHEventRecord::tau1);
  double tau1FitEnergy = tau1entry->E();

  for(int bVariation = 0; bVariation < NbVariations; ++bVariation){
    for(int tauVariation = 0; tauVariation < NtauVariations; ++tauVariation){
      double b1Energy = m_bStartEnergy + ((float)bVariation/(float)NbVariations - 0.5) * 5.0 * b1entry->dE();
      if(b1Energy < 0.01)
	b1Energy = 0.01;

      double tau1Energy = m_tauStartEnergy + ((float)tauVariation/(float)NtauVariations - 0.5) * 0.8 * m_tauStartEnergy;

      std::pair<double, double> energies = std::pair<double, double>(b1Energy, tau1Energy);

      m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEEtaPhiM(b1Energy, b1entry->Eta(), b1entry->Phi(), b1entry->M()*b1Energy/b1entry->E()); //Change b2 Mass
      m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEkeepM(tau1Energy);
      ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
      ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);

      double chi2_b1 = Chi2V4(HHEventRecord::b1);
      double chi2_b2 = Chi2V4(HHEventRecord::b2);
      double chi2_balance = Chi2Balance();
      double chi2 = chi2_b1 + chi2_b2 + chi2_balance;

      returnMap.insert(std::pair<std::pair<double, double>, double>(energies, chi2));
    }
  }

  m_fitrecord->UpdateEntry(HHEventRecord::b1)->SetEEtaPhiM(b1FitEnergy, b1entry->Eta(), b1entry->Phi(), b1entry->M()*b1FitEnergy/b1entry->E()); //Change b2 Mass
  m_fitrecord->UpdateEntry(HHEventRecord::tau1)->SetEkeepM(tau1FitEnergy);
  ConstrainE2(HHEventRecord::hb, HHEventRecord::b1, HHEventRecord::b2);
  ConstrainE2(HHEventRecord::htau, HHEventRecord::tau1, HHEventRecord::tau2);

  return returnMap;
}

void
HHKinFit::SetPrintLevel(Int_t level){
  m_printlevel = level;
}

void
HHKinFit::SetGraphicsLevel(Int_t level){
  m_graphicslevel=level;
}

void
HHKinFit::SetMaxLoops(Int_t loops){
  m_maxloops = loops;
}

Double_t
HHKinFit::GetChi2()
{
  return m_chi2;
}

Double_t
HHKinFit::GetChi2_b1()
{
  return m_chi2_b1;
}

Double_t
HHKinFit::GetChi2_b2()
{
  return m_chi2_b2;
}

Double_t
HHKinFit::GetChi2_balance()
{
  return m_chi2_balance;
}

Double_t
HHKinFit::GetFittedMH()
{
  return m_fittedmH;
}

Double_t
HHKinFit::GetConvergence()
{
  return m_convergence;
}

HHEventRecord*
HHKinFit::GetFitrecord(){
  return m_fitrecord;
}

void
HHKinFit::SetAdvancedBalance(Bool_t flag)
{
  m_advancedBalance = flag;
}

void
HHKinFit::SetLogLevel(Int_t level)
{
  m_logLevel = level;
}


double
HHKinFit::GetPullE(Int_t iv4){
  HHV4Vector* fv = m_fitrecord->GetEntry(iv4);
  HHV4Vector* rv = m_recrecord->GetEntry(iv4);
  return (fv->E()-rv->E())/rv->dE();
}


double
HHKinFit::GetPullBalance(){
  Double_t px_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Px();
  Double_t py_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Py();

  Double_t px_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Px();
  Double_t py_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Py();

  Double_t px_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Px();
  Double_t py_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Py();

  Double_t px_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Px();
  Double_t py_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Py();

  Double_t px_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Px();
  Double_t py_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Py();

  Double_t px_H_reco = px_MET + px_b1 + px_b2 + px_tauvis1 + px_tauvis2;
  Double_t py_H_reco = py_MET + py_b1 + py_b2 + py_tauvis1 + py_tauvis2;
  Double_t pt_H_reco = sqrt(pow(px_H_reco,2) + pow(py_H_reco,2));


  Double_t pull=-99;
  if (m_advancedBalance){
    if(m_chi2_balance >=0 ) 
      pull = sqrt(m_chi2_balance);
  }
  else
    pull =  (pt_H_reco - m_fitrecord->GetEntry(HHEventRecord::H)->Pt()) / (m_recrecord->GetEntry(HHEventRecord::MET)->dE());
  
  return pull;
}

double
HHKinFit::GetPullBalanceX(){

  Double_t px_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Px();

  Double_t px_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Px();
  Double_t px_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Px();
  Double_t px_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Px();
  Double_t px_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Px();
 
  Double_t px_H_reco = px_MET + px_b1 + px_b2 + px_tauvis1 + px_tauvis2;

  Double_t pull= -999;
  
  if(m_advancedBalance)
    pull =  (px_H_reco - m_fitrecord->GetEntry(HHEventRecord::H)->Px())/sqrt(m_covRecoil(0,0) );
  
  return pull;
}

double
HHKinFit::GetPullBalanceY(){

  Double_t py_MET = m_recrecord->GetEntry(HHEventRecord::MET)->Py();

  Double_t py_b1 = m_recrecord->GetEntry(HHEventRecord::b1)->Py();
  Double_t py_b2 = m_recrecord->GetEntry(HHEventRecord::b2)->Py();
  Double_t py_tauvis1 = m_recrecord->GetEntry(HHEventRecord::tauvis1)->Py();
  Double_t py_tauvis2 = m_recrecord->GetEntry(HHEventRecord::tauvis2)->Py();
 
  Double_t py_H_reco = py_MET + py_b1 + py_b2 + py_tauvis1 + py_tauvis2;

  Double_t pull= -999;
  
  if(m_advancedBalance)
    pull =  (py_H_reco - m_fitrecord->GetEntry(HHEventRecord::H)->Py())/sqrt(m_covRecoil(1,1) );
  
  return pull;
}
