#include "UserCode/ICHiggsTauTau/Analysis/Modules/interface/HttSync.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/interface/Tau.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "TMVA/Reader.h"
#include "TVector3.h"


namespace ic {

  HttSync::HttSync(std::string const& name, std::string const& output_name, int mode) : ModuleBase(name) {
    output_name_ = output_name;
    mode_ = mode;
    jet_eta_ = 4.7;
    is_embedded_ = false;
    reader = new TMVA::Reader( "!Color:!Silent" );
    vbfvars = std::vector<float>(8,0);
    //
    // Set up the TMVA reader
    //
    reader->AddVariable("mjj", &vbfvars[0]);
    reader->AddVariable("dEta", &vbfvars[1]);
    reader->AddVariable("dPhi", &vbfvars[2]);
    reader->AddVariable("ditau_pt", &vbfvars[3]);
    reader->AddVariable("dijet_pt", &vbfvars[4]);
    reader->AddVariable("dPhi_hj", &vbfvars[5]);
    reader->AddVariable("C1", &vbfvars[6]);
    reader->AddVariable("C2", &vbfvars[7]);
    std::string file = "data/vbf_mva/VBFMVA_BDTG.weights.xml";
    if (mode_ == 2) file = "data/vbf_mva/VBFMVA_EMu_BDTG.weights.xml";
    reader->BookMVA("BDTG", file);

    select_sel_mode_ = -1;
    select_category_ = "";


  }

  HttSync::~HttSync() {
    ;
  }

