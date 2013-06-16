#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/EmbeddingKineReweightProducer.h"
#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTConfig.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnRootTools.h"
#include "UserCode/ICHiggsTauTau/interface/GenParticle.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "TH2D.h"
#include "TFile.h"
namespace ic {

  EmbeddingKineReweightProducer::EmbeddingKineReweightProducer(std::string const& name) : ModuleBase(name) {
    genparticle_label_ = "genParticlesTaus";
    file_ = "";
  }

  EmbeddingKineReweightProducer::~EmbeddingKineReweightProducer() {
    ;
  }

  int EmbeddingKineReweightProducer::PreAnalysis() {
    std::cout << "-------------------------------------" << std::endl;
    std::cout << "EmbeddingKineReweightProducer" << std::endl;
    std::cout << "-------------------------------------" << std::endl;    
    std::cout << boost::format(param_fmt()) % "genparticle_label" % genparticle_label_;
    std::cout << boost::format(param_fmt()) % "file"              % file_;
    TFile *f = new TFile(file_.c_str());
    genTau2PtVsGenTau1Pt_ = GetFromTFile<TH2D>(f, "/", "embeddingKineReweight_muon2Pt_vs_muon1Pt");
    genTau2EtaVsGenTau1Eta_ = GetFromTFile<TH2D>(f, "/", "embeddingKineReweight_muon2Eta_vs_muon1Eta");
    genDiTauMassVsGenDiTauPt_ = GetFromTFile<TH2D>(f, "/", "embeddingKineReweight_diMuonMass_vs_diMuonPt");
    f->Close();
    delete f;
    return 0;
  }

  int EmbeddingKineReweightProducer::Execute(TreeEvent *event) {
    std::vector<GenParticle *> parts = event->GetPtrVec<GenParticle>(genparticle_label_);
    std::vector<GenParticle *> taus;
    for (unsigned i = 0; i < parts.size(); ++i) {
      if (abs(parts[i]->pdgid()) != 15 || parts[i]->status() != 2) continue;
      auto daughters = parts[i]->daughters();
      bool has_tau_daughter = false;
      for (unsigned j = 0; j < parts.size(); ++j) {
        if (std::find(daughters.begin(),daughters.end(),parts[j]->index()) == daughters.end()) continue;
        if (abs(parts[j]->pdgid()) == 15) has_tau_daughter = true;
      }
      if (has_tau_daughter) continue;
      taus.push_back(parts[i]);
    }
    if (taus.size() != 2) {
      std::cout << "Warning in EmbeddingKineReweightProducer:  have " << taus.size() << " taus!" << std::endl;
      return 1;
    }
    EventInfo *eventInfo = event->GetPtr<EventInfo>("eventInfo");

    double x,y;
    int bin_x, bin_y;

    x = taus[0]->pt();
    y = taus[1]->pt();
    bin_x = genTau2PtVsGenTau1Pt_.GetXaxis()->FindBin(x);
    bin_y = genTau2PtVsGenTau1Pt_.GetYaxis()->FindBin(y);
    if (bin_x <= 1) bin_x = 1;
    if (bin_y <= 1) bin_y = 1;
    if (bin_x >= genTau2PtVsGenTau1Pt_.GetXaxis()->GetNbins()) bin_x = genTau2PtVsGenTau1Pt_.GetXaxis()->GetNbins();
    if (bin_y >= genTau2PtVsGenTau1Pt_.GetYaxis()->GetNbins()) bin_y = genTau2PtVsGenTau1Pt_.GetYaxis()->GetNbins();
    double new_kin_weight1 = genTau2PtVsGenTau1Pt_.GetBinContent(bin_x, bin_y);
    if (new_kin_weight1 > 10.0) new_kin_weight1 = 10.0;
    eventInfo->set_weight("kin_weight1",new_kin_weight1);

    x = taus[0]->eta();
    y = taus[1]->eta();
    bin_x = genTau2EtaVsGenTau1Eta_.GetXaxis()->FindBin(x);
    bin_y = genTau2EtaVsGenTau1Eta_.GetYaxis()->FindBin(y);
    if (bin_x <= 1) bin_x = 1;
    if (bin_y <= 1) bin_y = 1;
    if (bin_x >= genTau2EtaVsGenTau1Eta_.GetXaxis()->GetNbins()) bin_x = genTau2EtaVsGenTau1Eta_.GetXaxis()->GetNbins();
    if (bin_y >= genTau2EtaVsGenTau1Eta_.GetYaxis()->GetNbins()) bin_y = genTau2EtaVsGenTau1Eta_.GetYaxis()->GetNbins();
    double new_kin_weight2 = genTau2EtaVsGenTau1Eta_.GetBinContent(bin_x, bin_y);
    if (new_kin_weight2 > 10.0) new_kin_weight2 = 10.0;
    eventInfo->set_weight("kin_weight2",new_kin_weight2);
    
    x = (taus[0]->vector()+taus[1]->vector()).pt();
    y = (taus[0]->vector()+taus[1]->vector()).M();
    bin_x = genDiTauMassVsGenDiTauPt_.GetXaxis()->FindBin(x);
    bin_y = genDiTauMassVsGenDiTauPt_.GetYaxis()->FindBin(y);
    if (bin_x <= 1) bin_x = 1;
    if (bin_y <= 1) bin_y = 1;
    if (bin_x >= genDiTauMassVsGenDiTauPt_.GetXaxis()->GetNbins()) bin_x = genDiTauMassVsGenDiTauPt_.GetXaxis()->GetNbins();
    if (bin_y >= genDiTauMassVsGenDiTauPt_.GetYaxis()->GetNbins()) bin_y = genDiTauMassVsGenDiTauPt_.GetYaxis()->GetNbins();
    double new_kin_weight3 = genDiTauMassVsGenDiTauPt_.GetBinContent(bin_x, bin_y);
    if (new_kin_weight3 > 10.0) new_kin_weight3 = 10.0;
    eventInfo->set_weight("kin_weight3",new_kin_weight3);

    return 0;
  }


