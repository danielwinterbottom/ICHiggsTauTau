#include "UserCode/ICHiggsTauTau/plugins/ICEventInfoProducer.hh"
#include <memory>
#include <string>
#include <vector>
#include "boost/format.hpp"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "SimDataFormats/GeneratorProducts/interface/GenFilterInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "FWCore/Utilities/interface/Exception.h"

ICEventInfoProducer::ICEventInfoProducer(const edm::ParameterSet& config)
    : branch_(config.getParameter<std::string>("branch")),
      is_nlo_(config.getParameter<bool>("isNlo")),
      lhe_collection_(config.getParameter<edm::InputTag>("lheProducer")),
      do_jets_rho_(config.getParameter<bool>("includeJetRho")),
      input_jets_rho_(config.getParameter<edm::InputTag>("inputJetRho")),
      do_leptons_rho_(config.getParameter<bool>("includeLeptonRho")),
      input_leptons_rho_(config.getParameter<edm::InputTag>("inputLeptonRho")),
      do_vertex_count_(config.getParameter<bool>("includeVertexCount")),
      input_vertices_(config.getParameter<edm::InputTag>("inputVertices")),
      do_csc_filter_(config.getParameter<bool>("includeCSCFilter")),
      input_csc_filter_(config.getParameter<edm::InputTag>("inputCSCFilter")),
      do_lhe_weights_(config.getParameter<bool>("includeLHEWeights")),
      do_filtersfromtrig_(config.getParameter<bool>("includeFiltersFromTrig")),
      filtersfromtrig_(config.getParameter<std::vector<std::string> >("filtersfromtrig")),
      filtersfromtrig_input_(config.getParameter<edm::InputTag>("inputfiltersfromtrig")){
  edm::ParameterSet filter_params =
      config.getParameter<edm::ParameterSet>("filters");
  std::vector<std::string> filter_names =
      filter_params.getParameterNamesForType<edm::InputTag>();
  for (unsigned i = 0; i < filter_names.size(); ++i) {
    filters_.push_back(std::make_pair(
        filter_names[i],
        filter_params.getParameter<edm::InputTag>(filter_names[i])));
    if (filters_.back().second.label().at(0) == '!') {
      std::cout << "Info in <ICEventInfoProducer>: Inverting logic for filter: "
                << filters_.back().first << std::endl;
      std::string new_label = filters_.back().second.label();
      new_label.erase(0, 1);
      filters_.back().second =
          edm::InputTag(new_label, filters_.back().second.instance(),
                        filters_.back().second.process());
      invert_filter_logic_.insert(filters_.back().first);
    }
  }

  edm::ParameterSet wt_pset = config.getParameter<edm::ParameterSet>("weights");
  std::vector<std::string> wt =
      wt_pset.getParameterNamesForType<edm::InputTag>();
  for (unsigned i = 0; i < wt.size(); ++i) {
    weights_.push_back(
        std::make_pair(wt[i], wt_pset.getParameter<edm::InputTag>(wt[i])));
  }

  edm::ParameterSet gwt_pset =
      config.getParameter<edm::ParameterSet>("genFilterWeights");
  std::vector<std::string> gwt =
      gwt_pset.getParameterNamesForType<edm::InputTag>();
  for (unsigned i = 0; i < gwt.size(); ++i) {
    gen_weights_.push_back(
        std::make_pair(gwt[i], gwt_pset.getParameter<edm::InputTag>(gwt[i])));
  }

  info_ = new ic::EventInfo();

  PrintHeaderWithBranch(config, branch_);
  PrintOptional(1, is_nlo_, "isNlo");
  PrintOptional(1, do_lhe_weights_, "includeLHEWeights");
  PrintOptional(1, do_jets_rho_, "includeJetRho");
  PrintOptional(1, do_leptons_rho_, "includeLeptonRho");
  PrintOptional(1, do_vertex_count_, "includeVertexCount");
  PrintOptional(1, do_csc_filter_, "includeCSCFilter");
}

ICEventInfoProducer::~ICEventInfoProducer() {
  delete info_;
}

void ICEventInfoProducer::endRun(edm::Run const& run, edm::EventSetup const& es) {
  if (!do_lhe_weights_) return;
  if (lhe_weight_labels_.size()) return;
  edm::Handle<LHERunInfoProduct> lhe_info;
  run.getByLabel("externalLHEProducer", lhe_info);
  bool record = false;
  for (auto it = lhe_info->headers_begin(); it != lhe_info->headers_end();
       ++it) {
    std::vector<std::string>::const_iterator iLt = it->begin();
    for (; iLt != it->end(); ++iLt) {
      std::string const& line = *iLt;
      if (line.find("<weightgroup")  != std::string::npos) record = true;
      if (line.find("</weightgroup") != std::string::npos) record = false;
      if (record) lhe_weight_labels_.push_back(line);
    }
  }
}

