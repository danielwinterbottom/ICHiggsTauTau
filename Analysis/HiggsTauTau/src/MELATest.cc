#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/MELATest.h"
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

  MELATest::MELATest(std::string const& name) : ModuleBase(name), channel_(channel::et)  {
  
    out_file_ = NULL;
    out_tree_ = NULL;

    out_event_ = 0;
    out_lumi_ = 0;
    out_run_ = 0;
    run_mode_ = 1;

    file_counter_ = 0;
    event_counter_ = 0;
    split_ = 5000;
    outname_ = "mela_test";
    outputadd_ = "";
    fullpath_ = "MELA/";
    read_all_ = true;
    
    met_label_="pfMET";
    dilepton_label_="ditau";
    jets_label_="ak4PFJetsCHS";
    
    use_svfit_vec_ = true;

  }

  MELATest::~MELATest() {
    ;
  }

  int MELATest::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "MELATest" << std::endl;
    std::cout << "-------------------------------------" << std::endl;

    std::cout << boost::format(param_fmt()) % "run_mode"                % run_mode_;
    std::cout << boost::format(param_fmt()) % "dilepton_label"          % dilepton_label_;
    std::cout << boost::format(param_fmt()) % "met_label"               % met_label_;
    std::cout << boost::format(param_fmt()) % "jets_label"               % jets_label_;


    // This code first strips the string ".root" from give filename
    // (if found) and appends "_MELA".  This is then taken as the
    // name of the output folder.  If the user has specified a full
    // path (fullpath_) this folder will be created there, otherwise
    // it will be created in the current directory
    std::string sub(".root");
    std::string::size_type foundpos = outname_.find(sub);
    if ( foundpos != std::string::npos )
      outname_.erase(outname_.begin() + foundpos, outname_.begin() + foundpos + sub.length());
    outputadd_ = outname_; 
    outname_ += "_MELA";
    boost::filesystem::path folder_p(outname_);
    std::cout << boost::format(param_fmt()) % "folder"         % outname_;
    if (fullpath_ == "") {
      fullpath_ = boost::filesystem::current_path().string();
    }
    std::cout << boost::format(param_fmt()) % "directory"      % fullpath_;

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
    boost::filesystem::create_directories(total_path_);
    if (run_mode_ == 2) {
      boost::filesystem::directory_iterator it(total_path_);
      for (; it != boost::filesystem::directory_iterator(); ++it) {
        std::string path = it->path().string();
        if (path.find(outputadd_.c_str()) != path.npos && path.find("output.root") != path.npos) {  
          std::cout << "Reading TFile: " << path << std::endl;
          TFile *ofile = new TFile(path.c_str());
          if (!ofile) {
            std::cout << "Warning, unable to open file " << path << std::endl;
            continue;
          }
          TTree *otree = dynamic_cast<TTree *>(ofile->Get("mela"));
          if (!otree) {
            std::cout << "Warning, unable to get tree in file " << path << std::endl;
            continue;
          }
          unsigned    event    = 0;
          unsigned    lumi     = 0;
          unsigned    run      = 0;
          float D0;
          float DCP;

          otree->SetBranchAddress("event"  , &event);
          otree->SetBranchAddress("lumi"  , &lumi);
          otree->SetBranchAddress("run"  , &run);
          otree->SetBranchAddress("D0"  , &D0);
          otree->SetBranchAddress("DCP"  , &DCP);  

          for (unsigned evt = 0; evt < otree->GetEntries(); ++evt) {
            otree->GetEntry(evt);
            mela_map[tri_unsigned(run, lumi, event)] = std::make_pair(D0, DCP); 
          }
          ofile->Close();
          delete ofile;
        } 
      }
    }
  return 0;
}

int MELATest::Execute(TreeEvent *event) {

  if (run_mode_ == 0) return 0;

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>(dilepton_label_);
  Candidate const* lep1 = dilepton.at(0)->GetCandidate("lepton1");
  Candidate const* lep2 = dilepton.at(0)->GetCandidate("lepton2");
  Met met = *(event->GetPtr<Met>(met_label_));

  if (run_mode_ == 1) {
    if (event_counter_%split_ == 0) {
      if (out_file_) {
        out_file_->Write();
        delete out_tree_;
        out_file_->Close();
        delete out_file_;
        out_file_ = NULL;
      }
      out_file_ = new TFile((total_path_.string()+"/mela_"+outputadd_+"_"+boost::lexical_cast<std::string>(file_counter_)+"_input.root").c_str(),"RECREATE");
      out_tree_ = new TTree("mela","mela");
      out_tree_->Branch("event", &out_event_, "event/i");
      out_tree_->Branch("lumi", &out_lumi_, "lumi/i");
      out_tree_->Branch("run", &out_run_, "run/i");
      out_tree_->Branch("Hpx", &Hpx_);
      out_tree_->Branch("Hpy", &Hpy_);
      out_tree_->Branch("Hpz", &Hpz_);
      out_tree_->Branch("HE", &HE_);
      out_tree_->Branch("jpx", &jpx_);
      out_tree_->Branch("jpy", &jpy_);
      out_tree_->Branch("jpz", &jpz_);
      out_tree_->Branch("jE", &jE_);
      out_tree_->Branch("n_jets", &n_jets_);
      ++file_counter_;
    }
  
  
    out_event_ = eventInfo->event();
    out_lumi_ = eventInfo->lumi_block();
    out_run_ = eventInfo->run();

    ic::Candidate higgs;
    if (use_svfit_vec_ && event->Exists("svfitHiggs")) higgs = event->Get<Candidate>("svfitHiggs");
    else higgs.set_vector(lep1->vector()+lep2->vector()+met.vector());
    
    Hpx_ = higgs.vector().Px();
    Hpy_ = higgs.vector().Py();
    Hpz_ = higgs.vector().Pz();
    HE_ = higgs.vector().E();

    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));    
    n_jets_ = jets.size(); 
    jpx_.clear();
    jpy_.clear();
    jpz_.clear();
    jE_.clear(); 
    for(unsigned i=0; i<n_jets_; ++i){
      jpx_.push_back(jets[i]->vector().Px());
      jpy_.push_back(jets[i]->vector().Py());
      jpz_.push_back(jets[i]->vector().Pz());
      jE_.push_back(jets[i]->vector().E());
    }         
    
    out_tree_->Fill();
    ++event_counter_;
  }

  if(run_mode_ == 2){
    auto it = mela_map.find(tri_unsigned(eventInfo->run(),eventInfo->lumi_block(), eventInfo->event()));
    if (it != mela_map.end()) {
      event->Add("D0", it->second.first);
      event->Add("DCP", it->second.second);
    } else { std::cout << "Error, MELA output not found!" << std::endl; throw; }
  }

  return 0;
  }
  int MELATest::PostAnalysis() {
    if (out_file_) {
      out_file_->Write();
      delete out_tree_;
      out_file_->Close();
      delete out_file_;
      out_file_ = NULL;
    }
    return 0;
  }

  void MELATest::PrintInfo() {
    ;
  }

}
