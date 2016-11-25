#include <iostream>
#include "HHKinFitMaster.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"


int main(int argc, char* argv[])
{
  TFile* f = new TFile("example/ntuple.root","READ");
  TTree* t = (TTree*) f->Get("EleTauKinFit");
  
  TH1F* h_mH    = new TH1F("h_mH",   "distribution of mH_{fit};mH_{fit} [GeV];entries/5GeV", 40,200,400);
  TH1F* h_chi2  = new TH1F("h_chi2", "distribution of #chi^{2}_{fit};#chi^{2}_{fit};entries/1", 100,0,25);
  TH1F* h_prob  = new TH1F("h_prob", "distribution of fit probability;P_{fit};entries/0.05", 100,0,1);
  TH1F* h_pull1 = new TH1F("h_pull1","pull distribution E_{b1};pull;entries/0.25", 20,-5,5);
  TH1F* h_pull2 = new TH1F("h_pull2","pull distribution E_{b2};pull;entries/0.25", 20,-5,5);
  TH1F* h_pullB = new TH1F("h_pullB","pull distribution balance;pull;entries/0.25", 20,-5,5);
  
  Int_t max_nevent = t->GetEntriesFast();

  //Leaf types  
   Int_t           njets;
   Double_t        b1_dR;
   Double_t        b1_csv;
   Double_t        b1_px;
   Double_t        b1_py;
   Double_t        b1_pz;
   Double_t        b1_E;
   Double_t        b2_dR;
   Double_t        b2_csv;
   Double_t        b2_px;
   Double_t        b2_py;
   Double_t        b2_pz;
   Double_t        b2_E;
   Double_t        tauvis1_dR;
   Double_t        tauvis1_px;
   Double_t        tauvis1_py;
   Double_t        tauvis1_pz;
   Double_t        tauvis1_E;
   Double_t        tauvis2_dR;
   Double_t        tauvis2_px;
   Double_t        tauvis2_py;
   Double_t        tauvis2_pz;
   Double_t        tauvis2_E;
   Double_t        met_pt;
   Double_t        met_px;
   Double_t        met_py;
   Double_t        met_cov00;
   Double_t        met_cov10;
   Double_t        met_cov01;
   Double_t        met_cov11;


  // List of branches
   TBranch        *b_njets;
   TBranch        *b_b1_dR;
   TBranch        *b_b1_csv;
   TBranch        *b_b1_px;
   TBranch        *b_b1_py;
   TBranch        *b_b1_pz;
   TBranch        *b_b1_E;
   TBranch        *b_b2_dR;
   TBranch        *b_b2_csv;
   TBranch        *b_b2_px;
   TBranch        *b_b2_py;
   TBranch        *b_b2_pz;
   TBranch        *b_b2_E;
   TBranch        *b_tauvis1_dR;
   TBranch        *b_tauvis1_px;
   TBranch        *b_tauvis1_py;
   TBranch        *b_tauvis1_pz;
   TBranch        *b_tauvis1_E;
   TBranch        *b_tauvis2_dR;
   TBranch        *b_tauvis2_px;
   TBranch        *b_tauvis2_py;
   TBranch        *b_tauvis2_pz;
   TBranch        *b_tauvis2_E;
   TBranch        *b_met_pt;
   TBranch        *b_met_px;
   TBranch        *b_met_py;
   TBranch        *b_met_cov00;
   TBranch        *b_met_cov10;
   TBranch        *b_met_cov01;
   TBranch        *b_met_cov11;
     
   t->SetBranchAddress("Njets", &njets, &b_njets);
   t->SetBranchAddress("b1_dR", &b1_dR, &b_b1_dR);
   t->SetBranchAddress("b1_csv", &b1_csv, &b_b1_csv);
   t->SetBranchAddress("b1_px", &b1_px, &b_b1_px);
   t->SetBranchAddress("b1_py", &b1_py, &b_b1_py);
   t->SetBranchAddress("b1_pz", &b1_pz, &b_b1_pz);
   t->SetBranchAddress("b1_E", &b1_E, &b_b1_E);
   t->SetBranchAddress("b2_dR", &b2_dR, &b_b2_dR);
   t->SetBranchAddress("b2_csv", &b2_csv, &b_b2_csv);
   t->SetBranchAddress("b2_px", &b2_px, &b_b2_px);
   t->SetBranchAddress("b2_py", &b2_py, &b_b2_py);
   t->SetBranchAddress("b2_pz", &b2_pz, &b_b2_pz);
   t->SetBranchAddress("b2_E", &b2_E, &b_b2_E);
   t->SetBranchAddress("tauvis1_dR", &tauvis1_dR, &b_tauvis1_dR);
   t->SetBranchAddress("tauvis1_px", &tauvis1_px, &b_tauvis1_px);
   t->SetBranchAddress("tauvis1_py", &tauvis1_py, &b_tauvis1_py);
   t->SetBranchAddress("tauvis1_pz", &tauvis1_pz, &b_tauvis1_pz);
   t->SetBranchAddress("tauvis1_E", &tauvis1_E, &b_tauvis1_E);
   t->SetBranchAddress("tauvis2_dR", &tauvis2_dR, &b_tauvis2_dR);
   t->SetBranchAddress("tauvis2_px", &tauvis2_px, &b_tauvis2_px);
   t->SetBranchAddress("tauvis2_py", &tauvis2_py, &b_tauvis2_py);
   t->SetBranchAddress("tauvis2_pz", &tauvis2_pz, &b_tauvis2_pz);
   t->SetBranchAddress("tauvis2_E", &tauvis2_E, &b_tauvis2_E);
   t->SetBranchAddress("met_pt", &met_pt, &b_met_pt);
   t->SetBranchAddress("met_px", &met_px, &b_met_px);
   t->SetBranchAddress("met_py", &met_py, &b_met_py);
   t->SetBranchAddress("met_cov00", &met_cov00, &b_met_cov00);
   t->SetBranchAddress("met_cov10", &met_cov10, &b_met_cov10);
   t->SetBranchAddress("met_cov01", &met_cov01, &b_met_cov01);
   t->SetBranchAddress("met_cov11", &met_cov11, &b_met_cov11);

  //define the testd hypotheses
  std::vector<Int_t> hypo_mh1;
  hypo_mh1.push_back(125);
  std::vector<Int_t> hypo_mh2;
  hypo_mh2.push_back(125);
  
  // event loop
  for (int i = 0; i < max_nevent; i++) {
    t->GetEntry(i);
    
    //use only btaged jets and check for correct gen match
    if (b1_csv<0.681 || b2_csv<0.681 || njets < 2 || b1_dR > 0.2 || b2_dR > 0.2 || tauvis1_dR > 0.1 || tauvis2_dR > 0.1)   continue;
    
    //define input vectors
    TLorentzVector b1      = TLorentzVector(b1_px,b1_py,b1_pz,b1_E);
    TLorentzVector b2      = TLorentzVector(b2_px,b2_py,b2_pz,b2_E);
    TLorentzVector tau1vis = TLorentzVector(tauvis1_px,tauvis1_py,tauvis1_pz,tauvis1_E);
    TLorentzVector tau2vis = TLorentzVector(tauvis2_px,tauvis2_py,tauvis2_pz,tauvis2_E);
    
    TLorentzVector ptmiss  = TLorentzVector(met_px,met_py,0,met_pt);
    TMatrixD metcov(2,2);
    metcov(0,0)=met_cov00;
    metcov(1,0)=met_cov10;
    metcov(0,1)=met_cov01;    
    metcov(1,1)=met_cov11;
    
    //intance of fitter master class
    HHKinFitMaster kinFits = HHKinFitMaster(&b1,&b2,&tau1vis,&tau2vis);
    kinFits.setAdvancedBalance(&ptmiss,metcov);
    //kinFits.setSimpleBalance(ptmiss.Pt(),10); //alternative which uses only the absolute value of ptmiss in the fit
    kinFits.addMh1Hypothesis(hypo_mh1);
    kinFits.addMh2Hypothesis(hypo_mh2);
    kinFits.doFullFit();

    //obtain results from different hypotheses
    Double_t chi2_best = kinFits.getBestChi2FullFit();
    Double_t mh_best = kinFits.getBestMHFullFit();
    std::pair<Int_t, Int_t> bestHypo = kinFits.getBestHypoFullFit();
    std::map< std::pair<Int_t, Int_t>, Double_t> fit_results_chi2 = kinFits.getChi2FullFit();
    std::map< std::pair<Int_t, Int_t>, Double_t> fit_results_fitprob = kinFits.getFitProbFullFit();
    std::map< std::pair<Int_t, Int_t>, Double_t> fit_results_mH = kinFits.getMHFullFit();
    std::map< std::pair<Int_t, Int_t>, Double_t> fit_results_pull_b1 = kinFits.getPullB1FullFit();
    std::map< std::pair<Int_t, Int_t>, Double_t> fit_results_pull_b2 = kinFits.getPullB2FullFit();
    std::map< std::pair<Int_t, Int_t>, Double_t> fit_results_pull_balance = kinFits.getPullBalanceFullFit();
    std::map< std::pair<Int_t, Int_t>, Int_t> fit_convergence = kinFits.getConvergenceFullFit();

    std::cout << "#############################" << std::endl;
    std::cout << "EVENT" << i << std::endl;
    std::cout << "#############################" << std::endl;
    std::cout << "=====================================" << std::endl;
    std::cout << "njets:           " << njets << std::endl;
    std::cout << "event:           " << i << std::endl;
    std::cout << "best chi2:       " << chi2_best << std::endl;
    std::cout << "best hypothesis: " << bestHypo.first << " " << bestHypo.second << std::endl;
    std::cout << "best mH=         " << mh_best << std::endl;
    std::cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;

   //individual loop over results
    for (std::vector<Int_t>::iterator mh2 = hypo_mh2.begin(); mh2 != hypo_mh2.end(); mh2++){
      for (std::vector<Int_t>::iterator mh1 = hypo_mh1.begin(); mh1 != hypo_mh1.end(); mh1++){
        std::pair< Int_t, Int_t > hypo(*mh1,*mh2);
        
        //sanity cuts: use only converged fits and events with chi2<25 and make sure used cov was positive definit (this needs to be fixed!)
        
          std::cout << *mh1 << " " << *mh2 << " " << "chi2_fit:    " << fit_results_chi2.at(hypo) << std::endl;
          std::cout << *mh1 << " " << *mh2 << " " << "prob_fit:    " << fit_results_fitprob.at(hypo) << std::endl;
          std::cout << *mh1 << " " << *mh2 << " " << "mH_fit=      " << fit_results_mH.at(hypo) << std::endl;
          std::cout << "*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" << std::endl;

          if (fit_convergence.at(hypo)>0 && fit_results_chi2.at(hypo)<25 && fit_results_pull_balance.at(hypo)>0){

          h_mH->Fill(fit_results_mH.at(hypo));
          h_chi2->Fill(fit_results_chi2.at(hypo));
          h_prob->Fill(fit_results_fitprob.at(hypo));
          h_pull1->Fill(fit_results_pull_b1.at(hypo));
          h_pull2->Fill(fit_results_pull_b2.at(hypo));
          h_pullB->Fill(fit_results_pull_balance.at(hypo));
        }
      }
    }    
  }

  TFile fout("result.root","RECREATE");
  h_mH->Write();
  h_chi2->Write();
  h_prob->Write();
  h_pull1->Write();
  h_pull2->Write();
  h_pullB->Write();
  
  return (0);
}
