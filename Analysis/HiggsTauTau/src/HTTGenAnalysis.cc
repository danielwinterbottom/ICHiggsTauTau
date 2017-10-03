#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/HTTGenAnalysis.h"
#include "UserCode/ICHiggsTauTau/Analysis/Utilities/interface/FnPredicates.h"

std::vector<ic::GenParticle> FamilyTree (std::vector<ic::GenParticle> &v, ic::GenParticle p, std::vector<ic::GenParticle*> gen_particles, unsigned &outputID){ 
  if(p.daughters().size() == 0){
    unsigned ID = std::fabs(p.pdgid());
    if(ID == 11) outputID = 11;
    else if(ID == 13) outputID = 13;
    if(!(ID==12 || ID==14 || ID==16)){
      v.push_back(p);
    }
  }
  else{
    for(size_t i = 0; i < p.daughters().size(); ++i ){
      ic::GenParticle d = *gen_particles[p.daughters().at(i)];
      FamilyTree(v,d, gen_particles, outputID);
    }
  }
  return v;
}

struct swap_labels{
  bool operator() (std::string a,std::string b) {
    if(a=="t" && b!="t") return false;
    else if(a=="m" &&  (b!="m" && b!="t")) return false;
    else return true;
  }
};

struct PtComparator{
  bool operator() (ic::Candidate a, ic::Candidate b) {
    return (a.vector().Pt() > b.vector().Pt());
  }
};
  
namespace ic {

  HTTGenAnalysis::HTTGenAnalysis(std::string const& name) : ModuleBase(name) {
    fs_ = NULL;
    bbtag_eff_ = nullptr;
  }

  HTTGenAnalysis::~HTTGenAnalysis() {
    ;
  }

