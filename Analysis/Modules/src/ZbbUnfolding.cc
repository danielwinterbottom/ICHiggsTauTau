#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/ZbbUnfolding.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/PileupInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/GenJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/BTagWeight.h"

#include "TEfficiency.h"


#include "boost/bind.hpp"


namespace ic {

  ZbbUnfolding::ZbbUnfolding(std::string const& name) : ModuleBase(name) {
    pu_rw_ = false;
    tp_rw_ = true;
    btag_rw_ = true;
    mode_ = 0; // 0 = ee 1 =mumu
    z_mass_low_ = 76.0;
    z_mass_high_ = 106.0;
    gen_jet_pt_ = 25.0;
    gen_jet_eta_ = 2.1;
    gen_jet_gen_lepton_dr_ = 0.5;
    gen_jet_bhadron_dr_ = 0.5;
    gen_elec_pt_ = 20.0;
    gen_elec_eta_ = 2.4;
    gen_muon_pt_ = 20.0;
    gen_muon_eta_ = 2.4;
    gen_elec_pt_baseline_ = 20.0;
    gen_elec_eta_baseline_ = 2.4;
    gen_muon_pt_baseline_ = 20.0;
    gen_muon_eta_baseline_ = 2.4;
    gen_reco_elec_dr_ = 0.3;
    gen_reco_muon_dr_ = 0.3;
    reco_elec_pt_ = 20.0;
    reco_elec_eta_ = 2.4;
    reco_muon_pt_ = 20.0;
    reco_muon_eta_ = 2.4;
    reco_jet_pt_ = 25.0;
    reco_jet_eta_ = 2.1;
    reco_jet_lepton_dr_ = 0.5;
    reco_gen_jet_dr_ = 0.5;

  }

  ZbbUnfolding::~ZbbUnfolding() {
    ;
  }

  int ZbbUnfolding::PreAnalysis() {
    return 0;
  }

