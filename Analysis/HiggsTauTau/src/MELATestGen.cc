#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/MELATestGen.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/interface/CompositeCandidate.hh"
#include "UserCode/ICHiggsTauTau/interface/Met.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/HistoSet.h"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include "boost/format.hpp"
#include "boost/filesystem.hpp"

std::vector<ic::GenParticle> GetDecayProducts (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles, unsigned &outputID){ 
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
      GetDecayProducts(v,d, gen_particles, outputID);
    }
  }
  return v;
}

namespace ic {

  MELATestGen::MELATestGen(std::string const& name) : ModuleBase(name), channel_(channel::et)  {
  
    out_file_ = NULL;
    out_tree_ = NULL;

    out_event_ = 0;
    out_lumi_ = 0;
    out_run_ = 0;
    run_mode_ = 1;

    file_counter_ = 0;
    event_counter_ = 0;
    split_ = 100000;
    outname_ = "mela_test";
    outputadd_ = "";
    fullpath_ = "MELA/";
    read_all_ = true;
    
    met_label_="pfMET";
    dilepton_label_="ditau";
    jets_label_="ak4PFJetsCHS";
    
    use_svfit_vec_ = true;

  }

  MELATestGen::~MELATestGen() {
    ;
  }

  int MELATestGen::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "MELATestGen" << std::endl;
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

int MELATestGen::Execute(TreeEvent *event) {

  if (run_mode_ == 0) return 0;

  EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
  std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
  std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

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
    
    ic::GenParticle higgs;
    std::vector<ic::GenParticle> higgs_products;
    for(unsigned i=0; i<gen_particles.size(); ++i){
      ic::GenParticle part = *gen_particles[i];  
      unsigned genID = std::fabs(part.pdgid());
      if((part.statusFlags()[FromHardProcessBeforeFSR] || part.statusFlags()[IsLastCopy]) && genID == 25) {
          higgs = part;
      }
      
      bool status_flag_t = part.statusFlags().at(0);
      bool status_flag_tlc = part.statusFlags().at(13);
      
      if(!(genID == 15 && status_flag_t && status_flag_tlc)) continue;
      std::vector<ic::GenParticle> family;
      unsigned outputID = 15;
      GetDecayProducts(family, part, gen_particles, outputID);
      if(family.size()==1 && (outputID ==11 || outputID ==13)){
        higgs_products.push_back(family[0]);
      } else {
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
    
    Hpx_ = higgs.vector().Px();
    Hpy_ = higgs.vector().Py();
    Hpz_ = higgs.vector().Pz();
    HE_ = higgs.vector().E();

    ic::erase_if(gen_jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));    
    
    for(unsigned i=0; i<gen_jets.size(); ++i){
      ic::GenJet jet = *gen_jets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) gen_jets.erase (gen_jets.begin()+i);
    }
    
    n_jets_ = gen_jets.size(); 
    jpx_.clear();
    jpy_.clear();
    jpz_.clear();
    jE_.clear(); 
    for(unsigned i=0; i<n_jets_; ++i){
      jpx_.push_back(gen_jets[i]->vector().Px());
      jpy_.push_back(gen_jets[i]->vector().Py());
      jpz_.push_back(gen_jets[i]->vector().Pz());
      jE_.push_back(gen_jets[i]->vector().E());
    }         
    
    out_tree_->Fill();
    ++event_counter_;
  }

  if(run_mode_ == 2){
    auto it = mela_map.find(tri_unsigned(eventInfo->run(),eventInfo->lumi_block(), eventInfo->event()));
    if (it != mela_map.end()) {
      event->Add("D0", it->second.first);
      event->Add("DCP", it->second.second);
    } else { std::cout << "Warning, MELA output not found!" << std::endl; }
  }

  return 0;
  }
  int MELATestGen::PostAnalysis() {
    if (out_file_) {
      out_file_->Write();
      delete out_tree_;
      out_file_->Close();
      delete out_file_;
      out_file_ = NULL;
    }
    return 0;
  }

  void MELATestGen::PrintInfo() {
    ;
  }

}
