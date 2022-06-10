#include "UserCode/ICHiggsTauTau/Analysis/4tau/interface/HTTGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/PolarimetricA1.h"

TVector3 GenIP (ic::GenParticle *h, ic::GenParticle *t) {

  TVector3 pvtosv(
           t->vtx().vx() - h->vtx().vx(),
           t->vtx().vy() - h->vtx().vy(),
           t->vtx().vz() - h->vtx().vz()
           );

  TVector3 momenta(
                  t->vector().Px(),
                  t->vector().Py(),
                  t->vector().Pz()
                  );

  double projection=pvtosv*momenta/momenta.Mag2();
  TVector3 gen_ip=pvtosv-momenta*projection;

  return gen_ip;
}

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles, unsigned &outputID){ 
  if(p.daughters().size() == 0){
    unsigned ID = std::fabs(p.pdgid());
    if(ID == 11) outputID = 11;
    else if(ID == 13) outputID = 13;
    if(!(ID==12 || ID==14 || ID==16)){
      v.push_back(p);
    }
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = *gen_particles[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles, outputID);
    }
  }
  return v;
}

struct swap_labels{
  bool operator() (std::string a,std::string b) {
    if(a=="t" && b!="t") return false;
    else if(a=="m" &&  (b!="m" && b!="t")) return false;
    else return true;
  }
};

struct PtComparator{
  bool operator() (ic::Candidate a, ic::Candidate b) {
    return (a.vector().Pt() > b.vector().Pt());
  }
};

struct PtComparatorGenPart{
  bool operator() (ic::GenParticle *a, ic::GenParticle *b) {
    return (a->vector().Pt() > b->vector().Pt());
  }
};

  
namespace ic {