void ICEventInfoProducer::produce(edm::Event& event,
                                  const edm::EventSetup& setup) {
  *info_ = ic::EventInfo();
  info_->set_is_data(event.isRealData());
  info_->set_run(event.run());
#if CMSSW_MAJOR_VERSION >=7 && CMSSW_MINOR_VERSION >= 3
  info_->set_event(event.id().event());
#else
  info_->set_event((unsigned long long)event.id().event());
#endif
  info_->set_lumi_block(event.luminosityBlock());
  info_->set_bunch_crossing(event.bunchCrossing());

  edm::Handle<double> jets_rho_handle;
  if (do_jets_rho_) {
    event.getByLabel(input_jets_rho_, jets_rho_handle);
    info_->set_jet_rho(*jets_rho_handle);
  }
  edm::Handle<double> lepton_rho_handle;
  if (do_leptons_rho_) {
    event.getByLabel(input_leptons_rho_, lepton_rho_handle);
    info_->set_lepton_rho(*lepton_rho_handle);
  }

  for (unsigned i = 0; i < weights_.size(); ++i) {
    edm::Handle<double> weight;
    event.getByLabel(weights_[i].second, weight);
    double weights_result = (*weight);
    info_->set_weight(weights_[i].first, weights_result);
  }


  for (unsigned i = 0; i < gen_weights_.size(); ++i) {
    edm::Handle<GenFilterInfo> weight;
    event.getByLabel(gen_weights_[i].second, weight);
    double weights_result = weight->filterEfficiency();
    info_->set_weight(gen_weights_[i].first, weights_result);
  }

  edm::Handle<edm::View<reco::Vertex> > vtxs_handle;
  if (do_vertex_count_) {
    event.getByLabel(input_vertices_, vtxs_handle);
    info_->set_good_vertices(vtxs_handle->size());
  }

  edm::Handle<LHEEventProduct> lhe_handle;
  edm::Handle<GenEventInfoProduct> gen_info_handle;
  if(!event.isRealData()){
    event.getByLabel("generator",gen_info_handle);
    if(gen_info_handle.isValid()){
      if(gen_info_handle->weight()>=0){
        info_->set_weight("wt_mc_sign",1);
      } else info_->set_weight("wt_mc_sign",-1);
    }
  }
  if(is_nlo_){
    event.getByLabel(lhe_collection_, lhe_handle); 
   /* Accessing global evt weights directly from the LHEEventProduct
      disabled and replaced by taking them from the GenEventInfoProduct
      which should exist in all samples, independent of which generator was used
      if(lhe_handle->hepeup().XWGTUP>=0){
      info_->set_weight("wt_mc_sign",1);
      } else info_->set_weight("wt_mc_sign",-1);
    */
    if (do_lhe_weights_) {
      double nominal_wt = lhe_handle->hepeup().XWGTUP;
      for (unsigned i = 0; i < lhe_handle->weights().size(); ++i) {
        info_->set_weight(lhe_handle->weights()[i].id,
                          lhe_handle->weights()[i].wgt / nominal_wt, false);
      }
    }
  }

  for (unsigned i = 0; i < filters_.size(); ++i) {
    edm::Handle<bool> filter;
    event.getByLabel(filters_[i].second, filter);
    bool filter_result = (*filter);
    if (invert_filter_logic_.find(filters_[i].first) !=
        invert_filter_logic_.end())
      filter_result = !filter_result;
    info_->set_filter_result(filters_[i].first, filter_result);
    observed_filters_[filters_[i].first] = CityHash64(filters_[i].first);
  }

   if(do_filtersfromtrig_){
     edm::Handle<edm::TriggerResults> triggerResults;
     event.getByLabel(filtersfromtrig_input_,triggerResults);
     if( !triggerResults.isValid() ){
       throw cms::Exception("TriggerNotValid")<<"Trigger Results is not valid\n";
     }
     
     const edm::TriggerNames &triggerNames = event.triggerNames(*triggerResults);
     for (unsigned iFilter=0;iFilter<filtersfromtrig_.size();iFilter++){
       bool found=false;
       bool opposite=false;
       std::string filtername=filtersfromtrig_[iFilter];
       if((filtersfromtrig_[iFilter]).at(0)=='!'){
	 filtername.erase(0,1);
	 opposite=true;
       }
       for(unsigned iTrigger=0;iTrigger<triggerResults->size();iTrigger++){
	 std::string trigName = triggerNames.triggerName(iTrigger);
     	 if(trigName.find(filtername)==std::string::npos) continue;
	 else{
	   found=true;
	   if(!triggerResults->wasrun(iTrigger)){
	     throw cms::Exception("TriggerNotRun")<<filtername<<" was not run\n";
	   }
	   bool filter_result=triggerResults->accept(iTrigger);
	   if(opposite)filter_result=!filter_result;
	   info_->set_filter_result(filtername, filter_result);
	   observed_filters_[filtername] = CityHash64(filtername);
	 }
       }
       if(!found){
	 throw cms::Exception("TriggerNotFound")<<filtername<<" was not found in trigger results\n";
       }
     }
   }

  edm::Handle<reco::BeamHaloSummary> beam_halo_handle;
  if (do_csc_filter_) {
    event.getByLabel(input_csc_filter_, beam_halo_handle);
    info_->set_filter_result("CSCTightHaloFilter",
                             beam_halo_handle->CSCTightHaloId());
    observed_filters_["CSCTightHaloFilter"] = CityHash64("CSCTightHaloFilter");
  }
}

void ICEventInfoProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &info_);
}

void ICEventInfoProducer::endJob() {
  if (!observed_filters_.empty()) {
    std::cout << std::string(78, '-') << "\n";
    std::cout << boost::format("%-56s  %20s\n") %
                     std::string("EventInfo Filters") %
                     std::string("Hash Summmary");
    std::map<std::string, std::size_t>::const_iterator iter;
    for (iter = observed_filters_.begin(); iter != observed_filters_.end();
         ++iter) {
      std::cout << boost::format("%-56s| %020i\n") % iter->first % iter->second;
    }
  }
  if (lhe_weight_labels_.size()) {
    std::cout << std::string(78, '-') << "\n";
    std::cout << "LHE event weights\n";
    for (unsigned l = 0; l < lhe_weight_labels_.size(); ++l) {
      std::cout << lhe_weight_labels_[l];
    }
  }
}

// define this as a plug-in
DEFINE_FWK_MODULE(ICEventInfoProducer);
