#include "HiggsTauTau/interface/SVFitServiceRun2.h"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"

namespace ic {

  SVFitServiceRun2::SVFitServiceRun2() {
    ;
  }
  
  SVFitServiceRun2::~SVFitServiceRun2() {
    ;
  }

  double SVFitServiceRun2::SVFitMassEleHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep->pt(), lep->eta(), lep->phi(), 0.000511));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M(),decm2));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    delete inputFile_visPtResolution;
    return algo.getMass();
  }

  double SVFitServiceRun2::SVFitMassMuHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep->pt(), lep->eta(), lep->phi(), 0.10566));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M(),decm2));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    delete inputFile_visPtResolution;
    return algo.getMass();
  }

  double SVFitServiceRun2::SVFitMassHadHad(Candidate const* had1, int decm1, Candidate const* had2, int decm2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had1->pt(), had1->eta(), had1->phi(), had1->M(),decm1));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had2->pt(), had2->eta(), had2->phi(), had2->M(),decm2));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    delete inputFile_visPtResolution;
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
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep1->pt(), lep1->eta(), lep1->phi(), 0.000511));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep2->pt(), lep2->eta(), lep2->phi(), 0.10566));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    delete inputFile_visPtResolution;
    return algo.getMass();
  }

  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateEleHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep->pt(), lep->eta(), lep->phi(), 0.000511));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M(),decm2));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(),met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));
    delete inputFile_visPtResolution;

    return std::make_pair(fitresult, algo.getMass());
  }

  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateMuHad(Candidate const* lep, Candidate const* had, int decm2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep->pt(), lep->eta(), lep->phi(),0.10566));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had->pt(), had->eta(), had->phi(), had->M(),decm2));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));
    delete inputFile_visPtResolution;
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
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToElecDecay, lep1->pt(),lep1->eta(), lep1->phi(), 0.000511));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToMuDecay, lep2->pt(), lep2->eta(), lep2->phi(), 0.10566));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));
    delete inputFile_visPtResolution;

    return std::make_pair(fitresult, algo.getMass());
  }

  std::pair<Candidate, double> SVFitServiceRun2::SVFitCandidateHadHad(Candidate const* had1, int decm1, Candidate const* had2, int decm2, Met const* met, bool MC) {
    svFitStandalone::Vector met_vec(met->vector().px(), met->vector().py(), met->vector().pz());
    TMatrixD covMET(2, 2);
    covMET(0,0) = met->xx_sig();
    covMET(1,0) = met->yx_sig();
    covMET(0,1) = met->xy_sig();
    covMET(1,1) = met->yy_sig();
    std::vector<svFitStandalone::MeasuredTauLepton> measuredTauLeptons;
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had1->pt(), had1->eta(), had1->phi(), had1->M(),decm1));
    measuredTauLeptons.push_back(svFitStandalone::MeasuredTauLepton(svFitStandalone::kTauToHadDecay, had2->pt(), had2->eta(), had2->phi(), had2->M(),decm2));
    SVfitStandaloneAlgorithm algo(measuredTauLeptons, met->vector().px(), met->vector().py(), covMET, 0);
    algo.addLogM(false);
    TH1::AddDirectory(false);
    TFile *inputFile_visPtResolution = new TFile("SVFitRun2/data/svFitVisMassAndPtResolutionPDF.root");
    algo.shiftVisPt(false, inputFile_visPtResolution);

    Candidate fitresult;

    if(MC) algo.integrateMarkovChain();
    else algo.integrateVEGAS();
    fitresult.set_vector(ROOT::Math::PtEtaPhiEVector(algo.fittedDiTauSystem()));
    delete inputFile_visPtResolution;

    return std::make_pair(fitresult, algo.getMass());
  }



}
