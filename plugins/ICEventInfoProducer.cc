#include "UserCode/ICHiggsTauTau/plugins/ICEventInfoProducer.hh"
#include <boost/functional/hash.hpp>
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"

#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"

#include "UserCode/ICHiggsTauTau/interface/city.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/METReco/interface/BeamHaloSummary.h"

#include "boost/format.hpp"




ICEventInfoProducer::ICEventInfoProducer(const edm::ParameterSet& iConfig) {
  jets_rho_name_ = iConfig.getParameter<std::string>("jetsRhoLabel");
  lepton_rho_name_ = iConfig.getParameter<std::string>("leptonRhoLabel");

  vertex_name_ = iConfig.getParameter<std::string>("vertexLabel");
  if (iConfig.exists("filters")) {
    edm::ParameterSet filter_params = iConfig.getParameter<edm::ParameterSet>("filters");
    std::vector<std::string> filter_names = filter_params.getParameterNamesForType<edm::InputTag>();
    for (std::vector<std::string>::const_iterator it = filter_names.begin(); it != filter_names.end(); ++it) {
     filters_.push_back(std::make_pair(*it, filter_params.getParameter<edm::InputTag>(*it)));
     if (filters_.back().second.label().at(0) == '!') {
      std::cout << "Info in <ICEventInfoProducer>: Inverting logic for filter: " << filters_.back().first << std::endl;
      std::string new_label = filters_.back().second.label();
      new_label.erase(0,1);
      filters_.back().second = edm::InputTag(new_label, filters_.back().second.instance(), filters_.back().second.process());
      invert_filter_logic_.insert(filters_.back().first);
     }
    } 
  }
  if (iConfig.exists("weights")) {
    edm::ParameterSet weight_params = iConfig.getParameter<edm::ParameterSet>("weights");
    std::vector<std::string> weight_names = weight_params.getParameterNamesForType<edm::InputTag>();
    for (std::vector<std::string>::const_iterator it = weight_names.begin(); it != weight_names.end(); ++it) {
     weights_.push_back(std::make_pair(*it, weight_params.getParameter<edm::InputTag>(*it)));
    } 
  }
  //embed_weight_ = iConfig.getParameter<std::string>("EmbedWeight");

  info_ = new ic::EventInfo();
}


ICEventInfoProducer::~ICEventInfoProducer() {
 // do anything here that needs to be done at desctruction time
 // (e.g. close files, deallocate resources etc.)
 delete info_;
}



// ------------ method called to produce the data  ------------
void ICEventInfoProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {
  *info_ = ic::EventInfo();
  info_->set_is_data(iEvent.isRealData());
  info_->set_run(iEvent.run());
  info_->set_event(iEvent.id().event());
  info_->set_lumi_block(iEvent.luminosityBlock());
  info_->set_bunch_crossing(iEvent.bunchCrossing());

  edm::Handle<double> jets_rho_handle;
  iEvent.getByLabel(edm::InputTag(jets_rho_name_,"rho"),jets_rho_handle);
  edm::Handle<double> lepton_rho_handle;
  iEvent.getByLabel(edm::InputTag(lepton_rho_name_,"rho"),lepton_rho_handle);

  edm::Handle<double> basic_weight_handle;
  edm::Handle<GenFilterInfo> info_weight_handle;
  // For 42X samples
  if (iEvent.getByLabel(edm::InputTag("generator","weight","EmbeddedRECO"),basic_weight_handle))
    info_->set_weight("embed_weight", *basic_weight_handle);
  // For 52X samples
  if (iEvent.getByLabel(edm::InputTag("generator","minVisPtFilter","EmbeddedRECO"),info_weight_handle))
    info_->set_weight("embed_weight", info_weight_handle->filterEfficiency());

  for (unsigned i = 0; i < weights_.size(); ++i) {
    edm::Handle<double> weight;
    iEvent.getByLabel(weights_[i].second, weight);
    double weights_result = (*weight);
    // std::cout << weights_[i].first << "\t\t" << weights_result << std::endl;
    info_->set_weight(weights_[i].first, weights_result);
  }
  edm::Handle<reco::VertexCollection> vertexCollection;
  iEvent.getByLabel(vertex_name_,vertexCollection);

  info_->set_good_vertices(vertexCollection->size());

  info_->set_jet_rho(*jets_rho_handle);
  info_->set_lepton_rho(*lepton_rho_handle);
    
  // MET filters
  for (unsigned i = 0; i < filters_.size(); ++i) {
    edm::Handle<bool> filter;
    iEvent.getByLabel(filters_[i].second, filter);
    bool filter_result = (*filter);
    if (invert_filter_logic_.find(filters_[i].first) != invert_filter_logic_.end()) filter_result = !filter_result;
    // std::cout << filters_[i].first << "\t\t" << filter_result << std::endl;
    info_->set_filter_result(filters_[i].first, filter_result);
    observed_filters_[filters_[i].first] = CityHash64(filters_[i].first);
  }
  edm::Handle<reco::BeamHaloSummary> beamHaloSummary;
  if (iEvent.getByLabel("BeamHaloSummary",beamHaloSummary)) {
    info_->set_filter_result("CSCTightHaloFilter", beamHaloSummary->CSCTightHaloId());
    observed_filters_["CSCTightHaloFilter"] = CityHash64("CSCTightHaloFilter");  
  }

}

// ------------ method called once each job just before starting event loop  ------------
void ICEventInfoProducer::beginJob() {
 ic::StaticTree::tree_->Branch("eventInfo",&info_);
}

// ------------ method called once each job just after ending the event loop  ------------
void ICEventInfoProducer::endJob() {
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::cout << "Info in <ICEventInfoProducerr>: EndJob Summary" << std::endl;
  std::cout << "-----------------------------------------------------------------" << std::endl;
  std::map<std::string, std::size_t>::const_iterator iter;
  std::cout << "Filter Hash Summary:" << std::endl;
  for (iter = observed_filters_.begin(); iter != observed_filters_.end(); ++iter) {
    std::cout << boost::format("%-30s %-30s\n") % iter->first % iter->second;
  }
}

// ------------ method called when starting to processes a run  ------------
void ICEventInfoProducer::beginRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a run  ------------
void ICEventInfoProducer::endRun(edm::Run&, edm::EventSetup const&) {
}

// ------------ method called when starting to processes a luminosity block  ------------
void ICEventInfoProducer::beginLuminosityBlock(edm::LuminosityBlock&, 
                                             edm::EventSetup const&) {
}

// ------------ method called when ending the processing of a luminosity block  ------------
void ICEventInfoProducer::endLuminosityBlock(edm::LuminosityBlock&, 
                                           edm::EventSetup const&) {
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ICEventInfoProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ICEventInfoProducer);