  int ZbbUnfolding::Execute(TreeEvent *event) {
    EventInfo * eventInfo = event->GetPtr<EventInfo>("eventInfo");
    using boost::bind;


    double weight = 1.0;

    if (pu_rw_) weight = eventInfo->total_weight();
    counters_["events_proc"] += weight;
    counters_["real_proc"] += 1.0;

    //Step 1a: Fill Z decay leptons, select decays of desired flavour
    std::vector<GenParticle *> gen_particles = event->GetPtrVec<GenParticle>("genParticles");
    std::vector<GenParticle *> gen_leptons = gen_particles;
    int decay_flav = 11;
    if (mode_ == 1) decay_flav = 13;
    erase_if(gen_leptons, !((bind(&GenParticle::status, _1) == 3) && ((bind(&GenParticle::pdgid, _1) == decay_flav)||(bind(&GenParticle::pdgid, _1) == -decay_flav))));
    if (gen_leptons.size() != 2) return 0;
    counters_["z_decay_flav"] += weight;

    //Step 2a: Select Z candidate if mass in range and leptons have opposite sign
    bool gen_Z_yes = false;
    CompositeCandidate gen_lepton_pair;
    gen_lepton_pair.AddCandidate("lepton1", gen_leptons[0]);
    gen_lepton_pair.AddCandidate("lepton2", gen_leptons[1]);
    bool gen_mass_charge = PairMassInRange(&gen_lepton_pair, z_mass_low_, z_mass_high_) && PairOppSign(&gen_lepton_pair);
    if (gen_mass_charge) {
      unsigned nGenLeptonsPass = 0;
      if (mode_ == 0) {
        nGenLeptonsPass = std::count_if(gen_leptons.begin(), gen_leptons.end(), 
                      bind(MinPtMaxEta, _1, gen_elec_pt_baseline_, gen_elec_eta_baseline_));
      } else {
        nGenLeptonsPass = std::count_if(gen_leptons.begin(), gen_leptons.end(), 
                      bind(MinPtMaxEta,_1,gen_muon_pt_baseline_, gen_muon_eta_baseline_));
      }
      if (nGenLeptonsPass == 2) gen_Z_yes = true;
    }


    
    //Step 2b: If using parton jets, fill a collection of b-flavour, if gen jets
    //fill the appropriate matching collection, and get matches
    unsigned nBGenJetsPass = 0;
    unsigned  gen_b = 0;
    std::vector<GenJet *> gen_jets = event->GetPtrVec<GenJet>("genJets");
    std::vector<GenParticle*> bhadrons = MakeFinalBHadronsCollection(gen_particles);
    //erase_if(bhadrons, !boost::bind(MinPtMaxEta, _1, 5.0, 1000.));
    std::vector< std::pair<GenJet*, GenParticle*> > genJgenBHMatch = MatchByDR(gen_jets, bhadrons, gen_jet_bhadron_dr_ , true, true);
    gen_jets = ExtractFirst(genJgenBHMatch);
    nBGenJetsPass = std::count_if(gen_jets.begin(), gen_jets.end(), bind(MinPtMaxEta, _1, gen_jet_pt_, gen_jet_eta_) && bind(MinDRToCollection<GenParticle*>, _1, gen_leptons, gen_jet_gen_lepton_dr_) ); 
    if (nBGenJetsPass == 1) gen_b = 1;
    if (nBGenJetsPass >= 2) gen_b = 2;
    //Update counters 
    gen_mat(2-gen_b,gen_Z_yes) += weight;

    //Step 3: Determine if both Gen Leptons would pass pT and Eta Cuts
    //---------------------------------------------------------------------------
    unsigned nGenLeptonsPass = 0;
    unsigned gen_Zb = 0;
    bool gen_Z_kin_yes = false;
    if (mode_ == 0) {
      nGenLeptonsPass = std::count_if(gen_leptons.begin(), gen_leptons.end(), bind(MinPtMaxEta, _1, gen_elec_pt_, gen_elec_eta_)); 
    } else {
      nGenLeptonsPass = std::count_if(gen_leptons.begin(), gen_leptons.end(), bind(MinPtMaxEta, _1, gen_muon_pt_, gen_muon_eta_));
    }
    if (nGenLeptonsPass == 2) gen_Z_kin_yes = gen_Z_yes;
    //Update counters
    gen_kin_mat(2-gen_b,gen_Z_kin_yes) += weight;
    if (gen_Z_kin_yes && gen_b == 1) gen_Zb = 1;
    if (gen_Z_kin_yes && gen_b >= 2) gen_Zb = 2;

    //Step 4a: Filter reco leptons on pT and Eta, select pairs with mass & charge
    //---------------------------------------------------------------------------
    unsigned rec_Zb = 0;
    bool rec_Z_yes = false;
    if (mode_ == 1) {
      gen_leptons = gen_particles;
      erase_if(gen_leptons, !((bind(&GenParticle::status, _1) == 1) && ((bind(&GenParticle::pdgid, _1) == decay_flav)||(bind(&GenParticle::pdgid, _1) == -decay_flav))));
    }
    std::vector<Electron *> reco_elecs = event->GetPtrVec<Electron>("electrons");
    std::vector<Muon *> reco_muons = event->GetPtrVec<Muon>("muonsPFlow");
    if (mode_ == 0) {//Electrons
      std::vector< std::pair<GenParticle*, Electron*> > GenPElecMatch = MatchByDR(gen_leptons, reco_elecs, gen_reco_elec_dr_ , true, true);
      reco_elecs = ExtractSecond(GenPElecMatch);
      erase_if(reco_elecs, !boost::bind(MinPtMaxEta, _1, reco_elec_pt_, reco_elec_eta_));
      erase_if(reco_elecs,  boost::bind(InEcalGap, _1));
      std::vector< std::pair<Electron*, Electron*> > elecPairs = MakePairs(reco_elecs);
      std::vector<CompositeCandidate *> elec_pairs(elecPairs.size(), NULL);
      for (unsigned i = 0; i < elecPairs.size(); ++i) {
        elec_pairs[i] = new CompositeCandidate();
        elec_pairs[i]->AddCandidate("elec1", elecPairs[i].first);
        elec_pairs[i]->AddCandidate("elec2", elecPairs[i].second);
      }
      erase_if(elec_pairs, !boost::bind(PairMassInRange, _1, z_mass_low_, z_mass_high_));
      erase_if(elec_pairs, !boost::bind(PairOppSign, _1));
      if (elec_pairs.size() > 1) {
          std::sort(elec_pairs.begin(),elec_pairs.end(), 
              boost::bind(MassDiffCompare,_1, _2, 91.19));
          elec_pairs.erase(elec_pairs.begin()+1, elec_pairs.end());
      }
      reco_elecs.clear();
      if (elec_pairs.size() == 1){
        rec_Z_yes = true;
        reco_elecs.resize(2);
        reco_elecs.at(0) = dynamic_cast<Electron *>(elec_pairs.at(0)->At(0));
        reco_elecs.at(1) = dynamic_cast<Electron *>(elec_pairs.at(0)->At(1));
      }
      for (unsigned i = 0; i < elec_pairs.size(); ++i) delete elec_pairs[i];
    } else {//Muons
      std::vector< std::pair<GenParticle*, Muon*> > GenPMuonMatch = MatchByDR(gen_leptons, reco_muons, gen_reco_muon_dr_, true, true);
      reco_muons = ExtractSecond(GenPMuonMatch);
      erase_if(reco_muons, !boost::bind(MinPtMaxEta, _1, reco_muon_pt_, reco_muon_eta_));
      std::vector< std::pair<Muon*, Muon*> > muonPairs = MakePairs(reco_muons);
      std::vector<CompositeCandidate *> muon_pairs(muonPairs.size(), NULL);
      for (unsigned i = 0; i < muonPairs.size(); ++i) {
        muon_pairs[i] = new CompositeCandidate();
        muon_pairs[i]->AddCandidate("muon1", muonPairs[i].first);
        muon_pairs[i]->AddCandidate("muon2", muonPairs[i].second);
      }
      erase_if(muon_pairs, !boost::bind(PairMassInRange, _1, z_mass_low_, z_mass_high_));
      erase_if(muon_pairs, !boost::bind(PairOppSign, _1));
      if (muon_pairs.size() > 1) {
          std::sort(muon_pairs.begin(),muon_pairs.end(), 
              boost::bind(MassDiffCompare,_1, _2, 91.19));
          muon_pairs.erase(muon_pairs.begin()+1, muon_pairs.end());
      }
      if (muon_pairs.size() == 1){
        rec_Z_yes = true;
        reco_muons.resize(2);
        reco_muons.at(0) = dynamic_cast<Muon *>(muon_pairs.at(0)->At(0));
        reco_muons.at(1) = dynamic_cast<Muon *>(muon_pairs.at(0)->At(1));
      }
      for (unsigned i = 0; i < muon_pairs.size(); ++i) delete muon_pairs[i];
    }
    if (rec_Z_yes) counters_["rec_Z_yes"] += weight;

    //Step 4b: Filter reco jets on pT, Eta and DR to Leptons (if found)
    //---------------------------------------------------------------------------
    unsigned rec_b = 0;
    std::vector<PFJet *> reco_jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    erase_if(reco_jets, !boost::bind(MinPtMaxEta,_1, reco_jet_pt_, reco_jet_eta_));
    if (mode_ == 0) {
    erase_if(reco_jets, !boost::bind(MinDRToCollection<Electron*>, _1, 
          reco_elecs, reco_jet_lepton_dr_)); 
    } else {
    erase_if(reco_jets, !boost::bind(MinDRToCollection<Muon*>, _1, 
          reco_muons, reco_jet_lepton_dr_)); 
    }
    std::vector< std::pair<PFJet*, GenJet*> > recJGenJMatch = MatchByDR(reco_jets, gen_jets, reco_gen_jet_dr_, true, true);
    rec_b = recJGenJMatch.size();
    reco_jets = ExtractFirst(recJGenJMatch);
    //Update counters
    if (rec_Z_yes && rec_b == 1) rec_Zb = 1;
    if (rec_Z_yes && rec_b >= 2) rec_Zb = 2;
    e_r_mat(2-rec_Zb ,gen_Zb) += weight;
    if (rec_b > 0) counters_["rec_b_yes"] += weight;
    if(!rec_Zb) return 0;
    if(rec_Zb == 1) counters_["rec1B"] += weight;
    if(rec_Zb >= 2) counters_["rec2B"] += weight;

    //Step 5: Apply Lepton ID & ISO
    //---------------------------------------------------------------------------
    bool lep_id_iso_yes = false;
    if (mode_ == 0) {//Electrons
      erase_if(reco_elecs, !bind(ElectronZbbID, _1));
      if (reco_elecs.size() > 1) counters_["rec_lep_id"] += weight;
      erase_if(reco_elecs, !bind(ElectronZbbIso, _1, false, 0.15));
      if (reco_elecs.size() > 1) counters_["rec_lep_iso"] += weight;
      erase_if(reco_elecs, !(bind(fabs, bind(&Electron::dxy_vertex, _1)) < 0.02));
      if (reco_elecs.size() > 1) counters_["rec_lep_db"] += weight;
      if (reco_elecs.size() > 1) {
        lep_id_iso_yes = true;
        if (tp_rw_) weight *= (ElectronIdIsoSF(reco_elecs[0])*ElectronIdIsoSF(reco_elecs[1])*ElectronTriggerSF(reco_elecs[0], reco_elecs[1]));
        counters_["rec_lep_id_iso"] += weight;
        if (rec_Zb == 1) counters_["rec1B_lep_id_iso"] += weight;
        if (rec_Zb >= 2) counters_["rec2B_lep_id_iso"] += weight;
      } else {
        return 0;
      }
    } else {//Muons
      erase_if(reco_muons, !bind(MuonTight, _1));
      erase_if(reco_muons, !bind(PF04Isolation<Muon>, _1, 0.5, 0.2));
      erase_if(reco_muons, !(bind(fabs, bind(&Muon::dxy_vertex, _1)) < 0.02));
      if (reco_muons.size() > 1) {
        lep_id_iso_yes = true;
        if (tp_rw_) weight *= (MuonIdIsoSF(reco_muons[0]) * MuonIdIsoSF(reco_muons[1]) * MuonTriggerSF(reco_muons[0], reco_muons[1]));
        counters_["rec_lep_id_iso"] += weight;//Re-weight TP here
        if (rec_Zb == 1) counters_["rec1B_lep_id_iso"] += weight;//Re-weight TP here
        if (rec_Zb >= 2) counters_["rec2B_lep_id_iso"] += weight;//Re-weight TP here
      } else {
        return 0;
      }
    }

    //Step 6: Apply Jet b-tagging
    //Apply b-tag reweighting here
    //---------------------------------------------------------------------------
    unsigned nHE = std::count_if(reco_jets.begin(),reco_jets.end(), bind(&PFJet::GetBDiscriminator, _1, "simpleSecondaryVertexHighEffBJetTags") > 1.74);
    unsigned nHP = std::count_if(reco_jets.begin(),reco_jets.end(), bind(&PFJet::GetBDiscriminator, _1, "simpleSecondaryVertexHighPurBJetTags") > 2.0);
    double bfactor_HE = 1.0;
    double bfactor_HP = 1.0;
     if (nHE >= 2 && btag_rw_) bfactor_HE = btag_weight.GetLouvainWeight(reco_jets, "SSVHEM", 2, 100);
     if (nHE == 1 && btag_rw_) bfactor_HE = btag_weight.GetLouvainWeight(reco_jets, "SSVHEM", 1, 1);
     if (nHP >= 2 && btag_rw_) bfactor_HP = btag_weight.GetLouvainWeight(reco_jets, "SSVHPT", 2, 100);
     if (nHP == 1 && btag_rw_) bfactor_HP = btag_weight.GetLouvainWeight(reco_jets, "SSVHPT", 1, 1);
     if (nHE > 2) nHE = 2;
     if (nHP > 2) nHP = 2;
    e_b_HE_mat(2-nHE,rec_Zb-1) += weight*bfactor_HE;
    e_b_HP_mat(2-nHP,rec_Zb-1) += weight*bfactor_HP;

    return 0;
  }


