#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttVbfCategory.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "TMVA/Reader.h"
#include "TVector3.h"

namespace ic {

  HttVbfCategory::HttVbfCategory(std::string const& name, std::string const& jets_label) : ModuleBase(name) {
    mode_ = 0;
    fs_ = NULL;

    jet_pt_ = 30.0;
    jet_eta_ = 4.7;
    vbf_cjv_pt_ = 30.0;
    mva_cut_ = 0.5;

    btag_jet_pt_ = 20.0;
    btag_jet_eta_ = 2.4;

    do_mva_pu_id_ = true;
    do_vbf_mva_ = true;
    do_jetvtx_assoc_ = false;
    do_mc_eff_ = false;
    do_cjv_ = true;
    make_plots_ = false;
    use_hcp_mva_ = true;
    is_2012_ = true;

    do_btag_weight_ = false;
    
    make_mva_tree_ = false;
    mva_name_ = "MVA_Tree_Default.root";

    jets_label_ = jets_label;
    met_label_ = "pfMVAMet";

    vbf_delta_eta_ = 3.5;
    vbf_mass_ = 500.0;

    loose_mjj_ = 200.0;
    loose_delta_eta_ = 2.0;

    n_real_events_ = 0.0;
    n_pu_events_ = 0.0;
    n_real_events_pass_ = 0.0;
    n_pu_events_pass_ = 0.0;

    twojet_plots_.resize(4);
    twojet_yields_.resize(6);
    vbf_plots_.resize(4);
    vbf_yields_.resize(6);
    vbf_loose_plots_.resize(4);
    vbf_loose_yields_.resize(6);
    vbfvars = std::vector<float>(8,0);
  }

  HttVbfCategory::~HttVbfCategory() {
    ;
  }

  int HttVbfCategory::PreAnalysis() {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "PreAnalysis Info for HTT VBF Category" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Mode: [" << mode_ << "]" << std::endl;
    std::cout << "is_2012?: [" << is_2012_ << "]" << std::endl;
    if (do_vbf_mva_) std::cout << "Use HCP MVA?: [" << use_hcp_mva_ << "]" << std::endl;
    std::cout << "Jets: [" << jets_label_ << "] [Pt > " << jet_pt_ << "] [|eta| < " << jet_eta_ << "]" << std::endl;
    if (!do_vbf_mva_) std::cout << "VBF: [DeltaEta > " << vbf_delta_eta_ << "] [Mass > " << vbf_mass_ << "]" << std::endl;
    if (!do_vbf_mva_) std::cout << "Loose: [DeltaEta > " << loose_delta_eta_ << "] [Mass > " << loose_mjj_ << "]" << std::endl;
    if (do_vbf_mva_) std::cout << "VBF: [MVA > " << mva_cut_ << "]" << std::endl;
    if (do_cjv_) {
      std::cout << "Central Jet Veto: [Pt > " << vbf_cjv_pt_ << "]" << std::endl;
    } else {
      std::cout << "No Central Jet Veto!" << std::endl;
    }
    reader = new TMVA::Reader( "!Color:!Silent" );
    std::string file;
    if (do_vbf_mva_) {
      if (!use_hcp_mva_) {
        reader->AddVariable("mjj", &vbfvars[0]);
        reader->AddVariable("dEta", &vbfvars[1]);
        reader->AddVariable("dPhi", &vbfvars[2]);
        reader->AddVariable("ditau_pt", &vbfvars[3]);
        reader->AddVariable("dijet_pt", &vbfvars[4]);
        reader->AddVariable("dPhi_hj", &vbfvars[5]);
        reader->AddVariable("C1", &vbfvars[6]);
        reader->AddVariable("C2", &vbfvars[7]);
        file = "data/vbf_mva/VBFMVA_BDTG.weights.xml";
        if (mode_ == 2) file = "data/vbf_mva/VBFMVA_EMu_BDTG.weights.xml";
      } else {
        reader->AddVariable("mjj",  &vbfvars[0]);
        reader->AddVariable("dEta", &vbfvars[1]);
        reader->AddVariable("C1",   &vbfvars[6]);
        reader->AddVariable("C2",   &vbfvars[7]);
        file = is_2012_ ? "data/vbf_mva/VBFMVA_BDTG_HCP_52X.weights.xml" : "data/vbf_mva/VBFMVA_BDTG_HCP_42X.weights.xml";
        if (mode_ == 2) file = is_2012_ ? "data/vbf_mva/VBFMVA_EMu_BDTG_HCP_53X.weights.xml" : "data/vbf_mva/VBFMVA_EMu_BDTG_HCP_53X.weights.xml";
      }
      reader->BookMVA("BDTG", file);
    }
    twojet_plots_[0] = new HttPlots(fs_->mkdir("twojet_os_sel"));
    twojet_plots_[1] = new HttPlots(fs_->mkdir("twojet_ss_sel"));
    twojet_plots_[2] = new HttPlots(fs_->mkdir("twojet_os_con"));
    twojet_plots_[3] = new HttPlots(fs_->mkdir("twojet_ss_con"));

    vbf_plots_[0] = new HttPlots(fs_->mkdir("vbf_os_sel"));
    vbf_plots_[1] = new HttPlots(fs_->mkdir("vbf_ss_sel"));
    vbf_plots_[2] = new HttPlots(fs_->mkdir("vbf_os_con"));
    vbf_plots_[3] = new HttPlots(fs_->mkdir("vbf_ss_con"));
    vbf_loose_plots_[0] = new HttPlots(fs_->mkdir("vbfloose_os_sel"));
    vbf_loose_plots_[1] = new HttPlots(fs_->mkdir("vbfloose_ss_sel"));
    vbf_loose_plots_[2] = new HttPlots(fs_->mkdir("vbfloose_os_con"));
    vbf_loose_plots_[3] = new HttPlots(fs_->mkdir("vbfloose_ss_con"));

    hset2d_ = new Dynamic2DHistoSet(fs_->mkdir("vbf_extra"));
    hset2d_->Create("twojet__jeta1_vs_jeta2", 40, -5, 5, 40, -5, 5);
    hset2d_->Create("vbf__jeta1_vs_jeta2", 40, -5, 5, 40, -5, 5);

    if (make_mva_tree_) {
      lOFile = new TFile(mva_name_.c_str(),"RECREATE");
      lOFile->cd();
      lOTree = new TTree("MVATree","MVATree");
      lOTree->Branch("weight"   ,&(weight_));
      lOTree->Branch("m_jj"   ,&(vbfvars[0]));
      lOTree->Branch("delta_eta_jj"   ,&(vbfvars[1]));
      lOTree->Branch("delta_phi_jj"   ,&(vbfvars[2]));
      lOTree->Branch("ditau_pt"   ,&(vbfvars[3]));
      lOTree->Branch("dijet_pt"   ,&(vbfvars[4]));
      lOTree->Branch("delta_phi_hj"   ,&(vbfvars[5]));
      lOTree->Branch("delta_eta_hj"   ,&(vbfvars[6]));
      lOTree->Branch("vis_ditau_pt"   ,&(vbfvars[7]));
    }

    return 0;
  }

