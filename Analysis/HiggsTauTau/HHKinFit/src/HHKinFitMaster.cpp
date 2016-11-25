#include "../include/HHKinFitMaster.h"

#include "../include/HHEventRecord.h"
#include "../include/HHKinFit.h"
#include "../include/HHParticleList.h"
#include "../include/HHPID.h"
#include "../include/HHV4Vector.h"
#include "TMatrixD.h"

#include "TRandom3.h"

#include <TMath.h>
#include <cmath>
#include <cstdlib>
#include <iterator>

void
HHKinFitMaster::doFullFit()
{
  //Setup event
  HHParticleList* particlelist = new HHParticleList();
  HHEventRecord eventrecord_rec(particlelist);

  eventrecord_rec.UpdateEntry(HHEventRecord::tauvis1)->SetVector(*m_tauvis1);
  eventrecord_rec.UpdateEntry(HHEventRecord::tauvis2)->SetVector(*m_tauvis2);

  eventrecord_rec.UpdateEntry(HHEventRecord::b1)->SetVector(*m_bjet1);
  if(m_truthInput)
    eventrecord_rec.UpdateEntry(HHEventRecord::b1)->SetErrors(m_bjet1Smear, 0, 0);
  else
    eventrecord_rec.UpdateEntry(HHEventRecord::b1)->SetErrors(GetBjetResolution(m_bjet1->Eta(),m_bjet1->Et()), 0, 0);

  eventrecord_rec.UpdateEntry(HHEventRecord::b2)->SetVector(*m_bjet2);
  if(m_truthInput)
    eventrecord_rec.UpdateEntry(HHEventRecord::b2)->SetErrors(m_bjet2Smear, 0, 0);
  else
    eventrecord_rec.UpdateEntry(HHEventRecord::b2)->SetErrors(GetBjetResolution(m_bjet2->Eta(),m_bjet2->Et()), 0, 0);

  if (!m_advancedBalance){
    eventrecord_rec.UpdateEntry(HHEventRecord::MET)->SetEEtaPhiM(m_simpleBalancePt,0,0,0);
    eventrecord_rec.UpdateEntry(HHEventRecord::MET)->SetErrors(m_simpleBalanceUncert,0,0);
  }
  else{
    if ((m_MET != NULL) && (m_MET_COV.IsValid())){
      eventrecord_rec.UpdateEntry(HHEventRecord::MET)->SetVector(*m_MET);
      eventrecord_rec.UpdateEntry(HHEventRecord::MET)->SetCov(m_MET_COV);
    }
  }
  //loop over all hypotheses
  for(std::vector<Int_t>::iterator mh1 = m_mh1.begin(); mh1 != m_mh1.end(); mh1++){
    for(std::vector<Int_t>::iterator mh2 = m_mh2.begin(); mh2 != m_mh2.end(); mh2++){
      particlelist->UpdateMass(HHPID::h1, *mh1);
      particlelist->UpdateMass(HHPID::h2, *mh2);

      HHKinFit advancedfitter(&eventrecord_rec);
      advancedfitter.SetPrintLevel(0);
      advancedfitter.SetLogLevel(0);
      advancedfitter.SetAdvancedBalance(m_advancedBalance);
      advancedfitter.Fit();
      
      Double_t chi2_full = advancedfitter.GetChi2();
      Double_t chi2_bjet1 = advancedfitter.GetChi2_b1();
      Double_t chi2_bjet2 = advancedfitter.GetChi2_b2();
      Double_t chi2_balance = advancedfitter.GetChi2_balance();
      Double_t prob_full = TMath::Prob(chi2_full,2);
      Double_t mH_full   = advancedfitter.GetFittedMH();
      std::pair< Int_t, Int_t > hypo_full(*mh1,*mh2);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_chi2_full (hypo_full, chi2_full);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_chi2_bjet1 (hypo_full, chi2_bjet1);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_chi2_bjet2 (hypo_full, chi2_bjet2);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_chi2_balance (hypo_full, chi2_balance);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_fitprob_full (hypo_full, prob_full);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_mH_full (hypo_full, mH_full);
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullb1_full (hypo_full, advancedfitter.GetPullE(HHEventRecord::b1));
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullb2_full (hypo_full, advancedfitter.GetPullE(HHEventRecord::b2));
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullbalance_full (hypo_full, advancedfitter.GetPullBalance());
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullbalance_fullX (hypo_full, advancedfitter.GetPullBalanceX());
      std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullbalance_fullY (hypo_full, advancedfitter.GetPullBalanceY());
      std::pair< std::pair< Int_t, Int_t >, Int_t >    entry_convergence_full (hypo_full, advancedfitter.GetConvergence());
      m_fullFitResultChi2.insert(entry_chi2_full);
      m_fullFitResultChi2BJet1.insert(entry_chi2_bjet1);
      m_fullFitResultChi2BJet2.insert(entry_chi2_bjet2);
      m_fullFitResultChi2Balance.insert(entry_chi2_balance);
      m_fullFitResultFitProb.insert(entry_fitprob_full);
      m_fullFitResultMH.insert(entry_mH_full);
      m_fullFitPullB1.insert(entry_pullb1_full);
      m_fullFitPullB2.insert(entry_pullb2_full);
      m_fullFitPullBalance.insert(entry_pullbalance_full);
      m_fullFitPullBalanceX.insert(entry_pullbalance_fullX);
      m_fullFitPullBalanceY.insert(entry_pullbalance_fullY);
      m_fullFitConvergence.insert(entry_convergence_full);
      if (chi2_full<m_bestChi2FullFit) {
        m_bestChi2FullFit = chi2_full;
        m_bestHypoFullFit = hypo_full;
        m_bestMHFullFit = mH_full;
      }
      
      m_bjet1_fitted = advancedfitter.GetFitParticle(HHEventRecord::b1);
      m_bjet2_fitted = advancedfitter.GetFitParticle(HHEventRecord::b2);
      m_tau1_fitted = advancedfitter.GetFitParticle(HHEventRecord::tau1);
      m_tau2_fitted = advancedfitter.GetFitParticle(HHEventRecord::tau2);
      m_fixedCovMatrix = advancedfitter.m_fixedCovMatrix;
      


      /*
      if( entry_convergence_full.second == 0 ){
      	Chi2Map chi2Map = advancedfitter.CreateChi2Map(15, 100);
	
	TCanvas* c1 = new TCanvas("canvas1");
	TGraph2D* graph2d = new TGraph2D( chi2Map.size() );
	
	int pointN = 0;
	for(Chi2Map::iterator iter = chi2Map.begin(); iter != chi2Map.end(); iter++){
	  graph2d->SetPoint(pointN, iter->first.first, iter->first.second, iter->second);
	  pointN++;
	}
	
	std::stringstream fileNameStream;
	TString fileName;
	fileNameStream << "MinB1_" << m_bjet1_fitted.E() << "_MinTau1_" << m_tau1_fitted.E() << std::endl;
	fileNameStream >> fileName;
	
	graph2d->Draw("Cont3COLZ");
	c1->SaveAs(fileName + ".pdf");
	
	/*
	Chi2Map chi2MapAroundStartvalues = advancedfitter.CreateChi2MapAroundStartvalues(15, 10);
	
	TCanvas* c2 = new TCanvas("canvas2");
	TGraph2D* graph2dAroundStartvalues = new TGraph2D( chi2MapAroundStartvalues.size() );
	
	pointN = 0;
	for(Chi2Map::iterator iter = chi2MapAroundStartvalues.begin(); iter != chi2MapAroundStartvalues.end(); iter++){
	  graph2dAroundStartvalues->SetPoint(pointN, iter->first.first, iter->first.second, iter->second);
	  pointN++;
	}
	
	graph2dAroundStartvalues->Draw("Cont3COLZ");
	c2->SaveAs(fileName + "AroundStartvalues.pdf");
	
	Chi2Map chi2MapTauValues = advancedfitter.CreateChi2Map(0, 100);

	TCanvas* c3 = new TCanvas("canvas3");
	TGraph* graph1dTauValues = new TGraph( chi2MapTauValues.size() );
	
	pointN = 0;
	for(Chi2Map::iterator iter = chi2MapTauValues.begin(); iter != chi2MapTauValues.end(); iter++){
	  graph1dTauValues->SetPoint(pointN, iter->first.second, iter->second);
	  pointN++;
	}

	graph1dTauValues->Draw("AC*");
	c3->SaveAs(fileName + "TauValues.pdf");
	
	
	delete graph1dTauValues;
	//delete graph2dAroundStartvalues;
	//delete c2;
	
	delete c3;
	
	delete c1;
	delete graph2d;
      }
      */
    }
  }
  
  delete particlelist;
}