  int HttSync::PreAnalysis() {

    lOFile = new TFile(output_name_.c_str(),"RECREATE");
    lOFile->cd();
    lOTree = new TTree("TauCheck","TauCheck");
lOTree->Branch("run"        ,&lRun           ,"lRun/I"     );//Run
lOTree->Branch("lumi"       ,&lLumi          ,"lLumi/I"    );//Lumi
lOTree->Branch("evt"        ,&lEvt           ,"lEvt/I"     );//Evt

    //Event Variables
lOTree->Branch("npv"        ,&lNPV           ,"lNPV/I"     );//NPV
lOTree->Branch("npu"        ,&lNPU           ,"lNPU/I"     );//NPU
lOTree->Branch("rho"        ,&lRho           ,"lRho/F"     );//Rho

    //Event Weights
lOTree->Branch("mcweight"   ,&lMCWeight      ,"lMCWeight/F");//MC Weight (xs/nevents * additional wieght (ie pt weight for gghiggs))
lOTree->Branch("puweight"   ,&lPUWeight      ,"lPUWeight/F");//Pielup Weight

lOTree->Branch("trigweight_1"  ,&lTrigweight_1     ,"trigweight_1/F");//Effieiency Scale factor (all components multiplied in)
lOTree->Branch("trigweight_2"  ,&lTrigweight_2     ,"trigweight_2/F");//Effieiency Scale factor (all components multiplied in)
lOTree->Branch("idweight_1"  ,&lIdweight_1     ,"idweight_1/F");//Effieiency Scale factor (all components multiplied in)
lOTree->Branch("idweight_2"  ,&lIdweight_2     ,"idweight_2/F");//Effieiency Scale factor (all components multiplied in)
lOTree->Branch("isoweight_1"  ,&lIsoweight_1     ,"isoweight_1/F");//Effieiency Scale factor (all components multiplied in)
lOTree->Branch("isoweight_2"  ,&lIsoweight_2     ,"isoweight_2/F");//Effieiency Scale factor (all components multiplied in)

lOTree->Branch("effweight"  ,&lEffWeight     ,"lEffWeight/F");//Effieiency Scale factor (all components multiplied in)
lOTree->Branch("weight"     ,&lWeight        ,"lWeight/F"  );//mcweight*puweight*effweight

    //SV Fit variables

lOTree->Branch("mvis"       ,&lMVis           ,"lMVis/F"     );//SV Fit using integration method

lOTree->Branch("m_sv"       ,&lMSV           ,"lMSV/F"     );//SV Fit using integration method
lOTree->Branch("m_sv_Up"    ,&lMSVUp         ,"lMSVUp/F"   );//High Energy scale shape
lOTree->Branch("m_sv_Down"  ,&lMSVDown       ,"lMSVDown/F" );//Low Energy Scale Shape

    ///First lepton :  muon for mu Tau, electron for e Tau, electron for e mu, Leading (in pT) Tau for Tau Tau
lOTree->Branch("pt_1"       ,&lPt1           ,"lPt1/F"     ); //pT 
lOTree->Branch("phi_1"      ,&lPhi1          ,"lPhi1/F"    ); //Phi 
lOTree->Branch("eta_1"      ,&lEta1          ,"lEta1/F"    ); //Eta 
lOTree->Branch("m_1"        ,&lM1            ,"lM1/F"      ); //Mass 
lOTree->Branch("q_1"        ,&lq1            ,"lq1/I"      ); //Mass 
lOTree->Branch("iso_1"      ,&lIso1          ,"lIso1/F"    ); //Delta Beta iso value 
lOTree->Branch("mva_1"      ,&lMVA1          ,"lMMVA1/F"   );//MVA id (when using electron) 0 otherwise
lOTree->Branch("d0_1"       ,&lD01           ,"lD0/F"      );//d0 with respect to primary vertex
lOTree->Branch("dZ_1"       ,&lDZ1           ,"lDZ/F"      );//dZ with respect to primary vertex
lOTree->Branch("passid_1"   ,&lPassId1       ,"lPassId1/B" );//Whether it passes id  (not necessarily iso)
lOTree->Branch("passiso_1"  ,&lPassIso1      ,"lPassIso1/B");//Whether it passes iso (not necessarily id)
lOTree->Branch("mt_1"       ,&lMt1           ,"lMt1/F"     );//mT of  first lepton wrt to MVA met

    ///Second lepton :  hadronic Tau for mu Tau had for e Tau, Muon for e mu, Trailing (in pT)  Tau for Tau Tau
lOTree->Branch("pt_2"       ,&lPt2           ,"lPt2/F"     );//pT
lOTree->Branch("phi_2"      ,&lPhi2          ,"lPhi2/F"    );//Phi
lOTree->Branch("eta_2"      ,&lEta2          ,"lEta2/F"    );//Eta
lOTree->Branch("m_2"        ,&lM2            ,"lM2/F"      );//Mass (visible mass for hadronic Tau)
lOTree->Branch("q_2"        ,&lq2            ,"lq2/I"      ); //Mass 
lOTree->Branch("iso_2"      ,&lIso2          ,"lIso2/F"    );//MVA iso for hadronic Tau, Delta Beta for muon
lOTree->Branch("mva_2"      ,&lMVA2          ,"lMMVA2/F"   );//MVA id (for anti electron id)
lOTree->Branch("passid_2"   ,&lPassId2       ,"lPassId2/B" );//Whether it passes id  (not necessarily iso)
lOTree->Branch("passiso_2"  ,&lPassIso2      ,"lPassIso2/B");//Whether it passes iso (not necessarily id)
lOTree->Branch("mt_2"       ,&lMt2           ,"lMt2/F"     );//mT of 2nd lepton wrt to MVA met

    //Met related variables
lOTree->Branch("met"        ,&lMet           ,"lMet/F"      ); //pfmet
lOTree->Branch("metphi"     ,&lMetPhi        ,"lMetPhi/F"   ); //pfmet Phi
lOTree->Branch("mvamet"     ,&lMVAMet        ,"lMet/F"      ); //mvamet
lOTree->Branch("mvametphi"  ,&lMVAMetPhi     ,"lMetPhi/F"   ); //mvamet Phi
lOTree->Branch("pzetavis"   ,&lPZetaVis      ,"lPZetaVis/F" ); //pZeta Visible
lOTree->Branch("pzetamiss"  ,&lPZetaMiss     ,"lPZetaMiss/F"); //pZeta Missing
    //MET covariance matrices
lOTree->Branch("metcov00"   ,&lMetCov00      ,"lMetCov00/F"); //pf met covariance matrix 00 
lOTree->Branch("metcov01"   ,&lMetCov01      ,"lMetCov01/F"); //pf met covariance matrix 01 
lOTree->Branch("metcov10"   ,&lMetCov10      ,"lMetCov10/F"); //pf met covariance matrix 10 
lOTree->Branch("metcov11"   ,&lMetCov11      ,"lMetCov11/F"); //pf met covariance matrix 11 
    //MVAMet covariance matrices
lOTree->Branch("mvacov00"   ,&lMVACov00      ,"lMVACov00/F"); //mva met covariance matrix 00 
lOTree->Branch("mvacov01"   ,&lMVACov01      ,"lMVACov01/F"); //mva met covariance matrix 01 
lOTree->Branch("mvacov10"   ,&lMVACov10      ,"lMVACov10/F"); //mva met covariance matrix 10 
lOTree->Branch("mvacov11"   ,&lMVACov11      ,"lMVACov11/F"); //mva met covariance matrix 11 

    //First Jet   : leading jet after applying Jet energy corrections (excluding hadronic Tau)
lOTree->Branch("jpt_1"      ,&lJPt1          ,"lJPt1/F"     );//Jet Pt after corrections
lOTree->Branch("jeta_1"     ,&lJEta1         ,"lJEta1/F"    );//Jet Eta
lOTree->Branch("jphi_1"     ,&lJPhi1         ,"lJPhi1/F"    );//Jet Phi     
lOTree->Branch("jptraw_1"   ,&lJPtRaw1       ,"lJPtRaw1/F"  );//Jet Raw Pt (before corrections)
lOTree->Branch("jptunc_1"   ,&lJPtUnc1       ,"lJPtUnc1/F"  );//Jet Unc (relative to Jet corrected pT)
lOTree->Branch("jmva_1"     ,&lJMVA1         ,"lJMVA1/F"    );//Jet MVA id value
lOTree->Branch("jpass_1"    ,&lJPass1        ,"lJPass1/B"   );//Whether Jet pass PU Id Loose WP

    //Second Jet  : 2nd leading jet (in pt) afer applying Jet energy corrections (excluding Tau)
lOTree->Branch("jpt_2"      ,&lJPt2          ,"lJPt2/F"     );//Jet Pt after corrections
lOTree->Branch("jeta_2"     ,&lJEta2         ,"lJEta2/F"    );//Jet Eta
lOTree->Branch("jphi_2"     ,&lJPhi2         ,"lJPhi2/F"    );//Jet Phi
lOTree->Branch("jptraw_2"   ,&lJPtRaw2       ,"lJPtRaw2/F"  );//Jet Raw Pt (before corrections)
lOTree->Branch("jptunc_2"   ,&lJPtUnc2       ,"lJPtUnc2/F"  );//Jet Unc (relative to Jet corrected pT)
lOTree->Branch("jmva_2"     ,&lJMVA2         ,"lJMVA2/F"    );//Jet MVA id value
lOTree->Branch("jpass_2"    ,&lJPass2        ,"lJPass2/B"   );//Whether jet passes PU Id Loose WP 

    //B Tagged Jet : leading btagged jet (in pt) passing btag wp (pt > 20 + cvs medium)
lOTree->Branch("bpt"        ,&lBTagPt        ,"lBTagPt/F"   );//Corrected BTag Pt
lOTree->Branch("beta"       ,&lBTagEta       ,"lBTagEta/F"  );//Btag Eta
lOTree->Branch("bphi"       ,&lBTagPhi       ,"lBTagPhi/F"  );//Btag Phi

    //Di Jet kinematic variables for VBF selection ==> Two leading pT Jets 
lOTree->Branch("mjj"        ,&lMJJ           ,"lMJJ/F"      );//Mass Di Jet system  
lOTree->Branch("jdeta"      ,&lJDEta         ,"lJDEta/F"    );//|jeta_1-jeta_2| 
lOTree->Branch("njetingap"  ,&lNJetInGap     ,"lNJetInGap/I");//# of Jets between two jets
lOTree->Branch("mva"        ,&lMVA           ,"lMVA/F"      );//VBF MVA value

    //Variables that go into the VBF MVA
lOTree->Branch("jdphi"      ,&lJDPhi         ,"lJDPhi/F"    );//Delta Phi between two leading jets
lOTree->Branch("dijetpt"    ,&lDiJetPt       ,"lDiJetPt/F"  );//Pt of the di jet system
lOTree->Branch("dijetphi"   ,&lDiJetPhi      ,"lDiJetPhi/F" );//Phi of the di jet system
lOTree->Branch("hdijetphi"  ,&lHDJetPhi      ,"lHDJetPhi/F" );//Phi of the di jet system - Higgs system phi
lOTree->Branch("visjeteta"  ,&lVisJetEta     ,"lVisJetEta/F");//TMath::Min(eta_vis - jeta,eta_vis,jeta2);
lOTree->Branch("ptvis"      ,&lPtVis         ,"lPtVis/F"    );//Pt Vis

    //number of btags passing btag id ( pt > 20 )
lOTree->Branch("nbtag"      ,&lNBTag         ,"lNBTag/I");

    //number of jets passing jet id ( pt > 30 )
lOTree->Branch("njets"      ,&lNJets         ,"lNJets/I");
lOTree->Branch("njetspt20"  ,&lNJetsPt20     ,"lNJetsPt20/I");


    return 0;
  }

