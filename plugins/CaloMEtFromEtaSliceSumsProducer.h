#ifndef LLRAnalysis_Utilities_CaloMEtFromEtaSliceSumsProducer_h
#define LLRAnalysis_Utilities_CaloMEtFromEtaSliceSumsProducer_h

/** \class CaloMEtFromEtaSliceSumsProducer
 *
 * Takes Px, Py sums of CaloTowers in slices of eta
 * and apply data/MC residual corrections to those sums,
 * in order to correct CaloMET without having to store 
 * full CaloTower collection in PAT-tuple
 *
 * \authors Christian Veelken, LLR
 *
 * \version $Revision: 1.1.2.2 $
 *
 * $Id: CaloMEtFromEtaSliceSumsProducer.h,v 1.1.2.2 2013/02/27 14:06:15 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/METReco/interface/CorrMETData.h"

#include "DataFormats/Candidate/interface/Candidate.h"

#include <string>
#include <vector>

class CaloMEtFromEtaSliceSumsProducer : public edm::EDProducer  
{
 public:

  explicit CaloMEtFromEtaSliceSumsProducer(const edm::ParameterSet&);
  ~CaloMEtFromEtaSliceSumsProducer();
    
 private:

  void produce(edm::Event&, const edm::EventSetup&);

  std::string moduleLabel_;

  edm::InputTag srcMEt_; // input MET collection (for sumEt, MET significance...)
  edm::InputTag srcCaloTowerEtaSliceSums_;

  struct binningEntryType
  {
    binningEntryType()
      : binLabel_(""),
        binCenter_(0)
    {}
    binningEntryType(const edm::ParameterSet& cfg)
    : binLabel_(cfg.getParameter<std::string>("binLabel")),
      binCenter_(cfg.getParameter<double>("binCenter"))
    {}
    ~binningEntryType() {}
    std::string binLabel_;
    double binCenter_;
  };
  std::vector<binningEntryType*> binning_;

  std::string residualCorrLabel_;
  double residualCorrEtaMax_;
  double extraCorrFactor_;

  bool isMC_;

  int verbosity_;
};

#endif


 