HHKinFitMaster::HHKinFitMaster(TLorentzVector* bjet1, TLorentzVector* bjet2, TLorentzVector* tauvis1, TLorentzVector* tauvis2, Bool_t truthinput, TLorentzVector* heavyhiggsgen):
    m_mh1(std::vector<Int_t>()),
    m_mh2(std::vector<Int_t>()),

    m_bjet1(bjet1),
    m_bjet2(bjet2),
    m_tauvis1(tauvis1),
    m_tauvis2(tauvis2),

    m_MET(NULL),
    m_MET_COV(TMatrixD(2,2)),

    m_truthInput(truthinput),
    m_advancedBalance(false),
    m_simpleBalancePt(0.0),
    m_simpleBalanceUncert(10.0),
    m_fullFitResultChi2(std::map< std::pair<Int_t,Int_t> , Double_t>()),
    m_fullFitResultMH(std::map< std::pair<Int_t,Int_t> , Double_t>()),
    m_bestChi2FullFit(999),
    m_bestMHFullFit(-1),
    m_bestHypoFullFit(std::pair<Int_t, Int_t>(-1,-1) )
{
  if (m_truthInput){
    TRandom3 r(0);   
    
    m_bjet1Smear = GetBjetResolution(bjet1->Eta(), bjet1->Et());
    Double_t bjet1_E  = r.Gaus(bjet1->E(), m_bjet1Smear);
    Double_t bjet1_P  = sqrt(pow(bjet1_E,2) - pow(bjet1->M(),2));
    Double_t bjet1_Pt = sin(bjet1->Theta())*bjet1_P;

    std::cout << "Jet1 smeared by: " <<   (bjet1_E - bjet1->E())/m_bjet1Smear << std::endl;

    bjet1->SetPtEtaPhiE(bjet1_Pt, bjet1->Eta(), bjet1->Phi(), bjet1_E);
    
    TMatrixD bjet1Cov(2,2);
    Double_t bjet1_dpt = sin(bjet1->Theta())*bjet1->E()/bjet1->P()*m_bjet1Smear;  // error propagation p=sqrt(e^2-m^2)
    bjet1Cov(0,0) = pow(cos(bjet1->Phi())*bjet1_dpt,2);                           bjet1Cov(0,1) = sin(bjet1->Phi())*cos(bjet1->Phi())*bjet1_dpt*bjet1_dpt;
    bjet1Cov(1,0) = sin(bjet1->Phi())*cos(bjet1->Phi())*bjet1_dpt*bjet1_dpt;      bjet1Cov(1,1) = pow(sin(bjet1->Phi())*bjet1_dpt,2);
   
    Double_t bjet2_res = GetBjetResolution(bjet2->Eta(), bjet2->Et());
    m_bjet2Smear = GetBjetResolution(bjet2->Eta(), bjet2->Et());
    Double_t bjet2_E  = r.Gaus(bjet2->E(), m_bjet2Smear);
    Double_t bjet2_P  = sqrt(pow(bjet2_E,2) - pow(bjet2->M(),2));
    Double_t bjet2_Pt = sin(bjet2->Theta())*bjet2_P;

    std::cout << "Jet1 smeared by: " <<   (bjet2_E - bjet2->E())/m_bjet2Smear << std::endl;

    bjet2->SetPtEtaPhiE(bjet2_Pt, bjet2->Eta(), bjet2->Phi(), bjet2_E);
    
    TMatrixD bjet2Cov(2,2);
    Double_t bjet2_dpt = sin(bjet2->Theta())*bjet2->E()/bjet2->P()*m_bjet2Smear;  // error propagation p=sqrt(e^2-m^2)
    bjet2Cov(0,0) = pow(cos(bjet2->Phi())*bjet2_dpt,2);                           bjet2Cov(0,1) = sin(bjet2->Phi())*cos(bjet2->Phi())*bjet2_dpt*bjet2_dpt;
    bjet2Cov(1,0) = sin(bjet2->Phi())*cos(bjet2->Phi())*bjet2_dpt*bjet2_dpt;      bjet2Cov(1,1) = pow(sin(bjet2->Phi())*bjet2_dpt,2);


    TLorentzVector* recoil;
    if(heavyhiggsgen != NULL){
       Double_t pxRecoil = r.Gaus(-(heavyhiggsgen->Px() ), 10.0);
       Double_t pyRecoil = r.Gaus(-(heavyhiggsgen->Py() ), 10.0);
       std::cout << "Higgs Recoil X smeared by: " << pxRecoil + heavyhiggsgen->Px() << std::endl;
       std::cout << "Higgs Recoil Y smeared by: " << pyRecoil + heavyhiggsgen->Py() << std::endl;
       recoil = new TLorentzVector(pxRecoil,pyRecoil,0,sqrt(pxRecoil*pxRecoil+pyRecoil*pyRecoil));
    }
    else{
      recoil = new TLorentzVector(0,0,0,0);
      std::cout << "WARNING! Truthinput mode active but no Heavy Higgs gen-information given! Setting Recoil to Zero!" << std::endl;  
    }
    
    TMatrixD recoilCov(2,2);
    recoilCov(0,0)=100;  recoilCov(0,1)=0;
    recoilCov(1,0)=0;    recoilCov(1,1)=100;

    TLorentzVector* met = new TLorentzVector(-(*bjet1 + *bjet2 + *tauvis1 + *tauvis2 + *recoil));
    
    TMatrixD metCov(2,2);
    metCov = recoilCov + bjet1Cov + bjet2Cov;
    
    setAdvancedBalance(met, metCov);

    m_bjet1_smeared = *bjet1;
    m_bjet2_smeared = *bjet2;
    m_met_smeared = *met;

    delete recoil;
  }
}

