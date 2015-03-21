#include "../include/HHDiJetKinFitMaster.h"

#include "../include/HHEventRecord.h"
#include "../include/HHDiJetKinFit.h"
//#include "../include/HHDiJetKinFitNewMini.h"
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
HHDiJetKinFitMaster::doFullFit()
{
  //Setup event
  HHParticleList* particlelist = new HHParticleList();
  HHEventRecord eventrecord_rec(particlelist);

  eventrecord_rec.UpdateEntry(HHEventRecord::b1)->SetVector(*m_bjet1);
  eventrecord_rec.UpdateEntry(HHEventRecord::b1)->SetErrors(GetBjetResolution(m_bjet1->Eta(),m_bjet1->Et()), 0, 0);
  eventrecord_rec.UpdateEntry(HHEventRecord::b2)->SetVector(*m_bjet2);
  eventrecord_rec.UpdateEntry(HHEventRecord::b2)->SetErrors(GetBjetResolution(m_bjet2->Eta(),m_bjet2->Et()), 0, 0);

  //loop over all hypotheses
  for(std::vector<Int_t>::iterator mh = m_mh.begin(); mh != m_mh.end(); mh++){
    particlelist->UpdateMass(HHPID::h2, *mh);


    HHDiJetKinFit advancedfitter(&eventrecord_rec);
    //HHDiJetKinFitNewMini advancedfitter(&eventrecord_rec); //Choose Minimiser

    advancedfitter.SetPrintLevel(2);
    advancedfitter.SetLogLevel(2);

    advancedfitter.Fit();
    
    Double_t chi2_full = advancedfitter.GetChi2();
    Double_t prob_full = TMath::Prob(chi2_full,3);
    std::pair< Int_t, Int_t > hypo_full(*mh,-1);
    std::pair< std::pair< Int_t, Int_t >, Double_t > entry_chi2_full (hypo_full, chi2_full);
    std::pair< std::pair< Int_t, Int_t >, Double_t > entry_fitprob_full (hypo_full, prob_full);
    std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullb1_full (hypo_full, advancedfitter.GetPullE(HHEventRecord::b1));
    std::pair< std::pair< Int_t, Int_t >, Double_t > entry_pullb2_full (hypo_full, advancedfitter.GetPullE(HHEventRecord::b2));
    std::pair< std::pair< Int_t, Int_t >, Int_t > entry_convergence_full (hypo_full, advancedfitter.GetConvergence());
    m_fullFitResultChi2.insert(entry_chi2_full);
    m_fullFitResultFitProb.insert(entry_fitprob_full);
    m_fullFitPullB1.insert(entry_pullb1_full);
    m_fullFitPullB2.insert(entry_pullb2_full);
    m_fullFitConvergence.insert(entry_convergence_full);
    if (chi2_full<m_bestChi2FullFit) {
      m_bestChi2FullFit = chi2_full;
      m_bestHypoFullFit = hypo_full;
    }

    m_bjet1_fitted = advancedfitter.GetFitJet1();
    m_bjet2_fitted = advancedfitter.GetFitJet2();
    m_chi2b1 = pow( (advancedfitter.GetFitJet1().E() - m_bjet1->E())/GetBjetResolution(m_bjet1->Eta(),m_bjet1->Et()),2);
    m_chi2b2 = pow( (advancedfitter.GetFitJet2().E() - m_bjet2->E())/GetBjetResolution(m_bjet2->Eta(),m_bjet2->Et()),2);
  }

  delete particlelist;
}

