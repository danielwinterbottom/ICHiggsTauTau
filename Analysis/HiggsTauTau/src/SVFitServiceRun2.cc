#include "HiggsTauTau/interface/SVFitServiceRun2.h"

namespace ic {

  SVFitServiceRun2::SVFitServiceRun2() {
    ;
  }
  
  SVFitServiceRun2::~SVFitServiceRun2() {
    ;
  }

  double SVFitServiceRun2::SVFitMassEleHad(Candidate const* lep, Candidate const* had, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep->pt(), lep->eta(), lep->phi(), lep->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    return algo.getMass();
  }

  double SVFitServiceRun2::SVFitMassMuHad(Candidate const* lep, Candidate const* had, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep->pt(), lep->eta(), lep->phi(), lep->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    return algo.getMass();
  }

  double SVFitServiceRun2::SVFitMassHadHad(Candidate const* had1, Candidate const* had2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had1->pt(), had1->eta(), had1->phi(), had1->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had2->pt(), had2->eta(), had2->phi(), had2->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    return algo.getMass();
  }


  double SVFitServiceRun2::SVFitMassEleMu(Candidate const* lep1, Candidate const* lep2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep1->pt(), lep1->eta(), lep1->phi(), lep1->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep2->pt(), lep2->eta(), lep2->phi(), lep2->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    return algo.getMass();
  }

  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateEleHad(Candidate const* lep, Candidate const* had, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep->pt(), lep->eta(), lep->phi(), lep->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(),met->vector().py(), covMET, 0);
    algo.addLogM(false);
    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));

    return std::make_pair(fitresult, algo.getMass());
  }

  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateMuHad(Candidate const* lep, Candidate const* had, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep->pt(), lep->eta(), lep->phi(),lep->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));

    return std::make_pair(fitresult, algo.getMass());
  }


  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateEleMu(Candidate const* lep1, Candidate const* lep2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep1->pt(),lep1->eta(), lep1->phi(), lep1->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep2->pt(), lep2->eta(), lep2->phi(), lep2->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));

    return std::make_pair(fitresult, algo.getMass());
  }

  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateHadHad(Candidate const* had1, Candidate const* had2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had1->pt(), had1->eta(), had1->phi(), had1->M()));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had2->pt(), had2->eta(), had2->phi(), had2->M()));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));

    return std::make_pair(fitresult, algo.getMass());
  }



}
