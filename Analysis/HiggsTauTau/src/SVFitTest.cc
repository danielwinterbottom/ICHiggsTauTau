#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/SVFitTest.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include "HiggsTauTau/interface/SVFitService.h"
#include <stdlib.h>
#include "boost/format.hpp"
#include "boost/filesystem.hpp"

namespace ic {

  SVFitTest::SVFitTest(std::string const& name) : ModuleBase(name), channel_(channel::et), strategy_(strategy::spring15) {
  
    out_file_ = NULL;
    out_tree_ = NULL;
    out_cand1_ = NULL;
    out_cand2_ = NULL;
    out_met_ = NULL;

    //out_event_hash_ = 0;
    out_event_ = 0;
    out_lumi_ = 0;
    out_run_ = 0;
    out_objects_hash_ = 0;
    decay_mode_ = 0;
    run_mode_ = 0;
    fail_mode_ = 0;
    require_inputs_match_ = true;
    legacy_svfit_ = false;
    from_grid_ = false;
    read_svfit_mt_ = false;
    do_preselection_ = false;
    tau_optimisation_ = false;
    read_all_ = false;
    dm1_ = -1;
    dm2_ = -1;

    file_counter_ = 0;
    event_counter_ = 0;
    split_ = 5000;
    outname_ = "svfit_test";
    outputadd_ = "";
    fullpath_ = "SVFIT_2012/";

    MC_ = false;
  }

  SVFitTest::~SVFitTest() {
    ;
  }

  int SVFitTest::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "SVFitTest" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    if(legacy_svfit_){
      if (channel_ == channel::et) decay_mode_ = 0;
      if (channel_ == channel::etmet) decay_mode_ = 0;
      if (channel_ == channel::mt) decay_mode_ = 0;
      if (channel_ == channel::mtmet) decay_mode_ = 0;
      if (channel_ == channel::em) decay_mode_ = 1;
    } else {
     //decay mode 0 mt 1 em 2 et 3 tt for now
      if (channel_ == channel::et) decay_mode_ = 2;
      if (channel_ == channel::mt) decay_mode_ = 0;
      if (channel_ == channel::tt) decay_mode_ = 3;
      if (channel_ == channel::em) decay_mode_ = 1;
    }

    std::cout << boost::format(param_fmt()) % "decay_mode"              % decay_mode_;
    std::cout << boost::format(param_fmt()) % "run_mode"                % run_mode_;
    std::cout << boost::format(param_fmt()) % "fail_mode"               % fail_mode_;
    std::cout << boost::format(param_fmt()) % "require_inputs_match"    % require_inputs_match_;
    std::cout << boost::format(param_fmt()) % "dilepton_label"          % dilepton_label_;
    std::cout << boost::format(param_fmt()) % "met_label"               % met_label_;

    // This code first strips the string ".root" from give filename
    // (if found) and appends "_SVIFIT".  This is then taken as the
    // name of the output folder.  If the user has specified a full
    // path (fullpath_) this folder will be created there, otherwise
    // it will be created in the current directory
    std::string sub(".root");
    std::string::size_type foundpos = outname_.find(sub);
    if ( foundpos != std::string::npos )
      outname_.erase(outname_.begin() + foundpos, outname_.begin() + foundpos + sub.length());
    outputadd_ = outname_; 
    outname_ += "_SVFIT";
    boost::filesystem::path folder_p(outname_);
    std::cout << boost::format(param_fmt()) % "folder"         % outname_;
    if (fullpath_ == "") {
      fullpath_ = boost::filesystem::current_path().string();
    }
    std::cout << boost::format(param_fmt()) % "directory"      % fullpath_;