  std::vector<GenParticle *> ZbbUnfolding::MakeFinalBHadronsCollection (std::vector<GenParticle *> const& partVec) const {
    std::vector<GenParticle *> bhadrons;
    std::vector<GenParticle *> bhadronsFinal;
    std::vector<int> bhadronIdx;
    BOOST_FOREACH (GenParticle *particle, partVec) {
      bool is_bflav = false;
      unsigned pdgidNoSign = unsigned(abs(particle->pdgid()));
      std::string asString = boost::lexical_cast<std::string>(pdgidNoSign);
      if (asString.size() > 1 && asString.at(0) == char('5')) is_bflav = true;
      if (is_bflav) {
        bhadrons.push_back(particle);
        bhadronIdx.push_back(particle->index());
      }
    }
    BOOST_FOREACH (GenParticle* bhadron, bhadrons) {
      bool has_bhadron_daughter = false;
      std::vector<int> daughterIdx = bhadron->daughters();
      BOOST_FOREACH (int Idx, daughterIdx) {
        if (std::count(bhadronIdx.begin(),bhadronIdx.end(), Idx) > 0){
          has_bhadron_daughter = true;
          break;//No need to keep looping
        }//If this daughter index is in the list of bhadron indices
      }//Loop through daughter indices, see if exists in bhadronIdx
      if (!has_bhadron_daughter && bhadron->pt() > 3) bhadronsFinal.push_back(bhadron);
    }//Loop through bhadrons
    return bhadronsFinal;
  }