  int HTTGenAnalysis::PreAnalysis() {
    rand = new TRandom3(0);
    if(fs_){  
      outtree_ = fs_->make<TTree>("gen_ntuple","gen_ntuple");
      outtree_->Branch("event"       , &event_       );
      outtree_->Branch("wt"       , &wt_       );
      if(do_theory_uncert_){
        outtree_->Branch("wt_mur1_muf1",    &scale1_);
        outtree_->Branch("wt_mur1_muf2",    &scale2_);
        outtree_->Branch("wt_mur1_muf0p5",  &scale3_);
        outtree_->Branch("wt_mur2_muf1",    &scale4_);
        outtree_->Branch("wt_mur2_muf2",    &scale5_);
        outtree_->Branch("wt_mur2_muf0p5",  &scale6_);
        outtree_->Branch("wt_mur0p5_muf1",  &scale7_);
        outtree_->Branch("wt_mur0p5_muf2",  &scale8_);
        outtree_->Branch("wt_mur0p5_muf0p5",&scale9_);
        outtree_->Branch("wt_pdf_1",&wt_pdf_1_);
        outtree_->Branch("wt_pdf_2",&wt_pdf_2_);
        outtree_->Branch("wt_pdf_3",&wt_pdf_3_);
        outtree_->Branch("wt_pdf_4",&wt_pdf_4_);
        outtree_->Branch("wt_pdf_5",&wt_pdf_5_);
        outtree_->Branch("wt_pdf_6",&wt_pdf_6_);
        outtree_->Branch("wt_pdf_7",&wt_pdf_7_);
        outtree_->Branch("wt_pdf_8",&wt_pdf_8_);
        outtree_->Branch("wt_pdf_9",&wt_pdf_9_);
        outtree_->Branch("wt_pdf_10",&wt_pdf_10_);
        outtree_->Branch("wt_pdf_11",&wt_pdf_11_);
        outtree_->Branch("wt_pdf_12",&wt_pdf_12_);
        outtree_->Branch("wt_pdf_13",&wt_pdf_13_);
        outtree_->Branch("wt_pdf_14",&wt_pdf_14_);
        outtree_->Branch("wt_pdf_15",&wt_pdf_15_);
        outtree_->Branch("wt_pdf_16",&wt_pdf_16_);
        outtree_->Branch("wt_pdf_17",&wt_pdf_17_);
        outtree_->Branch("wt_pdf_18",&wt_pdf_18_);
        outtree_->Branch("wt_pdf_19",&wt_pdf_19_);
        outtree_->Branch("wt_pdf_20",&wt_pdf_20_);
        outtree_->Branch("wt_pdf_21",&wt_pdf_21_);
        outtree_->Branch("wt_pdf_22",&wt_pdf_22_);
        outtree_->Branch("wt_pdf_23",&wt_pdf_23_);
        outtree_->Branch("wt_pdf_24",&wt_pdf_24_);
        outtree_->Branch("wt_pdf_25",&wt_pdf_25_);
        outtree_->Branch("wt_pdf_26",&wt_pdf_26_);
        outtree_->Branch("wt_pdf_27",&wt_pdf_27_);
        outtree_->Branch("wt_pdf_28",&wt_pdf_28_);
        outtree_->Branch("wt_pdf_29",&wt_pdf_29_);
        outtree_->Branch("wt_pdf_30",&wt_pdf_30_);
        outtree_->Branch("wt_pdf_31",&wt_pdf_31_);
        outtree_->Branch("wt_pdf_32",&wt_pdf_32_);
        outtree_->Branch("wt_pdf_33",&wt_pdf_33_);
        outtree_->Branch("wt_pdf_34",&wt_pdf_34_);
        outtree_->Branch("wt_pdf_35",&wt_pdf_35_);
        outtree_->Branch("wt_pdf_36",&wt_pdf_36_);
        outtree_->Branch("wt_pdf_37",&wt_pdf_37_);
        outtree_->Branch("wt_pdf_38",&wt_pdf_38_);
        outtree_->Branch("wt_pdf_39",&wt_pdf_39_);
        outtree_->Branch("wt_pdf_40",&wt_pdf_40_);
        outtree_->Branch("wt_pdf_41",&wt_pdf_41_);
        outtree_->Branch("wt_pdf_42",&wt_pdf_42_);
        outtree_->Branch("wt_pdf_43",&wt_pdf_43_);
        outtree_->Branch("wt_pdf_44",&wt_pdf_44_);
        outtree_->Branch("wt_pdf_45",&wt_pdf_45_);
        outtree_->Branch("wt_pdf_46",&wt_pdf_46_);
        outtree_->Branch("wt_pdf_47",&wt_pdf_47_);
        outtree_->Branch("wt_pdf_48",&wt_pdf_48_);
        outtree_->Branch("wt_pdf_49",&wt_pdf_49_);
        outtree_->Branch("wt_pdf_50",&wt_pdf_50_);
        outtree_->Branch("wt_pdf_51",&wt_pdf_51_);
        outtree_->Branch("wt_pdf_52",&wt_pdf_52_);
        outtree_->Branch("wt_pdf_53",&wt_pdf_53_);
        outtree_->Branch("wt_pdf_54",&wt_pdf_54_);
        outtree_->Branch("wt_pdf_55",&wt_pdf_55_);
        outtree_->Branch("wt_pdf_56",&wt_pdf_56_);
        outtree_->Branch("wt_pdf_57",&wt_pdf_57_);
        outtree_->Branch("wt_pdf_58",&wt_pdf_58_);
        outtree_->Branch("wt_pdf_59",&wt_pdf_59_);
        outtree_->Branch("wt_pdf_60",&wt_pdf_60_);
        outtree_->Branch("wt_pdf_61",&wt_pdf_61_);
        outtree_->Branch("wt_pdf_62",&wt_pdf_62_);
        outtree_->Branch("wt_pdf_63",&wt_pdf_63_);
        outtree_->Branch("wt_pdf_64",&wt_pdf_64_);
        outtree_->Branch("wt_pdf_65",&wt_pdf_65_);
        outtree_->Branch("wt_pdf_66",&wt_pdf_66_);
        outtree_->Branch("wt_pdf_67",&wt_pdf_67_);
        outtree_->Branch("wt_pdf_68",&wt_pdf_68_);
        outtree_->Branch("wt_pdf_69",&wt_pdf_69_);
        outtree_->Branch("wt_pdf_70",&wt_pdf_70_);
        outtree_->Branch("wt_pdf_71",&wt_pdf_71_);
        outtree_->Branch("wt_pdf_72",&wt_pdf_72_);
        outtree_->Branch("wt_pdf_73",&wt_pdf_73_);
        outtree_->Branch("wt_pdf_74",&wt_pdf_74_);
        outtree_->Branch("wt_pdf_75",&wt_pdf_75_);
        outtree_->Branch("wt_pdf_76",&wt_pdf_76_);
        outtree_->Branch("wt_pdf_77",&wt_pdf_77_);
        outtree_->Branch("wt_pdf_78",&wt_pdf_78_);
        outtree_->Branch("wt_pdf_79",&wt_pdf_79_);
        outtree_->Branch("wt_pdf_80",&wt_pdf_80_);
        outtree_->Branch("wt_pdf_81",&wt_pdf_81_);
        outtree_->Branch("wt_pdf_82",&wt_pdf_82_);
        outtree_->Branch("wt_pdf_83",&wt_pdf_83_);
        outtree_->Branch("wt_pdf_84",&wt_pdf_84_);
        outtree_->Branch("wt_pdf_85",&wt_pdf_85_);
        outtree_->Branch("wt_pdf_86",&wt_pdf_86_);
        outtree_->Branch("wt_pdf_87",&wt_pdf_87_);
        outtree_->Branch("wt_pdf_88",&wt_pdf_88_);
        outtree_->Branch("wt_pdf_89",&wt_pdf_89_);
        outtree_->Branch("wt_pdf_90",&wt_pdf_90_);
        outtree_->Branch("wt_pdf_91",&wt_pdf_91_);
        outtree_->Branch("wt_pdf_92",&wt_pdf_92_);
        outtree_->Branch("wt_pdf_93",&wt_pdf_93_);
        outtree_->Branch("wt_pdf_94",&wt_pdf_94_);
        outtree_->Branch("wt_pdf_95",&wt_pdf_95_);
        outtree_->Branch("wt_pdf_96",&wt_pdf_96_);
        outtree_->Branch("wt_pdf_97",&wt_pdf_97_);
        outtree_->Branch("wt_pdf_98",&wt_pdf_98_);
        outtree_->Branch("wt_pdf_99",&wt_pdf_99_);
        outtree_->Branch("wt_pdf_100",&wt_pdf_100_);
        
        outtree_->Branch("wt_alphasdown",&wt_alphasdown_);
        outtree_->Branch("wt_alphasup",&wt_alphasup_);
      }
      outtree_->Branch("passed"       ,&passed_       );
      outtree_->Branch("pt_1"        , &pt_1_        );
      outtree_->Branch("pt_2"        , &pt_2_        );
      outtree_->Branch("eta_1"       , &eta_1_       );
      outtree_->Branch("eta_2"       , &eta_2_       );
      outtree_->Branch("phi_1"       , &phi_1_       );
      outtree_->Branch("phi_2"       , &phi_2_       );
      outtree_->Branch("met"         , &met_         );
      outtree_->Branch("m_vis"       , &m_vis_       );
      outtree_->Branch("pt_tt"       , &pt_tt_       );
      outtree_->Branch("mt_1"        , &mt_1_        );
      outtree_->Branch("mt_2"        , &mt_2_        );
      outtree_->Branch("pzeta"       , &pzeta_       );
      outtree_->Branch("n_bjets"     , &n_bjets_     );
      outtree_->Branch("n_bjets_noscale"     , &n_bjets_noscale_);
      outtree_->Branch("n_jets"      , &n_jets_      );
      outtree_->Branch("n_jets_nofilter"      , &n_jets_nofilter_);
      outtree_->Branch("n_jetsingap" , &n_jetsingap_ );
      outtree_->Branch("jpt_1"       , &jpt_1_       );
      outtree_->Branch("jpt_2"       , &jpt_2_       );
      outtree_->Branch("jpt_3"       , &jpt_3_       );
      outtree_->Branch("jeta_1"      , &jeta_1_      );
      outtree_->Branch("jeta_2"      , &jeta_2_      );
      outtree_->Branch("jphi_1"      , &jphi_1_      );
      outtree_->Branch("jphi_2"      , &jphi_2_      );
      outtree_->Branch("mjj"         , &mjj_         );
      outtree_->Branch("jdeta"       , &jdeta_       );
      outtree_->Branch("higgsDecay"  , &decayType    );
      outtree_->Branch("genpt_1"     , &genpt_1_        );
      outtree_->Branch("genpt_2"     , &genpt_2_        );
      outtree_->Branch("geneta_2"    , &geneta_2_       );
      outtree_->Branch("geneta_1"    , &geneta_1_       );
      outtree_->Branch("HiggsPt"     , &HiggsPt_     );
      outtree_->Branch("HiggsPt"     , &HiggsPt_     );
      outtree_->Branch("n_jets_offline"     , &n_jets_offline_);
      outtree_->Branch("n_bjets_offline"     , &n_bjets_offline_);
    }
    count_ee_ = 0;
    count_em_ = 0;
    count_et_ = 0;
    count_mm_ = 0;
    count_mt_ = 0;
    count_tt_ = 0;
    
    return 0;
  }