    if(!from_grid_){
      total_path_ = operator/(fullpath_, folder_p);
    } else {
      if(read_all_ && run_mode_==2){
        std::string chan;
        if (channel_ == channel::et) chan = "_et_";
        if (channel_ == channel::mt) chan = "_mt_";
        if (channel_ == channel::em) chan = "_em_";
        if (channel_ == channel::tt) chan = "_tt_";
        std::string::size_type channelpos = outputadd_.find(chan);
        if(channelpos != std::string::npos){
          outputadd_.erase(outputadd_.begin() + channelpos +chan.length(),outputadd_.end());
        } 
      }
      boost::filesystem::path nofolder("");
      total_path_ = operator/(fullpath_,nofolder);
    }
    boost::filesystem::create_directories(total_path_);
    if (run_mode_ == 2) {
      boost::filesystem::directory_iterator it(total_path_);
      for (; it != boost::filesystem::directory_iterator(); ++it) {
        std::string path = it->path().string();
        if ((!from_grid_ && path.find("output.root") != path.npos)||(path.find(outputadd_.c_str()) != path.npos && path.find("output.root") != path.npos)) {
          std::cout << "Reading TFile: " << path << std::endl;
          TFile *ofile = new TFile(path.c_str());
          if (!ofile) {
            std::cout << "Warning, unable to open file " << path << std::endl;
            continue;
          }
          TTree *otree = dynamic_cast<TTree *>(ofile->Get("svfit"));
          if (!otree) {
            std::cout << "Warning, unable to get tree in file " << path << std::endl;
            continue;
          }
          unsigned    event    = 0;
          unsigned    lumi     = 0;
          unsigned    run      = 0;
          ULong64_t    objects_hash  = 0;
          double      svfit_mass    = 0;
          double      svfit_transverse_mass = 0;
          Candidate * svfit_vector  = nullptr;
          otree->SetBranchAddress("event"  , &event);
          otree->SetBranchAddress("lumi"  , &lumi);
          otree->SetBranchAddress("run"  , &run);
          otree->SetBranchAddress("objects_hash", &objects_hash);
          otree->SetBranchAddress("svfit_mass"  , &svfit_mass);
          if(read_svfit_mt_){
            otree->SetBranchAddress("svfit_transverse_mass",&svfit_transverse_mass);
          }
          otree->SetBranchAddress("svfit_vector"  , &svfit_vector);
          for (unsigned evt = 0; evt < otree->GetEntries(); ++evt) {
            otree->GetEntry(evt);
            if(!MC_) mass_map[tri_unsigned(run, lumi, event)] = std::make_pair(objects_hash, svfit_mass);
            else{
              if(read_svfit_mt_){
                p4_map[tri_unsigned(run, lumi, event)] = std::make_tuple(objects_hash, *svfit_vector, svfit_transverse_mass);
              } else p4_map[tri_unsigned(run, lumi, event)] = std::make_tuple(objects_hash, *svfit_vector, 0);
            }
          }
          ofile->Close();
          delete ofile;
        } 
      }
    }

  return 0;
}

