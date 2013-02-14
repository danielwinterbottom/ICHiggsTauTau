#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/SVFitService.h"

namespace ic {

  SVFitService::SVFitService() {
    ;
  }
  
  SVFitService::~SVFitService() {
    ;
  }

  double SVFitService::SVFitMassLepHad(Candidate const* lep, Candidate const* had, Met const* met) {
    NSVfitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, LorentzVector(lep->vector())));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, LorentzVector(had->vector())));
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met_vec, covMET, 0);
    algo.addLogM(false);
    #if defined(__CMSSW_5_3_7__)
      algo.integrateMarkovChain();
    #else
      algo.integrate();
    #endif
    return algo.getMass();
  }

  double SVFitService::SVFitMassLepLep(Candidate const* lep1, Candidate const* lep2, Met const* met) {
    NSVfitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, LorentzVector(lep1->vector())));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, LorentzVector(lep2->vector())));
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met_vec, covMET, 0);
    algo.addLogM(false);
    #if defined(__CMSSW_5_3_7__)
      algo.integrateMarkovChain();
    #else
      algo.integrate();
    #endif
    return algo.getMass();
  }

  std::pair<Candidate, double> SVFitService::SVFitCandidateLepHad(Candidate const* lep, Candidate const* had, Met const* met) {
    NSVfitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, LorentzVector(lep->vector())));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kHadDecay, LorentzVector(had->vector())));
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met_vec, covMET, 0);
    algo.addLogM(false);
    Candidate fitresult;

    #if defined(__CMSSW_5_3_7__)
      algo.integrateMarkovChain();
      fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));
    #else
      algo.integrate();
    #endif

    return std::make_pair(fitresult, algo.getMass());;
  }

  std::pair<Candidate, double> SVFitService::SVFitCandidateLepLep(Candidate const* lep1, Candidate const* lep2, Met const* met) {
    NSVfitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<NSVfitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, LorentzVector(lep1->vector())));
    measuredTauLeptons.push_back(NSVfitStandalone::MeasuredTauLepton(NSVfitStandalone::kLepDecay, LorentzVector(lep2->vector())));
    NSVfitStandaloneAlgorithm algo(measuredTauLeptons, met_vec, covMET, 0);
    algo.addLogM(false);
    Candidate fitresult;

    #if defined(__CMSSW_5_3_7__)
      algo.integrateMarkovChain();
      fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));
    #else
      algo.integrate();
    #endif

    return std::make_pair(fitresult, algo.getMass());
  }


}