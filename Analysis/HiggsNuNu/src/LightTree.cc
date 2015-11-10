#include "UserCode/ICHiggsTauTau/Analysis/HiggsNuNu/interface/LightTree.h"
#include "UserCode/ICHiggsTauTau/interface/PFJet.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPairs.h"
#include "UserCode/ICHiggsTauTau/interface/TriggerPath.hh"
#include "UserCode/ICHiggsTauTau/interface/TriggerObject.hh"
#include "UserCode/ICHiggsTauTau/interface/EventInfo.hh"
#include "UserCode/ICHiggsTauTau/interface/Vertex.hh"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"
#include "UserCode/ICHiggsTauTau/interface/city.h"
#include "TVector3.h"


namespace ic {

  bool ptorderedgenjetsort(GenJet * i,GenJet * j){return (i->pt())>(j->pt());}
  bool ptorderedjetsort(PFJet * i,PFJet * j){return (i->pt())>(j->pt());}
  bool ptorderedcandidatesort(Candidate * i,Candidate * j){return (i->pt())>(j->pt());}

  LightTree::LightTree(std::string const& name): ModuleBase(name){
    fs_ = NULL;
    met_label_ = "pfMetType1";
    jet_label_ = "pfJetsPFlow";
    dijet_label_ = "jjCandidates";
    sel_label_ = "JetPair";
    is_data_ = false;
    dotrigskim_ = false;
    do_promptskim_ = false;
    do_noskim_ = false;
    is_embedded_ = false;
    ignoreLeptons_=false;
    trig_obj_label_ = "triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu140";
    cont_trig_obj_label_ = "triggerObjectsDiPFJet40DEta3p5MJJ600PFMETNoMu80";
    trigger_path_ = "HLT_DiPFJet40_PFMETnoMu65_MJJ800VBF_AllJets_v";

    outputTree_ = 0;

    run_=-1;
    lumi_=-1;
    event_=-1;
    weight_nolep_=1;
    total_weight_lepveto_ = 1;
    total_weight_leptight_ = 1;
    puweight_up_scale_=1;
    puweight_down_scale_=1;
    topweight_up_scale_=1;
    topweight_down_scale_=1;

    for (unsigned ijet(0); ijet<nJetsSave_;++ijet){
      jet_pt_.push_back(-1);
      jet_eta_.push_back(-5);
      jet_phi_.push_back(-5);
      jet_E_.push_back(-1);
      jet_csv_.push_back(-1);
      jet_genjet_mindR_.push_back(-1);
      jet_genMatched_.push_back(0);
      jet_jetid_.push_back(-10);
      jet_puid_.push_back(-10);
      genjet_pt_.push_back(-1);
      genjet_eta_.push_back(-5);
      genjet_phi_.push_back(-5);
      genjet_E_.push_back(-5);
      jet_trigjet_mindR_.push_back(-1);
      jet_trigMatched_.push_back(0);
      trigjet_pt_.push_back(-1);
      trigjet_eta_.push_back(-5);
      trigjet_phi_.push_back(-5);
      trigjet_E_.push_back(-5);

    }
    sigtrigcalomet_=-1;
    sigtrigpfmet_=-1;
    conttrigcalomet_=-1;
    conttrigpfmet_=-1;

    dijet_M_ = 0;
    dijet_deta_ = 0;
    dijet_sumeta_ = 0;
    dijet_dphi_ = 0;
    met_ = 0;
    met_x_ = 0;
    met_y_ = 0;
    metnomu_x_ = 0;
    metnomu_y_ = 0;
    met_significance_ = 0;
    metnomu_significance_ = 0;
    sumet_ = 0;
    ht_ = 0;
    ht30_ = 0;
    mht_ = 0;
    sqrt_ht_ = 0;
    unclustered_et_ = 0;
    jet1met_dphi_ = 0;
    jet2met_dphi_ = 0;
    jet1metnomu_dphi_ = 0;
    jet2metnomu_dphi_ = 0;
    jetmet_mindphi_ = 0;
    jetmetnomu_mindphi_ = 0;
    alljetsmet_mindphi_ = 0;
    alljetsmetnomu_mindphi_ = 0;
    jetunclet_mindphi_ = 0;
    metunclet_dphi_ = 0;
    metnomuunclet_dphi_ = 0;
    dijetmet_scalarSum_pt_ = 0;
    dijetmet_vectorialSum_pt_ = 0;
    dijetmet_ptfraction_ = 0;
    jet1met_scalarprod_ = 0;
    jet2met_scalarprod_ = 0;
    dijetmetnomu_scalarSum_pt_ = 0;
    dijetmetnomu_vectorialSum_pt_ = 0;
    dijetmetnomu_ptfraction_ = 0;
    jet1metnomu_scalarprod_ = 0;
    jet2metnomu_scalarprod_ = 0;
    n_jets_cjv_30_ = 0;
    n_jets_cjv_20EB_30EE_ = 0;
    n_jets_15_ = 0;
    n_jets_30_ = 0;
    cjvjetpt_=-1;
    pass_sigtrigger_ = -1;
    pass_mettrigger_ = -1;
    pass_controltrigger_ = -1;
    pass_singlejettrigger_ = -1;
    pass_htquadjettrigger_ = -1;
    l1met_ = 0;
    metnomuons_ =0;
    nvetomuons_=0;
    nselmuons_=0;
    nvetoelectrons_=0;
    nselelectrons_=0;
    ntaus_=0;
    m_mumu_=-1;
    m_ee_=-1;
    m_mumu_gen_=-1;
    m_ee_gen_=-1;
    genlep1_pt_=-1;
    genlep1_eta_=-10000;
    genlep1_phi_=-1;
    genlep1_id_=-1;
    genlep2_pt_=-1;
    genlep2_eta_=-10000;
    genlep2_phi_=-1;
    genlep2_id_=-1;
    mu1_pt_=-1;
    mu1_eta_=-10000;
    mu1_phi_=-1;
    mu2_pt_=-1;
    mu2_eta_=-10000;
    mu2_phi_=-1;
    ele1_pt_=-1;
    ele1_eta_=-10000;
    ele1_phi_=-1;
    tau1_pt_=-1;
    tau1_eta_=-10000;
    tau1_phi_=-1;
    lep_mt_=-1;
    gamma1_pt_=-1;
    gamma1_eta_=-10000;
    gamma1_phi_=-1;
    nloosephotons_=0;
    nmediumphotons_=0;
    ntightphotons_=0;
    n_vertices_=-1;
    for (unsigned ijet(0); ijet<nGenJetsSave_;++ijet){
      genjetptordered_pt_.push_back(-1);
      genjetptordered_eta_.push_back(-5);
      genjetptordered_phi_.push_back(-5);
      genjetptordered_E_.push_back(-5);
    }
    for (unsigned ijet(0); ijet<nTrigJetsSave_;++ijet){
      trigjetptordered_pt_.push_back(-1);
      trigjetptordered_eta_.push_back(-5);
      trigjetptordered_phi_.push_back(-5);
      trigjetptordered_E_.push_back(-5);
    }
    digenjet_M_=-1;
    digenjet_deta_=-1;
    digenjet_dphi_=-1;
    ditrigjet_M_=-1;
    ditrigjet_deta_=-1;
    ditrigjet_dphi_=-1;
  }

