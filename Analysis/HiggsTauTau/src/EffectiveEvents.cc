#include "UserCode/ICHiggsTauTau/Analysis/HiggsTauTau/interface/EffectiveEvents.h"

namespace ic {
EffectiveEvents::EffectiveEvents(std::string const& name) : ModuleBase(name){
fs_ = NULL;
do_qcd_scale_wts_=false;
do_pdf_wts_=false;
}

EffectiveEvents::~EffectiveEvents(){
;
}

int EffectiveEvents::PreAnalysis(){
outtree_ = fs_->make<TTree>("effective","effective");
outtree_->Branch("wt",&mcsign_);
outtree_->Branch("gen_ht",&gen_ht_);
if(do_qcd_scale_wts_){
  outtree_->Branch("wt_mur1_muf1",    &scale1_);
  outtree_->Branch("wt_mur1_muf2",    &scale2_);
  outtree_->Branch("wt_mur1_muf0p5",  &scale3_);
  outtree_->Branch("wt_mur2_muf1",    &scale4_);
  outtree_->Branch("wt_mur2_muf2",    &scale5_);
  outtree_->Branch("wt_mur2_muf0p5",  &scale6_);
  outtree_->Branch("wt_mur0p5_muf1",  &scale7_);
  outtree_->Branch("wt_mur0p5_muf2",  &scale8_);
  outtree_->Branch("wt_mur0p5_muf0p5",&scale9_);
}
if(do_pdf_wts_){
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
return 0;
}

int EffectiveEvents::Execute(TreeEvent *event){
 EventInfo const* eventInfo = event->GetPtr<EventInfo>("eventInfo");

  //if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_ = 1.0;
  if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_= 1;
  gen_ht_ = eventInfo->gen_ht();
//  mcsign_ = eventInfo->weight;
//  if (eventInfo->weight_defined("wt_mc_sign")) mcsign_ = eventInfo->weight("wt_mc_sign"); else mcsign_ = 1.0;
//std::cout<<mcsign_<<std::endl;
 if(do_qcd_scale_wts_){
   // note some of these labels may be generator dependent so need to make sure you check before using them
   if(eventInfo->weight_defined("1001")) scale1_ = eventInfo->weight("1001")*mcsign_; else scale1_=mcsign_;
   if(eventInfo->weight_defined("1002")) scale2_ = eventInfo->weight("1002")*mcsign_; else scale2_=mcsign_;
   if(eventInfo->weight_defined("1003")) scale3_ = eventInfo->weight("1003")*mcsign_; else scale3_=mcsign_;
   if(eventInfo->weight_defined("1004")) scale4_ = eventInfo->weight("1004")*mcsign_; else scale4_=mcsign_;
   if(eventInfo->weight_defined("1005")) scale5_ = eventInfo->weight("1005")*mcsign_; else scale5_=mcsign_;
   if(eventInfo->weight_defined("1006")) scale6_ = eventInfo->weight("1006")*mcsign_; else scale6_=mcsign_;
   if(eventInfo->weight_defined("1007")) scale7_ = eventInfo->weight("1007")*mcsign_; else scale7_=mcsign_;
   if(eventInfo->weight_defined("1008")) scale8_ = eventInfo->weight("1008")*mcsign_; else scale8_=mcsign_;
   if(eventInfo->weight_defined("1009")) scale9_ = eventInfo->weight("1009")*mcsign_; else scale9_=mcsign_;    
 }
 if(do_pdf_wts_){ 
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
 
 outtree_->Fill();
 return 0;
 }

int EffectiveEvents::PostAnalysis(){
 return 0;
 }

 void EffectiveEvents::PrintInfo(){
;
}
}