  int HTTGenAnalysis::Execute(TreeEvent *event) {
    
    EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");
    event_ = (unsigned long long) eventInfo->event();
    wt_ = 1;
    if(eventInfo->weight_defined("wt_mc_sign")) wt_ = eventInfo->weight("wt_mc_sign");
    if(do_theory_uncert_){
      // note some of these labels may be generator dependent so need to make sure you check before using them
      if(eventInfo->weight_defined("1001")) scale1_ = eventInfo->weight("1001"); else scale1_=1.0;
      if(eventInfo->weight_defined("1002")) scale2_ = eventInfo->weight("1002"); else scale2_=1.0;
      if(eventInfo->weight_defined("1003")) scale3_ = eventInfo->weight("1003"); else scale3_=1.0;
      if(eventInfo->weight_defined("1004")) scale4_ = eventInfo->weight("1004"); else scale4_=1.0;
      if(eventInfo->weight_defined("1005")) scale5_ = eventInfo->weight("1005"); else scale5_=1.0;
      if(eventInfo->weight_defined("1006")) scale6_ = eventInfo->weight("1006"); else scale6_=1.0;
      if(eventInfo->weight_defined("1007")) scale7_ = eventInfo->weight("1007"); else scale7_=1.0;
      if(eventInfo->weight_defined("1008")) scale8_ = eventInfo->weight("1008"); else scale8_=1.0;
      if(eventInfo->weight_defined("1009")) scale9_ = eventInfo->weight("1009"); else scale9_=1.0; 
      //pdf variation weights
      if(eventInfo->weight_defined("2001")) wt_pdf_1_ = eventInfo->weight("2001"); else wt_pdf_1_=1.0;
      if(eventInfo->weight_defined("2002")) wt_pdf_2_ = eventInfo->weight("2002"); else wt_pdf_2_=1.0;
      if(eventInfo->weight_defined("2003")) wt_pdf_3_ = eventInfo->weight("2003"); else wt_pdf_3_=1.0;
      if(eventInfo->weight_defined("2004")) wt_pdf_4_ = eventInfo->weight("2004"); else wt_pdf_4_=1.0;
      if(eventInfo->weight_defined("2005")) wt_pdf_5_ = eventInfo->weight("2005"); else wt_pdf_5_=1.0;
      if(eventInfo->weight_defined("2006")) wt_pdf_6_ = eventInfo->weight("2006"); else wt_pdf_6_=1.0;
      if(eventInfo->weight_defined("2007")) wt_pdf_7_ = eventInfo->weight("2007"); else wt_pdf_7_=1.0;
      if(eventInfo->weight_defined("2008")) wt_pdf_8_ = eventInfo->weight("2008"); else wt_pdf_8_=1.0;
      if(eventInfo->weight_defined("2009")) wt_pdf_9_ = eventInfo->weight("2009"); else wt_pdf_9_=1.0;
      if(eventInfo->weight_defined("2010")) wt_pdf_10_ = eventInfo->weight("2010"); else wt_pdf_10_=1.0;
      if(eventInfo->weight_defined("2011")) wt_pdf_11_ = eventInfo->weight("2011"); else wt_pdf_11_=1.0;
      if(eventInfo->weight_defined("2012")) wt_pdf_12_ = eventInfo->weight("2012"); else wt_pdf_12_=1.0;
      if(eventInfo->weight_defined("2013")) wt_pdf_13_ = eventInfo->weight("2013"); else wt_pdf_13_=1.0;
      if(eventInfo->weight_defined("2014")) wt_pdf_14_ = eventInfo->weight("2014"); else wt_pdf_14_=1.0;
      if(eventInfo->weight_defined("2015")) wt_pdf_15_ = eventInfo->weight("2015"); else wt_pdf_15_=1.0;
      if(eventInfo->weight_defined("2016")) wt_pdf_16_ = eventInfo->weight("2016"); else wt_pdf_16_=1.0;
      if(eventInfo->weight_defined("2017")) wt_pdf_17_ = eventInfo->weight("2017"); else wt_pdf_17_=1.0;
      if(eventInfo->weight_defined("2018")) wt_pdf_18_ = eventInfo->weight("2018"); else wt_pdf_18_=1.0;
      if(eventInfo->weight_defined("2019")) wt_pdf_19_ = eventInfo->weight("2019"); else wt_pdf_19_=1.0;
      if(eventInfo->weight_defined("2020")) wt_pdf_20_ = eventInfo->weight("2020"); else wt_pdf_20_=1.0;
      if(eventInfo->weight_defined("2021")) wt_pdf_21_ = eventInfo->weight("2021"); else wt_pdf_21_=1.0;
      if(eventInfo->weight_defined("2022")) wt_pdf_22_ = eventInfo->weight("2022"); else wt_pdf_22_=1.0;
      if(eventInfo->weight_defined("2023")) wt_pdf_23_ = eventInfo->weight("2023"); else wt_pdf_23_=1.0;
      if(eventInfo->weight_defined("2024")) wt_pdf_24_ = eventInfo->weight("2024"); else wt_pdf_24_=1.0;
      if(eventInfo->weight_defined("2025")) wt_pdf_25_ = eventInfo->weight("2025"); else wt_pdf_25_=1.0;
      if(eventInfo->weight_defined("2026")) wt_pdf_26_ = eventInfo->weight("2026"); else wt_pdf_26_=1.0;
      if(eventInfo->weight_defined("2027")) wt_pdf_27_ = eventInfo->weight("2027"); else wt_pdf_27_=1.0;
      if(eventInfo->weight_defined("2028")) wt_pdf_28_ = eventInfo->weight("2028"); else wt_pdf_28_=1.0;
      if(eventInfo->weight_defined("2029")) wt_pdf_29_ = eventInfo->weight("2029"); else wt_pdf_29_=1.0;
      if(eventInfo->weight_defined("2030")) wt_pdf_30_ = eventInfo->weight("2030"); else wt_pdf_30_=1.0;
      if(eventInfo->weight_defined("2031")) wt_pdf_31_ = eventInfo->weight("2031"); else wt_pdf_31_=1.0;
      if(eventInfo->weight_defined("2032")) wt_pdf_32_ = eventInfo->weight("2032"); else wt_pdf_32_=1.0;
      if(eventInfo->weight_defined("2033")) wt_pdf_33_ = eventInfo->weight("2033"); else wt_pdf_33_=1.0;
      if(eventInfo->weight_defined("2034")) wt_pdf_34_ = eventInfo->weight("2034"); else wt_pdf_34_=1.0;
      if(eventInfo->weight_defined("2035")) wt_pdf_35_ = eventInfo->weight("2035"); else wt_pdf_35_=1.0;
      if(eventInfo->weight_defined("2036")) wt_pdf_36_ = eventInfo->weight("2036"); else wt_pdf_36_=1.0;
      if(eventInfo->weight_defined("2037")) wt_pdf_37_ = eventInfo->weight("2037"); else wt_pdf_37_=1.0;
      if(eventInfo->weight_defined("2038")) wt_pdf_38_ = eventInfo->weight("2038"); else wt_pdf_38_=1.0;
      if(eventInfo->weight_defined("2039")) wt_pdf_39_ = eventInfo->weight("2039"); else wt_pdf_39_=1.0;
      if(eventInfo->weight_defined("2040")) wt_pdf_40_ = eventInfo->weight("2040"); else wt_pdf_40_=1.0;
      if(eventInfo->weight_defined("2041")) wt_pdf_41_ = eventInfo->weight("2041"); else wt_pdf_41_=1.0;
      if(eventInfo->weight_defined("2042")) wt_pdf_42_ = eventInfo->weight("2042"); else wt_pdf_42_=1.0;
      if(eventInfo->weight_defined("2043")) wt_pdf_43_ = eventInfo->weight("2043"); else wt_pdf_43_=1.0;
      if(eventInfo->weight_defined("2044")) wt_pdf_44_ = eventInfo->weight("2044"); else wt_pdf_44_=1.0;
      if(eventInfo->weight_defined("2045")) wt_pdf_45_ = eventInfo->weight("2045"); else wt_pdf_45_=1.0;
      if(eventInfo->weight_defined("2046")) wt_pdf_46_ = eventInfo->weight("2046"); else wt_pdf_46_=1.0;
      if(eventInfo->weight_defined("2047")) wt_pdf_47_ = eventInfo->weight("2047"); else wt_pdf_47_=1.0;
      if(eventInfo->weight_defined("2048")) wt_pdf_48_ = eventInfo->weight("2048"); else wt_pdf_48_=1.0;
      if(eventInfo->weight_defined("2049")) wt_pdf_49_ = eventInfo->weight("2049"); else wt_pdf_49_=1.0;
      if(eventInfo->weight_defined("2050")) wt_pdf_50_ = eventInfo->weight("2050"); else wt_pdf_50_=1.0;
      if(eventInfo->weight_defined("2051")) wt_pdf_51_ = eventInfo->weight("2051"); else wt_pdf_51_=1.0;
      if(eventInfo->weight_defined("2052")) wt_pdf_52_ = eventInfo->weight("2052"); else wt_pdf_52_=1.0;
      if(eventInfo->weight_defined("2053")) wt_pdf_53_ = eventInfo->weight("2053"); else wt_pdf_53_=1.0;
      if(eventInfo->weight_defined("2054")) wt_pdf_54_ = eventInfo->weight("2054"); else wt_pdf_54_=1.0;
      if(eventInfo->weight_defined("2055")) wt_pdf_55_ = eventInfo->weight("2055"); else wt_pdf_55_=1.0;
      if(eventInfo->weight_defined("2056")) wt_pdf_56_ = eventInfo->weight("2056"); else wt_pdf_56_=1.0;
      if(eventInfo->weight_defined("2057")) wt_pdf_57_ = eventInfo->weight("2057"); else wt_pdf_57_=1.0;
      if(eventInfo->weight_defined("2058")) wt_pdf_58_ = eventInfo->weight("2058"); else wt_pdf_58_=1.0;
      if(eventInfo->weight_defined("2059")) wt_pdf_59_ = eventInfo->weight("2059"); else wt_pdf_59_=1.0;
      if(eventInfo->weight_defined("2060")) wt_pdf_60_ = eventInfo->weight("2060"); else wt_pdf_60_=1.0;
      if(eventInfo->weight_defined("2061")) wt_pdf_61_ = eventInfo->weight("2061"); else wt_pdf_61_=1.0;
      if(eventInfo->weight_defined("2062")) wt_pdf_62_ = eventInfo->weight("2062"); else wt_pdf_62_=1.0;
      if(eventInfo->weight_defined("2063")) wt_pdf_63_ = eventInfo->weight("2063"); else wt_pdf_63_=1.0;
      if(eventInfo->weight_defined("2064")) wt_pdf_64_ = eventInfo->weight("2064"); else wt_pdf_64_=1.0;
      if(eventInfo->weight_defined("2065")) wt_pdf_65_ = eventInfo->weight("2065"); else wt_pdf_65_=1.0;
      if(eventInfo->weight_defined("2066")) wt_pdf_66_ = eventInfo->weight("2066"); else wt_pdf_66_=1.0;
      if(eventInfo->weight_defined("2067")) wt_pdf_67_ = eventInfo->weight("2067"); else wt_pdf_67_=1.0;
      if(eventInfo->weight_defined("2068")) wt_pdf_68_ = eventInfo->weight("2068"); else wt_pdf_68_=1.0;
      if(eventInfo->weight_defined("2069")) wt_pdf_69_ = eventInfo->weight("2069"); else wt_pdf_69_=1.0;
      if(eventInfo->weight_defined("2070")) wt_pdf_70_ = eventInfo->weight("2070"); else wt_pdf_70_=1.0;
      if(eventInfo->weight_defined("2071")) wt_pdf_71_ = eventInfo->weight("2071"); else wt_pdf_71_=1.0;
      if(eventInfo->weight_defined("2072")) wt_pdf_72_ = eventInfo->weight("2072"); else wt_pdf_72_=1.0;
      if(eventInfo->weight_defined("2073")) wt_pdf_73_ = eventInfo->weight("2073"); else wt_pdf_73_=1.0;
      if(eventInfo->weight_defined("2074")) wt_pdf_74_ = eventInfo->weight("2074"); else wt_pdf_74_=1.0;
      if(eventInfo->weight_defined("2075")) wt_pdf_75_ = eventInfo->weight("2075"); else wt_pdf_75_=1.0;
      if(eventInfo->weight_defined("2076")) wt_pdf_76_ = eventInfo->weight("2076"); else wt_pdf_76_=1.0;
      if(eventInfo->weight_defined("2077")) wt_pdf_77_ = eventInfo->weight("2077"); else wt_pdf_77_=1.0;
      if(eventInfo->weight_defined("2078")) wt_pdf_78_ = eventInfo->weight("2078"); else wt_pdf_78_=1.0;
      if(eventInfo->weight_defined("2079")) wt_pdf_79_ = eventInfo->weight("2079"); else wt_pdf_79_=1.0;
      if(eventInfo->weight_defined("2080")) wt_pdf_80_ = eventInfo->weight("2080"); else wt_pdf_80_=1.0;
      if(eventInfo->weight_defined("2081")) wt_pdf_81_ = eventInfo->weight("2081"); else wt_pdf_81_=1.0;
      if(eventInfo->weight_defined("2082")) wt_pdf_82_ = eventInfo->weight("2082"); else wt_pdf_82_=1.0;
      if(eventInfo->weight_defined("2083")) wt_pdf_83_ = eventInfo->weight("2083"); else wt_pdf_83_=1.0;
      if(eventInfo->weight_defined("2084")) wt_pdf_84_ = eventInfo->weight("2084"); else wt_pdf_84_=1.0;
      if(eventInfo->weight_defined("2085")) wt_pdf_85_ = eventInfo->weight("2085"); else wt_pdf_85_=1.0;
      if(eventInfo->weight_defined("2086")) wt_pdf_86_ = eventInfo->weight("2086"); else wt_pdf_86_=1.0;
      if(eventInfo->weight_defined("2087")) wt_pdf_87_ = eventInfo->weight("2087"); else wt_pdf_87_=1.0;
      if(eventInfo->weight_defined("2088")) wt_pdf_88_ = eventInfo->weight("2088"); else wt_pdf_88_=1.0;
      if(eventInfo->weight_defined("2089")) wt_pdf_89_ = eventInfo->weight("2089"); else wt_pdf_89_=1.0;
      if(eventInfo->weight_defined("2090")) wt_pdf_90_ = eventInfo->weight("2090"); else wt_pdf_90_=1.0;
      if(eventInfo->weight_defined("2091")) wt_pdf_91_ = eventInfo->weight("2091"); else wt_pdf_91_=1.0;
      if(eventInfo->weight_defined("2092")) wt_pdf_92_ = eventInfo->weight("2092"); else wt_pdf_92_=1.0;
      if(eventInfo->weight_defined("2093")) wt_pdf_93_ = eventInfo->weight("2093"); else wt_pdf_93_=1.0;
      if(eventInfo->weight_defined("2094")) wt_pdf_94_ = eventInfo->weight("2094"); else wt_pdf_94_=1.0;
      if(eventInfo->weight_defined("2095")) wt_pdf_95_ = eventInfo->weight("2095"); else wt_pdf_95_=1.0;
      if(eventInfo->weight_defined("2096")) wt_pdf_96_ = eventInfo->weight("2096"); else wt_pdf_96_=1.0;
      if(eventInfo->weight_defined("2097")) wt_pdf_97_ = eventInfo->weight("2097"); else wt_pdf_97_=1.0;
      if(eventInfo->weight_defined("2098")) wt_pdf_98_ = eventInfo->weight("2098"); else wt_pdf_98_=1.0;
      if(eventInfo->weight_defined("2099")) wt_pdf_99_ = eventInfo->weight("2099"); else wt_pdf_99_=1.0;
      if(eventInfo->weight_defined("2100")) wt_pdf_100_ = eventInfo->weight("2100"); else wt_pdf_100_=1.0;
      
      //alpha_s variation weights
      if(eventInfo->weight_defined("2101")) wt_alphasdown_ = eventInfo->weight("2101"); else wt_alphasdown_=1.0; 
      if(eventInfo->weight_defined("2102")) wt_alphasup_ = eventInfo->weight("2102"); else wt_alphasup_=1.0;
    }
    
    std::vector<ic::GenParticle*> gen_particles = event->GetPtrVec<ic::GenParticle>("genParticles");
    std::vector<ic::GenJet*> gen_jets = event->GetPtrVec<ic::GenJet>("genJets");

    std::vector<ic::GenParticle> higgs_products;
    std::vector<ic::GenParticle> gen_taus;
    ic::Candidate met; 
    std::vector<ic::GenParticle> prompt_leptons;
    std::vector<std::string> decay_types;

    
    HiggsPt_=-9999;
    for(unsigned i=0; i<gen_particles.size(); ++i){
      if((gen_particles[i]->statusFlags()[FromHardProcessBeforeFSR] || gen_particles[i]->statusFlags()[IsLastCopy]) && gen_particles[i]->pdgid() == 25) {
          HiggsPt_ = gen_particles[i]->pt();
      }

      
      ic::GenParticle part = *gen_particles[i];
      ic::GenParticle higgs_product;
      
      unsigned genID = std::fabs(part.pdgid());
      bool status_flag_t = part.statusFlags().at(0);
      bool status_flag_tlc = part.statusFlags().at(13);

      // add neutrinos 4-vectors to get gen met
      if(genID == 12 || genID == 14 || genID == 16){
        met.set_vector(met.vector() + part.vector());
        continue;
      }
      
      
      if(!(genID == 15 && status_flag_t && status_flag_tlc)) continue;
      gen_taus.push_back(part);
      std::vector<ic::GenParticle> family;
      unsigned outputID = 15;
      FamilyTree(family, part, gen_particles, outputID);
      if(family.size()==1 && (outputID ==11 || outputID ==13)){
        higgs_products.push_back(family[0]);
        if (outputID == 11) {decay_types.push_back("e");}  
        else if (outputID == 13) {decay_types.push_back("m");}
      } else {
        decay_types.push_back("t");  
        ic::GenParticle had_tau;
        int charge = 0;
        int pdgid = 15;
        for(unsigned j=0; j<family.size(); ++j){
          had_tau.set_vector(had_tau.vector() + family[j].vector());
          charge += family[j].charge();
        }
        pdgid = 15*charge;
        had_tau.set_charge(charge);
        had_tau.set_pdgid(pdgid);
        higgs_products.push_back(had_tau);
      }
    }

    std::sort(higgs_products.begin(),higgs_products.end(),PtComparator());
    std::sort(gen_taus.begin(),gen_taus.end(),PtComparator());
    
    if(gen_taus.size()>=2){
      genpt_1_ = gen_taus[0].vector().Pt();
      genpt_2_ = gen_taus[1].vector().Pt();
      geneta_1_ = gen_taus[0].vector().Rapidity();
      geneta_2_ = gen_taus[1].vector().Rapidity();
    } else {
      genpt_1_ =  -9999;
      genpt_2_ =  -9999;
      geneta_1_ = -9999;
      geneta_2_ = -9999; 
    }
    
    std::vector<ic::GenParticle> electrons;
    std::vector<ic::GenParticle> muons;
    std::vector<ic::GenParticle> taus;
    
    double min_tau_pt[2];
    min_tau_pt[0]     = min_tau1_pt_;
    min_tau_pt[1]     = min_tau2_pt_;
    
    for(unsigned i=0; i<higgs_products.size(); ++i){
      unsigned ID = std::fabs(higgs_products[i].pdgid());
      double eta = std::fabs(higgs_products[i].vector().Rapidity());
      double pt = higgs_products[i].vector().Pt();
      if(ID == 11){
        if(pt > min_e_pt_ && eta < max_e_eta_) electrons.push_back(higgs_products[i]);  
      } else if(ID == 13){
        if(pt > min_mu_pt_ && eta < max_mu_eta_) muons.push_back(higgs_products[i]);  
      } else if(ID == 15){
        if(pt > min_tau_pt[i] && eta < max_tau_eta_) taus.push_back(higgs_products[i]);
      }
    }
    
    //size of decay_types vector should always be 2 but added this if statement just to be sure
    decayType = "";
    std::sort(decay_types.begin(),decay_types.end(),swap_labels());
    for(unsigned i=0; i< decay_types.size(); ++i){
      decayType += decay_types[i];
    }
    
    if(decayType == "ee") count_ee_++;
    if(decayType == "em") count_em_++;
    if(decayType == "et") count_et_++;
    if(decayType == "mm") count_mm_++;
    if(decayType == "mt") count_mt_++;
    if(decayType == "tt") count_tt_++;
    
    pt_1_ = -9999.;
    pt_2_ = -9999.;
    ic::Candidate lep1;
    ic::Candidate lep2;
    passed_ = false;

    if(channel_str_ == "em"){
      if(electrons.size() == 1 && muons.size() == 1){
        lep1 = electrons[0];
        lep2 = muons[0];
        passed_ = true;
      }
    } else if(channel_str_ == "et"){
      if(electrons.size() == 1 && taus.size() == 1){
        lep1 = electrons[0];
        lep2 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "mt"){
      if(muons.size() == 1 && taus.size() == 1){
        lep1 = muons[0];
        lep2 = taus[0];
        passed_ = true;
      }
    } else if(channel_str_ == "tt"){
      if(taus.size() == 2){
        lep1 = taus[0];
        lep2 = taus[1];
        passed_ = true;
      }
    }

    if(passed_){
      pt_1_  = lep1.vector().Pt();
      pt_2_  = lep2.vector().Pt();
      eta_1_ = lep1.vector().Rapidity();
      eta_2_ = lep2.vector().Rapidity();
      phi_1_ = lep1.vector().Phi();
      phi_2_ = lep2.vector().Phi();
      met_   = met.vector().Pt();
      pt_tt_ = (met.vector()+lep1.vector()+lep2.vector()).Pt();
      m_vis_ = (lep1.vector()+lep2.vector()).M();
      mt_1_ = MT(&lep1, &met);
      mt_2_ = MT(&lep2, &met);

      ic::CompositeCandidate *ditau = new ic::CompositeCandidate();
      ditau->AddCandidate("lep1",&lep1);
      ditau->AddCandidate("lep2",&lep2);
      pzeta_ = PZeta(ditau, &met , 0.85);
    } else {
      pt_1_  = -9999;
      pt_2_  = -9999;
      eta_1_ = -9999;
      eta_2_ = -9999;
      phi_1_ = -9999;
      phi_2_ = -9999;
      met_   = -9999;
      pt_tt_ = -9999;
      m_vis_ = -9999;
      mt_1_ = -9999;
      mt_2_ = -9999;
      pzeta_ = -9999;
    }
    
    std::vector<ic::GenJet> filtered_jets;
    std::vector<ic::GenJet> bjets;
    
    std::vector<GenParticle *> sel_bquarks;
    for (unsigned i=0; i < gen_particles.size(); ++i){
      std::vector<bool> status_flags = gen_particles[i]->statusFlags();
      unsigned id = abs(gen_particles[i]->pdgid());  
      if(id == 5 && status_flags[FromHardProcess] && status_flags[IsLastCopy] && gen_particles[i]->vector().Pt()>0){
        sel_bquarks.push_back(gen_particles[i]);
      }
    }
     
    for(unsigned i=0; i<gen_jets.size(); ++i){
      ic::GenJet jet = *gen_jets[i];
      double jetPt = jet.vector().Pt();
      double jetEta = std::fabs(jet.vector().Rapidity());
      if(jetPt > min_jet_pt_ && jetEta < max_jet_eta_) filtered_jets.push_back(jet); 
      if(jetPt > 20 && jetEta < 2.4){
          bool MatchedToB = false;
          for(unsigned j=0; j<sel_bquarks.size(); ++j) if(DRLessThan(std::make_pair(&jet, sel_bquarks[j]),0.5)) MatchedToB = true;
          if(MatchedToB) bjets.push_back(jet); 
      }
    }
    
    for(unsigned i=0; i<bjets.size(); ++i){
      ic::GenJet jet = bjets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) bjets.erase (bjets.begin()+i);
    }
    
    n_bjets_noscale_ = bjets.size();
    
    for(unsigned i=0;  i<bjets.size(); ++i){
      ic::GenJet jet = bjets[i];
      double pt = bjets[i].vector().Pt();
      double eta = fabs(bjets[i].vector().Rapidity());
      double eff=0;
      if(pt > bbtag_eff_->GetXaxis()->GetBinLowEdge(bbtag_eff_->GetNbinsX()+1)){
        eff = bbtag_eff_->GetBinContent(bbtag_eff_->GetNbinsX(),bbtag_eff_->GetYaxis()->FindBin(eta));
      } else{
        eff = bbtag_eff_->GetBinContent(bbtag_eff_->GetXaxis()->FindBin(pt),bbtag_eff_->GetYaxis()->FindBin(eta));
      }
      rand->SetSeed((int)((bjets[i].eta()+5)*100000));
      double randVal = rand->Uniform();
      if (randVal > eff) bjets.erase (bjets.begin()+i);
    }
    n_bjets_ = bjets.size();
    n_jets_nofilter_ = filtered_jets.size();
    
    for(unsigned i=0; i<filtered_jets.size(); ++i){
      ic::GenJet jet = filtered_jets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(&jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) filtered_jets.erase (filtered_jets.begin()+i);
    }
    
    std::string jets_label_ = "ak4PFJetsCHS";
    std::vector<PFJet*> jets = event->GetPtrVec<PFJet>(jets_label_);
    std::sort(jets.begin(), jets.end(), bind(&Candidate::pt, _1) > bind(&Candidate::pt, _2));
    
    for(unsigned i=0; i<jets.size(); ++i){
      ic::PFJet *jet = jets[i];
      bool MatchedToPrompt = false;
      for(unsigned j=0; j<higgs_products.size(); ++j){
        if(DRLessThan(std::make_pair(jet, &higgs_products[j]),0.5)) MatchedToPrompt = true;
      }
      //remove jets that are matched to Higgs decay products
      if(MatchedToPrompt) jets.erase (jets.begin()+i);
    }
    
    std::vector<PFJet*> offline_bjets = jets;
    ic::erase_if(jets,!boost::bind(MinPtMaxEta, _1, 30.0, 4.7));
    ic::erase_if(offline_bjets,!boost::bind(MinPtMaxEta, _1, 20.0, 2.4));
    
    std::string btag_label="pfCombinedInclusiveSecondaryVertexV2BJetTags";
    double btag_wp =  0.8484;
    if (event->Exists("retag_result")) {
      auto const& retag_result = event->Get<std::map<std::size_t,bool>>("retag_result"); 
      ic::erase_if(offline_bjets, !boost::bind(IsReBTagged, _1, retag_result));
    }else { 
      ic::erase_if(offline_bjets, boost::bind(&PFJet::GetBDiscriminator, _1, btag_label) < btag_wp);
    }
    
    
    n_jets_offline_ = jets.size();
    n_bjets_offline_ = offline_bjets.size();

    n_jets_ = filtered_jets.size();
    jpt_1_       = -9999;
    jeta_1_      = -9999;
    jphi_1_      = -9999;
    jpt_2_       = -9999;
    jpt_3_       = -9999;
    jeta_2_      = -9999;
    jphi_2_      = -9999;
    mjj_         = -9999;
    jdeta_       = -9999;
    n_jetsingap_ = 9999;
    if(n_jets_ > 0){
      jpt_1_  = filtered_jets[0].vector().Pt();
      jeta_1_ = filtered_jets[0].vector().Rapidity();
      jphi_1_ = filtered_jets[0].vector().Phi();
    }
    if(n_jets_ > 1){
      jpt_2_  = filtered_jets[1].vector().Pt();
      jeta_2_ = filtered_jets[1].vector().Rapidity();
      jphi_2_ = filtered_jets[1].vector().Phi();
      mjj_   = (filtered_jets[0].vector()+filtered_jets[1].vector()).M();
      jdeta_  = std::fabs(filtered_jets[0].vector().Rapidity() - filtered_jets[1].vector().Rapidity());
      double max_jeta = std::max(jeta_1_,jeta_2_);
      double min_jeta = std::min(jeta_1_,jeta_2_);
      n_jetsingap_ = 0;
      for(unsigned i=2; i<n_jets_; ++i){
         double jeta_3 = filtered_jets[i].vector().Rapidity();
         if(jeta_3 > min_jeta && jeta_3 < max_jeta) n_jetsingap_++;    
      }
    }
    if(n_jets_ > 2){
      jpt_3_  = filtered_jets[2].vector().Pt();      
    }

    if(fs_) outtree_->Fill();
    
    return 0;
  }
  int HTTGenAnalysis::PostAnalysis() {
    std::cout << "ee count = " << count_ee_ << std::endl;
    std::cout << "em count = " << count_em_ << std::endl;
    std::cout << "et count = " << count_et_ << std::endl;
    std::cout << "mm count = " << count_mm_ << std::endl;
    std::cout << "mt count = " << count_mt_ << std::endl;
    std::cout << "tt count = " << count_tt_ << std::endl;
    return 0;
  }

  void HTTGenAnalysis::PrintInfo() {
    ;
  }

}