  //void ZbbUnfolding::PrintEff(std::string name, double num, double den) {
  //    TEfficiency eff;
  //    std::cout << name << ":\t\t" << (num/den) << "\t" << (eff.ClopperPearson(den+0.5,num+0.5,0.683 ,1)-(num/den)) << std::endl;
  //}
  void ZbbUnfolding::PrintEff(std::string name, double num, double den) {
    TEfficiency eff;
    std::cout << name << ":\t\t" << /*num << "/" << den << " = " <<*/ (num/den) << "\t+/- " << (eff.ClopperPearson(den+0.5,num+0.5,0.683 ,1)-(num/den)) << std::endl;
  };
  void ZbbUnfolding::PrintEffRatio(std::string name, double num, double den) {
    double num_err = sqrt(num);
    double den_err = sqrt(den);
    double rat = num / den;
    double rat_err = (rat*rat) * ( 
        ((num_err*num_err)/(num*num)) 
        + ((den_err*den_err)/(den*den)) 
        - ((2.0*num_err*den_err)/(num*den)));
    rat_err = sqrt(rat_err);
    std::cout << name << ":\t\t" << rat << "\t+/-" << rat_err << std::endl;
  }



  int ZbbUnfolding::PostAnalysis() {
    std::cout << "-----------------------Zbb Unfolding Report-----------------------" << std::endl;
    std::cout << "Mode  = " << mode_ << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Events Processed: " << counters_["events_proc"] << std::endl;
    std::cout << "Real Processed: " << counters_["real_proc"] << std::endl;
    double tot = counters_["z_decay_flav"];
    std::cout << std::fixed << std::setprecision(5) << std::endl;
    std::cout << "Z Decays to chosen flavour: " << counters_["z_decay_flav"] << std::endl;
    std::cout << "Of these:" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Gen Level:" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    //std::cout << "Gen Level (Zbb):" << std::endl;
    //std::cout << "gen2B:\t" << gen_mat(0,0)/tot << "\t\t" <<
    //                            gen_mat(0,1)/tot << std::endl;
    //std::cout << "gen1B:\t" << gen_mat(1,0)/tot << "\t\t" <<
    //                            gen_mat(1,1)/tot << std::endl;
    //std::cout << "gen0B:\t" << gen_mat(2,0)/tot << "\t\t" <<
    //                            gen_mat(2,1)/tot << std::endl;
    //std::cout << "\tgenZ_N\t\tgenZ_Y" << std::endl;
    //std::cout << "---------------------------------------" << std::endl;
    std::cout << "Gen Level (Zbb+kin):" << std::endl;
    std::cout << "gen2B:\t" << gen_kin_mat(0,0)/tot << "\t\t" <<
                                gen_kin_mat(0,1)/tot << std::endl;
    std::cout << "gen1B:\t" << gen_kin_mat(1,0)/tot << "\t\t" <<
                                gen_kin_mat(1,1)/tot << std::endl;
    std::cout << "gen0B:\t" << gen_kin_mat(2,0)/tot << "\t\t" <<
                                gen_kin_mat(2,1)/tot << std::endl;
    std::cout << "\tgenZkin_N\t\tgenZkin_Y" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    PrintEff("a_l_1", gen_kin_mat(1,1), gen_mat(1,1));
    PrintEff("a_l_2", gen_kin_mat(0,1), gen_mat(0,1));
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "rec_Z_yes:\t" << counters_["rec_Z_yes"] << std::endl;
    std::cout << "rec_b_yes:\t" << counters_["rec_b_yes"] << std::endl;
    std::cout << "Rec2B:\t" <<  e_r_mat(0,0)/tot << "\t\t" <<
                                e_r_mat(0,1)/tot << "\t\t" <<
                                e_r_mat(0,2)/tot << std::endl;
    std::cout << "Rec1B:\t" <<  e_r_mat(1,0)/tot << "\t\t" <<
                                e_r_mat(1,1)/tot << "\t\t" <<
                                e_r_mat(1,2)/tot << std::endl;
    std::cout << "Rec0B:\t" <<  e_r_mat(2,0)/tot << "\t\t" <<
                                e_r_mat(2,1)/tot << "\t\t" <<
                                e_r_mat(2,2)/tot << std::endl;
    std::cout << "\tGen0B\t\tGen1B\t\tGen2B" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    double ch_L = e_r_mat(0,0) + e_r_mat(1,0) + e_r_mat(2,0);
    double ch_M = e_r_mat(0,1) + e_r_mat(1,1) + e_r_mat(2,1);
    double ch_N = e_r_mat(0,2) + e_r_mat(1,2) + e_r_mat(2,2);
    double c_had_2b_den = ch_N;
    double c_had_2b_num = e_r_mat(0,0) + e_r_mat(0,1) + e_r_mat(0,2);
    PrintEffRatio("C_2b_hadron", c_had_2b_num, c_had_2b_den);
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Rec2B:\t" <<  e_r_mat(0,0)/ch_L << "\t\t" <<
                                e_r_mat(0,1)/ch_M << "\t\t" <<
                                e_r_mat(0,2)/ch_N << std::endl;
    std::cout << "Rec1B:\t" <<  e_r_mat(1,0)/ch_L << "\t\t" <<
                                e_r_mat(1,1)/ch_M << "\t\t" <<
                                e_r_mat(1,2)/ch_N << std::endl;
    std::cout << "Rec0B:\t" <<  e_r_mat(2,0)/ch_L << "\t\t" <<
                                e_r_mat(2,1)/ch_M << "\t\t" <<
                                e_r_mat(2,2)/ch_N << std::endl;
    std::cout << "\tGen0B\t\tGen1B\t\tGen2B" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    PrintEff("e_r_11", e_r_mat(1,1), ch_M);
    PrintEff("e_r_21", e_r_mat(1,2), ch_N);
    PrintEff("e_r_12", e_r_mat(0,1), ch_M);
    PrintEff("e_r_22", e_r_mat(0,2), ch_N);
    PrintEff("e_r_01", e_r_mat(1,0), ch_L);
    PrintEff("e_r_02", e_r_mat(0,0), ch_L);
    PrintEffRatio("R", ch_L, (ch_M+ch_N));
    std::cout << "---------------------------------------" << std::endl;
    PrintEff("e_l_1", counters_["rec1B_lep_id_iso"], counters_["rec1B"]);
    PrintEff("e_l_2", counters_["rec2B_lep_id_iso"], counters_["rec2B"]);
    //std::cout << "e_l_den: " << counters_["rec1B"] << "\t" << counters_["rec2B"] << std::endl;
    //std::cout << "e_l_num: " << counters_["rec1B_lep_id_iso"] << "\t" << counters_["rec2B_lep_id_iso"] << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    double tot_HEHE_1B =  counters_["rec1B_lep_id_iso"];
    double tot_HEHE_2B =  counters_["rec2B_lep_id_iso"];
    std::cout << "Tag2BHE:\t" <<  e_b_HE_mat(0,0)/tot_HEHE_1B << "\t\t" <<
                                  e_b_HE_mat(0,1)/tot_HEHE_2B << std::endl;
    std::cout << "Tag1BHE:\t" <<  e_b_HE_mat(1,0)/tot_HEHE_1B << "\t\t" <<
                                  e_b_HE_mat(1,1)/tot_HEHE_2B << std::endl;
    std::cout << "Tag0BHE:\t" <<  e_b_HE_mat(2,0)/tot_HEHE_1B << "\t\t" <<
                                  e_b_HE_mat(2,1)/tot_HEHE_2B << std::endl;
    //std::cout <<  e_b_HE_mat(1,0) + e_b_HE_mat(1,1) << std::endl;
    //std::cout <<  e_b_HE_mat(0,0) + e_b_HE_mat(0,1) << std::endl;
    std::cout << "\tRec1B\t\tRec2B" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    double tot_HPHP_1B = counters_["rec1B_lep_id_iso"];
    double tot_HPHP_2B = counters_["rec2B_lep_id_iso"];
    std::cout << "Tag2BHP:\t" <<  e_b_HP_mat(0,0)/tot_HPHP_1B << "\t\t" <<
                                  e_b_HP_mat(0,1)/tot_HPHP_2B << std::endl;
    std::cout << "Tag1BHP:\t" <<  e_b_HP_mat(1,0)/tot_HPHP_1B << "\t\t" <<
                                  e_b_HP_mat(1,1)/tot_HPHP_2B << std::endl;
    std::cout << "Tag0BHP:\t" <<  e_b_HP_mat(2,0)/tot_HPHP_1B << "\t\t" <<
                                  e_b_HP_mat(2,1)/tot_HPHP_2B << std::endl;
    std::cout << "\tRec1B\t\tRec2B" << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "HE:" << std::endl;
    PrintEff("e_b_11", e_b_HE_mat(1,0), tot_HEHE_1B);
    PrintEff("e_b_21", e_b_HE_mat(1,1), tot_HEHE_2B);
    PrintEff("e_b_22", e_b_HE_mat(0,1), tot_HEHE_2B);
    std::cout << "HP:" << std::endl;
    PrintEff("e_b_11", e_b_HP_mat(1,0), tot_HEHE_1B);
    PrintEff("e_b_21", e_b_HP_mat(1,1), tot_HEHE_2B);
    PrintEff("e_b_22", e_b_HP_mat(0,1), tot_HEHE_2B);
    return 0;
  }

