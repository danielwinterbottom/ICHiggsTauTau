#include "UserCode/ICHiggsTauTau/plugins/ICGenVertexProducer.hh"
#include <string>
#include <vector>
#include "boost/format.hpp"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/View.h"
#include "UserCode/ICHiggsTauTau/interface/StaticTree.hh"
#include "UserCode/ICHiggsTauTau/plugins/PrintConfigTools.h"
#include "UserCode/ICHiggsTauTau/plugins/Consumes.h"

ICGenVertexProducer::ICGenVertexProducer(const edm::ParameterSet& config)
    : input_(config.getParameter<edm::InputTag>("input")),
      branch_(config.getParameter<std::string>("branch")),
      type_(config.getParameter<std::string>("type")) {
  consumes<edm::View<reco::GenParticle>>(input_);
  vertices_ = new std::vector<ic::Vertex>();
  if (type_ != "pp") {
    throw cms::Exception("OptionNotSupported") << "Vertex type " << type_
                                               << " not supported\n";
  }
  PrintHeaderWithProduces(config, input_, branch_);
}

ICGenVertexProducer::~ICGenVertexProducer() { delete vertices_; }

void ICGenVertexProducer::produce(edm::Event& event,
                                    const edm::EventSetup& setup) {
  edm::Handle<edm::View<reco::GenParticle> > parts_handle;
  event.getByLabel(input_, parts_handle);

  vertices_->clear();

  for (unsigned i = 0; i < parts_handle->size(); ++i) {
    reco::GenParticle const& src = parts_handle->at(i);
    // std::cout
    //  << boost::format(
    //    "%i st=%i id=%i vx=%f vy=%f vz=%f px=%f py=%f pz=%f e=%f nm=%i\n") %
    //    i % src.status() % src.pdgId() % src.vx() % src.vy() % src.vz() %
    //    src.px() % src.py() % src.pz() % src.energy() % src.numberOfMothers();
    if (type_ == "pp" && src.pdgId() == 2212 && src.numberOfMothers() == 0) {
      // We've found a colliding proton, but the vertex info is only added
      // for the daughters
      if (src.numberOfDaughters() > 0 && src.daughterRef(0).isNonnull()) {
        std::tuple<double, double, double> vtx_tuple =
            std::make_tuple(src.daughterRef(0)->vx(), src.daughterRef(0)->vy(),
                            src.daughterRef(0)->vz());
        if (!done_vtxs_.count(vtx_tuple)) {
          // Create new vertex now
          vertices_->push_back(ic::Vertex());
          ic::Vertex & dest = vertices_->back();
          dest.set_vx(std::get<0>(vtx_tuple));
          dest.set_vy(std::get<1>(vtx_tuple));
          dest.set_vz(std::get<2>(vtx_tuple));
          done_vtxs_.insert(vtx_tuple);
        }
      }
    }
  }
}

void ICGenVertexProducer::beginJob() {
  ic::StaticTree::tree_->Branch(branch_.c_str(), &vertices_);
}

void ICGenVertexProducer::endJob() {}

// define this as a plug-in
DEFINE_FWK_MODULE(ICGenVertexProducer);
