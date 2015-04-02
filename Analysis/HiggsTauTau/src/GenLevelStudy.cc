#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/GenLevelStudy.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTAnalysisTools.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "Math/SMatrix.h"
#include "Math/SVector.h"
#include "TVector3.h"
#include "Math/VectorUtil.h"

namespace ic {

  GenLevelStudy::GenLevelStudy(std::string const& name) : ModuleBase(name) {

    fs_ = NULL;
    make_plots_ = false;
    check_match_ = false;
    ditau_label_ = "emtauCandidates";
    met_label_ = "pfMVAMet";

}

  GenLevelStudy::~GenLevelStudy() {
    ;
  }

  int GenLevelStudy::PreAnalysis() {

    if (fs_) hset = new DynamicHistoSet(fs_->mkdir("gen_level_plots"));
    if(make_plots_)
    {
        hset->Create("hpt_1", 100, 0, 200);
        hset->Create("hpt_2", 100, 0, 200);
        hset->Create("bpt_1", 100, 0, 200);
        hset->Create("beta_1", 100, -5, 5);
        hset->Create("bpt_2", 100, 0, 200);
        hset->Create("beta_2", 100, -5, 5);
        hset->Create("n_bjets", 5, 0, 5);
        hset->Create("mbb", 50, 100, 150);
        hset->Create("bb_deta", 60, 0, 6);
        hset->Create("tpt_1", 100, 0, 200);
        hset->Create("eta_1", 100, -5, 5);
        hset->Create("tpt_2", 100, 0, 200);
        hset->Create("eta_2", 100, -5, 5);
        hset->Create("mtt", 50, 100, 150);
        hset->Create("tt_deta", 60, 0, 6);
        hset->Create("mH", 400, 100, 500);
        hset->Create("mttbb", 400, 100, 500);
        hset->Create("DR_btau",100 , 0, 10);

    }
    hset->Create("bcsv_1",25, 0,1);
    hset->Create("bcsv_2",25, 0,1);
    hset->Create("gen_bcsv_1",25, 0,1);
    hset->Create("gen_bcsv_2",25, 0,1);
    hset->Create("n_genjets", 10, 0, 10);
    hset->Create("n_prebjets", 10, 0, 10);
    hset->Create("csv_real",25, 0,1);
    hset->Create("csv_fake",25, 0,1);

    count_match_any_=0;
    count_b1_=0;
    count_b2_=0;
    //count_t1_=0;
    //count_t2_=0;
    count_3_=0;
    count_4_=0;
    nev_=0;
    
    return 0;
  }

