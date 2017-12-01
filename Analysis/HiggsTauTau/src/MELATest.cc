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

  MELATest::MELATest(std::string const& name) : ModuleBase(name), channel_(channel::et), strategy_(strategy::spring15) {
  
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
    from_grid_ = false;
    read_all_ = false;
    
    met_label_="pfMET";
    dilepton_label_="ditau";
    jets_label_="ak4PFJetsCHS";
    
    use_svfit_vec_ = false;

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

          otree->SetBranchAddress("event"  , &event);
          otree->SetBranchAddress("lumi"  , &lumi);
          otree->SetBranchAddress("run"  , &run);

          for (unsigned evt = 0; evt < otree->GetEntries(); ++evt) {
            otree->GetEntry(evt);
            //get values from output files here
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
      out_file_ = new TFile((total_path_.string()+"/svfit_"+outputadd_+"_"+boost::lexical_cast<std::string>(file_counter_)+"_input.root").c_str(),"RECREATE");
      out_tree_ = new TTree("svfit","svfit");
      out_tree_->Branch("event", &out_event_, "event/i");
      out_tree_->Branch("lumi", &out_lumi_, "lumi/i");
      out_tree_->Branch("run", &out_run_, "run/i");
      out_tree_->Branch("Hpx", &Hpx_);
      out_tree_->Branch("Hpy", &Hpy_);
      out_tree_->Branch("Hpz", &Hpz_);
      out_tree_->Branch("HE", &HE_);
      out_tree_->Branch("jpx_1", &jpx_1_);
      out_tree_->Branch("jpy_1", &jpy_1_);
      out_tree_->Branch("jpz_1", &jpz_1_);
      out_tree_->Branch("jE_1" , &jE_1_ );
      out_tree_->Branch("jpx_2", &jpx_2_);
      out_tree_->Branch("jpy_2", &jpy_2_);
      out_tree_->Branch("jpz_2", &jpz_2_);
      out_tree_->Branch("jE_2" , &jE_2_ );
  
      ++file_counter_;
    }
  
  
    out_event_ = eventInfo->event();
    out_lumi_ = eventInfo->lumi_block();
    out_run_ = eventInfo->run();
    
    Candidate higgs;
    if (use_svfit_vec_ && event->Exists("svfitHiggs")) higgs = event->Get<Candidate>("svfitHiggs");
    else higgs.set_vector(lep1->vector()+lep2->vector()+met.vector());

    Hpx_= higgs.vector().Px();
    Hpy_= higgs.vector().Py();
    Hpz_= higgs.vector().Pz();
    HE_ = higgs.vector().E();
    
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));    
    unsigned n_jets = jets.size();
    
    if(n_jets>=1){
      jpx_1_= jets[0]->vector().Px();
      jpy_1_= jets[0]->vector().Py();
      jpz_1_= jets[0]->vector().Pz();
      jE_1_ = jets[0]->vector().E();    
    } else {
      jpx_1_= -9999;
      jpy_1_= -9999;
      jpz_1_= -9999;
      jE_1_ = -9999;  
    }
    if(n_jets>=2){
      jpx_2_= jets[1]->vector().Px();
      jpy_2_= jets[1]->vector().Py();
      jpz_2_= jets[1]->vector().Pz();
      jE_2_ = jets[1]->vector().E();    
    } else {
      jpx_2_= -9999;
      jpy_2_= -9999;
      jpz_2_= -9999;
      jE_2_ = -9999;  
    }
  
    ++event_counter_;
  
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