  int HttSync::Execute(TreeEvent *event) {

    if (select_category_ != "") {
      if (event->Exists("cat_status")) {
        std::map<std::string, bool> cat_status = event->Get< std::map<std::string, bool> >("cat_status");
        if (cat_status.count(select_category_) > 0) {
          if (!(cat_status[select_category_])) return 0;
        } else {
          return 0;
        }
      }
    }

    if (select_sel_mode_ >= 0) {
      unsigned sel_mode  = event->Get<unsigned>("sel_mode");
      if (int(sel_mode) != select_sel_mode_) return 0;
    }


    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>("pfJetsPFlow");
    std::vector<Tau*> taus = event->GetPtrVec<Tau>("taus");
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 0.0, jet_eta_));

    std::vector<CompositeCandidate *> const& dilepton = event->GetPtrVec<CompositeCandidate>("emtauCandidates");
    //if (!PairOppSign(dilepton[0])) return 0;

    Met const* pfMet = event->GetPtr<Met>("pfMet");
    Met const* pfMetMVA = event->GetPtr<Met>("pfMVAMet");
    Met const* selectedMet = event->GetPtr<Met>(met_label_);

    std::vector<PileupInfo *> puInfo;
    float true_int = -1;

    if (event->Exists("pileupInfo")) {
      puInfo = event->GetPtrVec<PileupInfo>("pileupInfo");
      for (unsigned i = 0; i < puInfo.size(); ++i) {
        if (puInfo[i]->bunch_crossing() == 0) true_int = puInfo[i]->true_num_interactions();
      }
    }

    lRun = eventInfo->run();
    lLumi = eventInfo->lumi_block();
    lEvt = eventInfo->event();

    //Event Variables
    //lNPV = vertices.size();
    //lNPV = std::count_if(vertices.begin(), vertices.end(), GoodVertex);
    lNPV = eventInfo->good_vertices();
    lNPU = true_int;
    lRho = 0.0;
    
    //Event Weights
    lMCWeight = 0.0;
    if (!is_embedded_ && event->Exists("pileupInfo")) lPUWeight = eventInfo->weight("pileup");
    if (event->Exists("trigweight_1")) lTrigweight_1 = event->Get<double>("trigweight_1");
    if (event->Exists("trigweight_2")) lTrigweight_2 = event->Get<double>("trigweight_2");
    if (event->Exists("idweight_1")) lIdweight_1 = event->Get<double>("idweight_1");
    if (event->Exists("idweight_2")) lIdweight_2 = event->Get<double>("idweight_2");
    if (event->Exists("isoweight_1")) lIsoweight_1 = event->Get<double>("isoweight_1");
    if (event->Exists("isoweight_2")) lIsoweight_2 = event->Get<double>("isoweight_2");
    lEffWeight = eventInfo->weight("lepton");
    lWeight = eventInfo->total_weight();
    

    ///First lepton :  muon for mu Tau, electron for e Tau, electron for e mu, Leading (in pT) Tau for Tau Tau
    Candidate* lepton = dilepton.at(0)->GetCandidate("lepton1");
    Candidate* tau = dilepton.at(0)->GetCandidate("lepton2");

    lPt1 = lepton->pt();
    lPhi1 = lepton->phi();
    lEta1 = lepton->eta();
    lM1 = lepton->vector().M();
    lq1 = lepton->charge();
    lPt2 = tau->pt();
    lPhi2 = tau->phi();
    lEta2 = tau->eta();
    lM2 = tau->vector().M();
    lq2 = tau->charge();

    lMVis = dilepton.at(0)->M();

    if (mode_ == 0) {
      if (event->Exists("svfitMass")) {
        lMSV = event->Get<double>("svfitMass");
      } else {
        lMSV = -999.;
        //lMSV = sv_service_.SVFitMassLepHad(lepton, tau, pfMetMVA);
      }
      Electron* elec = dynamic_cast<Electron*>(lepton);
      double iso =  elec->dr04_pfiso_charged_all() 
                    + std::max(elec->dr04_pfiso_neutral() + elec->dr04_pfiso_gamma() - 0.5 * elec->dr04_pfiso_pu(), 0.0);
      iso = iso / elec->pt();
      lIso1 = iso;
      lMVA1 = elec->GetIdIso("mvaNonTrigV0");
      lD01 = elec->dxy_vertex();
      lDZ1 = elec->dz_vertex();
      lPassId1 = true;
      lPassIso1 = true;
      lMt1 = MT(elec, selectedMet);

      Tau* htau = dynamic_cast<Tau*>(tau);
      lIso2 = htau->GetTauID("byIsolationMVAraw");
      lMVA2 = htau->GetTauID("againstElectronMVA");
      lPassId2 = true;
      lPassIso2 = true;
      lMt2 = MT(htau, selectedMet);

    } else if (mode_ == 1) {
      if (event->Exists("svfitMass")) {
        lMSV = event->Get<double>("svfitMass");
      } else {
        lMSV = -999.;
        //lMSV = sv_service_.SVFitMassLepHad(lepton, tau, pfMetMVA);

      }      
      Muon* muon = dynamic_cast<Muon*>(lepton);
      double iso =  muon->dr04_pfiso_charged_all() 
                    + std::max(muon->dr04_pfiso_neutral() + muon->dr04_pfiso_gamma() - 0.5 * muon->dr04_pfiso_pu(), 0.0);
      iso = iso / muon->pt();
      lIso1 = iso;
      lMVA1 = 0.0;
      lD01 = muon->dxy_vertex();
      lDZ1 = muon->dz_vertex();
      lPassId1 = true;
      lPassIso1 = true;
      lMt1 = MT(muon, selectedMet);
      Tau* htau = dynamic_cast<Tau*>(tau);
      lIso2 = htau->GetTauID("byIsolationMVAraw");
      lMVA2 = htau->GetTauID("againstElectronMVA");
      lPassId2 = true;
      lPassIso2 = true;
      lMt2 = MT(htau, selectedMet);
    } else if (mode_ == 2) {
      if (event->Exists("svfitMass")) {
        lMSV = event->Get<double>("svfitMass");
      } else {
        lMSV = -999.;
        //lMSV = sv_service_.SVFitMassLepLep(lepton, tau, pfMetMVA);

      }      Electron* elec = dynamic_cast<Electron*>(lepton);
      double iso =  elec->dr04_pfiso_charged_all() 
                    + std::max(elec->dr04_pfiso_neutral() + elec->dr04_pfiso_gamma() - 0.5 * elec->dr04_pfiso_pu(), 0.0);
      iso = iso / elec->pt();
      lIso1 = iso;
      lMVA1 = elec->GetIdIso("mvaNonTrigV0");
      lD01 = elec->dxy_vertex();
      lDZ1 = elec->dz_vertex();
      lPassId1 = true;
      lPassIso1 = true;
      lMt1 = MT(elec, selectedMet);

      Muon* muon = dynamic_cast<Muon*>(tau);
      double isom =  muon->dr04_pfiso_charged_all() 
                    + std::max(muon->dr04_pfiso_neutral() + muon->dr04_pfiso_gamma() - 0.5 * muon->dr04_pfiso_pu(), 0.0);
      isom = isom / muon->pt();
      lIso2 = isom;
      lMVA2 = 0.0;
      lPassId2 = true;
      lPassIso2 = true;
      lMt2 = MT(muon, selectedMet);
    }



    //Met related variables
    lMet = pfMet->pt();
    lMetPhi = pfMet->phi();
    lMVAMet = pfMetMVA->pt();
    lMVAMetPhi = pfMetMVA->phi();
    lPZetaVis = PZetaVis(dilepton.at(0));
    lPZetaMiss = PZeta(dilepton.at(0), pfMetMVA, 0.0);
    //MET covariance matrices
    lMetCov00 = pfMet->xx_sig();
    lMetCov01 = pfMet->xy_sig();
    lMetCov10 = pfMet->yx_sig();
    lMetCov11 = pfMet->yy_sig();
    //MVAMet covariance matrices
    lMVACov00 = pfMetMVA->xx_sig();
    lMVACov01 = pfMetMVA->xy_sig();
    lMVACov10 = pfMetMVA->yx_sig();
    lMVACov11 = pfMetMVA->yy_sig();

    //First Jet   : leading jet after applying Jet energy corrections (excluding hadronic Tau)
    if (jets.size() >= 1) {
      lJPt1 = jets[0]->pt();
      lJEta1 = jets[0]->eta();
      lJPhi1 = jets[0]->phi();
      lJPtRaw1 = jets[0]->uncorrected_energy() * (jets[0]->pt() / jets[0]->energy());
      lJPtUnc1 = 0.0;
      lJMVA1 = jets[0]->pu_id_mva_value();
      lJPass1 = jets[0]->pu_id_mva_loose();
    } else {
      lJPt1 = -9999;
      lJEta1 = -9999;
      lJPhi1 = -9999;
      lJPtRaw1 = -9999;
      lJPtUnc1 = -9999;
      lJMVA1 = -9999;
      lJPass1 = false;
    }

    //Second Jet  : 2nd leading jet (in pt) afer applying Jet energy corrections (excluding Tau)
    if (jets.size() >= 2) {
      lJPt2 = jets[1]->pt();
      lJEta2 = jets[1]->eta();
      lJPhi2 = jets[1]->phi();
      lJPtRaw2 = jets[1]->uncorrected_energy() * (jets[1]->pt() / jets[1]->energy());
      lJPtUnc2 = 0.0;
      lJMVA2 = jets[1]->pu_id_mva_value();
      lJPass2 = jets[1]->pu_id_mva_loose();
    } else {
      lJPt2 = -9999;
      lJEta2 = -9999;
      lJPhi2 = -9999;
      lJPtRaw2 = -9999;
      lJPtUnc2 = -9999;
      lJMVA2 = -9999;
      lJPass2 = false;
    }
    
    std::vector<PFJet *> btag_jets = jets;
    ic::erase_if(btag_jets, boost::bind(&PFJet::GetBDiscriminator, _1, "combinedSecondaryVertexBJetTags") < 0.679);
    ic::erase_if(btag_jets, !boost::bind(MinPtMaxEta, _1, 20, 2.4));

    if (btag_jets.size() >= 1) {
    //B Tagged Jet : leading btagged jet (in pt) passing btag wp (pt > 20 + cvs medium)
    lBTagPt = btag_jets[0]->pt();
    lBTagEta = btag_jets[0]->eta();
    lBTagPhi = btag_jets[0]->phi();
    } else {
      lBTagPt = 0;
      lBTagEta = 0;
      lBTagPhi = 0;
    }
    
    //Di Jet kinematic variables for VBF selection ==> Two leading pT Jets 
    if (jets.size() >= 2 && jets[0]->pt() > 20.0 && jets[1]->pt() > 20.0) {
      lMJJ = (jets[0]->vector() + jets[1]->vector()).M();
      lJDEta = fabs(jets[0]->eta() - jets[1]->eta());
      std::vector<PFJet*> nonvbf_jets = jets;
      std::vector<PFJet *> vbf_jets;
      vbf_jets.push_back(jets[0]);
      vbf_jets.push_back(jets[1]);
      double eta_high = (vbf_jets[0]->eta() > vbf_jets[1]->eta()) ? vbf_jets[0]->eta() : vbf_jets[1]->eta();
      double eta_low = (vbf_jets[0]->eta() > vbf_jets[1]->eta()) ? vbf_jets[1]->eta() : vbf_jets[0]->eta();
      ic::erase_if(nonvbf_jets, boost::bind(FoundIdInCollection<PFJet, PFJet>, _1, vbf_jets));
      std::vector<PFJet*> cjv_jets;
      for (unsigned i = 0; i < nonvbf_jets.size(); ++i) {
        if (nonvbf_jets[i]->pt() > 30.0 &&  
            nonvbf_jets[i]->eta() > eta_low && 
            nonvbf_jets[i]->eta() < eta_high) cjv_jets.push_back(nonvbf_jets[i]);
      }
      lNJetInGap = cjv_jets.size();




      //
      // Compute MVA inputs
      //
      float mjj = lMJJ;
      float dEta = lJDEta;
      float dPhi = fabs(ROOT::Math::VectorUtil::DeltaPhi(jets[0]->vector(), jets[1]->vector()));

      // Di-tau transverse vector
      TVector3 vTau, vMu, vMET, vDiTau, vDiTauVis;
      vTau.SetPtEtaPhi(tau->pt(), tau->eta(), tau->phi());
      vMu.SetPtEtaPhi(lepton->pt(), lepton->eta(), lepton->phi());
      vMET.SetPtEtaPhi(selectedMet->pt(), 0, selectedMet->phi());
      vDiTau = vTau + vMu + vMET;
      vDiTauVis = vTau + vMu;

      // Di-jet transverse vector
      TVector3 vJet1, vJet2, vDiJet;
      vJet1.SetPtEtaPhi(vbf_jets[0]->pt(), vbf_jets[0]->eta(), vbf_jets[0]->phi());
      vJet2.SetPtEtaPhi(vbf_jets[1]->pt(), vbf_jets[1]->eta(), vbf_jets[1]->phi());
      vDiJet = vJet1 + vJet2;
      //float dijet_pt = vDiJet.Pt();
      //float dijet_phi = vDiJet.Phi();
      float dPhi_hj = fabs(ROOT::Math::VectorUtil::DeltaPhi(vDiTau, vDiJet));

      // Lorenzo's variables
      double C1 = std::min(fabs(vDiTauVis.Eta() - vbf_jets[0]->eta()), fabs(vDiTauVis.Eta() - vbf_jets[1]->eta()));
      double C2 = vDiTauVis.Pt();

      // Fill input vector
      vbfvars[0] = mjj;
      vbfvars[1] = dEta;
      vbfvars[2] = dPhi;
      vbfvars[3] = vDiTau.Pt();
      vbfvars[4] = vDiJet.Pt();
      vbfvars[5] = dPhi_hj;
      vbfvars[6] = C1;
      vbfvars[7] = C2;

      // Evaluate MVA
      float mvaValue = reader->EvaluateMVA(vbfvars, "BDTG");

      lMVA = mvaValue;
      lJDPhi = dPhi;
      lDiJetPt = vDiJet.Pt();
      lDiJetPhi = vDiJet.Phi();
      lHDJetPhi = dPhi_hj;
      lVisJetEta = C1;
      lPtVis =C2;
    } else {
      lMJJ = -999.;
      lJDEta = -999.;
      lNJetInGap = 0;
      lMVA = -999.;
      lJDPhi = -999.;
      lDiJetPt = -999.;
      lDiJetPhi = -999.;
      lHDJetPhi = -999.;
      lVisJetEta = -999.;
      lPtVis = -999.;

    }
    
    //Variables that go o the VBF MVA
    
    //number of btags passing btag id ( pt > 20 )
    lNBTag = btag_jets.size();

    //number of jets passing jet id ( pt > 30 )
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 20, jet_eta_));
    lNJetsPt20 = jets.size();
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30, jet_eta_));
    lNJets = jets.size();
    lOTree->Fill();
    return 0;
  }
  int HttSync::PostAnalysis() {
    lOFile->cd();
    lOTree->Write();
    lOFile->Close();
    return 0;
  }

  void HttSync::PrintInfo() {
    ;
  }
}