  int GenLevelStudy::Execute(TreeEvent *event) {

    std::vector<GenParticle *> const& parts = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenJet *> gen_jets = event->GetPtrVec<GenJet>("genJets");
    std::map< GenParticle const*, std::vector<int> > higgses;
    std::map< GenParticle const*, std::vector<int> > taus;
    std::map< GenParticle const*, std::vector<int> > muons;
    std::map< GenParticle const*, std::vector<int> > bjets;
    
    for (auto const& part : parts) {
      if (part->status() == 3 && part->pdgid() == 25) {
         higgses[part]=part->daughters();
      }
      if(fabs(part->pdgid()) == 15) {
          taus[part]=part->daughters();
      }
      if(fabs(part->pdgid()) == 13) {
          muons[part]=part->daughters();
      }
      if(fabs(part->pdgid()) == 5) {
          bjets[part]=part->daughters();
      }
    }
  
  std::vector<GenParticle const* > higgs;
  std::vector<GenParticle const* > tau;
  std::vector<GenParticle const* > mu;
  std::vector<GenParticle const* > bjet;
  std::vector<GenJet const* > genjet_bjet;
  std::map<GenParticle const*, std::vector<int> >::iterator it;

 
      
  //std::cout << genjet_bjet.size() << std::endl;    
  for(it=higgses.begin(); it!=higgses.end(); ++it) {
      higgs.push_back((*it).first);
  }
  std::sort(higgs.begin(), higgs.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
  if(make_plots_) hset->Fill("hpt_1", higgs[0]->pt());  
  if(make_plots_) hset->Fill("hpt_2", higgs[1]->pt());  

for(it=bjets.begin(); it!=bjets.end(); ++it) {
      bjet.push_back((*it).first);
  }
  std::sort(bjet.begin(), bjet.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
  if(make_plots_) hset->Fill("bpt_1", bjet[0]->pt());  
  if(make_plots_) hset->Fill("beta_1", bjet[0]->eta());  
  if(make_plots_) hset->Fill("bpt_2", bjet[1]->pt());
  if(make_plots_) hset->Fill("beta_2", bjet[1]->eta());  
  if(make_plots_) hset->Fill("n_bjets" , bjet.size());
  if(make_plots_) hset->Fill("mbb", (bjet[1]->vector() + bjet[0]->vector()).M()); 
  if(make_plots_) hset->Fill("bb_deta", fabs(bjet[0]->eta() - bjet[1]->eta()));
  

  for(it=taus.begin(); it!=taus.end(); ++it) {
      tau.push_back((*it).first);
  }
  std::sort(tau.begin(), tau.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
  if(make_plots_) hset->Fill("tpt_1", tau[0]->pt());  
  if(make_plots_) hset->Fill("tpt_2", tau[1]->pt());  
  if(make_plots_) hset->Fill("mtt", (tau[1]->vector() + tau[0]->vector()).M()); 
  if(make_plots_) hset->Fill("tt_deta", fabs(tau[0]->eta() - tau[1]->eta()));
  
  if(make_plots_) hset->Fill("mH", (higgs[1]->vector() + higgs[0]->vector()).M()); 
  if(make_plots_) hset->Fill("mttbb", (tau[1]->vector() + tau[0]->vector() + bjet[1]->vector() + bjet[0]->vector()).M()); 
  if(make_plots_) hset->Fill("DR_btau", (DR(bjet[0], tau[0]))); 
  if(make_plots_) hset->Fill("DR_btau", (DR(bjet[1], tau[0]))); 
  if(make_plots_) hset->Fill("DR_btau", (DR(bjet[0], tau[1]))); 
  if(make_plots_) hset->Fill("DR_btau", (DR(bjet[1], tau[1]))); 

  //End here if just using the module to make gen plots  
  if(!check_match_) return 0;
  

  std::vector<CompositeCandidate *> const& ditau_vec = event->GetPtrVec<CompositeCandidate>(ditau_label_);
  CompositeCandidate const* ditau = ditau_vec.at(0);
  Candidate const* lep1 = ditau->GetCandidate("lepton1");
  //Candidate const* lep2 = ditau->GetCandidate("lepton2");
  std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
  std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
  std::vector<PFJet*> lowpt_jets = jets;
  ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
  std::vector<PFJet*> prebjets = lowpt_jets;
  ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
  //ic::erase_if(prebjets,!boost::bind(MinPtMaxEta, _1, 10.0, 4.7));
  Met const* met = event->GetPtr<Met>(met_label_);
  std::vector<PFJet*> med_bjets = prebjets;
  ic::erase_if(med_bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
  std::vector<PFJet*> loose_bjets = prebjets;
  std::sort(prebjets.begin(), prebjets.end(), bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") > bind(&PFJet::GetBDiscriminator, _2, "combinedSecondaryVertexBJetTags"));
  //ic::erase_if(prebjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.244);
  
  if (event->Exists("retag_result")) {
    auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
    ic::erase_if(med_bjets, !boost::bind(IsReBTagged, _1, retag_result));
  } else {
    ic::erase_if(med_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
  } 

    mt_1_ = MT(lep1, met);
     
    if (PairOppSign(ditau)) {
      os_ = true;
    } else {
      os_ = false;
    }


//    double jpt_1=0;
//    if(jets.size()>0) {jpt_1=jets[0]->pt();}

    //Some testing of possibility of selecting candidate jet pair using mjj closest to 125.
    std::vector< std::pair<PFJet*,PFJet*> > pairs = MakePairs(prebjets);

    std::vector<CompositeCandidate> dijets;
    std::vector<CompositeCandidate *> ptr_dijets;
    for (unsigned i = 0; i < pairs.size(); ++i) {
      dijets.push_back(CompositeCandidate());
      CompositeCandidate & cand_ref = dijets.back();
      cand_ref.AddCandidate("leading_jet", pairs[i].first);
      cand_ref.AddCandidate("subleading_jet", pairs[i].second);
    }
   
    ptr_dijets.resize(dijets.size());
    for (unsigned i = 0; i < dijets.size(); ++i) {
      ptr_dijets[i] = &(dijets[i]);
    }
    //Sort the pairs by closest mass to the Higgs 
    //std::sort(ptr_dijets.begin(), ptr_dijets.end(), boost::bind(PairMassDiff, _1, 125.0) < boost::bind(PairMassDiff, _2, 125.0) );
    
    //Turn the pairs back into a vector of jets 
    if(prebjets.size()>1) {
      prebjets.resize(2*ptr_dijets.size());
      for(unsigned i=0; i<ptr_dijets.size(); i++) {
        prebjets[2*i]=(ic::PFJet*) ptr_dijets[i]->GetCandidate("leading_jet");
        prebjets[2*i+1]=(ic::PFJet*) ptr_dijets[i]->GetCandidate("subleading_jet");
      }

      std::vector<int> list_of_indices;
      for ( unsigned i = 0; i< ((prebjets.size())- 1) ; i++ ) {  
        for (  unsigned j=i+1; j < prebjets.size() ; j++ ) {
          //std::cout << prebjets[j]->pt()<< std::endl;
          PFJet* s1 = prebjets[i];
          PFJet* s2 = prebjets[j];
          if(DR(s1,s2)<0.1) {
             list_of_indices.push_back(j);
          }
        }
      }
      int j=0;
      std::sort(list_of_indices.begin(), list_of_indices.end());
      list_of_indices.erase( unique( list_of_indices.begin(), list_of_indices.end() ), list_of_indices.end() );
      for(unsigned i=0; i<list_of_indices.size(); i++) {
        prebjets.erase(prebjets.begin()+list_of_indices[i]-j);
        j++;
      }
    }
    
    hset->Fill("n_genjets",gen_jets.size());
    hset->Fill("n_prebjets",prebjets.size());
     
    match_any_jet_1_=false; 
    match_any_jet_2_=false;
    std::vector<ic::PFJet*> matches;
    PFJet* jet1_match=NULL;
    PFJet* jet2_match=NULL;
    int index1=-1, index2=-1;
    //Event pre-selection
    //if(os_ && mt_1_<30 && prebjets.size()>1 && med_bjets.size()>1) {
    //if(os_ && mt_1_<30 && prebjets.size()>1 && med_bjets.size()>0) {
    if(os_ && mt_1_<30 && prebjets.size()>1 ) {
        hset->Fill("bcsv_1",prebjets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags") );
        hset->Fill("bcsv_2",prebjets[1]->GetBDiscriminator("combinedSecondaryVertexBJetTags") );
        
        for(unsigned i=0; i<prebjets.size() ; i++) {
          if((DR(prebjets[i], bjet[0])<0.5) ) {
              matches.push_back(prebjets[i]);
          }
          if((DR(prebjets[i], bjet[1])<0.5) ) {
              matches.push_back(prebjets[i]);
          }
        }
        if(matches.size()>1) {
          std::sort(matches.begin(), matches.end(), bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") > bind(&PFJet::GetBDiscriminator, _2, "combinedSecondaryVertexBJetTags"));
          hset->Fill("gen_bcsv_1",matches[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags") );
          hset->Fill("gen_bcsv_2",matches[1]->GetBDiscriminator("combinedSecondaryVertexBJetTags") );
        }
      
      //Find the gen jets corresponding to the h->bb
      for(unsigned i=0; i<gen_jets.size(); i++) { 
        if((DR(gen_jets[i], bjet[0])<0.5) ) {
          genjet_bjet.push_back(gen_jets[i]);
         }
        else if((DR(gen_jets[i], bjet[1])<0.5) ) {
          genjet_bjet.push_back(gen_jets[i]);
        }
      }
      if(!(genjet_bjet.size()==2)) {std::cout << genjet_bjet.size() << std::endl;}

        nev_++;
        //Check matching of selected b jets
        match_b_1_ = ( DR(prebjets[0], bjet[0])<0.5 && DR(prebjets[1], bjet[1])<0.5);
        if(match_b_1_) count_b1_++;
        match_b_2_ = ( (DR(prebjets[1], bjet[0])<0.5 && DR(prebjets[0], bjet[1])<0.5));
        if(match_b_2_) count_b2_++;
        //Check matching of candidate ditau pair
        /*match_t_1_ = ( DR(lep1, tau[0])<0.5 && DR(lep2, tau[1])<0.5);
        if(match_t_1_) count_t1_++;
        match_t_2_ = ( DR(lep1, tau[1])<0.5 && DR(lep2, tau[0])<0.5);
        if(match_t_2_) count_t2_++;
        */
        for(unsigned j=0; j<tau.size(); j++) {
          if(DR(prebjets[0], tau[j])<0.5 || DR(prebjets[1], tau[j])<0.5)
          {
        //    count_4_++;
          }
        }
        //Print event info for unmatched b jets
        if(!match_b_1_ && !match_b_2_) {
          std::cout << "=================================" << std::endl;
          std::cout << "Gen jets: " << gen_jets.size() << ", Reco jets: " << prebjets.size() << std::endl;
          for(unsigned i=0; i<prebjets.size() ; i++) {
              std::cout << "Event number: " << nev_ << ", Index in event: " <<  i << " " << prebjets[i]->pt() << " " << prebjets[i]->GetBDiscriminator("combinedSecondaryVertexBJetTags") <<  ", match1?: " << (DR(prebjets[i], bjet[0])<0.5) << ", match2?: "
              << (DR(prebjets[i], bjet[1])<0.5) << std::endl;
              if((DR(prebjets[i], bjet[0])<0.5) )
              {
                  match_any_jet_1_=true;
                  jet1_match=prebjets[i];
                  index1=i;
              }
              if((DR(prebjets[i], bjet[1])<0.5) )
              {
                  match_any_jet_2_=true;
                  jet2_match=prebjets[i];
                  index2=i;
              }
              for(unsigned j=0; j<tau.size(); j++) {
                 if(DR(prebjets[i], tau[j])<0.5)
                 {
              //     std::cout << "Tau match?: " <<  (DR(prebjets[i], tau[j])<0.5) << ", tau index: " << j << std::endl;
                   count_3_++;
                 }
              }
            }
            if(!(match_any_jet_1_ && match_any_jet_2_)) {    
               count_match_any_++;
               std::cout << "=================================" << std::endl;
            } else { 
              if(genjet_bjet.size()==2) {
                count_4_++;
                if(match_any_jet_1_ && index1>1) {
                  hset->Fill("csv_real",jet1_match->GetBDiscriminator("combinedSecondaryVertexBJetTags")); 
                  if(DR(jet1_match,prebjets[0])<0.1) {
                    hset->Fill("csv_fake",prebjets[1]->GetBDiscriminator("combinedSecondaryVertexBJetTags"));   
                  } else {
                    hset->Fill("csv_fake",prebjets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags"));  
                  }
                }
                if(match_any_jet_2_ && index2>1) {
                  hset->Fill("csv_real",jet2_match->GetBDiscriminator("combinedSecondaryVertexBJetTags")); 
                  if(DR(jet2_match,prebjets[0])<0.1) {
                    hset->Fill("csv_fake",prebjets[1]->GetBDiscriminator("combinedSecondaryVertexBJetTags"));   
                  } else {
                    hset->Fill("csv_fake",prebjets[0]->GetBDiscriminator("combinedSecondaryVertexBJetTags"));  
                  }
                }                   
                
              }
            }
          }
        
        //Print event info for unmatched taus
        /*if(!match_t_1_ && !match_t_2_) {
            std::cout << "=================================" << std::endl;
            std::cout << "Event number: " << nev_ << " " << lep1->pt() << " " <<  lep2->pt() << std::endl;
            std::cout << "=================================" << std::endl;
        }*/
    }
     
    return 0;
  }
  int GenLevelStudy::PostAnalysis() {
    if(check_match_) std::cout << "Number of events: "<< nev_ << ", count 1: " << count_b1_ << ", count 2: " << count_b2_ << ", successful fraction: "
        << (double(count_b1_) + double(count_b2_))/double(nev_) << ", tau matches: " << count_3_ << ", tau matches in jet pair: " << /*count_4_ <<*/ ", nevents with b jet missing from collection: " << count_match_any_ 
        << ", Number of events with h->bb in gen jets: " << count_4_<< std::endl; 
    //if(check_match_) std::cout << "Number of events: "<< nev_ << ", count 1: " << count_t1_ << ", count 2: " << count_t2_ << ", successful fraction: "
    //    << (double(count_t1_) + double(count_t2_))/double(nev_) <<std::endl ;
    return 0;
  }

  void GenLevelStudy::PrintInfo() {
    ;
  }
}
