#ifndef UserCode_ICHiggsTauTau_ICMadgraphWeightsProducer_h
#define UserCode_ICHiggsTauTau_ICMadgraphWeightsProducer_h

#include <memory>
#include <vector>
#include <string>
#include "boost/functional/hash.hpp"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
//#include "UserCode/ICHiggsTauTau/interface/MadGraphTools.h"
#include "CMSAachen3B/MadGraphReweighting/interface/MadGraphTools.h"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"


/**
 * @brief See documentation [here](\ref objs-candidate)
 */
class ICMadgraphWeightsProducer : public edm::EDProducer {
 public:
  explicit ICMadgraphWeightsProducer(const edm::ParameterSet &);
  ~ICMadgraphWeightsProducer();

 private:
  virtual void beginJob();
  virtual void produce(edm::Event &, const edm::EventSetup &);
  virtual void endJob();
  
  std::vector<std::pair<std::string,double>> SplitString(std::string instring);
  ic::EventInfo *info_;
  std::vector<std::pair<std::string,double>> theta_vec_;
  
  edm::InputTag input_;
  std::string branch_;
  std::string theta_;
  double weight_;
  double theta_sample_;
  std::string process_dir_;
  std::string param_card_;
  std::string param_card_sample_;
  
  unsigned count_outpartons_;
  

};

#endif