int SVFitTest::Execute(TreeEvent *event) {

  if (run_mode_ == 0) return 0;

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
  Candidate c1 = *(dilepton.at(0)->GetCandidate("lepton1"));
  Candidate c2 = *(dilepton.at(0)->GetCandidate("lepton2"));
  Candidate const* lep1 = dilepton.at(0)->GetCandidate("lepton1");
  Candidate const* lep2 = dilepton.at(0)->GetCandidate("lepton2");
  Met met = *(event->GetPtr<Met>(met_label_));
  //std::size_t event_hash = RunLumiEvtHash(eventInfo->run(), eventInfo->lumi_block(), eventInfo->event());
  std::size_t objects_hash = ObjectsHash(&c1, &c2, &met);

 bool dilepton_veto_ = false;
 bool extraelec_veto_ = false;
 bool extramuon_veto_ = false;
 bool antiele_1_ = false;
 bool antimu_1_ = false;
 bool antiele_2_ = false;
 bool antimu_2_ = false;
 double iso_1_ = 0;
 double iso_2_ = 0; 
 bool iso_discr_1_ = 0;
 bool iso_discr_2_ = 0;
 //bool lbyMediumCombinedIsolation_1=0; 
 //bool lbyMediumCombinedIsolation_2=0; 
 bool lbyTightCombinedIsolation_1=0; 
 bool lbyTightCombinedIsolation_2=0; 
 bool pass_presel = false;

    if(channel_ == channel::et && do_preselection_) { 
        if(event->Exists("dielec_veto"))  dilepton_veto_ = event->Get<bool>("dielec_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        Electron const* elec = dynamic_cast<Electron const*>(lep1);
        Tau const* tau = dynamic_cast<Tau const*>(lep2);
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        //lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits");
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        if(strategy_==strategy::fall15||tau_optimisation_){
          iso_discr_2_ = tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
        }
        if(!tau_optimisation_&&strategy_==strategy::spring15){
          iso_discr_2_ = iso_2_ < 10;
        }
        /*lagainstElectronLooseMVA5_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;*/
        bool lagainstElectronTightMVA5_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        bool lagainstElectronTightMVA6_2 = tau->HasTauID("againstElectronTightMVA6") ? tau->GetTauID("againstElectronTightMVA6") : 0.;
/*        lagainstElectronVTightMVA5_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;
        lagainstElectronVLooseMVA5_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;*/
        bool lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
/*        lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwLTraw") ? tau->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;*/
        antiele_2_ = strategy_==strategy::spring15 ? lagainstElectronTightMVA5_2 : lagainstElectronTightMVA6_2;
        antimu_2_ = lagainstMuonLoose3_2;
        if(antiele_2_>0 && antimu_2_>0 && iso_discr_2_>0 && iso_1_<0.5) pass_presel = true;
    }
    if(channel_ == channel::mt && do_preselection_) { 
        if(event->Exists("dimuon_veto")) dilepton_veto_ = event->Get<bool>("dimuon_veto");
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        Muon const* muon  = dynamic_cast<Muon const*>(lep1);
        Tau const* tau = dynamic_cast<Tau const*>(lep2);
        iso_1_ = PF03IsolationVal(muon, 0.5, 0);
        //lbyMediumCombinedIsolation_2 = tau->HasTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits");
        iso_2_ = tau->GetTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits");
        if(tau_optimisation_||strategy_==strategy::fall15){
          iso_discr_2_ = tau->GetTauID("byTightIsolationMVArun2v1DBoldDMwLT");
        }
        if(!tau_optimisation_&&strategy_==strategy::spring15){
          iso_discr_2_ = iso_2_ < 10;
        }
/*        lagainstElectronLooseMVA5_2 = tau->HasTauID("againstElectronLooseMVA5") ? tau->GetTauID("againstElectronLooseMVA5") : 0.;
        lagainstElectronMediumMVA5_2 = tau->HasTauID("againstElectronMediumMVA5") ? tau->GetTauID("againstElectronMediumMVA5") : 0.;
        lagainstElectronTightMVA5_2 = tau->HasTauID("againstElectronTightMVA5") ? tau->GetTauID("againstElectronTightMVA5") : 0.;
        lagainstElectronVTightMVA5_2 = tau->HasTauID("againstElectronVTightMVA5") ? tau->GetTauID("againstElectronVTightMVA5") : 0.;*/
        bool lagainstElectronVLooseMVA5_2 = tau->HasTauID("againstElectronVLooseMVA5") ? tau->GetTauID("againstElectronVLooseMVA5") :0. ;
        bool lagainstElectronVLooseMVA6_2 = tau->HasTauID("againstElectronVLooseMVA6") ? tau->GetTauID("againstElectronVLooseMVA6") :0. ;
        //lagainstMuonLoose3_2 = tau->HasTauID("againstMuonLoose3") ? tau->GetTauID("againstMuonLoose3") : 0.;
        bool lagainstMuonTight3_2 = tau->HasTauID("againstMuonTight3") ? tau->GetTauID("againstMuonTight3") : 0.;
        /*lchargedIsoPtSum_2 = tau->HasTauID("chargedIsoPtSum") ? tau->GetTauID("chargedIsoPtSum") : 0.;
        lneutralIsoPtSum_2 = tau->HasTauID("neutralIsoPtSum") ? tau->GetTauID("neutralIsoPtSum") : 0.;
        lpuCorrPtSum_2 = tau->HasTauID("puCorrPtSum") ? tau->GetTauID("puCorrPtSum") : 0.;
        ldecayModeFindingOldDMs_2 = tau->HasTauID("decayModeFinding") ? tau->GetTauID("decayModeFinding") : 0;
        lbyIsolationMVA3newDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwoLTraw") ? tau->GetTauID("byIsolationMVA3newDMwoLTraw") : 0.;
        lbyIsolationMVA3oldDMwoLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwoLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwoLTraw") : 0.;
        lbyIsolationMVA3newDMwLTraw_2 = tau->HasTauID("byIsolationMVA3newDMwLTraw") ? tau->GetTauID("byIsolationMVA3newDMwLTraw") : 0.;
        lbyIsolationMVA3oldDMwLTraw_2 = tau->HasTauID("byIsolationMVA3oldDMwLTraw") ? tau->GetTauID("byIsolationMVA3oldDMwLTraw") : 0.;*/
        antiele_2_ = (strategy_ == strategy::spring15) ? lagainstElectronVLooseMVA5_2 : lagainstElectronVLooseMVA6_2;
        antimu_2_ = lagainstMuonTight3_2;
        if(antiele_2_>0 && antimu_2_>0 && iso_discr_2_>0 && iso_1_<0.5) pass_presel = true;
    }
    if(channel_ == channel::em && do_preselection_) { 
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        Electron  const* elec  = dynamic_cast<Electron const*>(lep1);
        Muon const* muon = dynamic_cast<Muon const*>(lep2);
        iso_1_ = PF03IsolationVal(elec, 0.5, 0);
        iso_2_ = PF03IsolationVal(muon, 0.5, 0);
        if(iso_2_<0.2 && iso_1_<0.2) pass_presel = true;
    }
    if(channel_ == channel::tt && do_preselection_) {
        if(event->Exists("extra_elec_veto")) extraelec_veto_ = event->Get<bool>("extra_elec_veto");
        if(event->Exists("extra_muon_veto")) extramuon_veto_ = event->Get<bool>("extra_muon_veto");
        Tau  const* tau1  = dynamic_cast<Tau const*>(lep1);
        Tau const* tau2 = dynamic_cast<Tau const*>(lep2);
        if(tau_optimisation_||strategy_==strategy::fall15){
          iso_discr_1_ = tau1->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT");
          iso_discr_2_ = tau2->GetTauID("byVTightIsolationMVArun2v1DBoldDMwLT");
        }
        if(!tau_optimisation_&&strategy_==strategy::spring15){
          lbyTightCombinedIsolation_1 = tau1->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits");
          lbyTightCombinedIsolation_2 = tau2->GetTauID("byTightCombinedIsolationDeltaBetaCorr3Hits");
          iso_discr_2_ = lbyTightCombinedIsolation_2;
          iso_discr_1_ = lbyTightCombinedIsolation_1;
        }
        //lagainstElectronLooseMVA5_1 = tau1->HasTauID("againstElectronLooseMVA5") ? tau1->GetTauID("againstElectronLooseMVA5") : 0.;
        //lagainstElectronMediumMVA5_1 = tau1->HasTauID("againstElectronMediumMVA5") ? tau1->GetTauID("againstElectronMediumMVA5") : 0.;
        //bool lagainstElectronTightMVA5_1 = tau1->HasTauID("againstElectronTightMVA5") ? tau1->GetTauID("againstElectronTightMVA5") : 0.;
        //lagainstElectronVTightMVA5_1= tau1->HasTauID("againstElectronVTightMVA5") ? tau1->GetTauID("againstElectronVTightMVA5") : 0.;
        bool lagainstElectronVLooseMVA5_1 = tau1->HasTauID("againstElectronVLooseMVA5") ? tau1->GetTauID("againstElectronVLooseMVA5") :0. ;
        bool lagainstElectronVLooseMVA6_1 = tau1->HasTauID("againstElectronVLooseMVA6") ? tau1->GetTauID("againstElectronVLooseMVA6") :0. ;
        bool lagainstMuonLoose3_1 = tau1->HasTauID("againstMuonLoose3") ? tau1->GetTauID("againstMuonLoose3") : 0.;
        //lagainstMuonTight3_1 = tau1->HasTauID("againstMuonTight3") ? tau1->GetTauID("againstMuonTight3") : 0.;
        //lagainstElectronLooseMVA5_2 = tau2->HasTauID("againstElectronLooseMVA5") ? tau2->GetTauID("againstElectronLooseMVA5") : 0.;
        //lagainstElectronMediumMVA5_2 = tau2->HasTauID("againstElectronMediumMVA5") ? tau2->GetTauID("againstElectronMediumMVA5") : 0.;*/
        //bool lagainstElectronTightMVA5_2 = tau2->HasTauID("againstElectronTightMVA5") ? tau2->GetTauID("againstElectronTightMVA5") : 0.;
        //lagainstElectronVTightMVA5_2 = tau2->HasTauID("againstElectronVTightMVA5") ? tau2->GetTauID("againstElectronVTightMVA5") : 0.;
        bool lagainstElectronVLooseMVA5_2 = tau2->HasTauID("againstElectronVLooseMVA5") ? tau2->GetTauID("againstElectronVLooseMVA5") :0. ;
        bool lagainstElectronVLooseMVA6_2 = tau2->HasTauID("againstElectronVLooseMVA6") ? tau2->GetTauID("againstElectronVLooseMVA6") :0. ;
        bool lagainstMuonLoose3_2 = tau2->HasTauID("againstMuonLoose3") ? tau2->GetTauID("againstMuonLoose3") : 0.;
        //lagainstMuonTight3_2 = tau2->HasTauID("againstMuonTight3") ? tau2->GetTauID("againstMuonTight3") : 0.;
        antiele_1_ =  (strategy_ == strategy::spring15) ? lagainstElectronVLooseMVA5_1 : lagainstElectronVLooseMVA6_1;
        antimu_1_ = lagainstMuonLoose3_1;
        antiele_2_ = (strategy_ == strategy::spring15) ? lagainstElectronVLooseMVA5_2 : lagainstElectronVLooseMVA6_2;
        antimu_2_ = lagainstMuonLoose3_2;
        if(iso_discr_1_>0 && iso_discr_2_>0 && antiele_1_>0 && antimu_1_>0 && antiele_2_>0 && antimu_2_>0) pass_presel = true;
    }


bool lepton_veto_ = dilepton_veto_ || extraelec_veto_ || extramuon_veto_;


if(!do_preselection_ || (pass_presel&&!lepton_veto_)){
  if (run_mode_ == 1) {
    if (event_counter_%split_ == 0) {
      if (out_file_) {
        out_file_->Write();
        delete out_tree_;
        out_file_->Close();
        delete out_file_;
        out_file_ = NULL;
      }
      out_file_ = new TFile((total_path_.string()+"/svfit_"+outputadd_+"_"+boost::lexical_cast<std::string>(file_counter_)+"_input.root").c_str(),"RECREATE");
      out_tree_ = new TTree("svfit","svfit");
      out_tree_->Branch("event", &out_event_, "event/i");
      out_tree_->Branch("lumi", &out_lumi_, "lumi/i");
      out_tree_->Branch("run", &out_run_, "run/i");
      out_tree_->Branch("objects_hash", &out_objects_hash_, "objects_hash/l");
      out_tree_->Branch("decay_mode", &decay_mode_);
      out_tree_->Branch("lepton1", &out_cand1_);
      out_tree_->Branch("dm1", &dm1_,"dm1/I");
      out_tree_->Branch("lepton2", &out_cand2_);
      out_tree_->Branch("dm2", &dm2_,"dm2/I");
      out_tree_->Branch("met", &out_met_);
      ++file_counter_;
    }
    //out_event_hash_ = event_hash;
    if(channel_==channel::tt){
     Tau * tau1 = dynamic_cast<Tau *>(dilepton.at(0)->GetCandidate("lepton1"));
     dm1_ = tau1->decay_mode();
     }
    if(channel_==channel::tt || channel_==channel::et||channel_==channel::mt){
     Tau * tau2 = dynamic_cast<Tau *>(dilepton.at(0)->GetCandidate("lepton2"));
     dm2_ = tau2->decay_mode();
    }

    out_event_ = eventInfo->event();
    out_lumi_ = eventInfo->lumi_block();
    out_run_ = eventInfo->run();
    out_objects_hash_ = objects_hash;
    out_cand1_ = &c1;
    out_cand2_ = &c2;
    out_met_ = &met;
    out_tree_->Fill();
    ++event_counter_;
  }



  if (run_mode_ == 2) {
    bool fail_state = false;
    //Different actions for Markov-Chain or Vegas integration
     if(!MC_){
        auto it = mass_map.find(tri_unsigned(eventInfo->run(),eventInfo->lumi_block(), eventInfo->event()));
        if (it != mass_map.end()) {
          ;
          if (require_inputs_match_ && it->second.first != objects_hash) {
            fail_state = true;
          } else {
            if (it->second.second < 1.) {
              std::cout << "Warning, SVFit mass is invalid: " << it->second.second << std::endl;
            } else {
              event->Add("svfitMass", it->second.second);
            }
          }
        } else {
          fail_state = true;
        }
     } else {  
        auto it = p4_map.find(tri_unsigned(eventInfo->run(),eventInfo->lumi_block(), eventInfo->event()));
        if (it != p4_map.end()) {
          ;
          if (require_inputs_match_ && std::get<0>(it->second) != objects_hash) {
            fail_state = true;
          } else {
            if ((std::get<1>(it->second)).M() < 1.) {
              std::cout << "Warning, SVFit mass is invalid: " << (std::get<1>(it->second)).M() << std::endl;
            } else {
              event->Add("svfitMass", (std::get<1>(it->second)).M());
              event->Add("svfitHiggs", std::get<1>(it->second));
              event->Add("svfitMT", std::get<2>(it->second));
            }
          }
        } else {
          fail_state = true;
        }
     }
    if (fail_state) {
      if (fail_mode_ == 0) {
        std::cout << "Warning, SVFitTest mass not found!" << std::endl;
        event->Add("svfitMass", double(-100.0));
      } else if (fail_mode_ == 1) {
        std::cout << "Error, SVFitTest mass not found!" << std::endl;
        throw;
      } else if (fail_mode_ == 3){
        if (event_counter_%split_ == 0) {
          if (out_file_) {
          out_file_->Write();
          delete out_tree_;
          out_file_->Close();
          delete out_file_;
          out_file_ = NULL;
        }
        out_file_ = new TFile((total_path_.string()+"/svfit_"+outputadd_+"_"+boost::lexical_cast<std::string>(file_counter_)+"_missing_input.root").c_str(),"RECREATE");
        out_tree_ = new TTree("svfit","svfit");
        out_tree_->Branch("event", &out_event_, "event/i");
        out_tree_->Branch("lumi", &out_lumi_, "lumi/i");
        out_tree_->Branch("run", &out_run_, "run/i");
        out_tree_->Branch("objects_hash", &out_objects_hash_, "objects_hash/l");
        out_tree_->Branch("decay_mode", &decay_mode_);
        out_tree_->Branch("lepton1", &out_cand1_);
        out_tree_->Branch("dm1", &dm1_,"dm1/I");
        out_tree_->Branch("lepton2", &out_cand2_);
        out_tree_->Branch("dm2", &dm2_,"dm2/I");
        out_tree_->Branch("met", &out_met_);
        ++file_counter_;
      }
      //out_event_hash_ = event_hash;
      if(channel_==channel::tt){
       Tau * tau1 = dynamic_cast<Tau *>(dilepton.at(0)->GetCandidate("lepton1"));
       dm1_ = tau1->decay_mode();
       }
      if(channel_==channel::tt || channel_==channel::et||channel_==channel::mt){
       Tau * tau2 = dynamic_cast<Tau *>(dilepton.at(0)->GetCandidate("lepton2"));
       dm2_ = tau2->decay_mode();
      }

      out_event_ = eventInfo->event();
      out_lumi_ = eventInfo->lumi_block();
      out_run_ = eventInfo->run();
      out_objects_hash_ = objects_hash;
      out_cand1_ = &c1;
      out_cand2_ = &c2;
      out_met_ = &met;
      out_tree_->Fill();
      ++event_counter_;
      } else {
        if(legacy_svfit_){
          std::cout << "Calculating mass on-the-fly" << std::endl;
          if (decay_mode_ == 0) {
            event->Add("svfitMass", SVFitService::SVFitMassLepHad(&c1, &c2, &met, MC_));
          } else {
            event->Add("svfitMass", SVFitService::SVFitMassLepLep(&c1, &c2, &met, MC_));
          }
        } else {
          std::cout << "On-the-fly mass calculation not supported!" << std::endl;
          throw;
         /* if (decay_mode_ == 0) {
            event->Add("svfitMass", SVFitService::SVFitMassMuHad(&c1, &c2, dm2_, &met, MC_));
          } else if (decay_mode_ == 1){
            event->Add("svfitMass", SVFitService::SVFitMassEleMu(&c1, &c2, &met, MC_));
          } else if (decay_mode_ == 2){
            event->Add("svfitMass", SVFitService::SVFitMassEleHad(&c1, &c2, dm2_, &met, MC_));
          } else if (decay_mode_ == 3){
            event->Add("svfitMass", SVFitService::SVFitMassHadHad(&c1, dm1_, &c2, dm2_, &met, MC_));
          }*/
        }
      }
    }
  }
}
  return 0;
  }
  int SVFitTest::PostAnalysis() {
    if (out_file_) {
      out_file_->Write();
      delete out_tree_;
      out_file_->Close();
      delete out_file_;
      out_file_ = NULL;
    }
    return 0;
  }

  void SVFitTest::PrintInfo() {
    ;
  }

}