  LightTree::~LightTree(){
    ;
  }


  int  LightTree::PreAnalysis(){
    std::cout << "--------------------------------------------- " << std::endl
	      << "------ PreAnalysis Info for LightTree ------" << std::endl
	      << "--------------------------------------------- " << std::endl;
    if (fs_) {
      std::cout << "MET Label: " << met_label_ << std::endl;
      std::cout << "dijet Label: " << dijet_label_ << std::endl;
      std::cout << "Selection Label: " << sel_label_ << std::endl;
      if (is_embedded_ && is_data_) std::cout << "Processing set for embedded data !" << std::endl;
      else if (is_data_) std::cout << "Processing set for data !" << std::endl;
      else if (is_embedded_) std::cout << "Processing set for embedded MC !" << std::endl;
      else  std::cout << "Processing set for MC !" << std::endl;
    }
    outputTree_=fs_->make<TTree>("LightTree","Tree containing LightTreeAna input variables");//    outputTree_ = new TTree("LightTree","Tree containing LightTreeAna input variables"); 
    outputTree_->Branch("run",&run_);
    outputTree_->Branch("lumi",&lumi_);
    outputTree_->Branch("event",&event_);
    outputTree_->Branch("weight_nolep",&weight_nolep_);
    outputTree_->Branch("total_weight_lepveto",&total_weight_lepveto_);
    outputTree_->Branch("total_weight_leptight",&total_weight_leptight_);
    outputTree_->Branch("puweight_up_scale",&puweight_up_scale_);
    outputTree_->Branch("puweight_down_scale",&puweight_down_scale_);
    outputTree_->Branch("topweight_up_scale",&topweight_up_scale_);
    outputTree_->Branch("topweight_down_scale",&topweight_down_scale_);

    for (unsigned ijet(0); ijet<nJetsSave_;++ijet){
      std::ostringstream label;
      label << "jet" << ijet+1;
      outputTree_->Branch((label.str()+"_pt").c_str(),&jet_pt_[ijet]);
      outputTree_->Branch((label.str()+"_eta").c_str(),&jet_eta_[ijet]);
      outputTree_->Branch((label.str()+"_phi").c_str(),&jet_phi_[ijet]);
      outputTree_->Branch((label.str()+"_E").c_str(),&jet_E_[ijet]);
      outputTree_->Branch((label.str()+"_csv").c_str(),&jet_csv_[ijet]);
      outputTree_->Branch((label.str()+"_jetid").c_str(),&jet_jetid_[ijet]);
      outputTree_->Branch((label.str()+"_puid").c_str(),&jet_puid_[ijet]);
      outputTree_->Branch((label.str()+"_genjet_mindR").c_str(),&jet_genjet_mindR_[ijet]);
      outputTree_->Branch((label.str()+"_genMatched").c_str(),&jet_genMatched_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_pt").c_str(),&genjet_pt_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_eta").c_str(),&genjet_eta_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_phi").c_str(),&genjet_phi_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_E").c_str(),&genjet_E_[ijet]);
      outputTree_->Branch((label.str()+"_trigjet_mindR").c_str(),&jet_trigjet_mindR_[ijet]);
      outputTree_->Branch((label.str()+"_trigMatched").c_str(),&jet_trigMatched_[ijet]);
      outputTree_->Branch(("trig"+label.str()+"_pt").c_str(),&trigjet_pt_[ijet]);
      outputTree_->Branch(("trig"+label.str()+"_eta").c_str(),&trigjet_eta_[ijet]);
      outputTree_->Branch(("trig"+label.str()+"_phi").c_str(),&trigjet_phi_[ijet]);
      outputTree_->Branch(("trig"+label.str()+"_E").c_str(),&trigjet_E_[ijet]);
    }
    outputTree_->Branch("sigtrigcalomet",&sigtrigcalomet_);
    outputTree_->Branch("sigtrigpfmet",&sigtrigpfmet_);
    outputTree_->Branch("conttrigcalomet",&conttrigcalomet_);
    outputTree_->Branch("conttrigpfmet",&conttrigpfmet_);
    outputTree_->Branch("forward_tag_eta",&forward_tag_eta_);
    outputTree_->Branch("central_tag_eta",&central_tag_eta_);
    outputTree_->Branch("dijet_M",&dijet_M_);
    outputTree_->Branch("dijet_deta",&dijet_deta_);
    outputTree_->Branch("dijet_sumeta",&dijet_sumeta_);
    outputTree_->Branch("dijet_dphi",&dijet_dphi_);
    outputTree_->Branch("met",&met_);
    outputTree_->Branch("met_x",&met_x_);
    outputTree_->Branch("met_y",&met_y_);
    outputTree_->Branch("metnomu_x",&metnomu_x_);
    outputTree_->Branch("metnomu_y",&metnomu_y_);
    outputTree_->Branch("met_significance",&met_significance_);
    outputTree_->Branch("metnomu_significance",&metnomu_significance_);
    outputTree_->Branch("sumet",&sumet_);
    outputTree_->Branch("ht",&ht_);
    outputTree_->Branch("ht30",&ht30_);
    outputTree_->Branch("mht",&mht_);
    outputTree_->Branch("sqrt_ht",&sqrt_ht_);
    outputTree_->Branch("unclustered_et",&unclustered_et_);
    outputTree_->Branch("jet1met_dphi",&jet1met_dphi_);
    outputTree_->Branch("jet2met_dphi",&jet2met_dphi_);
    outputTree_->Branch("jet1metnomu_dphi",&jet1metnomu_dphi_);
    outputTree_->Branch("jet2metnomu_dphi",&jet2metnomu_dphi_);
    outputTree_->Branch("jetmet_mindphi",&jetmet_mindphi_);
    outputTree_->Branch("jetmetnomu_mindphi",&jetmetnomu_mindphi_);
    outputTree_->Branch("alljetsmet_mindphi",&alljetsmet_mindphi_);
    outputTree_->Branch("alljetsmetnomu_mindphi",&alljetsmetnomu_mindphi_);
    outputTree_->Branch("jetunclet_mindphi",&jetunclet_mindphi_);
    outputTree_->Branch("metunclet_dphi",&metunclet_dphi_);
    outputTree_->Branch("metnomuunclet_dphi",&metnomuunclet_dphi_);
    outputTree_->Branch("dijetmet_scalarSum_pt",&dijetmet_scalarSum_pt_);
    outputTree_->Branch("dijetmet_vectorialSum_pt",&dijetmet_vectorialSum_pt_);
    outputTree_->Branch("dijetmet_ptfraction",&dijetmet_ptfraction_);
    outputTree_->Branch("jet1met_scalarprod",&jet1met_scalarprod_);
    outputTree_->Branch("jet2met_scalarprod",&jet2met_scalarprod_);
    outputTree_->Branch("dijetmetnomu_scalarSum_pt",&dijetmetnomu_scalarSum_pt_);
    outputTree_->Branch("dijetmetnomu_vectorialSum_pt",&dijetmetnomu_vectorialSum_pt_);
    outputTree_->Branch("dijetmetnomu_ptfraction",&dijetmetnomu_ptfraction_);
    outputTree_->Branch("jet1metnomu_scalarprod",&jet1metnomu_scalarprod_);
    outputTree_->Branch("jet2metnomu_scalarprod",&jet2metnomu_scalarprod_);
    outputTree_->Branch("n_jets_cjv_30",&n_jets_cjv_30_);
    outputTree_->Branch("n_jets_cjv_20EB_30EE",&n_jets_cjv_20EB_30EE_);
    outputTree_->Branch("n_jets_15",&n_jets_15_);
    outputTree_->Branch("n_jets_30",&n_jets_30_);
    outputTree_->Branch("cjvjetpt",&cjvjetpt_);
    outputTree_->Branch("pass_sigtrigger",&pass_sigtrigger_);
    outputTree_->Branch("pass_mettrigger",&pass_mettrigger_);
    outputTree_->Branch("pass_controltrigger",&pass_controltrigger_);
    outputTree_->Branch("pass_singlejettrigger",&pass_singlejettrigger_);
    outputTree_->Branch("pass_htquadjettrigger",&pass_htquadjettrigger_);
    outputTree_->Branch("l1met",&l1met_);
    outputTree_->Branch("metnomuons",&metnomuons_);
    outputTree_->Branch("nvetomuons",&nvetomuons_);
    outputTree_->Branch("nselmuons",&nselmuons_);
    outputTree_->Branch("nvetoelectrons",&nvetoelectrons_);
    outputTree_->Branch("nselelectrons",&nselelectrons_);
    outputTree_->Branch("ntaus",&ntaus_);
    outputTree_->Branch("m_mumu",&m_mumu_);
    outputTree_->Branch("m_ee",&m_ee_);
    outputTree_->Branch("m_mumu_gen",&m_mumu_gen_);
    outputTree_->Branch("m_ee_gen",&m_ee_gen_);
    outputTree_->Branch("genlep1_pt",&genlep1_pt_);
    outputTree_->Branch("genlep1_eta",&genlep1_eta_);
    outputTree_->Branch("genlep1_phi",&genlep1_phi_);
    outputTree_->Branch("genlep1_id",&genlep1_id_);
    outputTree_->Branch("genlep2_pt",&genlep2_pt_);
    outputTree_->Branch("genlep2_eta",&genlep2_eta_);
    outputTree_->Branch("genlep2_phi",&genlep2_phi_);
    outputTree_->Branch("genlep2_id",&genlep2_id_);
    outputTree_->Branch("mu1_pt",&mu1_pt_);
    outputTree_->Branch("mu1_eta",&mu1_eta_);
    outputTree_->Branch("mu1_phi",&mu1_phi_);
    outputTree_->Branch("mu2_pt",&mu2_pt_);
    outputTree_->Branch("mu2_eta",&mu2_eta_);
    outputTree_->Branch("mu2_phi",&mu2_phi_);
    outputTree_->Branch("ele1_pt",&ele1_pt_);
    outputTree_->Branch("ele1_eta",&ele1_eta_);
    outputTree_->Branch("ele1_phi",&ele1_phi_);
    outputTree_->Branch("tau1_pt",&tau1_pt_);
    outputTree_->Branch("tau1_eta",&tau1_eta_);
    outputTree_->Branch("tau1_phi",&tau1_phi_);
    outputTree_->Branch("lep_mt",&lep_mt_);
    outputTree_->Branch("gamma1_pt",&gamma1_pt_);
    outputTree_->Branch("gamma1_eta",&gamma1_eta_);
    outputTree_->Branch("gamma1_phi",&gamma1_phi_);
    outputTree_->Branch("nloosephotons",&nloosephotons_);
    outputTree_->Branch("nmediumphotons",&nmediumphotons_);
    outputTree_->Branch("ntightphotons",&ntightphotons_);
    outputTree_->Branch("n_vertices",&n_vertices_);

    for (unsigned ijet(0); ijet<nGenJetsSave_;++ijet){
      std::ostringstream label;
      label << "jetptordered" << ijet+1;
      outputTree_->Branch(("gen"+label.str()+"_pt").c_str(),&genjet_pt_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_eta").c_str(),&genjet_eta_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_phi").c_str(),&genjet_phi_[ijet]);
      outputTree_->Branch(("gen"+label.str()+"_E").c_str(),&genjet_E_[ijet]);
    }
    outputTree_->Branch("digenjet_M",&digenjet_M_);
    outputTree_->Branch("digenjet_deta",&digenjet_deta_);
    outputTree_->Branch("digenjet_dphi",&digenjet_dphi_);

    outputTree_->Branch("ditrigjet_M",&ditrigjet_M_);
    outputTree_->Branch("ditrigjet_deta",&ditrigjet_deta_);
    outputTree_->Branch("ditrigjet_dphi",&ditrigjet_dphi_);



    return 0;
  }

  int  LightTree::Execute(TreeEvent *event){
    //Reset all information
    run_=-1;
    lumi_=-1;
    event_=-1;
    weight_nolep_=1;
    total_weight_lepveto_ = 1;
    total_weight_leptight_ = 1;
    puweight_up_scale_=1;
    puweight_down_scale_=1;
    topweight_up_scale_=1;
    topweight_down_scale_=1;

    jet_pt_.clear();
    jet_eta_.clear();
    jet_phi_.clear();
    jet_E_.clear();
    jet_csv_.clear();
    jet_genjet_mindR_.clear();
    jet_genMatched_.clear();
    jet_jetid_.clear();
    jet_puid_.clear();
    genjet_pt_.clear();
    genjet_eta_.clear();
    genjet_phi_.clear();
    genjet_E_.clear();
    jet_trigjet_mindR_.clear();
    jet_trigMatched_.clear();
    trigjet_pt_.clear();
    trigjet_eta_.clear();
    trigjet_phi_.clear();
    trigjet_E_.clear();

    for (unsigned ijet(0); ijet<nJetsSave_;++ijet){
      jet_pt_.push_back(-1);
      jet_eta_.push_back(-5);
      jet_phi_.push_back(-5);
      jet_E_.push_back(-1);
      jet_csv_.push_back(-1);
      jet_genjet_mindR_.push_back(-1);
      jet_genMatched_.push_back(0);
      jet_jetid_.push_back(-10);
      jet_puid_.push_back(-10);
      genjet_pt_.push_back(-1);
      genjet_eta_.push_back(-5);
      genjet_phi_.push_back(-5);
      genjet_E_.push_back(-5);
      jet_trigjet_mindR_.push_back(-1);
      jet_trigMatched_.push_back(0);
      trigjet_pt_.push_back(-1);
      trigjet_eta_.push_back(-5);
      trigjet_phi_.push_back(-5);
      trigjet_E_.push_back(-5);
    }
    sigtrigcalomet_=-1;
    sigtrigpfmet_=-1;
    conttrigcalomet_=-1;
    conttrigpfmet_=-1;

    dijet_M_ = 0;
    dijet_deta_ = 0;
    dijet_sumeta_ = 0;
    dijet_dphi_ = 0;
    met_ = 0;
    met_x_ = 0;
    met_y_ = 0;
    metnomu_x_ = 0;
    metnomu_y_ = 0;
    met_significance_ = 0;
    metnomu_significance_ = 0;
    sumet_ = 0;
    ht_ = 0;
    ht30_ = 0;
    mht_ = 0;
    sqrt_ht_ = 0;
    unclustered_et_ = 0;
    jet1met_dphi_ = 0;
    jet2met_dphi_ = 0;
    jet1metnomu_dphi_ = 0;
    jet2metnomu_dphi_ = 0;
    jetmet_mindphi_ = 0;
    jetmetnomu_mindphi_ = 0;
    alljetsmet_mindphi_ = 0;
    alljetsmetnomu_mindphi_ = 0;
    jetunclet_mindphi_ = 0;
    metunclet_dphi_ = 0;
    metnomuunclet_dphi_ = 0;
    dijetmet_scalarSum_pt_ = 0;
    dijetmet_vectorialSum_pt_ = 0;
    dijetmet_ptfraction_ = 0;
    jet1met_scalarprod_ = 0;
    jet2met_scalarprod_ = 0;
    dijetmetnomu_scalarSum_pt_ = 0;
    dijetmetnomu_vectorialSum_pt_ = 0;
    dijetmetnomu_ptfraction_ = 0;
    jet1metnomu_scalarprod_ = 0;
    jet2metnomu_scalarprod_ = 0;
    n_jets_cjv_30_ = 0;
    n_jets_cjv_20EB_30EE_ = 0;
    n_jets_15_ = 0;
    n_jets_30_ = 0;
    cjvjetpt_=-1;
    pass_sigtrigger_ = -1;
    pass_mettrigger_ = -1;
    pass_controltrigger_ = -1;
    pass_singlejettrigger_ = -1;
    pass_htquadjettrigger_ = -1;
    l1met_ = 0;
    metnomuons_ =0;
    nvetomuons_=0;
    nselmuons_=0;
    nvetoelectrons_=0;
    nselelectrons_=0;
    ntaus_=0;
    m_mumu_=-1;
    m_ee_=-1;
    m_mumu_gen_=-1;
    m_ee_gen_=-1;
    genlep1_pt_=-1;
    genlep1_eta_=-10000;
    genlep1_phi_=-1;
    genlep1_id_=-1;
    genlep2_pt_=-1;
    genlep2_eta_=-10000;
    genlep2_phi_=-1;
    genlep2_id_=-1;
    mu1_pt_=-1;
    mu1_eta_=-10000;
    mu1_phi_=-1;
    mu2_pt_=-1;
    mu2_eta_=-10000;
    mu2_phi_=-1;
    ele1_pt_=-1;
    ele1_eta_=-10000;
    ele1_phi_=-1;
    tau1_pt_=-1;
    tau1_eta_=-10000;
    tau1_phi_=-1;
    lep_mt_=-1;
    gamma1_pt_=-1;
    gamma1_eta_=-10000;
    gamma1_phi_=-1;
    nloosephotons_=0;
    nmediumphotons_=0;
    ntightphotons_=0;
    n_vertices_=-1;

    genjetptordered_pt_.clear();
    genjetptordered_eta_.clear();
    genjetptordered_phi_.clear();
    genjetptordered_E_.clear();
    for (unsigned ijet(0); ijet<nGenJetsSave_;++ijet){
      genjetptordered_pt_.push_back(-1);
      genjetptordered_eta_.push_back(-5);
      genjetptordered_phi_.push_back(-5);
      genjetptordered_E_.push_back(-5);
    }
    digenjet_M_=-1;
    digenjet_deta_=-1;
    digenjet_dphi_=-1;
    trigjetptordered_pt_.clear();
    trigjetptordered_eta_.clear();
    trigjetptordered_phi_.clear();
    trigjetptordered_E_.clear();
    for (unsigned ijet(0); ijet<nTrigJetsSave_;++ijet){
      trigjetptordered_pt_.push_back(-1);
      trigjetptordered_eta_.push_back(-5);
      trigjetptordered_phi_.push_back(-5);
      trigjetptordered_E_.push_back(-5);
    }
    ditrigjet_M_=-1;
    ditrigjet_deta_=-1;
    ditrigjet_dphi_=-1;


    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    run_= eventInfo->run();
    lumi_= eventInfo->lumi_block();
    event_= eventInfo->event();
    n_vertices_=eventInfo->good_vertices();

    //if (is_data_) {
	
    auto const& triggerPathPtrVec =
      event->GetPtrVec<TriggerPath>("triggerPathPtrVec","triggerPaths");
    //EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo"); //Can be used in future, but commented out to remove compiler warnings      
    //unsigned run = eventInfo->run(); //Can be used in future, but commented out to remove compiler warnings                                         
    pass_sigtrigger_=-1;
    pass_mettrigger_=-1;
    pass_controltrigger_=-1;
    for (unsigned i = 0; i < triggerPathPtrVec.size(); ++i) {
      std::string name = triggerPathPtrVec[i]->name();
      triggerPathPtrVec[i]->prescale();
      if (name.find("HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu140_") != name.npos) pass_sigtrigger_ = 1;
      if (name.find("HLT_PFMET170_NoiseCleaned_v") != name.npos) pass_mettrigger_ = 1;
      if (name.find("HLT_DiPFJet40_DEta3p5_MJJ600_PFMETNoMu80_") != name.npos) pass_controltrigger_ = 1;
      if (name.find("HLT_DiPFJetAve40_") != name.npos) pass_singlejettrigger_ = 1;
      if (name.find("HLT_PFHT750_4JetPt50_") != name.npos) pass_htquadjettrigger_ = 1;
    }
    if(dotrigskim_){
      if(!(pass_sigtrigger_==1||pass_controltrigger_==1||pass_mettrigger_==1||pass_singlejettrigger_==1||pass_htquadjettrigger_==1)){
	return 1;
      }
    }
    
    double wt = eventInfo->total_weight();
    double vetowt=1;
    double tightwt=1;
    double pileupwt=1;
    double pileupwtup=1;
    double pileupwtdown=1;
    double topwt=1;
    double topwtup=1;
    double topwtdown=1;
    if(!is_data_){
      vetowt= eventInfo->weight("idisoVeto");
      tightwt = eventInfo->weight("idisoTight");
      pileupwt=eventInfo->weight("pileup");
      pileupwtup=eventInfo->weight("pileup_up");
      pileupwtdown=eventInfo->weight("pileup_down");
      topwt=eventInfo->weight("tquark_weight");
      topwtup=eventInfo->weight("tquark_weight_up");
      topwtdown=eventInfo->weight("tquark_weight_up");
    }
    puweight_up_scale_=pileupwtup/pileupwt;
    puweight_down_scale_=pileupwtdown/pileupwt;
    topweight_up_scale_=topwtup/topwt;
    topweight_down_scale_=topwtdown/topwt;

    weight_nolep_ = wt;
    total_weight_lepveto_ =wt*vetowt;
    if(total_weight_lepveto_!=total_weight_lepveto_)std::cout<<"NAN lepveto weight: "<<total_weight_lepveto_<<" "<<wt<<" "<<vetowt<<" "<<mu1_pt_<<" "<<mu1_eta_<<std::endl;//!!
    total_weight_leptight_=wt*tightwt;
    
    //get collections
    std::vector<CompositeCandidate *> const& dijet_vec = event->GetPtrVec<CompositeCandidate>(dijet_label_);
    //Met const* met = event->GetPtr<Met>(met_label_);
    Met *met = 0;
    try {
      std::vector<Met*> metCol = event->GetPtrVec<Met>(met_label_);
      met = metCol[0];
    } catch (...){
      //std::cout << " Met vec not found..." << std::endl;
      met = event->GetPtr<Met>(met_label_);
      if (!met) {
	std::cerr << " -- Found no MET " << met_label_ << " in event! Exiting..." << std::endl;
	exit(1);
      }
    }
    Met const* metnomuons = event->GetPtr<Met>("metNoMuons");
    std::vector<Candidate *> const& l1met = event->GetPtrVec<Candidate>("l1extraMET");
    std::vector<Candidate *> const& l1forwardjets = event->GetPtrVec<Candidate>("l1extraForwardJets");
    std::vector<Candidate *> const& l1centraljets = event->GetPtrVec<Candidate>("l1extraCentralJets");
    //!!if(l1met.size()!=1)std::cout<<"There seem to be "<<l1met.size()<<" l1mets!!"<<std::endl;
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jet_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::vector<Muon*> vetomuons=event->GetPtrVec<Muon>("vetoMuons");
    std::vector<Muon*> selmuons=event->GetPtrVec<Muon>("selMuons");
    std::vector<Electron*> vetoelectrons=event->GetPtrVec<Electron>("vetoElectrons");
    std::vector<Electron*> selelectrons=event->GetPtrVec<Electron>("selElectrons");
    std::vector<Tau*> taus=event->GetPtrVec<Tau>("taus");
    std::vector<Photon*> loosephotons=event->GetPtrVec<Photon>("loosePhotons");
    std::vector<Photon*> mediumphotons=event->GetPtrVec<Photon>("mediumPhotons");
    std::vector<Photon*> tightphotons=event->GetPtrVec<Photon>("tightPhotons");
    
    std::vector<TriggerObject*> triggerobjects=event->GetPtrVec<TriggerObject>(trig_obj_label_);
    std::vector<TriggerObject*> conttriggerobjects=event->GetPtrVec<TriggerObject>(cont_trig_obj_label_);

    std::vector<GenJet *> genvec;
    if(!is_data_)genvec= event->GetPtrVec<GenJet>("genJets");
    //std::vector<Vertex*> & vertices = event->GetPtrVec<Vertex>("vertices");

    //Get vector of all trigger jets
    std::vector<Candidate *> l1alljets = l1forwardjets;
    l1alljets.insert(l1alljets.end(),l1centraljets.begin(),l1centraljets.end());

    if(!ignoreLeptons_) nvetomuons_=vetomuons.size();
    else nvetomuons_=0;
    nselmuons_=selmuons.size();
    nvetoelectrons_=vetoelectrons.size();
    nselelectrons_=selelectrons.size();
    ntaus_=taus.size();

    nloosephotons_=loosephotons.size();
    nmediumphotons_=mediumphotons.size();
    ntightphotons_=tightphotons.size();

    std::sort(selmuons.begin(), selmuons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::sort(selelectrons.begin(), selelectrons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    std::sort(tightphotons.begin(), tightphotons.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));

    if(ntightphotons_>=1){
      gamma1_pt_=tightphotons[0]->pt();
      gamma1_eta_=tightphotons[0]->eta();
      gamma1_phi_=tightphotons[0]->phi();
    }

    //Get MT
    if(nselmuons_==1&&nselelectrons_==0&&ntaus_==0){//If 1 muon and no electrons use muon to make mt
      lep_mt_=sqrt(2*selmuons[0]->pt()*met->pt()*(1-cos(selmuons[0]->phi()-met->phi())));
    }
    else if(nselelectrons_==1&&nselmuons_==0&&ntaus_==0){//If 1 electron and no muons use electron to make mt
      lep_mt_=sqrt(2*selelectrons[0]->pt()*met->pt()*(1-cos(selelectrons[0]->phi()-met->phi())));      
    }
    else if(ntaus_==1&&nselelectrons_==0&&nselmuons_==0){//If 1 electron and no muons use electron to make mt
      lep_mt_=sqrt(2*taus[0]->pt()*met->pt()*(1-cos(taus[0]->phi()-met->phi())));      
    }
    if(nselmuons_>=1){
      mu1_pt_=selmuons[0]->pt();
      mu1_eta_=selmuons[0]->eta();
      mu1_phi_=selmuons[0]->phi();
      if(nselmuons_>=2){
	mu2_pt_=selmuons[1]->pt();
	mu2_eta_=selmuons[1]->eta();
	mu2_phi_=selmuons[1]->phi();
      }
    }
    if(nselelectrons_>=1){
      ele1_pt_=selelectrons[0]->pt();
      ele1_eta_=selelectrons[0]->eta();
      ele1_phi_=selelectrons[0]->phi();
    }

    if(ntaus_>=1){
      tau1_pt_=taus[0]->pt();
      tau1_eta_=taus[0]->eta();
      tau1_phi_=taus[0]->phi();
    }

    if(nselmuons_==2){
      m_mumu_=((selmuons.at(0)->vector())+(selmuons.at(1)->vector())).M();
    }

    if(nselelectrons_==2){
      m_ee_=((selelectrons.at(0)->vector())+(selelectrons.at(1)->vector())).M();
    }

    //Get gen z mass
    int ngenmuplus=0;
    int ngenmuminus=0;
    int ngeneplus=0;
    int ngeneminus=0;

    if(!is_data_){
      std::vector<GenParticle*> const& parts = event->GetPtrVec<GenParticle>("genParticles");
      GenParticle* muplus = 0;
      GenParticle* muminus = 0;
      GenParticle* eplus = 0;
      GenParticle* eminus = 0;

      for (unsigned i = 0; i < parts.size(); ++i) {
	if (parts[i]->status() != 3) continue;
	
	int id = parts[i]->pdgid();

	if (id == static_cast<int>(13)) {
	  muminus = parts[i];
	  ngenmuminus++;
	}
	else if (id == static_cast<int>(-13)) {
	  muplus = parts[i];
	  ngenmuplus++;
	}
	else if (id == static_cast<int>(11)) {
	  eminus = parts[i];
	  ngeneminus++;
	}
	else if (id == static_cast<int>(-11)) {
	  eplus = parts[i];
	  ngeneplus++;
	}

	if((abs(id)==11)||(abs(id)==13)||(abs(id)==15)){
	  if(parts[i]->pt()>genlep1_pt_){
	    genlep2_pt_=genlep1_pt_;
	    genlep2_eta_=genlep1_eta_;
	    genlep2_phi_=genlep1_phi_;
	    genlep2_id_=genlep1_id_;
	    genlep1_pt_=parts[i]->pt();
	    genlep1_eta_=parts[i]->eta();
	    genlep1_phi_=parts[i]->phi();
	    genlep1_id_=id;

	  }
	  else if(parts[i]->pt()>genlep2_pt_){
	    genlep2_pt_=parts[i]->pt();
	    genlep2_eta_=parts[i]->eta();
	    genlep2_phi_=parts[i]->phi();
	    genlep2_id_=id;
	  }
	}
      }//loop on genparticles                                                                                                                                  
      
      if (ngenmuminus==1&&ngenmuplus==1) {
	m_mumu_gen_ = (muplus->vector()+muminus->vector()).M();
      }
      if (ngeneminus==1&&ngeneplus==1) {
	m_ee_gen_ = (eplus->vector()+eminus->vector()).M();
      }
    }

    if (dijet_vec.size() != 0) {
      
      CompositeCandidate const* dijet = dijet_vec.at(0);

      Candidate const* jet1 = dijet->GetCandidate("jet1");
      Candidate const* jet2 = dijet->GetCandidate("jet2");
      ROOT::Math::PtEtaPhiEVector jet1vec = jet1->vector();
      ROOT::Math::PtEtaPhiEVector jet2vec = jet2->vector();
      ROOT::Math::PtEtaPhiEVector metvec = met->vector();
      ROOT::Math::PtEtaPhiEVector metnomuvec = metnomuons->vector();

      if(fabs(jet1->eta())>fabs(jet2->eta())){
	forward_tag_eta_=jet1->eta();
	central_tag_eta_=jet2->eta();
      }
      else{
	forward_tag_eta_=jet2->eta();
	central_tag_eta_=jet1->eta();
      }
      dijet_M_ = dijet->M();
      dijet_deta_ = fabs(jet1->eta() - jet2->eta());
      dijet_sumeta_ = jet1->eta() + jet2->eta();
      dijet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,jet2vec));

      //Find highest 4 pt gen jets
      if(!is_data_){
	std::vector<GenJet*> genveccopy;
	genveccopy=genvec;
	std::sort(genveccopy.begin(),genveccopy.end(),ptorderedgenjetsort);
	for(unsigned igenjet=0;(igenjet<genveccopy.size()&&igenjet<nGenJetsSave_);igenjet++){
	  genjetptordered_pt_[igenjet]=genveccopy[igenjet]->pt();
	  genjetptordered_eta_[igenjet]=genveccopy[igenjet]->eta();
	  genjetptordered_phi_[igenjet]=genveccopy[igenjet]->phi();
	  genjetptordered_E_[igenjet]=genveccopy[igenjet]->energy();
	}
	if(genveccopy.size()>=2){
	  ROOT::Math::PtEtaPhiEVector genjet1vec = genveccopy[0]->vector();
	  ROOT::Math::PtEtaPhiEVector genjet2vec = genveccopy[1]->vector();
	  ROOT::Math::PtEtaPhiEVector digenjetvec = genjet1vec+genjet2vec;
	  digenjet_M_=digenjetvec.M();
	  digenjet_deta_ = fabs(genveccopy[0]->eta() - genveccopy[1]->eta());
	  digenjet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(genjet1vec,genjet2vec));
	}
      }
      std::vector<Candidate*> trigveccopy;
      trigveccopy=l1alljets;
      std::sort(trigveccopy.begin(),trigveccopy.end(),ptorderedcandidatesort);
      for(unsigned itrigjet=0;(itrigjet<trigveccopy.size()&&itrigjet<nTrigJetsSave_);itrigjet++){
	trigjetptordered_pt_[itrigjet]=trigveccopy[itrigjet]->pt();
	trigjetptordered_eta_[itrigjet]=trigveccopy[itrigjet]->eta();
	trigjetptordered_phi_[itrigjet]=trigveccopy[itrigjet]->phi();
	trigjetptordered_E_[itrigjet]=trigveccopy[itrigjet]->energy();
      }
      if(trigveccopy.size()>=2){
	ROOT::Math::PtEtaPhiEVector trigjet1vec = trigveccopy[0]->vector();
	ROOT::Math::PtEtaPhiEVector trigjet2vec = trigveccopy[1]->vector();
	ROOT::Math::PtEtaPhiEVector ditrigjetvec = trigjet1vec+trigjet2vec;
	ditrigjet_M_=ditrigjetvec.M();
	ditrigjet_deta_ = fabs(trigveccopy[0]->eta() - trigveccopy[1]->eta());
	ditrigjet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(trigjet1vec,trigjet2vec));
      }

      met_ = met->pt();
      met_x_ = metvec.Px();
      met_y_ = metvec.Py();
      met_significance_ = met->et_sig();
      sumet_ = met->sum_et();
      //      if(l1met.size()==1){//!!
      l1met_ = l1met[0]->energy();
      //}
      metnomuons_ = metnomuons->pt();
      metnomu_x_ = metnomuvec.Px();
      metnomu_y_ = metnomuvec.Py();
      metnomu_significance_ = met_significance_/met_*metnomuons_;

      double ht =0;
      double ht30 =0;
      ROOT::Math::PtEtaPhiEVector mhtVec(0,0,0,0);
      for(unsigned i =0; i<jets.size();++i){
	ht+=jets[i]->vector().Et();
	if(jets[i]->pt()>30)	ht30+=jets[i]->vector().Et();
	mhtVec += jets[i]->vector();
      }
      ROOT::Math::PtEtaPhiEVector unclVec = mhtVec + metvec;

      ht_ = ht;
      ht30_=ht30;
      mht_ = mhtVec.Et();
      sqrt_ht_ = sqrt(ht);
      unclustered_et_ = unclVec.Et();

      double dphi1 = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,metvec));
      double dphi2 = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,metvec));
      double nomudphi1 = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,metnomuvec));
      double nomudphi2 = fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,metnomuvec));
      jet1met_dphi_ = dphi1;
      jet2met_dphi_ = dphi2;
      jet1metnomu_dphi_ = nomudphi1;
      jet2metnomu_dphi_ = nomudphi2;
      jetmet_mindphi_ = std::min(dphi1,dphi2);
      jetmetnomu_mindphi_ = std::min(nomudphi1,nomudphi2);
      

      dijetmet_scalarSum_pt_ = jet1->pt()+jet2->pt()+met->pt();
      dijetmet_vectorialSum_pt_ = (jet1vec+jet2vec+metvec).Pt();
      dijetmet_ptfraction_ = dijet->pt()/(dijet->pt()+met->pt());
      dijetmetnomu_scalarSum_pt_ = jet1->pt()+jet2->pt()+metnomuons->pt();
      dijetmetnomu_vectorialSum_pt_ = (jet1vec+jet2vec+metnomuvec).Pt();
      dijetmetnomu_ptfraction_ = dijet->pt()/(dijet->pt()+metnomuons->pt());

      jet1met_scalarprod_ = (jet1vec.Px()*met_x_+jet1vec.Py()*met_y_)/met_;
      jet2met_scalarprod_ = (jet2vec.Px()*met_x_+jet2vec.Py()*met_y_)/met_;

      jet1metnomu_scalarprod_ = (jet1vec.Px()*metnomu_x_+jet1vec.Py()*metnomu_y_)/met_;
      jet2metnomu_scalarprod_ = (jet2vec.Px()*metnomu_x_+jet2vec.Py()*metnomu_y_)/met_;

      jetunclet_mindphi_ = std::min(fabs(ROOT::Math::VectorUtil::DeltaPhi(jet1vec,unclVec)),
				    fabs(ROOT::Math::VectorUtil::DeltaPhi(jet2vec,unclVec)));
      metunclet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(unclVec,metvec));
      metnomuunclet_dphi_ = fabs(ROOT::Math::VectorUtil::DeltaPhi(unclVec,metnomuvec));

      double eta_high = (jet1->eta() > jet2->eta()) ? jet1->eta() : jet2->eta();
      double eta_low = (jet1->eta() > jet2->eta()) ? jet2->eta() : jet1->eta();
      alljetsmetnomu_mindphi_=jetmetnomu_mindphi_;
      alljetsmet_mindphi_=jetmet_mindphi_;


      std::vector<PFJet*> jetveccopy;
      jetveccopy=jets;
      std::sort(jetveccopy.begin(),jetveccopy.end(),ptorderedjetsort);
      for(unsigned ijet=0;ijet<jetveccopy.size();ijet++){
	if(ijet<nJetsSave_){
	  jet_pt_[ijet]=jetveccopy[ijet]->pt();
	  jet_eta_[ijet]=jetveccopy[ijet]->eta();
	  jet_phi_[ijet]=jetveccopy[ijet]->phi();
	  jet_E_[ijet]=jetveccopy[ijet]->energy();
	  jet_csv_[ijet]=jetveccopy[ijet]->GetBDiscriminator("combinedSecondaryVertexBJetTags");
	  jet_jetid_[ijet]=PFJetID2015(jetveccopy[ijet]);
	  jet_puid_[ijet]=PileupJetID(jetveccopy[ijet],2);

	  if(!is_data_){
	    //fill gen matched
	    double mindR = 1000;
	    int whichgenjet=-1;
	    for (unsigned iGenJet = 0; iGenJet < genvec.size(); ++iGenJet) {//loop on genjets
	      double dR = ROOT::Math::VectorUtil::DeltaR(genvec[iGenJet]->vector(),jetveccopy[ijet]->vector());
	      if (dR<mindR){
		mindR = dR;
		whichgenjet = iGenJet;
	      }
	    }//loop on genjets
	    jet_genjet_mindR_[ijet]=mindR;
	    if (mindR<0.4){
	      jet_genMatched_[ijet]=jets[ijet]->parton_flavour();
	      genjet_pt_[ijet]=genvec[whichgenjet]->pt();
	      genjet_eta_[ijet]=genvec[whichgenjet]->eta();
	      genjet_phi_[ijet]=genvec[whichgenjet]->phi();
	    }
	  }

	  //fill trig matched
	  // std::vector<std::string> filters;
	  // filters.push_back("hltMET90");
	  // filters.push_back("hltMETClean80");
	  // filters.push_back("hltMETCleanUsingJetID80");
	  // filters.push_back("hltPFMET170Filter");
	  // filters.push_back("hltDiCaloJet30MJJ500DEta3p0");
	  // filters.push_back("hltDiPFJet40MJJ600DEta3p5");
	  // //	  filters.push_back("hltL1sL1ETM60ORETM70");
	  // filters.push_back("hltMET80");
	  // filters.push_back("hltMETCleanUsingJetID80");
	  // filters.push_back("hltPFMETNoMu140");
	  // filters.push_back("hltDiCaloJet30MJJ500DEta3p0");
	  // filters.push_back("hltDiPFJet40MJJ600DEta3p5");
	  // filters.push_back("hltL1sL1ETM50");
	  // filters.push_back("hltMET50");
	  // filters.push_back("hltMETCleanUsingJetID50");
	  // filters.push_back("hltPFMETNoMu80");

	  // for(unsigned ifilter=0;ifilter<filters.size();ifilter++){
	  //   std::pair<bool,unsigned> trigmatch=IsFilterMatchedWithIndex(jetveccopy[ijet],triggerobjects,filters[ifilter],0.4);
	  //   if(trigmatch.first){
	  //     std::cout<<"found a trig match for"<<filters[ifilter]<<std::endl;
	  //   }
	  // }

	  //std::pair<bool,unsigned> trigmatch=IsFilterMatchedWithIndex(jetveccopy[ijet],triggerobjects,"hltDiPFJet40MJJ600DEta3p5",0.4);
	  std::pair<bool,unsigned> trigmatch=IsFilterMatchedWithIndex(jetveccopy[ijet],triggerobjects,"hltDiCaloJet30MJJ500DEta3p0",0.4);
	  if(trigmatch.first){
	    jet_trigMatched_[ijet]=1;
	    trigjet_pt_[ijet]=triggerobjects[trigmatch.second]->pt();
	    trigjet_eta_[ijet]=triggerobjects[trigmatch.second]->eta();
	    trigjet_phi_[ijet]=triggerobjects[trigmatch.second]->phi();
            jet_trigjet_mindR_[ijet]=ROOT::Math::VectorUtil::DeltaR(triggerobjects[trigmatch.second]->vector(),jetveccopy[ijet]->vector());
	  }

	  std::pair<bool,unsigned> sigcalomatch=IsFilterMatchedWithIndex(met,triggerobjects,"hltMET80",11);
	  std::pair<bool,unsigned> sigpfmatch=IsFilterMatchedWithIndex(metnomuons,triggerobjects,"hltPFMETNoMu140",11);
	  std::pair<bool,unsigned> contcalomatch=IsFilterMatchedWithIndex(met,conttriggerobjects,"hltMET50",11);
	  std::pair<bool,unsigned> contpfmatch=IsFilterMatchedWithIndex(metnomuons,conttriggerobjects,"hltPFMETNoMu80",11);
	  
	  if(sigcalomatch.first)sigtrigcalomet_=triggerobjects[sigcalomatch.second]->pt();
	  if(sigpfmatch.first)sigtrigpfmet_=triggerobjects[sigpfmatch.second]->pt();
	  if(contcalomatch.first)conttrigcalomet_=conttriggerobjects[contcalomatch.second]->pt();
	  if(contpfmatch.first)conttrigpfmet_=conttriggerobjects[contpfmatch.second]->pt();

	  // double trigmindR = 1000;
	  // int whichtrigjet=-1;
	  // for (unsigned iTrigJet = 0; iTrigJet < l1alljets.size(); ++iTrigJet) {//loop on trigjets
	  //   double dR = ROOT::Math::VectorUtil::DeltaR(l1alljets[iTrigJet]->vector(),jetveccopy[ijet]->vector());
	  //   if (dR<trigmindR){
	  //     trigmindR = dR;
	  //     whichtrigjet = iTrigJet;
	  //   }
	  // }//loop on trigjets

	  // //!!DO WRT HLT WITH TRIG OBJECTS ISFILTERMATCHED FNPREDICATES FUNCTION
	  // //triggerobjects collection
	  // jet_trigjet_mindR_[ijet]=trigmindR;
	  // if (trigmindR<0.4){
	  //   jet_trigMatched_[ijet]=1;
	  //   trigjet_pt_[ijet]=l1alljets[whichtrigjet]->pt();
	  //   trigjet_eta_[ijet]=l1alljets[whichtrigjet]->eta();
	  //   trigjet_phi_[ijet]=l1alljets[whichtrigjet]->phi();
	  // }


	}
	if(jetveccopy[ijet]->pt()>15)n_jets_15_++;
	if(jetveccopy[ijet]->pt()>30)n_jets_30_++;
      }

      if (jets.size() > 2) {
	for (unsigned i = 0; i < jets.size(); ++i) {
	  bool isInCentralGap = fabs(jets[i]->eta())<4.7 && jets[i]->eta() > eta_low && jets[i]->eta() < eta_high;
	  double tmppt=jets[i]->pt();
	  if(isInCentralGap&&(tmppt>cjvjetpt_)){
	    cjvjetpt_=tmppt;
	  }
	  if (jets[i]->pt() > 30.0 && isInCentralGap){
	    ++n_jets_cjv_30_;
	  }
	  if ( ((jets[i]->eta()<2.4 && jets[i]->pt() > 20.0) ||
		(jets[i]->eta()>=2.4 && jets[i]->pt() > 30.0)) && 
	       isInCentralGap){
	    ++n_jets_cjv_20EB_30EE_;
	  }
	  if(jets[i]->pt()>30.0){
	    double thisjetmetnomudphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(jets[i]->vector(),metnomuvec));
	    if(thisjetmetnomudphi<alljetsmetnomu_mindphi_)alljetsmetnomu_mindphi_=thisjetmetnomudphi;
	    double thisjetmetdphi = fabs(ROOT::Math::VectorUtil::DeltaPhi(jets[i]->vector(),metvec));
	    if(thisjetmetdphi<alljetsmet_mindphi_)alljetsmet_mindphi_=thisjetmetdphi;
	  }
	}
      }

      static unsigned processed = 0;
      //IF PASSES CUTS FILL TREE
      if(!ignoreLeptons_){
	if(!do_promptskim_){
	  if(!do_noskim_){
	    if (jet_pt_[1]>40&& dijet_M_ > 600 &&  dijet_deta_>3.6){
	      //if (dijet_M_>1000 &&  dijet_deta_>3.6 && metnomuons_>100 && jet1_pt_>50){//for prompt presel
	      outputTree_->Fill();
	      ++processed;
	    }
	  }
	  else{
	    outputTree_->Fill();
	    ++processed;
	  }
	}
	else{
	  if ((pass_sigtrigger_==1||pass_controltrigger_==1)&&dijet_deta_>3.6&&metnomuons_>90&&jet_pt_[0]>50){
	    //if (dijet_M_>1000 &&  dijet_deta_>3.6 && metnomuons_>100 && jet1_pt_>50){//for prompt presel
	    outputTree_->Fill();
	    ++processed;
	  }
	}
      }
      else{
	if(!do_promptskim_){
	  if (metnomu_significance_ > 3.0 &&  dijet_deta_>3.6 &&m_mumu_gen_>80&&m_mumu_gen_<100){
	    //if (dijet_M_>1000 &&  dijet_deta_>3.6 && metnomuons_>100 && jet1_pt_>50){//for prompt presel
	    outputTree_->Fill();
	    ++processed;
	  }
	}
	else{
	  if ((pass_sigtrigger_==1||pass_controltrigger_==1)&&dijet_deta_>3.6&&metnomuons_>90&&jet_pt_[0]>50){
	    //if (dijet_M_>1000 &&  dijet_deta_>3.6 && metnomuons_>100 && jet1_pt_>50){//for prompt presel
	    outputTree_->Fill();
	    ++processed;
	  }
	}
	
      }
      if (processed == 500) outputTree_->OptimizeBaskets();
    }

    return 0;
  }

  int  LightTree::PostAnalysis(){
    
    return 0;
  }

  void  LightTree::PrintInfo(){
    ;
  }

}//namespace