Double_t
HHKinFitMaster::getBestChi2FullFit()
{
  return m_bestChi2FullFit;
}

Double_t
HHKinFitMaster::getBestMHFullFit()
{
  return m_bestMHFullFit;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getChi2FullFit(){
  return m_fullFitResultChi2;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getFitProbFullFit(){
  return m_fullFitResultFitProb;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getMHFullFit(){
  return m_fullFitResultMH;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getPullB1FullFit(){
  return m_fullFitPullB1;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getPullB2FullFit(){
  return m_fullFitPullB2;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getPullBalanceFullFit(){
  return m_fullFitPullBalance;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getPullBalanceFullFitX(){
  return m_fullFitPullBalanceX;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHKinFitMaster::getPullBalanceFullFitY(){
  return m_fullFitPullBalanceY;
}

std::map< std::pair< Int_t, Int_t >, Int_t >
HHKinFitMaster::getConvergenceFullFit(){
  return m_fullFitConvergence;
}

std::pair<Int_t, Int_t>
HHKinFitMaster::getBestHypoFullFit()
{
  return m_bestHypoFullFit;
}


void
HHKinFitMaster::addMh1Hypothesis(Double_t m1, Double_t m2, Double_t m3, Double_t m4, Double_t m5, Double_t m6, Double_t m7, Double_t m8, Double_t m9, Double_t m10)
{
  if (m1 != 0) m_mh1.push_back(m1);
  if (m2 != 0) m_mh1.push_back(m2);
  if (m3 != 0) m_mh1.push_back(m3);
  if (m4 != 0) m_mh1.push_back(m4);
  if (m5 != 0) m_mh1.push_back(m5);
  if (m6 != 0) m_mh1.push_back(m6);
  if (m7 != 0) m_mh1.push_back(m7);
  if (m8 != 0) m_mh1.push_back(m8);
  if (m9 != 0) m_mh1.push_back(m9);
  if (m10 != 0) m_mh1.push_back(m10);
}

void
HHKinFitMaster::addMh1Hypothesis(std::vector<Int_t> v)
{
  m_mh1 = v;
}

void
HHKinFitMaster::addMh2Hypothesis(Double_t m1, Double_t m2, Double_t m3, Double_t m4, Double_t m5, Double_t m6, Double_t m7, Double_t m8, Double_t m9, Double_t m10)
{
  if (m1 != 0) m_mh2.push_back(m1);
  if (m2 != 0) m_mh2.push_back(m2);
  if (m3 != 0) m_mh2.push_back(m3);
  if (m4 != 0) m_mh2.push_back(m4);
  if (m5 != 0) m_mh2.push_back(m5);
  if (m6 != 0) m_mh2.push_back(m6);
  if (m7 != 0) m_mh2.push_back(m7);
  if (m8 != 0) m_mh2.push_back(m8);
  if (m9 != 0) m_mh2.push_back(m9);
  if (m10 != 0) m_mh2.push_back(m10);
}

void
HHKinFitMaster::addMh2Hypothesis(std::vector<Int_t> v)
{
  m_mh2 = v;
}

void
HHKinFitMaster::setAdvancedBalance(TLorentzVector* met, TMatrixD met_cov)
{
  m_advancedBalance = true;
  m_MET = met;
  m_MET_COV = met_cov;
}

void
HHKinFitMaster::setSimpleBalance(Double_t balancePt, Double_t balanceUncert)
{
  m_advancedBalance = false;
  m_simpleBalancePt = balancePt;
  m_simpleBalanceUncert = balanceUncert;
}

double
HHKinFitMaster::GetBjetResolution(double eta, double et){
  double det=0;
  double de=10;

  if(0.000<=abs(eta) && abs(eta)<0.087){
  det = et * (sqrt(0.0686*0.0686 + (1.03/sqrt(et))*(1.03/sqrt(et)) + (1.68/et)*(1.68/et)));
  de = 1.0/sin(2 * atan(exp(-(0.000+0.087)/2))) * det;
  }

  if(0.087<=abs(eta) && abs(eta)<0.174){
  det = et * (sqrt(0.0737*0.0737 + (1.01/sqrt(et))*(1.01/sqrt(et)) + (1.74/et)*(1.74/et)));
  de = 1.0/sin(2 * atan(exp(-(0.087+0.174)/2))) * det;
  }

  if(0.174<=abs(eta) && abs(eta)<0.261){
  det = et * (sqrt(0.0657*0.0657 + (1.07/sqrt(et))*(1.07/sqrt(et)) + (5.16e-06/et)*(5.16e-06/et)));
  de = 1.0/sin(2 * atan(exp(-(0.174+0.261)/2))) * det;
  }

  if(0.261<=abs(eta) && abs(eta)<0.348){
  det = et * (sqrt(0.062*0.062 + (1.07/sqrt(et))*(1.07/sqrt(et)) + (0.000134/et)*(0.000134/et)));
  de = 1.0/sin(2 * atan(exp(-(0.261+0.348)/2))) * det;
  }

  if(0.348<=abs(eta) && abs(eta)<0.435){
  det = et * (sqrt(0.0605*0.0605 + (1.07/sqrt(et))*(1.07/sqrt(et)) + (1.84e-07/et)*(1.84e-07/et)));
  de = 1.0/sin(2 * atan(exp(-(0.348+0.435)/2))) * det;
  }

  if(0.435<=abs(eta) && abs(eta)<0.522){
  det = et * (sqrt(0.059*0.059 + (1.08/sqrt(et))*(1.08/sqrt(et)) + (9.06e-09/et)*(9.06e-09/et)));
  de = 1.0/sin(2 * atan(exp(-(0.435+0.522)/2))) * det;
  }

  if(0.522<=abs(eta) && abs(eta)<0.609){
  det = et * (sqrt(0.0577*0.0577 + (1.08/sqrt(et))*(1.08/sqrt(et)) + (5.46e-06/et)*(5.46e-06/et)));
  de = 1.0/sin(2 * atan(exp(-(0.522+0.609)/2))) * det;
  }

  if(0.609<=abs(eta) && abs(eta)<0.696){
  det = et * (sqrt(0.0525*0.0525 + (1.09/sqrt(et))*(1.09/sqrt(et)) + (4.05e-05/et)*(4.05e-05/et)));
  de = 1.0/sin(2 * atan(exp(-(0.609+0.696)/2))) * det;
  }

  if(0.696<=abs(eta) && abs(eta)<0.783){
  det = et * (sqrt(0.0582*0.0582 + (1.09/sqrt(et))*(1.09/sqrt(et)) + (1.17e-05/et)*(1.17e-05/et)));
  de = 1.0/sin(2 * atan(exp(-(0.696+0.783)/2))) * det;
  }

  if(0.783<=abs(eta) && abs(eta)<0.870){
  det = et * (sqrt(0.0649*0.0649 + (1.08/sqrt(et))*(1.08/sqrt(et)) + (7.85e-06/et)*(7.85e-06/et)));
  de = 1.0/sin(2 * atan(exp(-(0.783+0.870)/2))) * det;
  }

  if(0.870<=abs(eta) && abs(eta)<0.957){
  det = et * (sqrt(0.0654*0.0654 + (1.1/sqrt(et))*(1.1/sqrt(et)) + (1.09e-07/et)*(1.09e-07/et)));
  de = 1.0/sin(2 * atan(exp(-(0.870+0.957)/2))) * det;
  }

  if(0.957<=abs(eta) && abs(eta)<1.044){
  det = et * (sqrt(0.0669*0.0669 + (1.11/sqrt(et))*(1.11/sqrt(et)) + (1.87e-06/et)*(1.87e-06/et)));
  de = 1.0/sin(2 * atan(exp(-(0.957+1.044)/2))) * det;
  }

  if(1.044<=abs(eta) && abs(eta)<1.131){
  det = et * (sqrt(0.0643*0.0643 + (1.15/sqrt(et))*(1.15/sqrt(et)) + (2.76e-05/et)*(2.76e-05/et)));
  de = 1.0/sin(2 * atan(exp(-(1.044+1.131)/2))) * det;
  }

  if(1.131<=abs(eta) && abs(eta)<1.218){
  det = et * (sqrt(0.0645*0.0645 + (1.16/sqrt(et))*(1.16/sqrt(et)) + (1.04e-06/et)*(1.04e-06/et)));
  de = 1.0/sin(2 * atan(exp(-(1.131+1.218)/2))) * det;
  }

  if(1.218<=abs(eta) && abs(eta)<1.305){
  det = et * (sqrt(0.0637*0.0637 + (1.19/sqrt(et))*(1.19/sqrt(et)) + (1.08e-07/et)*(1.08e-07/et)));
  de = 1.0/sin(2 * atan(exp(-(1.218+1.305)/2))) * det;
  }

  if(1.305<=abs(eta) && abs(eta)<1.392){
  det = et * (sqrt(0.0695*0.0695 + (1.21/sqrt(et))*(1.21/sqrt(et)) + (5.75e-06/et)*(5.75e-06/et)));
  de = 1.0/sin(2 * atan(exp(-(1.305+1.392)/2))) * det;
  }

  if(1.392<=abs(eta) && abs(eta)<1.479){
  det = et * (sqrt(0.0748*0.0748 + (1.2/sqrt(et))*(1.2/sqrt(et)) + (5.15e-08/et)*(5.15e-08/et)));
  de = 1.0/sin(2 * atan(exp(-(1.392+1.479)/2))) * det;
  }

  if(1.479<=abs(eta) && abs(eta)<1.566){
  det = et * (sqrt(0.0624*0.0624 + (1.23/sqrt(et))*(1.23/sqrt(et)) + (2.28e-05/et)*(2.28e-05/et)));
  de = 1.0/sin(2 * atan(exp(-(1.479+1.566)/2))) * det;
  }

  if(1.566<=abs(eta) && abs(eta)<1.653){
  det = et * (sqrt(0.0283*0.0283 + (1.25/sqrt(et))*(1.25/sqrt(et)) + (4.79e-07/et)*(4.79e-07/et)));
  de = 1.0/sin(2 * atan(exp(-(1.566+1.653)/2))) * det;
  }

  if(1.653<=abs(eta) && abs(eta)<1.740){
  det = et * (sqrt(0.0316*0.0316 + (1.21/sqrt(et))*(1.21/sqrt(et)) + (5e-05/et)*(5e-05/et)));
  de = 1.0/sin(2 * atan(exp(-(1.653+1.740)/2))) * det;
  }

  if(1.740<=abs(eta) && abs(eta)<1.830){
  det = et * (sqrt(2.29e-07*2.29e-07 + (1.2/sqrt(et))*(1.2/sqrt(et)) + (1.71e-05/et)*(1.71e-05/et)));
  de = 1.0/sin(2 * atan(exp(-(1.740+1.830)/2))) * det;
  }

  if(1.830<=abs(eta) && abs(eta)<1.930){
  det = et * (sqrt(5.18e-09*5.18e-09 + (1.14/sqrt(et))*(1.14/sqrt(et)) + (1.7/et)*(1.7/et)));
  de = 1.0/sin(2 * atan(exp(-(1.830+1.930)/2))) * det;
  }

  if(1.930<=abs(eta) && abs(eta)<2.043){
  det = et * (sqrt(2.17e-07*2.17e-07 + (1.09/sqrt(et))*(1.09/sqrt(et)) + (2.08/et)*(2.08/et)));
  de = 1.0/sin(2 * atan(exp(-(1.930+2.043)/2))) * det;
  }

  if(2.043<=abs(eta) && abs(eta)<2.172){
  det = et * (sqrt(3.65e-07*3.65e-07 + (1.09/sqrt(et))*(1.09/sqrt(et)) + (1.63/et)*(1.63/et)));
  de = 1.0/sin(2 * atan(exp(-(2.043+2.172)/2))) * det;
  }

  if(2.172<=abs(eta) && abs(eta)<2.322){
  det = et * (sqrt(2.02e-07*2.02e-07 + (1.09/sqrt(et))*(1.09/sqrt(et)) + (1.68/et)*(1.68/et)));
  de = 1.0/sin(2 * atan(exp(-(2.172+2.322)/2))) * det;
  }

  if(2.322<=abs(eta) && abs(eta)<2.500){
  det = et * (sqrt(5.27e-07*5.27e-07 + (1.12/sqrt(et))*(1.12/sqrt(et)) + (1.78/et)*(1.78/et)));
  de = 1.0/sin(2 * atan(exp(-(2.322+2.500)/2))) * det;
  }

  return de;

}