HHDiJetKinFitMaster::HHDiJetKinFitMaster(TLorentzVector* bjet1, TLorentzVector* bjet2, Bool_t truthinput):
    m_mh(std::vector<Int_t>()),
    m_bjet1(bjet1),
    m_bjet2(bjet2),
    m_fullFitResultChi2(std::map< std::pair<Int_t,Int_t> , Double_t>()),
    m_bestChi2FullFit(999),
    m_bestHypoFullFit(std::pair<Int_t, Int_t>(-1,-1))
{
  m_invMassAfterSmearing = -1.0;
  m_chi2Truth = -1.0;
  m_bJet1Diff = -1.0;
  m_bJet2Diff = -1.0;
  m_chi2b1 = -999.0;
  m_chi2b2 = -999.0;
  if (truthinput){
    TRandom3 r(0);
    
    Double_t bjet1_res = GetBjetResolution(bjet1->Eta(), bjet1->Et());
    Double_t bjet1_ETruth = bjet1->E();
    Double_t bjet1_E  = r.Gaus(bjet1->E(),bjet1_res);
    Double_t bjet1_P  = sqrt(pow(bjet1_E,2) - pow(bjet1->M(),2));
    Double_t bjet1_Pt = sin(bjet1->Theta())*bjet1_P;

    m_bJet1Diff = bjet1->E() - bjet1_E;
    m_bJet1DiffPt = bjet1->Pt() - bjet1_Pt;

    bjet1->SetPtEtaPhiE(bjet1_Pt, bjet1->Eta(), bjet1->Phi(), bjet1_E);

    m_bjet1MassAfterSmearing = bjet1->M();

    Double_t bjet2_res = GetBjetResolution(bjet2->Eta(), bjet2->Et());
    Double_t bjet2_ETruth = bjet2->E();
    Double_t bjet2_E  = r.Gaus(bjet2->E(),bjet2_res);
    Double_t bjet2_P  = sqrt(pow(bjet2_E,2) - pow(bjet2->M(),2));
    Double_t bjet2_Pt = sin(bjet2->Theta())*bjet2_P;

    m_bJet2Diff = bjet2->E() - bjet2_E;
    m_bJet2DiffPt = bjet2->Pt() - bjet2_Pt;

    bjet2->SetPtEtaPhiE(bjet2_Pt, bjet2->Eta(), bjet2->Phi(), bjet2_E); 

    m_bjet2MassAfterSmearing = bjet2->M();
    double chi2Truthb1 = pow((bjet1_E - bjet1_ETruth)/GetBjetResolution(bjet1->Eta(), bjet1->Et() ),2);
    double chi2Truthb2 = pow((bjet2_E - bjet2_ETruth)/GetBjetResolution(bjet2->Eta(), bjet2->Et() ),2);
    m_chi2Truth = chi2Truthb1 + chi2Truthb2;

    m_invMassAfterSmearing = (*bjet1 + *bjet2).M();
  }
}

Double_t
HHDiJetKinFitMaster::getBestChi2FullFit()
{
  return m_bestChi2FullFit;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHDiJetKinFitMaster::getChi2FullFit(){
  return m_fullFitResultChi2;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHDiJetKinFitMaster::getFitProbFullFit(){
  return m_fullFitResultFitProb;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHDiJetKinFitMaster::getPullB1FullFit(){
  return m_fullFitPullB1;
}

std::map< std::pair< Int_t, Int_t >, Double_t >
HHDiJetKinFitMaster::getPullB2FullFit(){
  return m_fullFitPullB2;
}

std::map< std::pair< Int_t, Int_t >, Int_t >
HHDiJetKinFitMaster::getConvergenceFullFit(){
  return m_fullFitConvergence;
}

std::pair<Int_t, Int_t>
HHDiJetKinFitMaster::getBestHypoFullFit()
{
  return m_bestHypoFullFit;
}

void
HHDiJetKinFitMaster::addMhHypothesis(Double_t m1, Double_t m2, Double_t m3, Double_t m4, Double_t m5, Double_t m6, Double_t m7, Double_t m8, Double_t m9, Double_t m10)
{
  if (m1 != 0) m_mh.push_back(m1);
  if (m2 != 0) m_mh.push_back(m2);
  if (m3 != 0) m_mh.push_back(m3);
  if (m4 != 0) m_mh.push_back(m4);
  if (m5 != 0) m_mh.push_back(m5);
  if (m6 != 0) m_mh.push_back(m6);
  if (m7 != 0) m_mh.push_back(m7);
  if (m8 != 0) m_mh.push_back(m8);
  if (m9 != 0) m_mh.push_back(m9);
  if (m10 != 0) m_mh.push_back(m10);
}

void
HHDiJetKinFitMaster::addMhHypothesis(std::vector<Int_t> v)
{
  m_mh = v;
}

double
HHDiJetKinFitMaster::GetBjetResolution(double eta, double et){
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