  int HttVbfCategory::Execute(TreeEvent *event) {

    // Get the category status if it exists, otherwise create it, set vbf flag to false
    std::map<std::string, bool> cat_status;
    if (event->Exists("cat_status")) {
      cat_status = event->Get< std::map<std::string, bool> >("cat_status");
    }
    cat_status["vbf"] = false;

    // Get what we'll need from the event
    EventInfo                         const*  eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<Vertex *>             const&  vertices  = event->GetPtrVec<Vertex>("vertices");
    Met                               const*  pfMVAMet  = event->GetPtr<Met>(met_label_);
    std::vector<CompositeCandidate *> const&  dilepton  = event->GetPtrVec<CompositeCandidate>("emtauCandidates");
    unsigned                                  sel_mode  = event->Get<unsigned>("sel_mode");
    
    // Weight needed for yields, plotting and for make mva tree
    double wt = eventInfo->total_weight();
    weight_ = wt;

    // Copy the jet collection, sort by pt (leading first), do pileup ID if requested
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_); // Make a copy of the jet collection
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    if (do_mva_pu_id_) ic::erase_if(jets, !boost::bind(&PFJet::pu_id_mva_loose, _1));

    // Make a copy for b-tagging
    std::vector<PFJet*> btag_jets = jets;
    ic::erase_if(btag_jets,!boost::bind(MinPtMaxEta, _1, btag_jet_pt_, btag_jet_eta_));

