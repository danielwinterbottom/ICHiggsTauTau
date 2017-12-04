#include "UserCode/ICHiggsTauTau/plugins/ICMadgraphWeightsProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "Math/Vector4D.h"
#include "Math/Vector4Dfwd.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"


ICMadgraphWeightsProducer::ICMadgraphWeightsProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      theta_(config.getParameter<std::string>("theta")),
      theta_sample_(config.getParameter<double>("theta_sample")),
      process_dir_(config.getParameter<std::string>("process_dir")),
      param_card_(config.getParameter<std::string>("param_card")),
      param_card_sample_(config.getParameter<std::string>("param_card_sample")){
  consumes<LHEEventProduct>(input_);
  PrintHeaderWithProduces(config, input_, branch_);

  info_ = new ic::EventInfo();
}

ICMadgraphWeightsProducer::~ICMadgraphWeightsProducer() {}

std::vector<std::pair<std::string,double>> ICMadgraphWeightsProducer::SplitString(std::string instring){
  std::vector<std::pair<std::string,double>> out;
  std::stringstream ss(instring);   
  std::string splitstring;  
  while(std::getline(ss, splitstring, ',')){
    double val = std::stod(splitstring);
    if(splitstring.find(".") != std::string::npos) splitstring.replace(splitstring.find("."),1,"p");
    std::string weight_name = "wt_cp_"+splitstring;    
    out.push_back(std::make_pair(weight_name,val)); 
  }
  return out;
}

void ICMadgraphWeightsProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  
  edm::Handle<LHEEventProduct> lhe_handle;
  event.getByLabel(input_, lhe_handle);
  std::vector<lhef::HEPEUP::FiveVector> lheParticles = lhe_handle->hepeup().PUP;
  std::vector< std::pair<int, int> > mothers = lhe_handle->hepeup().MOTHUP;
  std::vector<LHEParticle*> parts;
  count_outpartons_=0;
  for (unsigned i = 0; i < lheParticles.size(); ++i) {
    LHEParticle part;  
    part.p4 = ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float>>(lheParticles[i][0],lheParticles[i][1],lheParticles[i][2],lheParticles[i][3]);
    part.pdgId = lhe_handle->hepeup().IDUP[i];
    parts.push_back(new LHEParticle(part));
  }
  MadGraphTools* madGraphTools = new MadGraphTools((float)theta_sample_, process_dir_, param_card_sample_, 0.118, false);
  double w0_ = madGraphTools->GetMatrixElementSquared(parts);
  for(unsigned i=0; i<theta_vec_.size(); ++i){
    float theta_val_ = theta_vec_[i].second;
    std::string weight_name_ = theta_vec_[i].first;
    MadGraphTools* madGraphTools = new MadGraphTools((float)theta_val_, process_dir_, param_card_, 0.118, false);
    double weight_ = madGraphTools->GetMatrixElementSquared(parts);
    if(w0_!=0) info_->set_weight(weight_name_,weight_/w0_,false);
    else info_->set_weight(weight_name_,0.,false);
  }

}

void ICMadgraphWeightsProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &info_);
  theta_vec_ = SplitString(theta_);   
}

void ICMadgraphWeightsProducer::endJob() {}

DEFINE_FWK_MODULE(ICMadgraphWeightsProducer);