  HTTGenAnalysis::HTTGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
  }

  HTTGenAnalysis::~HTTGenAnalysis() {
    ;
  }

  int HTTGenAnalysis::PreAnalysis() {
      
    rand = new TRandom3(0);
    if(fs_){  
      outtree_ = fs_->make<TTree>("gen_ntuple","gen_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("wt"       , &wt_       );
      outtree_->Branch("gen_wt"       , &gen_wt_       );
      //outtree_->Branch("parent_1"        , &parent_1_        );
      //outtree_->Branch("parent_2"        , &parent_2_        );
      //outtree_->Branch("parent_3"        , &parent_3_        );
      //outtree_->Branch("parent_4"        , &parent_4_        );
      outtree_->Branch("pt_1"        , &pt_1_        );
      outtree_->Branch("pt_2"        , &pt_2_        );
      outtree_->Branch("pt_3"        , &pt_3_        );
      outtree_->Branch("pt_4"        , &pt_4_        );
      outtree_->Branch("genpt_1"        , &genpt_1_        );
      outtree_->Branch("genpt_2"        , &genpt_2_        );
      outtree_->Branch("genpt_3"        , &genpt_3_        );
      outtree_->Branch("genpt_4"        , &genpt_4_        );
      outtree_->Branch("eta_1"       , &eta_1_       );
      outtree_->Branch("eta_2"       , &eta_2_       );
      outtree_->Branch("eta_3"       , &eta_3_       );
      outtree_->Branch("eta_4"       , &eta_4_       );
      outtree_->Branch("geneta_1"       , &geneta_1_       );
      outtree_->Branch("geneta_2"       , &geneta_2_       );
      outtree_->Branch("geneta_3"       , &geneta_3_       );
      outtree_->Branch("geneta_4"       , &geneta_4_       );
      outtree_->Branch("phi_1"       , &phi_1_       );
      outtree_->Branch("phi_2"       , &phi_2_       );
      outtree_->Branch("phi_3"       , &phi_3_       );
      outtree_->Branch("phi_4"       , &phi_4_       );
      outtree_->Branch("met"         , &met_         );
    }
    count_eett_ = 0;
    count_emtt_ = 0;
    count_ettt_ = 0;
    count_mmtt_ = 0;
    count_mttt_ = 0;
    count_tttt_ = 0;
    count_eemt_ = 0;
    count_emmt_ = 0;
    count_eeet_ = 0;
    count_mmmt_ = 0;
    count_eeem_ = 0;
    count_eemm_ = 0;
    count_emmm_ = 0;
    count_eeee_ = 0;
    count_mmmm_ = 0;
    count_channel_ = 0;
    return 0;
}

int HTTGenAnalysis::Execute(TreeEvent *event) {
  
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = (unsigned long long) eventInfo->event();
    wt_ = 1;
    rand->SetSeed(event_);
    rand_ = rand->Uniform();   

    wt_ = eventInfo->total_weight();

    if (eventInfo->weight_defined("gen_weight")) gen_wt_ = eventInfo->weight("gen_weight");
    
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::vector<ic::GenJet*> gen_jets;
    if(event->ExistsInTree("genJets")) gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

    std::vector<ic::GenParticle> higgs_products;
    std::vector<ic::GenParticle> gen_taus;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;
    
    for(unsigned i=0; i<gen_particles.size(); ++i){

      ic::GenParticle part = *gen_particles[i];
      
      unsigned genID = std::fabs(part.pdgid());
      bool status_flag_t = part.statusFlags().at(0);
      bool status_flag_tlc = part.statusFlags().at(13);
      
      //if((genID==25||genID==35||genID==36) && gen_particles[i]->statusFlags()[IsLastCopy]){
      //  pT = gen_particles[i]->vector().Pt();
      //  pT_A_ = pT;
      //}

      // add tau_neutrinos 4-vectors to get gen met
      if(genID == 12 || genID == 14 || genID == 16){
        met.set_vector(met.vector() + part.vector());
        continue;
      }
      if(!(genID == 15 && status_flag_t && status_flag_tlc)) continue;
      gen_taus.push_back(part);
      std::vector<ic::GenParticle> family;
      unsigned outputID = 15;
      FamilyTree(family, part, gen_particles, outputID);
      if(family.size()==1 && (outputID ==11 || outputID ==13)){
        higgs_products.push_back(family[0]);
        if (outputID == 11) {decay_types.push_back("e");}  
        else if (outputID == 13) {decay_types.push_back("m");}
      } else {
        decay_types.push_back("t");  
        ic::GenParticle had_tau;
        int charge = 0;
        int pdgid = 15;
        for(unsigned j=0; j<family.size(); ++j){
          had_tau.set_vector(had_tau.vector() + family[j].vector());
          charge += family[j].charge();
        }
        pdgid = 15*charge;
        had_tau.set_charge(charge);
        had_tau.set_pdgid(pdgid);
        higgs_products.push_back(had_tau);
      }
    }

    std::sort(higgs_products.begin(),higgs_products.end(),PtComparator());
    std::sort(gen_taus.begin(),gen_taus.end(),PtComparator());
    
    if(gen_taus.size()>=4){
      genpt_1_ = gen_taus[0].vector().Pt();
      genpt_2_ = gen_taus[1].vector().Pt();
      genpt_3_ = gen_taus[2].vector().Pt();
      genpt_4_ = gen_taus[3].vector().Pt();
      geneta_1_ = gen_taus[0].vector().Rapidity();
      geneta_2_ = gen_taus[1].vector().Rapidity();
      geneta_3_ = gen_taus[3].vector().Rapidity();
      geneta_4_ = gen_taus[4].vector().Rapidity();
    } else {
      genpt_1_ =  -9999;
      genpt_2_ =  -9999;
      genpt_3_ =  -9999;
      genpt_4_ =  -9999;
      geneta_1_ = -9999;
      geneta_2_ = -9999; 
      geneta_3_ = -9999;
      geneta_4_ = -9999;
    }
    
    std::vector<ic::GenParticle> electrons;
    std::vector<ic::GenParticle> muons;
    std::vector<ic::GenParticle> taus;
    
    for(unsigned i=0; i<higgs_products.size(); ++i){
      unsigned ID = std::fabs(higgs_products[i].pdgid());
      double eta = std::fabs(higgs_products[i].vector().Rapidity());
      double pt = higgs_products[i].vector().Pt();
      if(ID == 11){
        if(pt > min_e_pt_ && eta < max_e_eta_) electrons.push_back(higgs_products[i]);  
      } else if(ID == 13){
        if(pt > min_mu_pt_ && eta < max_mu_eta_) muons.push_back(higgs_products[i]);  
      } else if(ID == 15){
        if(pt > min_tau_pt_ && eta < max_tau_eta_) taus.push_back(higgs_products[i]);
      }
    }

    //size of decay_types vector should always be 2 but added this if statement just to be sure
    decayType = "";
    std::sort(decay_types.begin(),decay_types.end(),swap_labels());
    for(unsigned i=0; i< decay_types.size(); ++i){
      decayType += decay_types[i];
    }
   
    if(decayType == "eett") count_eett_++;
    if(decayType == "emtt") count_emtt_++;
    if(decayType == "ettt") count_ettt_++;
    if(decayType == "mmtt") count_mmtt_++;
    if(decayType == "mttt") count_mttt_++;
    if(decayType == "tttt") count_tttt_++;
    if(decayType == "eemt") count_eemt_++;
    if(decayType == "emmt") count_emmt_++;
    if(decayType == "eeet") count_eeet_++;
    if(decayType == "mmmt") count_mmmt_++;
    if(decayType == "eeem") count_eeem_++;
    if(decayType == "eemm") count_eemm_++;
    if(decayType == "emmm") count_emmm_++;
    if(decayType == "eeee") count_eeee_++;
    if(decayType == "mmmm") count_mmmm_++;
    if(decayType == "eettt" || decayType == "emtt" || decayType == "ettt" || decayType == "mmtt" || decayType == "mttt" || decayType == "tttt" || decayType == "eemt" || decayType == "emmt" || decayType == "eeet" || decayType == "mmmt" || decayType == "eeem" || decayType == "eemm" || decayType == "emmm" || decayType == "eeee" || decayType == "mmmm") {
      count_channel_++;
    }


    pt_1_ = -9999.;
    pt_2_ = -9999.;
    pt_3_ = -9999.;
    pt_4_ = -9999.;
    ic::Candidate lep1;
    ic::Candidate lep2;
    ic::Candidate lep3;
    ic::Candidate lep4;
    passed_ = false;
   
    if(channel_str_ == "eett"){
      if(electrons.size() == 2 && taus.size() == 2 && muons.size()==0){
        lep1 = electrons[0];
        lep2 = electrons[1];
        lep3 = taus[0];
        lep4 = taus[1];
        passed_ = true;
      }
    } else if(channel_str_ == "mmtt"){
      if(muons.size() == 2 && taus.size() == 2 && electrons.size() == 0){
        lep1 = muons[0];
        lep2 = muons[1];
        lep3 = taus[0];
        lep4 = taus[1];
        passed_ = true;
      }
    } else if(channel_str_ == "emtt"){
      if(electrons.size() == 1 && muons.size() == 1 && taus.size() == 2){
        lep1 = electrons[0];
        lep2 = muons[0];
        lep3 = taus[0];
        lep4 = taus[1];
        passed_ = true;
      }
    } else if(channel_str_ == "ettt"){
      if(electrons.size() == 1 && taus.size() == 3 && muons.size() == 0){
        lep1 = electrons[0];
        lep2 = taus[0];
        lep3 = taus[1];
        lep4 = taus[2];
        passed_ = true;
      }
    } else if(channel_str_ == "mttt"){
      if(muons.size() == 1 && taus.size() == 3 && electrons.size() == 0){
        lep1 = muons[0];
        lep2 = taus[0];
        lep3 = taus[1];
        lep4 = taus[2];
        passed_ = true;
      }
    } else if(channel_str_ == "tttt"){
      if(taus.size() == 4 && muons.size() == 0 && electrons.size()==0 ){
        lep1 = taus[0];
        lep2 = taus[1];
        lep3 = taus[2];
        lep4 = taus[3];
        passed_ = true;
      }
    } else if(channel_str_ == "eemt"){
      if(electrons.size() == 2 && muons.size() == 1 && taus.size() == 1){
        lep1 = electrons[0];
        lep2 = electrons[1];
        lep3 = muons[0];
        lep4 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "emmt"){
      if(electrons.size() == 1 && muons.size() == 2 && taus.size() == 1){
        lep1 = electrons[0];
        lep2 = muons[0];
        lep3 = muons[1];
        lep4 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "eeet"){
      if(electrons.size() == 3 && taus.size() == 1 && muons.size() == 0){
        lep1 = electrons[0];
        lep2 = electrons[1];
        lep3 = electrons[2];
        lep4 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "mmmt"){
      if(muons.size() == 3 && taus.size() == 1 && electrons.size() == 0){
        lep1 = muons[0];
        lep2 = muons[1];
        lep3 = muons[2];
        lep4 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "eeem"){
      if(electrons.size() == 3 && muons.size() == 1 && taus.size() == 0){
        lep1 = electrons[0];
        lep2 = electrons[1];
        lep3 = electrons[2];
        lep4 = muons[0];
        passed_ = true;
      }
    } else if(channel_str_ == "eemm"){
      if(electrons.size() == 2 && muons.size() == 2 && taus.size() == 0){
        lep1 = electrons[0];
        lep2 = electrons[1];
        lep3 = muons[0];
        lep4 = muons[1];
        passed_ = true;
      }
    } else if(channel_str_ == "emmm"){
      if(electrons.size() == 1 && muons.size() == 3 && taus.size() == 0){
        lep1 = electrons[0];
        lep2 = muons[0];
        lep3 = muons[1];
        lep4 = muons[2];
        passed_ = true;
      }
    } else if(channel_str_ == "eeee"){
      if(electrons.size() == 4 && taus.size() == 0 && muons.size() == 0){
        lep1 = electrons[0];
        lep2 = electrons[1];
        lep3 = electrons[2];
        lep4 = electrons[3];
        passed_ = true;
      }
    } else if(channel_str_ == "mmmm"){
      if(muons.size() == 4 && taus.size() == 0 && electrons.size() == 0){
        lep1 = muons[0];
        lep2 = muons[1];
        lep3 = muons[2];
        lep4 = muons[3];
        passed_ = true;
      }
    }


    if(passed_){
      pt_1_  = lep1.vector().Pt();
      pt_2_  = lep2.vector().Pt();
      pt_3_  = lep3.vector().Pt();
      pt_4_  = lep4.vector().Pt();
      eta_1_ = lep1.vector().Rapidity();
      eta_2_ = lep2.vector().Rapidity();
      eta_3_ = lep3.vector().Rapidity();
      eta_4_ = lep4.vector().Rapidity();
      phi_1_ = lep1.vector().Phi();
      phi_2_ = lep2.vector().Phi();
      phi_3_ = lep3.vector().Phi();
      phi_4_ = lep4.vector().Phi();
      met_   = met.vector().Pt();
    } else {
      pt_1_  = -9999;
      pt_2_  = -9999;
      pt_3_  = -9999;
      pt_4_  = -9999;
      eta_1_ = -9999;
      eta_2_ = -9999;
      eta_3_ = -9999;
      eta_4_ = -9999;
      phi_1_ = -9999;
      phi_2_ = -9999;
      phi_3_ = -9999;
      phi_4_ = -9999;
      met_   = -9999;
    }
    

    std::vector<PileupInfo *> puInfo;
    float true_int = -1;
    
    if(event->ExistsInTree("pileupInfo")){
      puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
        for (unsigned i = 0; i < puInfo.size(); ++i) {
          if (puInfo[i]->bunch_crossing() == 0)
            true_int = puInfo[i]->true_num_interactions();
        }
    
      n_pu_ = true_int;
    }

    if(fs_) outtree_->Fill();
    return 0;
  }
  int HTTGenAnalysis::PostAnalysis() {
    std::cout << "eett count = " << count_eett_ << " -- " << 100*(double)count_eett_/count_channel_ << "%" << std::endl;
    std::cout << "emtt count = " << count_emtt_ << " -- " << 100*(double)count_emtt_/count_channel_ << "%" << std::endl;
    std::cout << "ettt count = " << count_ettt_ << " -- " << 100*(double)count_ettt_/count_channel_ << "%" << std::endl;
    std::cout << "mmtt count = " << count_mmtt_ << " -- " << 100*(double)count_mmtt_/count_channel_ << "%" << std::endl;
    std::cout << "mttt count = " << count_mttt_ << " -- " << 100*(double)count_mttt_/count_channel_ << "%" << std::endl;
    std::cout << "tttt count = " << count_tttt_ << " -- " << 100*(double)count_tttt_/count_channel_ << "%" << std::endl;
    std::cout << "eemt count = " << count_eemt_ << " -- " << 100*(double)count_eemt_/count_channel_ << "%" << std::endl;
    std::cout << "emmt count = " << count_emmt_ << " -- " << 100*(double)count_emmt_/count_channel_ << "%" << std::endl;
    std::cout << "eeet count = " << count_eeet_ << " -- " << 100*(double)count_eeet_/count_channel_ << "%" << std::endl;
    std::cout << "mmmt count = " << count_mmmt_ << " -- " << 100*(double)count_mmmt_/count_channel_ << "%" << std::endl;
    std::cout << "eeem count = " << count_eeem_ << " -- " << 100*(double)count_eeem_/count_channel_ << "%" << std::endl;
    std::cout << "eemm count = " << count_eemm_ << " -- " << 100*(double)count_eemm_/count_channel_ << "%" << std::endl;
    std::cout << "emmm count = " << count_emmm_ << " -- " << 100*(double)count_emmm_/count_channel_ << "%" << std::endl;
    std::cout << "eeee count = " << count_eeee_ << " -- " << 100*(double)count_eeee_/count_channel_ << "%" << std::endl;
    std::cout << "mmmm count = " << count_mmmm_ << " -- " << 100*(double)count_mmmm_/count_channel_ << "%" << std::endl;
    std::cout << "totol count= " << count_channel_ << std::endl;

    return 0;
  }

  void HTTGenAnalysis::PrintInfo() {
    ;
  }

}