    // Before cutting on pt and eta for the tagging jets, make a copy that we'll use
    // for the central jet veto
    std::vector<PFJet*> nonvbf_jets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, jet_pt_, jet_eta_));
    
    // If there are more than two jets, fill plots and create vbf_jets vector to contain two highest
    // pt tagging jets
    std::vector<PFJet*> vbf_jets;
    if (jets.size() >= 2) {
        twojet_yields_[sel_mode] += wt;
      if (fs_ && sel_mode < 4) {
        twojet_plots_[sel_mode]->FillVertexPlots(eventInfo->good_vertices(), wt);
        twojet_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
        twojet_plots_[sel_mode]->FillJetPlots(jets, wt);
        twojet_plots_[sel_mode]->FillPt20JetPlots(btag_jets, wt, 1.0);
        if (event->Exists("svfitMass")) twojet_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
      }
      if (fs_ && sel_mode%2 == 0) twojet_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
      if (fs_ && sel_mode%2 == 1) twojet_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
      if (fs_ && sel_mode == 2) twojet_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
      if (fs_ && sel_mode == 3) twojet_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);

      vbf_jets.push_back(jets[0]);
      vbf_jets.push_back(jets[1]);
    } else {
      event->ForceAdd("cat_status", cat_status);
      return 2; // Event fails, but don't stop processing
    }

    if (mode_ == 2) {
      ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
      if (btag_jets.size() > 0) {
        event->ForceAdd("cat_status", cat_status);
        return 2;
      } else {
        if (do_btag_weight_) wt *= event->Get<double>("no_btag_weight");
      }
    }

    // If requested, determine if both jets or real or either are from pileup
    unsigned n_real_jets = 0;
    if (do_mc_eff_ && sel_mode == 0) {
      std::vector<GenJet *> & genjets = event->GetPtrVec<GenJet>("genJets");
      ic::erase_if(genjets,!boost::bind(MinPtMaxEta, _1, 15.0, 5.5));
      std::vector<std::pair<PFJet*, GenJet*> > matches = MatchByDR(vbf_jets, genjets, 0.5, true, true);
      n_real_jets = matches.size();
      if (n_real_jets == 2) {
        n_real_events_ += wt;
      } else {
        n_pu_events_ += wt;
      }
    }

    // Build a collection of jets in the central region, applying beta cut if the mva id hasn't been used
    ic::erase_if(nonvbf_jets, boost::bind(FoundIdInCollection<PFJet, PFJet>, _1, vbf_jets));
    double eta_high = (vbf_jets[0]->eta() > vbf_jets[1]->eta()) ? vbf_jets[0]->eta() : vbf_jets[1]->eta();
    double eta_low = (vbf_jets[0]->eta() > vbf_jets[1]->eta()) ? vbf_jets[1]->eta() : vbf_jets[0]->eta();
    std::vector<PFJet*> cjv_jets;
    for (unsigned i = 0; i < nonvbf_jets.size(); ++i) {
      double beta = nonvbf_jets[i]->beta();
      double charged_frac = (nonvbf_jets[i]->charged_em_energy() + nonvbf_jets[i]->charged_had_energy()) / nonvbf_jets[i]->uncorrected_energy();
      if (do_mva_pu_id_ && nonvbf_jets[i]->pt() > vbf_cjv_pt_ &&  
          nonvbf_jets[i]->eta() > eta_low && 
          nonvbf_jets[i]->eta() < eta_high) cjv_jets.push_back(nonvbf_jets[i]);
      if (!do_mva_pu_id_ && beta > 0.1 && charged_frac > 0.3 && nonvbf_jets[i]->pt() > vbf_cjv_pt_ &&  
          nonvbf_jets[i]->eta() > eta_low && 
          nonvbf_jets[i]->eta() < eta_high) cjv_jets.push_back(nonvbf_jets[i]);
     }




    // Apply CJV if requested
    if (cjv_jets.size() > 0 && do_cjv_) {
      //std::cout << "Event " << eventInfo->event() << " fails CJV!" << std::endl;
      event->ForceAdd("cat_status", cat_status);
      return 2; // Event fails, but don't stop processing
    }

    hset2d_->Fill("twojet__jeta1_vs_jeta2", vbf_jets[0]->eta(), vbf_jets[1]->eta(), wt);

    // Calculate the mva variables
    float mvaValue = 0.0;
    float mjj = (vbf_jets[0]->vector() + vbf_jets[1]->vector()).M();
    float dEta = fabs(vbf_jets[0]->eta() - vbf_jets[1]->eta());
    float dPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(vbf_jets[0]->vector(), vbf_jets[1]->vector()));
    TVector3 vTau, vMu, vMET, vDiTau, vDiTauVis;
    Candidate* lepton = dilepton.at(0)->GetCandidate("lepton1");
    Candidate* tau = dilepton.at(0)->GetCandidate("lepton2");
    vTau.SetPtEtaPhi(tau->pt(), tau->eta(), tau->phi());
    vMu.SetPtEtaPhi(lepton->pt(), lepton->eta(), lepton->phi());
    vMET.SetPtEtaPhi(pfMVAMet->pt(), 0, pfMVAMet->phi());
    vDiTau = vTau + vMu + vMET;
    vDiTauVis = vTau + vMu;
    TVector3 vJet1, vJet2, vDiJet;
    vJet1.SetPtEtaPhi(vbf_jets[0]->pt(), vbf_jets[0]->eta(), vbf_jets[0]->phi());
    vJet2.SetPtEtaPhi(vbf_jets[1]->pt(), vbf_jets[1]->eta(), vbf_jets[1]->phi());
    vDiJet = vJet1 + vJet2;
    float dPhi_hj = fabs(ROOT::Math::VectorUtil::DeltaPhi(vDiTau, vDiJet));
    double C1 = std::min(fabs(vDiTauVis.Eta() - vbf_jets[0]->eta()), fabs(vDiTauVis.Eta() - vbf_jets[1]->eta()));
    double C2 = vDiTauVis.Pt();
    vbfvars[0] = mjj;
    vbfvars[1] = dEta;
    vbfvars[2] = dPhi;
    vbfvars[3] = vDiTau.Pt();
    vbfvars[4] = vDiJet.Pt();
    vbfvars[5] = dPhi_hj;
    vbfvars[6] = C1;
    vbfvars[7] = C2;

    if (make_mva_tree_ && sel_mode == 0) lOTree->Fill();
    
    // Do MVA selection
    if (do_vbf_mva_) {
      mvaValue = reader->EvaluateMVA(vbfvars, "BDTG");
      if (fs_ && sel_mode < 4) {
        twojet_plots_[sel_mode]->FillVbfMvaPlots(mjj, dEta, dPhi, vDiTau.Pt(), vDiJet.Pt(), dPhi_hj, C1, C2, mvaValue, wt);
        bool pass_cuts = false;
        if (mjj > vbf_mass_ && dEta > vbf_delta_eta_) pass_cuts = true;
        bool pass_mva = false;
        if (mvaValue > mva_cut_) pass_mva = true;
        vbf_plots_[sel_mode]->FillVbfMvaPlots(mjj, dEta, pass_mva, pass_cuts, wt);

        // Fill loose vbf anyway for ichep comparison
        std::vector<std::pair<PFJet*, PFJet*> > vbf_pairs, loose_pairs;
        vbf_pairs = ic::MakePairs(vbf_jets);
        loose_pairs = vbf_pairs;
        ic::erase_if(loose_pairs, boost::bind(DEtaLessThan<PFJet*, PFJet*>, _1, loose_delta_eta_));
        ic::erase_if(loose_pairs, boost::bind(MassLessThan<PFJet*, PFJet*>, _1, loose_mjj_));
        if (loose_pairs.size() > 0) {
            vbf_loose_yields_[sel_mode] += wt;
          if (fs_ && sel_mode < 4) {
            vbf_loose_plots_[sel_mode]->FillVertexPlots(vertices, wt);
            vbf_loose_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
            vbf_loose_plots_[sel_mode]->FillJetPlots(vbf_jets, wt);
            if (event->Exists("svfitMass")) vbf_loose_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
          }
          if (fs_ && sel_mode%2 == 0) vbf_loose_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
          if (fs_ && sel_mode%2 == 1) vbf_loose_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
          if (fs_ && sel_mode == 2) vbf_loose_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
          if (fs_ && sel_mode == 3) vbf_loose_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);

        }


      }
      if (mvaValue > mva_cut_) {
        cat_status["vbf"] = true;
      } else {
        event->ForceAdd("cat_status", cat_status);
        return 2; // Event fails, but don't stop processing
      }
    } else { //Do cut based selection
      if (fs_ && sel_mode < 4) {
        twojet_plots_[sel_mode]->FillVbfMvaPlots(mjj, dEta, dPhi, vDiTau.Pt(), vDiJet.Pt(), dPhi_hj, C1, C2, mvaValue, wt);
      }
      std::vector<std::pair<PFJet*, PFJet*> > vbf_pairs, loose_pairs;
      vbf_pairs = ic::MakePairs(vbf_jets);
      loose_pairs = vbf_pairs;
      ic::erase_if(loose_pairs, boost::bind(DEtaLessThan<PFJet*, PFJet*>, _1, loose_delta_eta_));
      ic::erase_if(loose_pairs, boost::bind(MassLessThan<PFJet*, PFJet*>, _1, loose_mjj_));
      if (loose_pairs.size() > 0) {
          vbf_loose_yields_[sel_mode] += wt;
        if (fs_ && sel_mode < 4) {
          vbf_loose_plots_[sel_mode]->FillVertexPlots(vertices, wt);
          vbf_loose_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
          vbf_loose_plots_[sel_mode]->FillJetPlots(vbf_jets, wt);
          if (event->Exists("svfitMass")) vbf_loose_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
        }
        if (fs_ && sel_mode%2 == 0) vbf_loose_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        if (fs_ && sel_mode%2 == 1) vbf_loose_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        if (fs_ && sel_mode == 2) vbf_loose_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
        if (fs_ && sel_mode == 3) vbf_loose_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);

      }
      ic::erase_if(vbf_pairs, boost::bind(DEtaLessThan<PFJet*, PFJet*>, _1, vbf_delta_eta_));
      ic::erase_if(vbf_pairs, boost::bind(MassLessThan<PFJet*, PFJet*>, _1, vbf_mass_));
      if (vbf_pairs.size() > 0) { 
        cat_status["vbf"] = true;
      } else {
        event->ForceAdd("cat_status", cat_status);
        return 2; // Event fails, but don't stop processing
      }
    }

    if (do_jetvtx_assoc_) {
        bool pass_beta = true;
        bool jet_1_assoc = false;
        bool jet_2_assoc = false;
        for (unsigned i = 0; i < vbf_jets.size(); ++i) {
            double beta = vbf_jets[i]->beta();
            double charged_frac = (vbf_jets[i]->charged_em_energy() + vbf_jets[i]->charged_had_energy()) / vbf_jets[i]->uncorrected_energy();
            if (charged_frac > 0.3 && beta < 0.1) pass_beta = false;
            if (charged_frac > 0.3 && beta >= 0.1 && i == 0) jet_1_assoc = true;
            if (charged_frac > 0.3 && beta >= 0.1 && i == 1) jet_2_assoc = true;
        }
        if (!pass_beta) {
            event->ForceAdd("cat_status", cat_status);
            return 2; // Event fails, but don't stop processing
        }
        CompositeCandidate z = *(dilepton.at(0));
        Met * met = event->GetPtr<Met>("pfMet");
        CompositeCandidate z_j0;
        z_j0.AddCandidate("z", &z);
        z_j0.AddCandidate("j0", vbf_jets[0]);
        CompositeCandidate z_j1;
        z_j1.AddCandidate("z", &z);
        z_j1.AddCandidate("j1", vbf_jets[1]);
        CompositeCandidate z_j0j1;
        z_j0j1.AddCandidate("z", &z);
        z_j0j1.AddCandidate("j0", vbf_jets[0]);
        z_j0j1.AddCandidate("j1", vbf_jets[1]);
        std::vector<PFJet *> non_main_jets;
        std::vector<PFJet *> all_jets = event->GetPtrVec<PFJet>(jets_label_);
        for (unsigned i = 0; i < all_jets.size(); ++i) {
          if (all_jets[i] != vbf_jets[0] && all_jets[i] != vbf_jets[1]) non_main_jets.push_back(all_jets[i]);
        }
        ic::erase_if(non_main_jets,!boost::bind(MinPtMaxEta, _1, 15.0, 2.1));
        for (unsigned i = 0; i < non_main_jets.size(); ++i) {
          double beta = non_main_jets[i]->beta();
          double charged_frac = (non_main_jets[i]->charged_em_energy() + non_main_jets[i]->charged_had_energy()) / non_main_jets[i]->uncorrected_energy();
          if (beta < 0.3 || charged_frac < 0.3) continue;
          z.AddCandidate(boost::lexical_cast<std::string>(i), non_main_jets[i]);
          z_j0.AddCandidate(boost::lexical_cast<std::string>(i), non_main_jets[i]);
          z_j1.AddCandidate(boost::lexical_cast<std::string>(i), non_main_jets[i]);
          z_j0j1.AddCandidate(boost::lexical_cast<std::string>(i), non_main_jets[i]);
        }
        z.AddCandidate("met", met);
        z_j0.AddCandidate("met", met);
        z_j1.AddCandidate("met", met);
        z_j0j1.AddCandidate("met", met);
        double zj0j1_r_zj0 = z_j0j1.pt() / z_j0.pt();
        double zj0j1_r_zj1 = z_j0j1.pt() / z_j1.pt();
        double zj0j1_r_z = z_j0j1.pt() / z.pt();
        bool pass_kin = false;
        if (!jet_1_assoc && jet_2_assoc) {
            if (zj0j1_r_zj1 < 1.3) pass_kin =true;
        }
        if (jet_1_assoc && !jet_2_assoc) {
            if (zj0j1_r_zj0 < 1.3) pass_kin =true;
        }
        if (!jet_1_assoc && !jet_2_assoc) {
            if ((zj0j1_r_zj0 < 1.1 || zj0j1_r_zj1 < 1.1) && zj0j1_r_z < 1.0) pass_kin =true;
        }
        if (jet_1_assoc && jet_2_assoc) {
            pass_kin =true;
        }
        if (!pass_kin) {
            event->ForceAdd("cat_status", cat_status);
            return 2; // Event fails, but don't stop processing
        }
    }

    if (do_mc_eff_ && sel_mode == 0) {
      if (n_real_jets == 2) {
        n_real_events_pass_ += wt;
      } else {
        n_pu_events_pass_ += wt;
      }
    }


    //Do Plotting for passing
    vbf_yields_[sel_mode] += wt;

    hset2d_->Fill("vbf__jeta1_vs_jeta2", vbf_jets[0]->eta(), vbf_jets[1]->eta(), wt);


    if (fs_ && sel_mode < 4) {
      vbf_plots_[sel_mode]->FillVbfMvaPlots(mjj, dEta, dPhi, vDiTau.Pt(), vDiJet.Pt(), dPhi_hj, C1, C2, mvaValue, wt);
      vbf_plots_[sel_mode]->FillVertexPlots(vertices, wt);
      vbf_plots_[sel_mode]->FillLeptonMetPlots(*(dilepton[0]), *pfMVAMet, wt);
      vbf_plots_[sel_mode]->FillJetPlots(vbf_jets, wt);
      //if (event->Exists("genParticles")) {
      //  std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>("genParticles");
      //  vbf_plots_[sel_mode]->FillNPartons(parts, wt);
      //}
      if (event->Exists("svfitMass")) vbf_plots_[sel_mode]->FillSVFitMassPlot(event->Get<double>("svfitMass"),wt);
    }
    if (fs_ && sel_mode%2 == 0) vbf_plots_[0]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode%2 == 1) vbf_plots_[1]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 2) vbf_plots_[2]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);
    if (fs_ && sel_mode == 3) vbf_plots_[3]->FillIncMT(*(dilepton[0]), *pfMVAMet, wt);



    event->ForceAdd("cat_status", cat_status);
    return 0;
  }
  int HttVbfCategory::PostAnalysis() {
        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Post-Analysis Info for HTT VBF Category" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "Sel Mode:" % "OS-Sel" % "SS-Sel" % "OS-Con" % "SS-Con" % "OS-Out" % "SS-Out";
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "twojet:" % twojet_yields_[0] % twojet_yields_[1] % twojet_yields_[2] % twojet_yields_[3] % twojet_yields_[4] % twojet_yields_[5];
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "vbfloose:" % vbf_loose_yields_[0] % vbf_loose_yields_[1] % vbf_loose_yields_[2] % vbf_loose_yields_[3] % vbf_loose_yields_[4] % vbf_loose_yields_[5];
        std::cout << boost::format("%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n") 
          % "vbf:" % vbf_yields_[0] % vbf_yields_[1] % vbf_yields_[2] % vbf_yields_[3] % vbf_yields_[4] % vbf_yields_[5];
    if (do_mc_eff_) {
        std::cout << "Do MVA PU ID: " << do_mva_pu_id_ << std::endl;
        std::cout << "Do VBF MVA Selection: " << do_vbf_mva_ << std::endl;
        std::cout << "Do beta+pt-balance Selection: " << do_jetvtx_assoc_ << std::endl;
        std::cout << "Apply CJV: " << do_cjv_ << std::endl;
        PrintEff("Real Events", n_real_events_pass_, n_real_events_);
        PrintEff("PU Events", n_pu_events_pass_, n_pu_events_);
        PrintEff("Total Events", n_real_events_pass_+n_pu_events_pass_, n_real_events_+n_pu_events_);
        if (make_mva_tree_) {
          lOFile->cd();
          lOTree->Write();
          lOFile->Close();
        }
    }
    return 0;
  }

  void HttVbfCategory::PrintInfo() {
    ;
  }
}