  int EmbeddingKineReweightProducer::PostAnalysis() {
    return 0;
  }

  void EmbeddingKineReweightProducer::PrintInfo() {
    ;
  }

}
// EmbeddingKineReweightProducer::EmbeddingKineReweightProducer(const edm::ParameterSet& cfg)
// {
//   srcGenDiTaus_ = cfg.getParameter<edm::InputTag>("srcGenDiTaus"); 

//   edm::FileInPath inputFileName = cfg.getParameter<edm::FileInPath>("inputFileName");
//   if ( !inputFileName.isLocal()) 
//     throw cms::Exception("EmbeddingReweightProducer") 
//       << " Failed to find File = " << inputFileName << " !!\n";
//   std::auto_ptr<TFile> inputFile(new TFile(inputFileName.fullPath().data()));

//   edm::ParameterSet cfgLUTs = cfg.getParameter<edm::ParameterSet>("lutNames"); 
//   std::vector<std::string> variables = cfgLUTs.getParameterNamesForType<std::string>();
//   for ( std::vector<std::string>::const_iterator variable = variables.begin(); 
//         variable != variables.end(); ++variable ) {
//     std::string lutName = cfgLUTs.getParameter<std::string>(*variable);
//     lutEntryType* lutEntry = 
//       new lutEntryType(*inputFile, *variable, lutName);
//     lutEntries_.push_back(lutEntry);
//   }
  
//   minWeight_ = cfg.getParameter<double>("minWeight"); 
//   maxWeight_ = cfg.getParameter<double>("maxWeight");

//   verbosity_ = ( cfg.exists("verbosity") ) ?
//     cfg.getParameter<int>("verbosity") : 0;

//   for ( std::vector<lutEntryType*>::iterator lutEntry = lutEntries_.begin();
// 	lutEntry != lutEntries_.end(); ++lutEntry ) {
//     produces<double>((*lutEntry)->variableName_);
//   }
// }

// EmbeddingKineReweightProducer::~EmbeddingKineReweightProducer()
// {
//   for ( std::vector<lutEntryType*>::iterator it = lutEntries_.begin();
// 	it != lutEntries_.end(); ++it ) {
//     delete (*it);
//   }
// }

// void EmbeddingKineReweightProducer::produce(edm::Event& evt, const edm::EventSetup& es)
// {
//   if ( verbosity_ ) {
//     std::cout << "<EmbeddingKineReweightProducer::produce>:" << std::endl;
//   }

//   typedef edm::View<reco::Candidate> CandidateView;
//   edm::Handle<CandidateView> genDiTaus;
//   evt.getByLabel(srcGenDiTaus_, genDiTaus);
//   if ( genDiTaus->size() != 1 )
//     throw cms::Exception("EmbeddingKineReweightProducer") 
//       << "Failed to find unique genDiTau object !!\n";  
//   const reco::Candidate& genDiTau = genDiTaus->front();
//   if ( verbosity_ ) {
//     std::cout << "diTau: Pt = " << genDiTau.pt() << ", eta = " << genDiTau.eta() << ", phi = " << genDiTau.phi() << ", mass = " << genDiTau.mass() << std::endl;
//   }

//   for ( std::vector<lutEntryType*>::const_iterator lutEntry = lutEntries_.begin();
// 	lutEntry != lutEntries_.end(); ++lutEntry ) {
//     double weight = (**lutEntry)(genDiTau);
//     if ( weight < minWeight_ ) weight = minWeight_;
//     if ( weight > maxWeight_ ) weight = maxWeight_;
//     if ( verbosity_ ) {
//       std::cout << " " << (*lutEntry)->variableName_ << " = " << weight << std::endl;
//     }
//     std::auto_ptr<double> weightPtr(new double(weight));
//     evt.put(weightPtr, (*lutEntry)->variableName_);
//   }
// }

// #include "FWCore/Framework/interface/MakerMacros.h"

// DEFINE_FWK_MODULE(EmbeddingKineReweightProducer);