  void ZbbUnfolding::PrintInfo() {
    ;
  }


  double ZbbUnfolding::ElectronIdIsoSF(Electron const* elec) const {
    double eta = fabs(elec->sc_eta());
    if (eta < 0.9) { 
      return (0.997 * 1.00);
    } else if (eta >= 0.9 && eta < 2.1) {
      return (1.000 * 0.993);
    } else {
      return (1.010 * 1.010);
    }
  }

  double ZbbUnfolding::MuonIdIsoSF(Muon const* muon) const {
      double eta = fabs(muon->eta());
      if (eta < 0.9) { 
        return (0.995 * 0.993);
      } else if (eta >= 0.9 && eta < 2.1) {
        return (0.972 * 0.997);
      } else {
        return (0.978 * 1.010);
      }
  }
  double ZbbUnfolding::ElectronTriggerSF(Electron const* elec1, Electron const* elec2) const {
    double pt[2];
    double eta[2];
    double sfH[2];
    double sfL[2];
    pt[0] = elec1->pt();
    pt[1] = elec2->pt();
    eta[0] = fabs(elec1->sc_eta());
    eta[1] = fabs(elec2->sc_eta());
    for (unsigned i = 0; i < 2; ++i) {
      if (eta[i] < 1.4) {
        if (pt[i] > 20.0 && pt[i] <= 30.0) {
          sfH[i] = 0.983;
          sfL[i] = 0.995;
        } else if (pt[i] > 30.0 && pt[i] <= 50.0) {
          sfH[i] = 0.995;
          sfL[i] = 0.998;
        } else {
          sfH[i] = 0.998;
          sfL[i] = 0.999;
        }
      } else {
        if (pt[i] > 20.0 && pt[i] <= 30.0) {
          sfH[i] = 0.982;
          sfL[i] = 0.998;
        } else if (pt[i] > 30.0 && pt[i] <= 50.0) {
          sfH[i] = 0.995;
          sfL[i] = 0.999;
        } else {
          sfH[i] = 0.998;
          sfL[i] = 0.999;
        }
      }
    }
    return ( (sfH[0]*sfL[1]) + (sfL[0]*sfH[1]) - (sfH[0]*sfH[1]));
  }
  double ZbbUnfolding::MuonTriggerSF(Muon const* muon1, Muon const* muon2) const {
    double pt[2];
    double eta[2];
    double sfMu7[2];
    double sfMu13Mu8H[2];
    double sfMu13Mu8L[2];
    double sfMu17Mu8H[2];
    double sfMu17Mu8L[2];
    pt[0] = muon1->pt();
    pt[1] = muon2->pt();
    eta[0] = fabs(muon1->eta());
    eta[1] = fabs(muon2->eta());
    for (unsigned i = 0; i < 2; ++i) {
      if (eta[i] < 1.2) {
        sfMu7[i] = 0.971;
        if (pt[i] > 20.0 && pt[i] <= 30.0) {
          sfMu13Mu8H[i] = 0.967;
          sfMu13Mu8L[i] = 0.968;
          sfMu17Mu8H[i] = 0.960;
          sfMu17Mu8L[i] = 0.962;
        } else if (pt[i] > 30.0 && pt[i] <= 50.0) {
          sfMu13Mu8H[i] = 0.967;
          sfMu13Mu8L[i] = 0.967;
          sfMu17Mu8H[i] = 0.965;
          sfMu17Mu8L[i] = 0.967;
        } else {
          sfMu13Mu8H[i] = 0.968;
          sfMu13Mu8L[i] = 0.968;
          sfMu17Mu8H[i] = 0.966;
          sfMu17Mu8L[i] = 0.967;
        }
      } else {
        sfMu7[i] = 0.948;
        if (pt[i] > 20.0 && pt[i] <= 30.0) {
          sfMu13Mu8H[i] = 0.924;
          sfMu13Mu8L[i] = 0.935;
          sfMu17Mu8H[i] = 0.907;
          sfMu17Mu8L[i] = 0.919;
        } else if (pt[i] > 30.0 && pt[i] <= 50.0) {
          sfMu13Mu8H[i] = 0.927;
          sfMu13Mu8L[i] = 0.933;
          sfMu17Mu8H[i] = 0.919;
          sfMu17Mu8L[i] = 0.929;
        } else {
          sfMu13Mu8H[i] = 0.934;
          sfMu13Mu8L[i] = 0.939;
          sfMu17Mu8H[i] = 0.923;
          sfMu17Mu8L[i] = 0.930;
        }
      }
    }
    double valMu7 = sfMu7[0] * sfMu7[1];
    double valMu13Mu8 = ((sfMu13Mu8H[0]*sfMu13Mu8L[1]) + (sfMu13Mu8L[0]*sfMu13Mu8H[1]) - (sfMu13Mu8H[0]*sfMu13Mu8H[1]));
    double valMu17Mu8 = ((sfMu17Mu8H[0]*sfMu17Mu8L[1]) + (sfMu17Mu8L[0]*sfMu17Mu8H[1]) - (sfMu17Mu8H[0]*sfMu17Mu8H[1]));
    return (0.04*valMu7 + 0.80*valMu13Mu8 + 0.16*valMu17Mu8 );
  }
}
